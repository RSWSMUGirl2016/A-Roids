struct vertex_in_t
{
	float3 position : POSITION;
    float4 tint : TINT;
    float2 uv : UV;
};

struct vertex_to_fragment_t
{
	float4 position : SV_Position;
    float4 tint : TINT;
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

float4 GetAsFloats(float r, float g, float b, float a)
{
    float inverse255 = 1.f / 255.f;
    float out_normalizedRed = (float)r*inverse255;
    float out_normalizedGreen = (float)g*inverse255;
    float out_normalizedBlue = (float)b*inverse255;
    float out_normalizedAlpha = (float)a*inverse255;
    return float4(out_normalizedRed, out_normalizedGreen, out_normalizedBlue, out_normalizedAlpha);
}

// NOP VERTEX FUNCTION
vertex_to_fragment_t VertexFunction(vertex_in_t vertex)
{
    vertex_to_fragment_t out_data = (vertex_to_fragment_t)0;
    
    float4 model_position = float4(vertex.position, 1.0f);
    float4 world_position = mul(model_position, MODEL);
    float4 view_position = mul(world_position, VIEW);
    float4 clip_position = mul(view_position, PROJECTION);

    out_data.position = clip_position;
    out_data.uv = vertex.uv;
    out_data.tint = vertex.tint;
    return out_data;
}

// COLOR (PIXEL/FRAGMENT) FUNCTION
float4 FragmentFunction( vertex_to_fragment_t data ) : SV_Target0
{
    float4 color = tTexture.Sample(sSampler, data.uv);   
    float4 t = color * data.tint;
    return t;
}