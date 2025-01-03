// Copyright (C) Gaijin Games KFT.  All rights reserved.
#pragma once

#if 0
//
// Generated by Microsoft (R) D3D Shader Disassembler
//
//
//   fxc /T vs_4_0 /O3 default_clear_vs.vsh /Qstrip_debug /Qstrip_reflect /Fh
//    default_clear_vs.h
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// POSITION                 0   xyzw        0     NONE  float   xyzw
// TEXCOORD                 0   xyzw        1     NONE  float   xyzw
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// SV_POSITION              0   xyzw        0      POS  float   xyzw
// TEXCOORD                 0   xyzw        1     NONE  float   xyzw
//
vs_4_0
dcl_input v0.xyzw
dcl_input v1.xyzw
dcl_output_siv o0.xyzw, position
dcl_output o1.xyzw
mov o0.xyzw, v0.xyzw
mov o1.xyzw, v1.xyzw
ret 
// Approximately 0 instruction slots used
#endif

const BYTE g_main[] = {68, 88, 66, 67, 68, 48, 250, 15, 231, 243, 71, 222, 64, 28, 252, 3, 252, 38, 177, 6, 1, 0, 0, 0, 72, 1, 0, 0, 3,
  0, 0, 0, 44, 0, 0, 0, 128, 0, 0, 0, 216, 0, 0, 0, 73, 83, 71, 78, 76, 0, 0, 0, 2, 0, 0, 0, 8, 0, 0, 0, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 15, 15, 0, 0, 65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 1, 0, 0, 0, 15, 15, 0, 0, 80, 79, 83,
  73, 84, 73, 79, 78, 0, 84, 69, 88, 67, 79, 79, 82, 68, 0, 171, 171, 79, 83, 71, 78, 80, 0, 0, 0, 2, 0, 0, 0, 8, 0, 0, 0, 56, 0, 0, 0,
  0, 0, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 68, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 1, 0, 0, 0, 15, 0, 0,
  0, 83, 86, 95, 80, 79, 83, 73, 84, 73, 79, 78, 0, 84, 69, 88, 67, 79, 79, 82, 68, 0, 171, 171, 171, 83, 72, 68, 82, 104, 0, 0, 0, 64,
  0, 1, 0, 26, 0, 0, 0, 95, 0, 0, 3, 242, 16, 16, 0, 0, 0, 0, 0, 95, 0, 0, 3, 242, 16, 16, 0, 1, 0, 0, 0, 103, 0, 0, 4, 242, 32, 16, 0,
  0, 0, 0, 0, 1, 0, 0, 0, 101, 0, 0, 3, 242, 32, 16, 0, 1, 0, 0, 0, 54, 0, 0, 5, 242, 32, 16, 0, 0, 0, 0, 0, 70, 30, 16, 0, 0, 0, 0, 0,
  54, 0, 0, 5, 242, 32, 16, 0, 1, 0, 0, 0, 70, 30, 16, 0, 1, 0, 0, 0, 62, 0, 0, 1};
