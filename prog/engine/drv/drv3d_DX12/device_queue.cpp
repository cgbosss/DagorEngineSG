// Copyright (C) Gaijin Games KFT.  All rights reserved.

#include <util/dag_watchdog.h>

#include "device_queue.h"
#include "device.h"

using namespace drv3d_dx12;

#if _TARGET_XBOX
#include "device_queue_xbox.inl.cpp"
#endif

#if _TARGET_PC_WIN

void TileMapperAccel::beginTileMapping(ID3D12Resource *tex_in, ID3D12Heap *heap_in, size_t heap_base, size_t mapping_count)
{
  G_ASSERT(trcs.empty() && trss.empty() && offsets.empty() && rangeFlags.empty());

  trcs.reserve(mapping_count);
  trss.reserve(mapping_count);
  offsets.reserve(mapping_count);
  counts.reserve(mapping_count);

  rangeFlags.resize(mapping_count, heap ? D3D12_TILE_RANGE_FLAG_NONE : D3D12_TILE_RANGE_FLAG_NULL);

  tex = tex_in;
  heap = heap_in;
  heapBase = heap_base;
}

void TileMapperAccel::addTileMappings(const TileMapping *mapping, size_t mapping_count)
{
  for (auto &map : eastl::span{mapping, mapping_count})
  {
    D3D12_TILED_RESOURCE_COORDINATE &trc = trcs.emplace_back();
    trc.X = map.texX;
    trc.Y = map.texY;
    trc.Z = map.texZ;
    trc.Subresource = map.texSubresource;

    D3D12_TILE_REGION_SIZE &trs = trss.emplace_back();
    trs.UseBox = (map.heapTileSpan == 1) ? TRUE : FALSE;
    trs.Width = map.heapTileSpan;
    trs.Height = 1;
    trs.Depth = 1;
    trs.NumTiles = map.heapTileSpan;

    offsets.emplace_back(map.heapTileIndex + heapBase);
    counts.emplace_back(map.heapTileSpan);
  }
}

void TileMapperAccel::endTileMapping(ID3D12CommandQueue *queue)
{
  G_ASSERT((trcs.size() & trss.size() & offsets.size() & counts.size() & rangeFlags.size()) == trcs.size());

  queue->UpdateTileMappings(tex, trcs.size(), trcs.data(), trss.data(), heap, trcs.size(), rangeFlags.data(), offsets.data(),
    counts.data(), D3D12_TILE_MAPPING_FLAG_NONE);

  clear();
}

void TileMapperAccel::clear()
{
  trcs.clear();
  trss.clear();
  offsets.clear();
  counts.clear();
  rangeFlags.clear();
}

#endif

TileMapperAccel DeviceQueue::tileMapperAccel;

bool DeviceQueueGroup::init(ID3D12Device *device, bool give_names)
{
  ComPtr<ID3D12CommandQueue> graphicsQueue, computeQueue, uploadQueue, readBackQueue;
#if _TARGET_PC
  D3D12_COMMAND_QUEUE_DESC cqd = {};
  cqd.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;

  cqd.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
  if (!DX12_CHECK_OK(device->CreateCommandQueue(&cqd, COM_ARGS(&graphicsQueue))))
  {
    return false;
  }

  cqd.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
  if (!DX12_CHECK_OK(device->CreateCommandQueue(&cqd, COM_ARGS(&computeQueue))))
  {
    return false;
  }

  cqd.Type = D3D12_COMMAND_LIST_TYPE_COPY;
  if (!DX12_CHECK_OK(device->CreateCommandQueue(&cqd, COM_ARGS(&uploadQueue))))
  {
    return false;
  }

  cqd.Type = D3D12_COMMAND_LIST_TYPE_COPY; // -V1048
  if (!DX12_CHECK_OK(device->CreateCommandQueue(&cqd, COM_ARGS(&readBackQueue))))
  {
    return false;
  }
#else
  if (!xbox_create_queues(device, graphicsQueue, computeQueue, uploadQueue, readBackQueue))
    return false;
#endif

  if (!uploadProgress.reset(device, give_names ? L"UploadProgress" : nullptr))
  {
    return false;
  }
  if (!graphicsProgress.reset(device, give_names ? L"GraphicsProgress" : nullptr))
  {
    return false;
  }
  if (!frameProgress.reset(device, give_names ? L"FrameProgress" : nullptr))
  {
    return false;
  }

  if (give_names)
  {
    DX12_SET_DEBUG_OBJ_NAME(uploadQueue, L"UploadQueue");
    DX12_SET_DEBUG_OBJ_NAME(readBackQueue, L"ReadBackQueue");
    DX12_SET_DEBUG_OBJ_NAME(computeQueue, L"ComputeQueue");
    DX12_SET_DEBUG_OBJ_NAME(graphicsQueue, L"GraphicsQueue");
  }

  group[static_cast<uint32_t>(DeviceQueueType::GRAPHICS)] = DeviceQueue(eastl::move(graphicsQueue));
  group[static_cast<uint32_t>(DeviceQueueType::COMPUTE)] = DeviceQueue(eastl::move(computeQueue));
  group[static_cast<uint32_t>(DeviceQueueType::UPLOAD)] = DeviceQueue(eastl::move(uploadQueue));
  group[static_cast<uint32_t>(DeviceQueueType::READ_BACK)] = DeviceQueue(eastl::move(readBackQueue));

  return true;
}

void DeviceQueueGroup::shutdown()
{
  for (auto &&q : group)
    q = DeviceQueue{};
  uploadProgress.reset();
  graphicsProgress.reset();
  frameProgress.reset();
}

DeviceQueue::~DeviceQueue() {}

bool drv3d_dx12::wait_for_frame_progress_with_event_slow_path(DeviceQueueGroup &qs, uint64_t progress, HANDLE event, const char *what)
{
#if LOCK_PROFILER_ENABLED
  using namespace da_profiler;
  static desc_id_t wffpdesc =
    add_description(DA_PROFILE_FILE_NAMES ? __FILE__ : nullptr, __LINE__, IsWait, "DX12_waitForFrameProgress");
  ScopeLockProfiler<da_profiler::NoDesc> lp(wffpdesc);
#endif
  if (!qs.waitForFrameProgress(progress, event))
  {
    return false;
  }

  auto result = WaitForSingleObject(event, MAX_WAIT_OBJECT_TIMEOUT_MS);
  while (result == WAIT_TIMEOUT)
  {
    if (get_device().isInErrorState())
    {
      logdbg("DX12: While waiting for frame progress %u - %s, WaitForSingleObject(%p, MAX_WAIT_OBJECT_TIMEOUT_MS) returned "
             "WAIT_TIMEOUT, but device is in error state, return false to avoid infinite loops",
        progress, what, event);
      return false;
    }
    logdbg("DX12: While waiting for frame progress %u - %s, WaitForSingleObject(%p, MAX_WAIT_OBJECT_TIMEOUT_MS) returned "
           "WAIT_TIMEOUT, continue",
      progress, what, event);
    result = WaitForSingleObject(event, MAX_WAIT_OBJECT_TIMEOUT_MS);
  }

  if (WAIT_OBJECT_0 == result)
  {
    return true;
  }
  if (DAGOR_UNLIKELY(WAIT_FAILED == result))
  {
    logwarn("DX12: While waiting for frame progress %u - %s, WaitForSingleObject(%p, MAX_WAIT_OBJECT_TIMEOUT_MS) "
            "failed with 0x%08X, trying to continue with alternatives",
      progress, what, event, GetLastError());
  }
  if (qs.checkFrameProgress() >= progress)
  {
    logdbg("DX12: Progress check allowed continuation");
    return true;
  }
  logdbg("DX12: Calling SetEventOnCompletion with nullptr");
  return qs.waitForFrameProgress(progress, nullptr);
}
