#include "Engine/RHI/ComputeJob.hpp"
#include "Engine/RHI/RHITypes.hpp"

//*******************************************************************
ComputeJob::ComputeJob()
    : m_shaderToRun(nullptr)
{
    MemZero(&m_color_data);
    m_color_cb = new ConstantBuffer(m_context->m_device, &m_color_data, sizeof(m_color_data));
    m_context->SetConstantBuffer(COLOR_BUFFER_INDEX, m_color_cb);

    MemZero(&m_keyboard_data);
    m_keyboard_cb = new ConstantBuffer(m_context->m_device, &m_keyboard_data, sizeof(m_keyboard_data));
    m_context->SetConstantBuffer(KEYBOARD_BUFFER_INDEX, m_keyboard_cb);

    MemZero(&m_mandelbrot_data);
    m_mandelbrot_cb = new ConstantBuffer(m_context->m_device, &m_mandelbrot_data, sizeof(m_mandelbrot_data));
    m_context->SetConstantBuffer(MANDELBROT_BUFFER_INDEX, m_mandelbrot_cb);
}

//*******************************************************************
ComputeJob::ComputeJob(ComputeShader *cs, RHIDeviceContext* dxContext)
    : m_shaderToRun(cs)
{
    MemZero(&m_color_data);
    m_context = dxContext;
    m_color_cb = new ConstantBuffer(m_context->m_device, &m_color_data, sizeof(m_color_data));
    m_context->SetConstantBuffer(COLOR_BUFFER_INDEX, m_color_cb);

    MemZero(&m_keyboard_data);
    m_keyboard_cb = new ConstantBuffer(m_context->m_device, &m_keyboard_data, sizeof(m_keyboard_data));
    m_context->SetConstantBuffer(KEYBOARD_BUFFER_INDEX, m_keyboard_cb);

    MemZero(&m_mandelbrot_data);
    m_mandelbrot_cb = new ConstantBuffer(m_context->m_device, &m_mandelbrot_data, sizeof(m_mandelbrot_data));
    m_context->SetConstantBuffer(MANDELBROT_BUFFER_INDEX, m_mandelbrot_cb);
}

//*******************************************************************
ComputeJob::~ComputeJob()
{
    if (m_shaderToRun != nullptr)
    {
        delete m_shaderToRun;
        m_shaderToRun = nullptr;
    }
}

//*******************************************************************
void ComputeJob::SetShader(ComputeShader *cs)
{
    m_shaderToRun = cs;
    m_context->SetComputeShader(m_shaderToRun);
}

//*******************************************************************
void ComputeJob::SetGridDimensions(uint x, uint y, uint z /*= 1*/)
{
    m_gridSize.x = x;
    m_gridSize.y = y;
    m_gridSize.z = z;
}

//*******************************************************************
void ComputeJob::SetSampler(uint index, Sampler* sampler)
{
    m_context->SetSampler(index, sampler);
}

//*******************************************************************
void ComputeJob::SetRWTexture(Texture2D* texture)
{
    m_texture = texture;
    m_context->SetTexture(0, texture);
}

//*******************************************************************
void ComputeJob::SetColor(Vector4 rgba)
{
    m_color_data.m_rgba = rgba;
    m_color_cb->Update(m_context, &m_color_data);
}

//*******************************************************************
void ComputeJob::SetInitialKeyboardValues(float xShift, float yShift, float zoom)
{
    m_keyboard_data.m_xShift = xShift;
    m_keyboard_data.m_yShift = yShift;
    m_keyboard_data.m_zoom = zoom;
    m_keyboard_cb->Update(m_context, &m_keyboard_data);
}

//*******************************************************************
void ComputeJob::SetLeftOrRightKeyboardShift(float shiftValue)
{
    m_keyboard_data.m_xShift = m_keyboard_data.m_xShift + shiftValue;
    m_keyboard_cb->Update(m_context, &m_keyboard_data);
}

//*******************************************************************
void ComputeJob::SetDownOrUpKeyboardShift(float shiftValue)
{
    m_keyboard_data.m_yShift = m_keyboard_data.m_yShift + shiftValue;
    m_keyboard_cb->Update(m_context, &m_keyboard_data);
}

//*******************************************************************
void ComputeJob::SetKeyboardZoom(float zoomValue)
{
    m_keyboard_data.m_zoom = m_keyboard_data.m_zoom + zoomValue;
    m_keyboard_cb->Update(m_context, &m_keyboard_data);
}

//*******************************************************************
void ComputeJob::SetMandelBrot(float iterations, float power)
{
    m_mandelbrot_data.m_iterations = iterations;
    m_mandelbrot_data.m_power = power;
    m_mandelbrot_cb->Update(m_context, &m_mandelbrot_data);
}

//*******************************************************************
void ComputeJob::SetIterations(float iterations)
{
    m_mandelbrot_data.m_iterations = iterations;
    m_mandelbrot_cb->Update(m_context, &m_mandelbrot_data);
}

//*******************************************************************
void ComputeJob::SetPower(float deltaPower)
{
    m_mandelbrot_data.m_power = m_mandelbrot_data.m_power + deltaPower;
    m_mandelbrot_cb->Update(m_context, &m_mandelbrot_data);
}
