//--------------------------------------------------------------------------------------
// Input/Output Structures
//--------------------------------------------------------------------------------------
// https://msdn.microsoft.com/en-us/library/windows/desktop/ff471566(v=vs.85).asxyp
struct compute_input_t
{
    uint3 group_id : SV_GroupID; // What is my index within the context of a the dispatch call
    uint group_idx : SV_GroupIndex; // Flattend version of the top 
                                             // (z * (width * height) + y * width + x)
    uint3 group_thread_id : SV_GroupThreadID; // My thread within the context of the group
    uint3 dispatch_id : SV_DispatchThreadID; // My thread within context of the entire dispatch 
                                             // [5x5x1, with threads 5,5,1]  If I was in 2, 2, 0 group 
                                             // and 3, 3, 0 thread, I would be in 13 13 0 thread id
};

//--------------------------------------------------------------------------------------
// Textures and Samplers
//--------------------------------------------------------------------------------------

cbuffer CLEAR_CONSTANTS : register(b4)
{
    float4 CLEAR_COLOR;
};

cbuffer KEYBOARD_CONSTANTS : register(b5)
{
    float XSHIFT;
    float YSHIFT;
    float ZOOM;
    float KEY2;
}

cbuffer MANDELBROT_CONSTANTS : register(b6)
{
    float ITERATIONS;
    float POWER;
    float TEMP1;
    float TEMP2;
}

RWTexture2D<float4> tOutput : register(u0);

uint CheckMandlebrotSet(float x0, float y0, uint MAX_ITERATIONS)
{
    float x = 0.0f;
    float y = 0.0f;

    uint iteration = 0;
    while ((((x * x) + (y * y)) < 4.0f) && (iteration < MAX_ITERATIONS))
    {
        //float xtemp = x * x - y * y + x0;
        //y = 2.0f * x * y + y0;
        //x = xtemp;

        float absoluteZ = sqrt((x * x) + (y * y));
        float theta = 0.0f;
          
        if (absoluteZ != 0)
        {
            theta = atan2(y, x);
        }

        float cumAbsZ = absoluteZ;
        float cumTheta = POWER * theta;

        for (int index = 1; index < POWER; index++)
        {
            cumAbsZ *= absoluteZ;
        }

        x = cumAbsZ * cos(cumTheta) + x0;
        y = cumAbsZ * sin(cumTheta) + y0;
        
        iteration++;
    }
        
    return iteration;
}

float RangeMapFloat(float inValue, float inStart, float inEnd, float outStart, float outEnd)
{
    if (inEnd == inStart)
        return 0.5f * (outStart + outEnd);

    float fractionInInRange = (inValue - inStart) / (inEnd - inStart);
    float outValue = outStart + (fractionInInRange * (outEnd - outStart));
    return outValue;
}

float Lerp(float v0, float v1, float t)
{
    return (1 - t) * v0 + t * v1;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
[numthreads(16, 16, 1)]
void Main(compute_input_t input)
{
    uint width;
    uint height;
    tOutput.GetDimensions(width, height);
    
    //float x = RangeMapFloat((float) input.dispatch_id.x, 0.f, (float)width, -2.5f, 1.0f);
    //float y = RangeMapFloat((float) input.dispatch_id.y, 0.f, (float)height, -2.5f, 1.0f);

    //x += input.dispatch_id.x;
    //y += input.dispatch_id.y;

    float x = ((float) input.dispatch_id.x / (float) width) * (3.5f * ZOOM) /*division for zoom*/ - XSHIFT /*plus and minus by key press*/;
    float y = ((float) input.dispatch_id.y / (float) height) * (2.0f * ZOOM) - YSHIFT;

    float colorInt = CheckMandlebrotSet(x, y, (uint)ITERATIONS);

    
    //Smooth Stepping
    //if (colorInt < ITERATIONS)
    //{
    //    float logZn = log((x * x) + (y * y)) / 2.0f;
    //    float nu = log(logZn / log(2.0f)) / log(2.0f);
    //    colorInt = colorInt + 1 - nu;
    //}
    
    //float colorIntOne = colorInt;
    //float colorIntTwo = (colorInt + 1.0f);
    //colorInt = Lerp(colorIntOne, colorIntTwo, colorInt % 1.0f);
    
    float t = colorInt / ITERATIONS;
    float r = 15.0f * (1.0f - t) * (1.0f - t) * t * t;
    float g = 8.5f * (1.0f - t) * (1.0f - t) * (1.0f - t) * t;
    float b = 9.0f * (1.0f - t) * t * t * t;

    float4 rgba = float4(r, g, b, 1.0f);
    tOutput[input.dispatch_id.xy] = rgba;
}
