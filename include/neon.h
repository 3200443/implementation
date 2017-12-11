#pragma once
#include "defneon.h"



void routine_FrameDifference_SSE2(uint8 **It, uint8 **Itm1, uint8 **Et, long vi0,long vi1,long vj0,long vj1, uint8 seuil);
void routine_SigmaDelta_step0SSE2(uint8** I, uint8 **M, uint8 **V, long vi0, long vi1, long vj0, long vj1);
void routine_SigmaDelta_1stepSSE2(uint8 **It, uint8 **Itm1, uint8**Vt, uint8 **Vtm1, uint8**Mt, uint8 **Mtm1, uint8 **Et,  long vi0, long vi1, long vj0, long vj1 );
void erosion3x3_SIMD(uint8 **It,uint8 **It1,long vi0,long vi1,long vj0,long vj1);
void dilatation3x3_SIMD(uint8 **It,uint8 **It1,long vi0,long vi1,long vj0,long vj1);
void test_routine_FrameDifference_SSE2(int seuil);
void dup_vui8matrix(vuint8 **X, int i0, int i1, int j0, int j1, vuint8 **Y);
void test_routine_sigmaDelta_SSE2();