#if !defined(CAN_COMMON_HLSLI) && !defined(__cplusplus)
#error Do not include this header directly in shader files. Only include this file via Common.hlsli.
#endif

struct GlobalShaderData
{
    float4x4 View;
    float4x4 Projection;
    float4x4 InvProjection;
    float4x4 ViewProjection;
    float4x4 InvViewProjection;
    
    float3 CameraPosition;
    float ViewWidth;
    
    float3 CameraDirection;
    float ViewHeight;
    
    uint NumDirectionalLights;
    float DeltaTime;
};

struct PerObjectData
{
    float4x4 World;
    float4x4 InvWorld;
    float4x4 WorldViewProjection;
};

struct Plane
{
    float3 Normal;
    float Distance;
};

struct Sphere
{
    float3 Center;
    float Radius;
};

struct Cone
{
    float3 Tip;
    float Height;
    float3 Direction;
    float Radius;
};

struct Frustum
{
    Plane Planes[4];
};

#ifndef __cplusplus
struct ComputeShaderInput
{
    uint3 GroupID : SV_GroupID;
    uint3 GroupThreadID : SV_GroupThreadID;
    uint3 DispatchThreadID : SV_DispatchThreadID;
    uint GroupIndex : SV_GroupIndex;
};
#endif

struct LightCullingDispacthParameters
{
    uint2 NumThreadGroups;
    uint2 NumThreads;
    uint NumLights;
    uint DepthBufferSrvIndex;
};

struct LightCullingInfo
{
    float3 Position;
    float Range;
    
    float3 Direction;
    float ConeRadius;
    
    uint Type;
    float3 _pad;
};

struct LightParameters
{
    float3 Position;
    float Intensity;
    
    float3 Direction;
    uint Type;
    
    float3 Color;
    float Range;
    
    float3 Attenuation;
    float CosUmbra;

    float CosPenumbra;
    float3 _pad;
};

struct DirectionalLightParameters
{
    float3 Direction;
    float Intensity;
    
    float3 Color;
    float _pad;
};

#ifdef __cplusplus
static_assert((sizeof(PerObjectData) % 16) == 0, "Make sure PerObjectData is formatted in 16-byte chunks without any implicit padding.");
static_assert((sizeof(LightParameters) % 16) == 0, "Make sure LightParameters is formatted in 16-byte chunks without any implicit padding.");
static_assert((sizeof(LightCullingInfo) % 16) == 0, "Make sure LightCullingInfo is formatted in 16-byte chunks without any implicit padding.");
static_assert((sizeof(DirectionalLightParameters) % 16) == 0, "Make sure DirectionalLightParameters is formatted in 16-byte chunks without any implicit padding.");
#endif