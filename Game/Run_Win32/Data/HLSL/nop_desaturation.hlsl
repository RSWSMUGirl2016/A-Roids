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

    float t = frac(TIME); // returns decimal part of number
    
    //Desaturate the Image, aka grayscale
    float3 desaturate = float3(0.3, 0.59, 0.11);
    float g = dot(desaturate, color.xyz);
    float4 grayscale = float4(g, g, g, 1);

    //Lerps between color and grayscale over time
    float4 final_color = lerp(color, grayscale, t);

    return grayscale;
}