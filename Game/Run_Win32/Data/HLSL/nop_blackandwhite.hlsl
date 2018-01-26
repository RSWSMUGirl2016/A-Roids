struct vertex_in_t
{
    float3 position : POSITION;
    float tint : TINT;
    float2 uv : UV;
};

struct vertex_to_fragment_t
{
    float4 position : SV_Position;
    float2 uv : UV;
};

cbuffer matrix_cb : register(b0)
{
    float4x4 MODEL;
    float4x4 VIEW;
    float4x4 PROJECTION;
};

cbuffer time_cb : register(b1)
{
    float TIME;

    float3 PADDING;
};

Texture2D <float4> tTexture : register(t0);
SamplerState sSampler : register(s0);

// NOP VERTEX FUNCTION
vertex_to_fragment_t VertexFunction(vertex_in_t vertex)
{
    vertex_to_fragment_t out_data = (vertex_to_fragment_t)0;
    float4 transformed_position = mul(float4(vertex.position, 1.0f), PROJECTION);

    out_data.position = transformed_position;
    out_data.uv = vertex.uv;
    return out_data;
}

// COLOR (PIXEL/FRAGMENT) FUNCTION
float4 FragmentFunction( vertex_to_fragment_t data ) : SV_Target0
{
    float4 color = tTexture.Sample(sSampler, data.uv);
    
    color.xyz = (color.x + color.y + color.z) / 3.0f;
    if (color.x<0.2 || color.x>0.9) color.x = 0; else color.x = 1.0f;
    if (color.y<0.2 || color.y>0.9) color.y = 0; else color.y = 1.0f;
    if (color.z<0.2 || color.z>0.9) color.z = 0; else color.z = 1.0f;

    return color;
}