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

bool PointInsidePlane(float3 p, Plane plane)
{
    return dot(plane.Normal, p) - plane.Distance < 0;
}

bool SphereInsidePlane(Sphere sphere, Plane plane)
{
    return dot(plane.Normal, sphere.Center) - plane.Distance < -sphere.Radius;
}

bool ConeInsidePlane(Cone cone, Plane plane)
{
    float3 m = cross(cross(plane.Normal, cone.Direction), cone.Direction);
    float3 Q = cone.Tip + cone.Direction * cone.Height - m * cone.Radius;

    return PointInsidePlane(cone.Tip, plane) && PointInsidePlane(Q, plane);
}

bool SphereInsideFrustum(Sphere sphere, Frustum frustum, float zNear, float zFar)
{
    return !(
        (sphere.Center.z - sphere.Radius > zNear || sphere.Center.z + sphere.Radius < zFar) ||
        SphereInsidePlane(sphere, frustum.Planes[0]) ||
        SphereInsidePlane(sphere, frustum.Planes[1]) ||
        SphereInsidePlane(sphere, frustum.Planes[2]) ||
        SphereInsidePlane(sphere, frustum.Planes[3])
    );
}

bool ConeInsideFrustum(Cone cone, Frustum frustum, float zNear, float zFar)
{
    Plane nearPlane = { float3(0, 0, -1), -zNear };
    Plane farPlane = { float3(0, 0, 1), zFar };

    return !(
        ConeInsidePlane(cone, nearPlane) || 
        ConeInsidePlane(cone, farPlane) ||
        ConeInsidePlane(cone, frustum.Planes[0]) ||
        ConeInsidePlane(cone, frustum.Planes[1]) ||
        ConeInsidePlane(cone, frustum.Planes[2]) ||
        ConeInsidePlane(cone, frustum.Planes[3])
    );
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