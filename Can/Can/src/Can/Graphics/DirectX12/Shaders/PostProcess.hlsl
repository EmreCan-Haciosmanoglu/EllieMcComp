#include "Common.hlsli"

struct ShaderContants
{
    uint GPassMainBufferIndex;
};

#define FRUSTUM_VISUALIZATION 1
#define INDEX_VISUALIZATION 0

ConstantBuffer<GlobalShaderData> GlobalData : register(b0, space0);
ConstantBuffer<ShaderContants> ShaderParams : register(b1, space0);

// TODO: Temporary for visualization
#if FRUSTUM_VISUALIZATION | INDEX_VISUALIZATION
#define TILE_SIZE 16
uint GetGridIndex(float2 posXY, float viewWidth)
{
    const uint2 pos = uint2(posXY);
    const uint tileX = ceil(viewWidth / TILE_SIZE);
    return (pos.x / TILE_SIZE) + (tileX * (pos.y / TILE_SIZE));
}
StructuredBuffer<Frustum> Frustums : register(t0, space0);
#endif

float4 PostProcessPS(in noperspective float4 Position : SV_Position,
                     in noperspective float2 UV : TEXCOORD) : SV_Target0
{
#if FRUSTUM_VISUALIZATION
    
    const float w = GlobalData.ViewWidth;
    const uint gridIndex = GetGridIndex(Position.xy, w);
    
    const Frustum f = Frustums[gridIndex];
    const uint halfTile = TILE_SIZE / 2;
    float3 color = abs(f.Planes[1].Normal);

    if (GetGridIndex(float2(Position.x + halfTile, Position.y), w) == gridIndex && GetGridIndex(float2(Position.x, Position.y + halfTile), w) == gridIndex)
    {
        color = abs(f.Planes[0].Normal);
    }
    else if (GetGridIndex(float2(Position.x + halfTile, Position.y), w) != gridIndex && GetGridIndex(float2(Position.x, Position.y + halfTile), w) == gridIndex)
    {
        color = abs(f.Planes[2].Normal);
    }
    else if (GetGridIndex(float2(Position.x + halfTile, Position.y), w) == gridIndex && GetGridIndex(float2(Position.x, Position.y + halfTile), w) != gridIndex)
    {
        color = abs(f.Planes[3].Normal);
    }

    Texture2D gpassMain = ResourceDescriptorHeap[ShaderParams.GPassMainBufferIndex];
    color = lerp(gpassMain[Position.xy].xyz, color, 0.25f);
    return float4(color, 1.0f);
    
#elif INDEX_VISUALIZATION
    
    const uint2 pos = uint2(Position.xy);
    const uint tileX = ceil(GlobalData.ViewWidth / TILE_SIZE);
    const uint2 idx = pos / (uint2)TILE_SIZE;
    
    float c = (idx.x + tileX * idx.y) * 0.00001f;
    
    if (idx.x % 2 == 0)
    {
        c += 0.1f;
    }
    if (idx.y % 2 == 0)
    {
        c += 0.1f;
    }
    
    return float4((float3) c, 1.0f);
    
#else // SCENE
    
    Texture2D gpassMain = ResourceDescriptorHeap[ShaderParams.GPassMainBufferIndex];
    return float4(gpassMain[Position.xy].xyz, 1.0f);
    
#endif
    }