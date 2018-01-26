#pragma once
#include "Engine/RHI/ComputeShader.hpp"
#include "Engine/RHI/Sampler.hpp"
#include "Engine/RHI/Texture2D.hpp"
#include "Engine/RHI/RHIDeviceContext.hpp"
#include "Engine/EngineCommon.hpp"
#include "Engine/Core/Rgba.hpp"
#include "Engine/RHI/ConstantBuffer.hpp"
#include "Engine/Math/Vector2.hpp"

#define COLOR_BUFFER_INDEX (4)
#define KEYBOARD_BUFFER_INDEX (5)
#define MANDELBROT_BUFFER_INDEX (6)

struct color_constant_t
{
    Vector4 m_rgba;
};

struct keyboard_constant_t
{
    float m_xShift;
    float m_yShift;
    float m_zoom;
    float m_tempTwo;
};

struct mandelbrot_constant_t
{
    float m_iterations;
    float m_power;
    float m_temp;
    float m_tempTwo;
};

class ComputeJob
{
public:
    ComputeJob();
    ComputeJob(ComputeShader *cs, RHIDeviceContext* dxContext);
    ~ComputeJob();
    void SetShader(ComputeShader *cs);
    void SetGridDimensions(uint x, uint y, uint z = 1);
    void SetSampler(uint index, Sampler* sampler);
    void SetRWTexture(Texture2D* texture);
    // set constant buffer
    // set index texture
    // bind to vectors
    
    void SetColor(Vector4 rgba);
    void SetInitialKeyboardValues(float xShift, float yShift, float zoom);
    void SetLeftOrRightKeyboardShift(float shiftValue);
    void SetDownOrUpKeyboardShift(float shiftValue);
    void SetKeyboardZoom(float zoomValue);
    void SetMandelBrot(float iterations, float power);
    void SetIterations(float iterations);
    void SetPower(float deltaPower);

    ComputeShader *m_shaderToRun;
    Texture2D *m_texture;
    uint3 m_gridSize;
    RHIDeviceContext* m_context;

    color_constant_t m_color_data;
    ConstantBuffer *m_color_cb;

    keyboard_constant_t m_keyboard_data;
    ConstantBuffer *m_keyboard_cb;

    mandelbrot_constant_t m_mandelbrot_data;
    ConstantBuffer *m_mandelbrot_cb;
};