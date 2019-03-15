// This file is part of the AliceVision project.
// Copyright (c) 2017 AliceVision contributors.
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <aliceVision/depthMap/cuda/planeSweeping/device_utils.cu>

#include <math_constants.h>

namespace aliceVision {
namespace depthMap {

// Global data handlers and parameters

texture<int2,          cudaTextureType2D, cudaReadModeElementType> pixsTex;
texture<float2,        cudaTextureType2D, cudaReadModeElementType> gradTex;
texture<float,         cudaTextureType2D, cudaReadModeElementType> depthsTex;
texture<float,         cudaTextureType2D, cudaReadModeElementType> depthsTex1;
texture<float4,        cudaTextureType2D, cudaReadModeElementType> normalsTex;
texture<float,         cudaTextureType2D, cudaReadModeElementType> sliceTex;
texture<float2,        cudaTextureType2D, cudaReadModeElementType> sliceTexFloat2;
texture<float,         cudaTextureType2D, cudaReadModeElementType> sliceTexUChar;
texture<float2,         cudaTextureType2D, cudaReadModeElementType> sliceTexUInt2;
texture<float,         cudaTextureType2D, cudaReadModeElementType> sliceTexUInt;
texture<uchar4,        cudaTextureType2D, cudaReadModeElementType> rTexU4;
texture<uchar4,        cudaTextureType2D, cudaReadModeElementType> tTexU4;
texture<float4,        cudaTextureType2D, cudaReadModeElementType> f4Tex;

//////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline __device__ void swap( T& a, T& b )
{
    T tmp = a;
    a = b;
    b = tmp;
}

__device__ unsigned char computeGradientSizeOfL( cudaTextureObject_t rc_tex, int x, int y)
{

    float xM1 = 255.0f * (tex2D<float4>(rc_tex, (float)(x - 1) + 0.5f, (float)(y + 0) + 0.5f).x);
    float xP1 = 255.0f * (tex2D<float4>(rc_tex, (float)(x + 1) + 0.5f, (float)(y + 0) + 0.5f).x);
    float yM1 = 255.0f * (tex2D<float4>(rc_tex, (float)(x + 0) + 0.5f, (float)(y - 1) + 0.5f).x);
    float yP1 = 255.0f * (tex2D<float4>(rc_tex, (float)(x + 0) + 0.5f, (float)(y + 1) + 0.5f).x);

    // not divided by 2?
    float2 g = make_float2(xM1 - xP1, yM1 - yP1);

    return (unsigned char)size(g);
}

__global__ void compute_varLofLABtoW_kernel(cudaTextureObject_t rc_tex, uchar4* labMap, int labMap_p, int width, int height)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if(x < width && y < height)
    {
        uchar4* val = get2DBufferAt(labMap, labMap_p, x, y);
        unsigned char grad = computeGradientSizeOfL(rc_tex, x, y);
        val->w = grad;
    }
}

__device__ void move3DPointByRcPixSize(float3& p, float rcPixSize)
{
    float3 rpv = p - sg_s_r.C;
    normalize(rpv);
    p = p + rpv * rcPixSize;
}

__device__ void move3DPointByTcPixStep(float3& p, float tcPixStep)

{
    float3 rpv = sg_s_r.C - p;
    float3 prp = p;
    float3 prp1 = p + rpv / 2.0f;

    float2 rp;
    getPixelFor3DPointRC(rp, prp);

    float2 tpo;
    getPixelFor3DPointTC(tpo, prp);

    float2 tpv;
    getPixelFor3DPointTC(tpv, prp1);

    tpv = tpv - tpo;
    normalize(tpv);

    float2 tpd = tpo + tpv * tcPixStep;

    p = triangulateMatchRef(rp, tpd);
}

__device__ float move3DPointByTcOrRcPixStep(int2& pix, float3& p, float pixStep, bool moveByTcOrRc)
{
    if(moveByTcOrRc == true)
    {
        move3DPointByTcPixStep(p, pixStep);
        return 0.0f;
    }
    else
    {
        float pixSize = pixStep * computePixSize(p);
        move3DPointByRcPixSize(p, pixSize);

        return pixSize;
    }
}

__global__ void getSilhoueteMap_kernel(bool* out, int out_p, int step, int width, int height, const uchar4 maskColorLab)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if((x * step < width) && (y * step < height))
    {
        uchar4 col = tex2D(rTexU4, x * step, y * step);
        *get2DBufferAt(out, out_p, x, y) = ((maskColorLab.x == col.x) && (maskColorLab.y == col.y) && (maskColorLab.z == col.z));
    }
}

} // namespace depthMap
} // namespace aliceVision
