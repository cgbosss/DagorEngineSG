// Copyright (C) Gaijin Games KFT.  All rights reserved.
#pragma once

#if 0
//
// Generated by Microsoft (R) D3D Shader Disassembler
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// POSITION                 0   xyzw        0     NONE   float   xyzw
// COLOR                    0   xyzw        1     NONE   float   xyzw
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float   xyzw
// TEXCOORD                 0   xyzw        1     NONE   float   xyzw
//
vs_5_1
dcl_globalFlags refactoringAllowed
dcl_constantbuffer CB0[0:0][4], immediateIndexed, space=0
dcl_input v0.xyzw
dcl_input v1.xyzw
dcl_output_siv o0.xyzw, position
dcl_output o1.xyzw
dp4 o0.x, v0.xyzw, CB0[0][0].xyzw
dp4 o0.y, v0.xyzw, CB0[0][1].xyzw
dp4 o0.z, v0.xyzw, CB0[0][2].xyzw
dp4 o0.w, v0.xyzw, CB0[0][3].xyzw
mov o1.xyzw, v1.xyzw
ret
// Approximately 0 instruction slots used
#endif

const BYTE g_main[] = {68, 88, 66, 67, 119, 182, 80, 112, 42, 211, 204, 237, 80, 206, 225, 79, 56, 50, 172, 231, 1, 0, 0, 0, 224, 1, 0,
  0, 3, 0, 0, 0, 44, 0, 0, 0, 124, 0, 0, 0, 212, 0, 0, 0, 73, 83, 71, 78, 72, 0, 0, 0, 2, 0, 0, 0, 8, 0, 0, 0, 56, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 15, 15, 0, 0, 65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 1, 0, 0, 0, 15, 15, 0, 0, 80, 79,
  83, 73, 84, 73, 79, 78, 0, 67, 79, 76, 79, 82, 0, 171, 79, 83, 71, 78, 80, 0, 0, 0, 2, 0, 0, 0, 8, 0, 0, 0, 56, 0, 0, 0, 0, 0, 0, 0,
  1, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 68, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 1, 0, 0, 0, 15, 0, 0, 0, 83, 86,
  95, 80, 79, 83, 73, 84, 73, 79, 78, 0, 84, 69, 88, 67, 79, 79, 82, 68, 0, 171, 171, 171, 83, 72, 69, 88, 4, 1, 0, 0, 81, 0, 1, 0, 65,
  0, 0, 0, 106, 8, 0, 1, 89, 0, 0, 7, 70, 142, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 95, 0, 0, 3, 242, 16,
  16, 0, 0, 0, 0, 0, 95, 0, 0, 3, 242, 16, 16, 0, 1, 0, 0, 0, 103, 0, 0, 4, 242, 32, 16, 0, 0, 0, 0, 0, 1, 0, 0, 0, 101, 0, 0, 3, 242,
  32, 16, 0, 1, 0, 0, 0, 17, 0, 0, 9, 18, 32, 16, 0, 0, 0, 0, 0, 70, 30, 16, 0, 0, 0, 0, 0, 70, 142, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 17, 0, 0, 9, 34, 32, 16, 0, 0, 0, 0, 0, 70, 30, 16, 0, 0, 0, 0, 0, 70, 142, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 17,
  0, 0, 9, 66, 32, 16, 0, 0, 0, 0, 0, 70, 30, 16, 0, 0, 0, 0, 0, 70, 142, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 17, 0, 0, 9, 130,
  32, 16, 0, 0, 0, 0, 0, 70, 30, 16, 0, 0, 0, 0, 0, 70, 142, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 54, 0, 0, 5, 242, 32, 16, 0, 1,
  0, 0, 0, 70, 30, 16, 0, 1, 0, 0, 0, 62, 0, 0, 1};
