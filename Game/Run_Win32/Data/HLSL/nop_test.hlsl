struct vertex_in_t
{
    float3 position : POSITION;
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
float4 FragmentFunction(vertex_to_fragment_t data) : SV_Target0
{
    float4 color = tTexture.Sample(sSampler, data.uv);
    
    float x = (data.uv.x - 0.5f) * (128.0f / 72.0f) / (0.575f);
    float y = (data.uv.y - 0.5f) * (128.0f / 72.0f);

    //use x and y, calculate the r
    float dist = sqrt((x * x) + (y * y));
    
    if (dist > 0.5)
    {
        color.x = color.x*sin(data.uv.y * 1000 + TIME * 10);
        color.y = color.y*sin(data.uv.y * 1000 + TIME * 10);
        color.z = color.z*sin(data.uv.y * 1000 + TIME * 10);
        return float4(color.x, color.y, color.z, 1);        
    }    
    else if ((dist > 0.49f) && (dist < 0.5f))
    {
        return float4(1.0f, 0.0f, 0.0f, 1.0f);
    }
    else
    {
        //scope in here
        //bigger the number the smaller the image gets
        float2 scale = float2(0.5f, 0.5f);
        float2 scaleCenter = float2(0.5f, 0.5f);
        data.uv = (data.uv - scaleCenter) * scale + scaleCenter;
        return tTexture.Sample(sSampler, data.uv);
    }
        

    return color;
}