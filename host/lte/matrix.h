/*
 * Copyright (c) 2013 The Regents of The University of Michigan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Yajing Chen
 *          Qi Zheng
 */
#pragma once
#include <stdio.h>
#include <cmath>
#include <complex>
#include <iostream>

using namespace std;
template<class U,class T>
T InnerProd(U Len, T* v1,T* v2);

template<class U, class T>
void VectorPlus(U len, T* v1, T* v2, T* vout);

template<class U, class T>
void VectorMinus(U len, T* v1, T* v2, T* vout);

template<class U,class T>
void MatrixProd(U d1, U d2, U d3, T** M1, T** M2, T** oM);

template<class U,class T>
void MatrixProd(U d1, U d2, U d3, T** M1, T* V2, T* oV);

template<class U,class T>
void CircularConvolution(U Ld,U Sd,T* pVL,T* pVS,T* oV);

template<class U,class T>
double norm(U len, T* v);

template<class U, class T>
double p1_norm(U len, T* v);

template<class U, class T>
void MatrixInv(U sz,T** pM,T** pInvM);
