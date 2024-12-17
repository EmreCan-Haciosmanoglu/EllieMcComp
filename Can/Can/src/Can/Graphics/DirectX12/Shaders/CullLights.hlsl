#include "Common.hlsli"

static const uint MaxLightsPerGroup = 1024;

groupshared uint _minDepthVS;
groupshared uint _maxDepthVS;
groupshared uint _lightCount;
groupshared uint _lightIndexStartOffset;
groupshared uint _lightIndexList[MaxLightsPerGroup];

ConstantBuffer<GlobalShaderData> GlobalData : register(b0, space0);
ConstantBuffer<LightCullingDispacthParameters> ShaderParams : register(b1, space0);
StructuredBuffer<Frustum> Frustums : register(t0, space0);
StructuredBuffer<LightCullingInfo> Lights : register(t1, space0);

RWStructuredBuffer<uint> LightIndexCounter : register(u0, space0);
RWStructuredBuffer<uint2> LightGrid_Opaque : register(u1, space0);
RWStructuredBuffer<uint2> LightIndexList_Opaque : register(u3, space0);

[numthreads(TILE_SIZE, TILE_SIZE, 1)]
void CullLightsCS(ComputeShaderInput csIn)
{
    // INIT
    if (csIn.GroupIndex == 0) // Only the first thread in the group needs to initialize groupshared memory
    {
        _minDepthVS = 0x7f7fffff; // FLT_MAX as uint
        _maxDepthVS = 0x0;
        _lightCount = 0x0;
    }
    
    //reusable index variables.
    uint i = 0;
    uint index = 0;
    // MIN/MAX
    GroupMemoryBarrierWithGroupSync();

    const float depth = Texture2D(ResourceDescriptorHeap[ShaderParams.DepthBufferSrvIndex])[csIn.DispatchThreadID.xy].r;
    const float depthVS = ClipToView(float4(0.0f, 0.0f, depth, 1.0f), GlobalData.InvViewProjection).z;
    const uint z = asuint(-depthVS);
    
    if (depth != 0)
    {
        InterlockedMin(_minDepthVS, z);
        InterlockedMax(_maxDepthVS, z);
    }
    
    // CULLING
    GroupMemoryBarrierWithGroupSync();
    
    const uint gridIndex = csIn.GroupID.x + (csIn.GroupID.y * ShaderParams.NumThreadGroups.x);
    const Frustum frustum = Frustums[gridIndex];
    const float minDepthVS = -asfloat(_minDepthVS);
    const float maxDepthVS = -asfloat(_maxDepthVS);

    for (i = csIn.GroupIndex; i < ShaderParams.NumLights; i += TILE_SIZE * TILE_SIZE)
    {
        LightCullingInfo light = Lights[i];
        const float3 lightPositionVS = mul(GlobalData.View, float4(light.Position, 1.0f)).xyz;
        
        if (light.Type == LIGHT_TYPE_POINT_LIGHT)
        {
            const Sphere sphere = { lightPositionVS, light.Range };
            if (SphereInsideFrustum(sphere, frustum, minDepthVS, maxDepthVS))
            {
                InterlockedAdd(_lightCount, 1, index);
                if (index < MaxLightsPerGroup)
                {
                    _lightIndexList[index] = i;
                }
            }
        }
        else if (light.Type == LIGHT_TYPE_SPOT_LIGHT)
        {
            const float3 lightDirectionVS = mul(GlobalData.View, float4(light.Direction, 0.0f)).xyz;
            const Cone cone = { lightPositionVS, light.Range, lightDirectionVS, light.ConeRadius };
            if (ConeInsideFrustum(cone, frustum, minDepthVS, maxDepthVS))
            {
                InterlockedAdd(_lightCount, 1, index);
                if (index < MaxLightsPerGroup)
                {
                    _lightIndexList[index] = i;
                }
            }
        }
    }
    
    // LIGHT_GRID
    GroupMemoryBarrierWithGroupSync();
    
    const uint lightCount = min(_lightCount, MaxLightsPerGroup);
    
    if(csIn.GroupIndex == 0)
    {
        InterlockedAdd(LightIndexCounter[0], lightCount, _lightIndexStartOffset);
        LightGrid_Opaque[gridIndex] = uint2(_lightIndexStartOffset, lightCount);
    }
    
    // LIGHT_INDEX
    GroupMemoryBarrierWithGroupSync();

    for (i = csIn.GroupIndex; i < lightCount; i += TILE_SIZE * TILE_SIZE)
    {
        LightIndexList_Opaque[_lightIndexStartOffset + i] = _lightIndexList[i];
    }
}