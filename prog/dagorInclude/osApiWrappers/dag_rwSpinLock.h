//
// Dagor Engine 6.5
// Copyright (C) Gaijin Games KFT.  All rights reserved.
//
#pragma once

#include <perfMon/dag_daProfilerToken.h>
#include <osApiWrappers/dag_threadSafety.h>
#include <osApiWrappers/dag_miscApi.h>
#include <osApiWrappers/dag_atomic.h>

// all primives in this file are very simple read-write locks designed around spinlock/atomics
// they are small (8 bytes), light-weighed, non-reentrant and NOT intent to be used in heavy contention cases!
// if you need "fair" or reentrant RWlocks, use OSReadWriteLock or OSReentrantReadWriteLock
// which are based on OS primitives (and they use limited OS resources).
// use spinlock primitives only on low contention cases, especially low "write"-contention cases


// Warn: not reentrant (recursive), use OSReentrantReadWriteLock if you need it
// This mimics OSReadWriteLock class interface over spinlock and atomic int16.
// this is Raynal's read-preferring RWlock, with additional explicit writers preferential
// compared to SpinLockReadWriteLock it is one relaxed_load more costly for lockRead, and two atomic counter cost for lockWrite
// this prioritize writers but still there is absolutely no ordering,
// meaning both writers and readers obtain access regardless on "when" they tried to.
// so, if there is enough writers contention, no any reader will ever gain access

struct DAG_TS_CAPABILITY("mutex") SpinLockReadWriteLock
{
  SpinLockReadWriteLock() {}
  void lockRead() DAG_TS_ACQUIRE_SHARED()
  {
    int64_t reft = dagor_lock_profiler_start();
    // first we try to wait till there are no 'waiting' writers, this will prioritize writers
    if (interlocked_relaxed_load(waitingWritersCount))
      spin_wait_no_profile([&]() { return interlocked_acquire_load(waitingWritersCount) != 0; });
    // first we try to wait till there are no 'waiting' writers, this will prioritize writers
    if (interlocked_increment(writerReadersWord) >= WRITER_BIT)
      spin_wait_no_profile([&] { return interlocked_acquire_load(writerReadersWord) >= WRITER_BIT; });
    dagor_lock_profiler_stop(reft, da_profiler::DescReadLock, 1);
  }
  void unlockRead() DAG_TS_RELEASE_SHARED() { interlocked_decrement(writerReadersWord); }
  void lockWrite() DAG_TS_ACQUIRE()
  {
    interlocked_increment(waitingWritersCount);
    int64_t reft = dagor_lock_profiler_start();
    while (!tryLockWrite())
      spin_wait_no_profile([&] { return interlocked_acquire_load(writerReadersWord) != 0; });
    dagor_lock_profiler_stop(reft, da_profiler::DescWriteLock, 10);
  }
  void unlockWrite() DAG_TS_RELEASE()
  {
    interlocked_and(writerReadersWord, ~WRITER_BIT);
    interlocked_decrement(waitingWritersCount);
  }

protected:
  static constexpr uint32_t WRITER_BIT = 1UL << 30;
  bool tryLockWrite() { return interlocked_compare_exchange(writerReadersWord, WRITER_BIT, 0) == 0; }
  SpinLockReadWriteLock(const SpinLockReadWriteLock &) = delete;
  SpinLockReadWriteLock &operator=(const SpinLockReadWriteLock &) = delete;
  volatile uint32_t writerReadersWord = 0, waitingWritersCount = 0;
};