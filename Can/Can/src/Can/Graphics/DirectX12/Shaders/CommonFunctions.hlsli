#if !defined(CAN_COMMON_HLSLI) && !defined(__cplusplus)
#error Do not include this header directly in shader files. Only include this file via Common.hlsli.
#endif

Plane ComputePlane(float3 p0, float3 p1, float3 p2)
{
    Plane plane;
    
    const float3 v0 = p1 - p0;
    const float3 v2 = p2 - p0;
    
    plane.Normal = normalize(cross(v0, v2));
    plane.Distance = dot(plane.Normal, p0);
    
    return plane;
}

float4 ClipToView(float4 clip, float4x4 inverseProjection)
{
    float4 view = mul(inverseProjection, clip);
    view /= view.w;
    return view;
}

float4 ScreenToView(float4 screen, float2 invViewDimensions, float4x4 inverseProjection)
{
    float2 texCoord = screen.xy * invViewDimensions;
    
    float4 clip = float4(float2(texCoord.x, 1.0f - texCoord.y) * 2.0f - 1.0f, screen.z, screen.w);
    
    return ClipToView(clip, inverseProjection);

}