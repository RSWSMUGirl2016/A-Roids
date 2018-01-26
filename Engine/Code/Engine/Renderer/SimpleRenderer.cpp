#include "Engine/Renderer/SimpleRenderer.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vertex3.hpp"
#include "Engine/RHI/VertexBuffer.hpp"
#include "Engine/RHI/Sampler.hpp"
#include "Engine/RHI/RHITypes.hpp"
#include "Engine/Math/MathUtilities.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/EngineCommon.hpp"
#include "Engine/Profiler/Profiler.hpp"
#include "Engine/Math/MathUtilities.hpp"
#include "Game/GameCommon.hpp"

//*******************************************************************
SimpleRenderer::SimpleRenderer()
    : m_context(nullptr)
    , m_device(nullptr)
    , m_output(nullptr)
    , m_pointSampler(nullptr)
    , m_linearSampler(nullptr)
    , m_currentTarget(nullptr)
    , m_defaultColorTarget(nullptr)
    , m_currentColorTarget(nullptr)
    , m_defaultDepthStencil(nullptr)
    , m_currentDepthStencil(nullptr)
    , m_whiteSample(nullptr)
    , m_diffuseTexture(nullptr)
    , m_spaceShipDiffuse(nullptr)
    , m_normalTexture(nullptr)
    , m_spaceShipNormal(nullptr)
    , flatNormalTexture(nullptr)
    , texSample(nullptr)
    , shaderProgram(nullptr)
    , m_colorShader(nullptr)
    , m_unlitShader(nullptr)
    , m_lightShader(nullptr)
    , m_skinningShader(nullptr)
    , m_skinnedBoneWeightsShader(nullptr)
    , timeConstants(nullptr)
    , m_defaultDepthStencilState(nullptr)
    , m_rasterState(nullptr)
    , m_tempVbo(nullptr)
    , m_tempIbo(nullptr)
    , m_currentBlendState(nullptr)
    , time_cb(nullptr)
    , matrix_cb(nullptr)
    , light_cb(nullptr)
    , m_font(nullptr)
    , m_fontTexture(nullptr)
    , m_meshBuilderCube(nullptr)
    , m_skeletonCube(nullptr)
    , m_motionCube(nullptr)
    , m_uavSample(nullptr)
{
}

//*******************************************************************
SimpleRenderer::~SimpleRenderer()
{
    //m_context->ClearState();

    if (m_fontTexture != nullptr)
    {
        delete m_fontTexture;
        m_fontTexture = nullptr;
    }

    if (m_font != nullptr)
    {
        delete m_font;
        m_font = nullptr;
    }

    if (light_cb != nullptr)
    {
        delete light_cb;
        light_cb = nullptr;
    }

    if (matrix_cb != nullptr)
    {
        delete matrix_cb;
        matrix_cb = nullptr;
    }

    if (time_cb != nullptr)
    {
        delete time_cb;
        time_cb = nullptr;
    }

    if (m_currentBlendState != nullptr)
    {
        delete m_currentBlendState;
        m_currentBlendState = nullptr;
    }

    if (m_tempIbo != nullptr)
    {
        delete m_tempIbo;
        m_tempIbo = nullptr;
    }

    if (m_tempVbo != nullptr)
    {
        delete m_tempVbo;
        m_tempVbo = nullptr;
    }

    if (m_rasterState != nullptr)
    {
        delete m_rasterState;
        m_rasterState = nullptr;
    }

    if (m_defaultDepthStencilState != nullptr)
    {
        delete m_defaultDepthStencilState;
        m_defaultDepthStencilState = nullptr;
    }

    if (timeConstants != nullptr)
    {
        delete timeConstants;
        timeConstants = nullptr;
    }

    for (int index = 0; index < (int)shaderPrograms.size(); index++)
    {
        if (shaderPrograms[index] == nullptr)
        {
            delete shaderPrograms[index];
            shaderPrograms[index] = nullptr;
        }
    }
    shaderPrograms.clear();

    if (m_lightShader != nullptr)
    {
        delete m_lightShader;
        m_lightShader = nullptr;
    }

    if (m_unlitShader != nullptr)
    {
        delete m_unlitShader;
        m_unlitShader = nullptr;
    }

    if (m_colorShader != nullptr)
    {
        delete m_colorShader;
        m_colorShader = nullptr;
    }

    if (shaderProgram != nullptr)
    {
        delete shaderProgram;
        shaderProgram = nullptr;
    }

    for (int index = 0; index < (int)texSamples.size(); index++)
    {
        if (texSamples[index] == nullptr)
        {
            delete texSamples[index];
            texSamples[index] = nullptr;
        }
    }
    texSamples.clear();

    if (texSample != nullptr)
    {
        delete texSample;
        texSample = nullptr;
    }

    if (flatNormalTexture != nullptr)
    {
        delete flatNormalTexture;
        flatNormalTexture = nullptr;
    }

    if (m_spaceShipNormal != nullptr)
    {
        delete m_spaceShipNormal;
        m_spaceShipNormal = nullptr;
    }

    if (m_normalTexture != nullptr)
    {
        delete m_normalTexture;
        m_normalTexture = nullptr;
    }

    if (m_spaceShipDiffuse != nullptr)
    {
        delete m_spaceShipDiffuse;
        m_spaceShipDiffuse = nullptr;
    }

    if (m_diffuseTexture != nullptr)
    {
        delete m_diffuseTexture;
        m_diffuseTexture = nullptr;
    }

    if (m_whiteSample != nullptr)
    {
        delete m_whiteSample;
        m_whiteSample = nullptr;
    }

    /*if (m_currentDepthStencil != nullptr)
    {
        delete m_currentDepthStencil;
        m_currentDepthStencil = nullptr;
    }*/

    if (m_defaultDepthStencil != nullptr)
    {
        delete m_defaultDepthStencil;
        m_defaultDepthStencil = nullptr;
    }

    /*if (m_currentColorTarget != nullptr)
    {
        delete m_currentColorTarget;
        m_currentColorTarget = nullptr;
    }*/

    if (m_defaultColorTarget != nullptr)
    {
        delete m_defaultColorTarget;
        m_defaultColorTarget = nullptr;
    }

    if (m_currentTarget != nullptr)
    {
        delete m_currentTarget;
        m_currentTarget = nullptr;
    }

    if (m_linearSampler != nullptr)
    {
        delete m_linearSampler;
        m_linearSampler = nullptr;
    }

    if (m_pointSampler != nullptr)
    {
        delete m_pointSampler;
        m_pointSampler = nullptr;
    }

    /*if (m_output != nullptr)
    {
        delete m_output;
        m_output = nullptr;
    }*/

    if (m_device != nullptr)
    {
        delete m_device;
        m_device = nullptr;
    }

    if (m_context != nullptr)
    {
        delete m_context;
        m_context = nullptr;
    }
}

//*******************************************************************
void SimpleRenderer::SetUp(WindowMessageHandler windowMessageHandler)
{
    RHIInstance *ri = RHIInstance::GetInstance();

    m_output = ri->CreateOutput(windowMessageHandler);
    m_device = m_output->m_device;
    m_context = m_device->m_immediateContext;

    m_rasterState = new RasterState(m_device);
    m_context->SetRasterState(m_rasterState);

    matrix_data.eyePosition = Vector4(0.f, 0.f, 0.f, 1.f);
    MemZero(&time_data);

    matrix_cb = new ConstantBuffer(m_device, &matrix_data, sizeof(matrix_data));
    time_cb = new ConstantBuffer(m_device, &time_data, sizeof(time_data));
    light_cb = new ConstantBuffer(m_device, &light_data, sizeof(light_data));

    m_context->SetConstantBuffer(MATRIX_BUFFER_INDEX, matrix_cb);
    m_context->SetConstantBuffer(TIME_BUFFER_INDEX, time_cb);
    m_context->SetConstantBuffer(LIGHT_BUFFER_INDEX, light_cb);


    MemZero(&m_mouse_position_data);
    m_mouse_position_cb = new ConstantBuffer(m_context->m_device, &m_mouse_position_data, sizeof(m_mouse_position_data));
    m_context->SetConstantBuffer(MOUSE_BUFFER_INDEX, m_mouse_position_cb);

    //m_structureBuffer = new StructuredBuffer(m_device, /*data*/, /*sizepf(data)*/, /*object_count*/);

    m_pointSampler = new Sampler(m_device, FILTER_POINT, FILTER_POINT);
    m_linearSampler = new Sampler(m_device, FILTER_LINEAR, FILTER_LINEAR);

    m_defaultColorTarget = m_output->GetRenderTarget();

    m_tempVbo = new VertexBuffer(m_device, nullptr, 32, BUFFERUSAGE_DYNAMIC);
    m_vertexCount = 0;
    m_tempIbo = new IndexBuffer(m_device, nullptr, 32, BUFFERUSAGE_DYNAMIC);

    m_font = new Font();
    m_font->LoadFromFile("Data/Fonts/arial32_0.fnt");
    m_fontTexture = new Texture2D(m_device, m_font->m_textureFilepath.c_str());

    //int windowsResolutionX;
    //int windowsResolutionY;
    //m_output->m_window->m_gameConfig.GetWindowResolutionX(windowsResolutionX);
    //m_output->m_window->m_gameConfig.GetWindowResolutionY(windowsResolutionY);
    unsigned int width = (unsigned int)WINDOW_WIDTH;
    unsigned int height = (unsigned int)WINDOW_HEIGHT;
    m_defaultDepthStencil = new Texture2D(m_device, width, height, IMAGEFORMAT_D24S8);
    m_currentDepthStencil = nullptr;

    m_depthStencilDesc.depth_test_enabled = false;
    m_depthStencilDesc.depth_writing_enabled = true;
    m_defaultDepthStencilState = new DepthStencilState(m_device, m_depthStencilDesc);
    m_context->SetDepthStencilState(m_defaultDepthStencilState);

    LoadTextures();
    LoadShaders();

    SetRenderTarget(nullptr, nullptr);
    SetSampler(nullptr);
    SetTexture(nullptr);
}

//*******************************************************************
void SimpleRenderer::SetTitle(char const* title)
{
    WCHAR wTitle[1024];
    MultiByteToWideChar(GetACP(), 0, title, -1, wTitle, sizeof(wTitle) / sizeof(wTitle[0]));
    m_output->m_window->SetWindowTitle(wTitle);
}

//*******************************************************************
void SimpleRenderer::Destroy()
{
}

//*******************************************************************
void SimpleRenderer::SetViewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{
    m_context->SetViewport(x, y, w, h);
}

//*******************************************************************
void SimpleRenderer::Update(float deltaSeconds)
{
    ProcessMessages();

    time_data.gameDeltaTime = deltaSeconds;
    time_data.systemDeltaTime = deltaSeconds;
    time_data.gameTime += deltaSeconds;
    time_data.systemTime += deltaSeconds;

    time_cb->Update(m_context, &time_data);
}

//*******************************************************************
void SimpleRenderer::ProcessMessages()
{
    m_output->m_window->ProcessMessages();
}

//*******************************************************************
bool SimpleRenderer::IsClosed() const
{
    return m_output->m_window->IsClosed();
}

//*******************************************************************
void SimpleRenderer::LoadTextures()
{
    /*std::vector<std::string> images;
    m_output->m_window->m_gameConfig.GetImages(images);
    for (int imagesIndex = 0; imagesIndex < 1; imagesIndex++)
    {
        texSamples.push_back(new Texture2D(m_device, images[imagesIndex].c_str()));
    }
    texSample = texSamples[0];*/

    std::string imageName = "Data/Images/White.png";
    m_whiteSample = new Texture2D(m_device, imageName.c_str());

    m_uavSample = new Texture2D(m_device, (unsigned int)ORHTO_WIDTH, (unsigned int)ORHTO_HEIGHT, IMAGEFORMAT_RGBA8, true);

    //std::string diffuseName = "Data/Images/Drotek_DFS.png";
    //m_diffuseTexture = new Texture2D(m_device, diffuseName.c_str());
    //std::string normalName = "Data/Images/Drotek_NRM.png";
    ////std::string normalName = "Data/Images/test_normal.jpg";
    //m_normalTexture = new Texture2D(m_device, normalName.c_str());
    //std::string spaceShipeDiffuseName = "Data/Images/SciFi_Fighter-MK6-diffuse.jpg";
    //m_spaceShipDiffuse = new Texture2D(m_device, spaceShipeDiffuseName.c_str());
    //std::string spaceShipNormalName = "Data/Images/SciFi_Fighter-MK6-normal.jpg";
    //m_spaceShipNormal = new Texture2D(m_device, spaceShipNormalName.c_str());


    //Image image;
    //image.CreateClear(1280, 720, Rgba::WHITE);
    //whiteSample = new Texture2D(m_device, image);
    //image.CreateClear(1, 1, Rgba(127, 127, 255, 255));
    //flatNormalTexture = new Texture2D(m_device, image);    
}

//*******************************************************************
void SimpleRenderer::LoadShaders()
{
    /*std::vector<std::string> shaders;
    m_output->m_window->m_gameConfig.GetShaders(shaders);
    for (int shadersIndex = 0; shadersIndex < (int)shaders.size(); shadersIndex++)
    {
        shaderPrograms.push_back(m_device->CreateShaderFromHlslFile(shaders[shadersIndex].c_str()));
    }
    shaderProgram = shaderPrograms[0];*/

    std::string colorShaderName = "Data/HLSL/color.hlsl";
    std::string unlitShaderName = "Data/HLSL/unlit.hlsl";
    std::string lightShaderName = "Data/HLSL/light.hlsl";
    std::string skinningShaderName = "Data/HLSL/skinwieghts.hlsl";
    std::string skinnedBoneWeightsShaderName = "Data/HLSL/skinned_boneweight.hlsl";

    m_colorShader = m_device->CreateShaderFromHlslFile(colorShaderName.c_str());
    m_unlitShader = m_device->CreateShaderFromHlslFile(unlitShaderName.c_str());
    m_lightShader = m_device->CreateShaderFromHlslFile(lightShaderName.c_str());
    m_skinningShader = m_device->CreateShaderFromHlslFile(skinningShaderName.c_str());
    m_skinnedBoneWeightsShader = m_device->CreateShaderFromHlslFile(skinnedBoneWeightsShaderName.c_str());

    //std::string mandelShaderName = "Data/HLSL/mandelbrot.hlsl";
    //ComputeShader *mandelShader = m_device->CreateComputeShaderFromHlslFile(mandelShaderName.c_str());
    //s_computeShaders[mandelShaderName] = mandelShader;
}

//*******************************************************************
SpriteSheet SimpleRenderer::CreateSpriteSheet(const std::string& imageFilePath, int tilesWide, int tilesHigh)
{
    Texture2D* texture = new Texture2D(m_device, imageFilePath.c_str());
    return SpriteSheet(texture, tilesWide, tilesHigh);
}

//*******************************************************************
SpriteSheet* SimpleRenderer::CreateSpriteSheetPointer(const std::string& imageFilePath, int tilesWide, int tilesHigh)
{
    Texture2D* texture = new Texture2D(m_device, imageFilePath.c_str());
    return new SpriteSheet(texture, tilesWide, tilesHigh);
}

//*******************************************************************
BitmapFont* SimpleRenderer::GetFont(const std::string& bitmapFontName)
{
    if (m_loadedBitmapFonts.empty())
        return nullptr;

    BitmapFont* bitmapFont = m_loadedBitmapFonts[bitmapFontName];
    if (bitmapFont != nullptr)
        return bitmapFont;

    return nullptr;
}

//*******************************************************************
BitmapFont* SimpleRenderer::CreateOrGetFont(const std::string& bitmapFontName)
{
    BitmapFont* bitmapFont = GetFont(bitmapFontName);
    if (bitmapFont)
        return bitmapFont;

    bitmapFont = new BitmapFont(bitmapFontName);
    m_loadedBitmapFonts[bitmapFontName] = bitmapFont;
    return bitmapFont;
}

//*******************************************************************
void SimpleRenderer::DrawText2D(const Vector2& startBottomLeft, const std::string& asciiText, float cellHeight, const Rgba& tint /*= Rgba::WHITE*/, const BitmapFont* font /*= nullptr*/)
{
    EnableBlend(BLEND_SRC_ALPHA, BLEND_INV_SRC_ALPHA);
    SetTexture(font->GetTexture2D());

    for (int asciiTextIndex = 0; asciiTextIndex < (int)asciiText.length(); asciiTextIndex++)
    {
        Vector2 mins = Vector2(startBottomLeft.x + (asciiTextIndex * cellHeight), startBottomLeft.y);
        Vector2 maxs = Vector2(mins.x + cellHeight, mins.y + cellHeight);
        AABB2D characterBounds = AABB2D(mins, maxs);

        char character = asciiText.at(asciiTextIndex);
        AABB2D characterTexCoords = font->GetTexCoordsForGlyph(character);
        AABB2D properCharacterTextCoords = characterTexCoords;
        properCharacterTextCoords.m_mins = Vector2(characterTexCoords.m_maxs.y, characterTexCoords.m_maxs.x);
        properCharacterTextCoords.m_maxs = Vector2(characterTexCoords.m_mins.y, characterTexCoords.m_mins.x);

        DrawQuad(mins, maxs, characterTexCoords.m_mins, characterTexCoords.m_maxs, tint);
    }

    DisableBlend();
}

//*******************************************************************
void SimpleRenderer::SetModelMatrix(Matrix4 const &model)
{
    //matrix_data.model = model;
    matrix_data.model = model.get_transpose();
    matrix_cb->Update(m_context, &matrix_data);
}

//*******************************************************************
void SimpleRenderer::SetViewMatrix(Matrix4 const &view)
{
    matrix_data.view = view.get_transpose();
    matrix_cb->Update(m_context, &matrix_data);
}

//*******************************************************************
void SimpleRenderer::SetProjectionMatrix(Matrix4 const &projection)
{
    matrix_data.projection = projection.get_transpose();
    matrix_cb->Update(m_context, &matrix_data);
}

//*******************************************************************
void SimpleRenderer::SetCameraMatrix(Matrix4 const &camera)
{
    Vector3 position = camera.get_translation();
    matrix_data.eyePosition = Vector4(position.x, position.y, position.z, 1.f);

    SetViewMatrix(camera.get_inverse_orthonormal());
}

//*******************************************************************
void SimpleRenderer::SetEyePosition(Vector3 const &eyePosition)
{
    matrix_data.eyePosition = Vector4(eyePosition.x, eyePosition.y, eyePosition.z, 1.f);
    matrix_cb->Update(m_context, &matrix_data);
}

//*******************************************************************
void SimpleRenderer::SetOrthoProjection(Vector2 const &bottomLeft, Vector2 const &topRight)
{
    Matrix4 projection = MakeOrthoProjection(bottomLeft.x, topRight.x, bottomLeft.y, topRight.y, 0.0f, 1.0f);
    SetProjectionMatrix(projection);
}

//*******************************************************************
void SimpleRenderer::SetPerspectiveProjection(float const fovRadians, float aspectRatio, float const nz, float const fz)
{
    Matrix4 projection = MakePerspectiveProjection(fovRadians, aspectRatio, nz, fz);
    SetProjectionMatrix(projection);
}

//*******************************************************************
void SimpleRenderer::EnableBlend(eBlendFactor src, eBlendFactor dest)
{
    if (m_currentBlendState != nullptr)
    {
        if ((m_blendState.enabled == true)
            && (m_blendState.srcFactor == src)
            && (m_blendState.destFactor == dest))
        {
            return;
        }
    }

    if (m_currentBlendState != nullptr)
    {
        delete m_currentBlendState;
        m_currentBlendState = nullptr;
    }

    BlendState *bs = new BlendState(m_device, true, src, dest);
    m_context->SetBlendState(bs);

    m_blendState.enabled = true;
    m_blendState.srcFactor = src;
    m_blendState.destFactor = dest;

    m_currentBlendState = bs;
}

//*******************************************************************
void SimpleRenderer::DisableBlend()
{
    if (!m_blendState.enabled)
    {
        return;
    }

    if (m_currentBlendState != nullptr)
    {
        delete m_currentBlendState;
        m_currentBlendState = nullptr;
    }

    BlendState *bs = new BlendState(m_device, false);
    m_context->SetBlendState(bs);

    m_blendState.enabled = false;

    m_currentBlendState = bs;
}

//*******************************************************************
void SimpleRenderer::EnableDepthTest(bool enable)
{
    if (m_depthStencilDesc.depth_test_enabled != enable) {
        m_depthStencilDesc.depth_test_enabled = enable;

        if (m_defaultDepthStencilState != nullptr)
        {
            delete m_defaultDepthStencilState;
            m_defaultDepthStencilState = nullptr;
        }


        m_defaultDepthStencilState = new DepthStencilState(m_device, m_depthStencilDesc);
        m_context->SetDepthStencilState(m_defaultDepthStencilState);
    }
}

//*******************************************************************
void SimpleRenderer::EnableDepthWrite(bool enable)
{
    if (m_depthStencilDesc.depth_writing_enabled != enable) {
        m_depthStencilDesc.depth_writing_enabled = enable;

        if (m_defaultDepthStencilState != nullptr)
        {
            delete m_defaultDepthStencilState;
            m_defaultDepthStencilState = nullptr;
        }


        m_defaultDepthStencilState = new DepthStencilState(m_device, m_depthStencilDesc);
        m_context->SetDepthStencilState(m_defaultDepthStencilState);
    }
}

//*******************************************************************
void SimpleRenderer::EnableDepth(bool test, bool write)
{
    if ((m_depthStencilDesc.depth_writing_enabled != write) || (m_depthStencilDesc.depth_test_enabled != test))
    {
        m_depthStencilDesc.depth_writing_enabled = write;
        m_depthStencilDesc.depth_test_enabled = test;
        if (m_defaultDepthStencilState != nullptr)
        {
            delete m_defaultDepthStencilState;
            m_defaultDepthStencilState = nullptr;
        }
        m_defaultDepthStencilState = new DepthStencilState(m_device, m_depthStencilDesc);
        m_context->SetDepthStencilState(m_defaultDepthStencilState);
    }
}

//*******************************************************************
void SimpleRenderer::ClearColor(Rgba const &color)
{
    return m_context->ClearColorTarget(m_currentColorTarget, color);
}

//*******************************************************************
void SimpleRenderer::ClearDepth(float depth /*= 1.0f*/, uint8_t stencil /*= 0*/)
{
    return m_context->ClearDepthTarget(m_currentDepthStencil, depth, stencil);
}

//*******************************************************************
void SimpleRenderer::ClearTargetColor(Texture2D *target, Rgba const &color)
{
    m_context->ClearColorTarget(target, color);
}

//*******************************************************************
void SimpleRenderer::Present()
{
    m_output->Present();
}

//*******************************************************************
Texture2D* SimpleRenderer::CreateRenderTarget()
{
    ID3D11Texture2D *backBuffer = nullptr;
    HRESULT hr = m_output->m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    if (FAILED(hr))
    {
        ASSERT_OR_DIE(false, "Get Buffer for swap chain failed.");
    }
    m_output->m_renderTarget = new Texture2D(m_device, backBuffer);
    return m_output->m_renderTarget;
}

//*******************************************************************
void SimpleRenderer::SetRenderTarget(Texture2D *colorTarget, Texture2D *depthTarget)
{
    if (colorTarget != nullptr)
    {
        if (colorTarget->IsRenderTarget())
        {
            m_currentColorTarget = colorTarget;
        }
    }
    else
    {
        m_currentColorTarget = m_defaultColorTarget;
    }

    if (depthTarget == nullptr)
    {
        depthTarget = m_defaultDepthStencil;
    }

    m_currentDepthStencil = depthTarget;
    m_context->SetColorTarget(m_currentColorTarget, depthTarget);
}

//*******************************************************************
void SimpleRenderer::SetShader(ShaderProgram *shader)
{
    m_context->SetShaderProgram(shader);
}

//*******************************************************************
void SimpleRenderer::SetTexture(unsigned int texture_index, Texture2D *texture)
{
    if (texture == nullptr)
    {
        texture = m_whiteSample;
    }

    m_context->SetTexture(texture_index, texture);
}

//*******************************************************************
void SimpleRenderer::SetStructureBuffer(unsigned int structureBufferID, StructuredBuffer *structureBuffer)
{
    m_context->SetStructuredBuffer(structureBufferID, structureBuffer);
}

//*******************************************************************
void SimpleRenderer::SetSampler(unsigned int sampler_index, Sampler *sampler)
{
    if (sampler == nullptr)
    {
        sampler = m_linearSampler;
    }

    m_context->SetSampler(sampler_index, sampler);
}

//*******************************************************************
void SimpleRenderer::SetAmbientLight(float intensity, Rgba const &color /*= Rgba::WHITE*/)
{
    float r;
    float g;
    float b;
    float a;
    color.GetAsFloats(r, g, b, a);

    light_data.m_ambient = Vector4(r, g, b, intensity);
    light_cb->Update(m_context, &light_data);
}

//*******************************************************************
void SimpleRenderer::SetSpecularConstants(float const specPower, float const specFactor /*= 1.0f*/)
{
    light_data.m_specFactor = specFactor;
    light_data.m_specPower = specPower;

    light_cb->Update(m_context, &light_data);
}

//*******************************************************************
void SimpleRenderer::EnablePointLight(int const &lightIndex, Vector3 const &position, Rgba const &color, float intesity /*= 1.0f*/, Vector3 const &attentuation /*= Vector3(0, 0, 1)*/, Vector3 const &specAttentuation /*= Vector3(0, 0, 1)*/)
{
    float r;
    float g;
    float b;
    float a;
    color.GetAsFloats(r, g, b, a);

    light_data.m_lights[lightIndex].m_lightPosition = Vector4(position.x, position.y, position.z, 1.0f);
    light_data.m_lights[lightIndex].m_lightColor = Vector4(r, g, b, intesity);
    light_data.m_lights[lightIndex].m_attenuation = Vector4(attentuation.x, attentuation.y, attentuation.z, 1.0f);
    light_data.m_lights[lightIndex].m_specAttenuation = Vector4(specAttentuation.x, specAttentuation.y, specAttentuation.z, 1.0f);

    light_cb->Update(m_context, &light_data);
}

//*******************************************************************
void SimpleRenderer::SetPointLight(int const &lightIndex, Vector3 const &position, Rgba const &color, float intensity /*= 1.0f*/, Vector3 const &attenuation /*= Vector3(0, 0, 1)*/)
{
    float r;
    float g;
    float b;
    float a;
    color.GetAsFloats(r, g, b, a);

    light_data.m_lights[lightIndex].m_lightPosition = Vector4(position.x, position.y, position.z, 1.0f);
    light_data.m_lights[lightIndex].m_lightColor = Vector4(r, g, b, intensity);
    light_data.m_lights[lightIndex].m_attenuation = Vector4(attenuation.x, attenuation.y, attenuation.z, 1.0f);

    light_cb->Update(m_context, &light_data);
}

//*******************************************************************
void SimpleRenderer::SetMousePosition(Vector2 position)
{
    m_mouse_position_data.m_mousePosition.x = position.x;
    m_mouse_position_data.m_mousePosition.y = position.y;
    m_mouse_position_data.m_mousePosition.z = 0.0f;
    m_mouse_position_data.m_mousePosition.w = 0.0f;
    m_mouse_position_cb->Update(m_context, &m_mouse_position_data);
}


//*******************************************************************
void SimpleRenderer::Draw(const ePrimitiveType& topology, VertexBuffer *vbo, unsigned int const vertexCount)
{
    m_context->Draw(topology, vbo, vertexCount);
}

//*******************************************************************
void SimpleRenderer::DrawIndexed(const ePrimitiveType& topology, VertexBuffer* vbo, IndexBuffer* ibo, unsigned int const indexCount)
{
    m_context->Draw(topology, vbo, indexCount);
    m_context->DrawIndexed(topology, vbo, ibo, indexCount);
}

//*******************************************************************
void SimpleRenderer::UpdateVBO(Vertex3 *vertices, unsigned int vertexCount)
{
    m_tempVbo->Update(m_context, vertices, vertexCount);
    m_vertexCount += vertexCount;
}

//*******************************************************************
void SimpleRenderer::DrawVertexArrays(Vertex3 *vertices, unsigned int vertexCount)
{
    m_tempVbo->Update(m_context, vertices, vertexCount);
    Draw(ePrimitiveType::PRIMITIVE_TRIANGLES, m_tempVbo, vertexCount);
}

//*******************************************************************
void SimpleRenderer::DrawVertexArrays(Vertex3 *vertices, unsigned int vertexCount, VertexBuffer* vbo)
{
    vbo->Update(m_context, vertices, vertexCount);
    Draw(ePrimitiveType::PRIMITIVE_TRIANGLES, vbo, vertexCount);
}

//*******************************************************************
void SimpleRenderer::DrawVertexArrays(unsigned int vertexCount, VertexBuffer* vbo)
{
    Draw(ePrimitiveType::PRIMITIVE_TRIANGLES, vbo, vertexCount);
}

//*******************************************************************
void SimpleRenderer::DrawIndexArrays(Vertex3 *vertices, unsigned int vertexCount, unsigned int *indices, unsigned int indexCount)
{
    m_tempVbo->Update(m_context, vertices, vertexCount);
    m_tempIbo->Update(m_context, indices, indexCount);
    DrawIndexed(ePrimitiveType::PRIMITIVE_TRIANGLES, m_tempVbo, m_tempIbo, indexCount);
}

//*******************************************************************
void SimpleRenderer::DrawVertexArrays(ePrimitiveType type, Vertex3 *vertices, unsigned int vertexCount)
{
    m_tempVbo->Update(m_context, vertices, vertexCount);
    Draw(type, m_tempVbo, vertexCount);
}

//*******************************************************************
void SimpleRenderer::DrawIndexArrays(ePrimitiveType type, Vertex3 *vertices, unsigned int vertexCount, unsigned int *indices, unsigned int indexCount)
{
    m_tempVbo->Update(m_context, vertices, vertexCount);
    m_tempIbo->Update(m_context, indices, indexCount);
    DrawIndexed(type, m_tempVbo, m_tempIbo, indexCount);
}

//*******************************************************************
void SimpleRenderer::DrawLine2D(float thickness, Vector3 startPosition, Vector3 endPosition, Rgba startColor, Rgba endColor)
{
    SetTexture(m_whiteSample);

    Vector3 halfThickness = Vector3(0.f, thickness / 2.f, 0.f);

    Vector3 bl = startPosition - halfThickness;
    Vector3 tl = startPosition + halfThickness;
    Vector3 br = endPosition - halfThickness;
    Vector3 tr = endPosition + halfThickness;

    Vertex3 vertices[] = {
        Vertex3(bl, startColor, Vector2(0.0f, 1.0f)),
        Vertex3(tl, startColor, Vector2(0.0f, 0.0f)),
        Vertex3(tr, endColor, Vector2(1.0f, 0.0f)),
        Vertex3(br, endColor, Vector2(1.0f, 1.0f)),
    };

    unsigned int indices[] = { 0, 3, 2, 0, 2, 1 };

    DrawIndexArrays(ePrimitiveType::PRIMITIVE_TRIANGLES, vertices, 4, indices, 6);
}

//*******************************************************************
void SimpleRenderer::DrawSprite2D(Texture2D* texture, Vector2 uvBottomLeft, Vector2 uvTopRight, AABB2D bounds, Rgba color)
{
    SetTexture(texture);
    Vector3 bl = Vector3(bounds.m_mins.x, bounds.m_mins.y, 0.f);
    Vector3 tl = Vector3(bounds.m_mins.x, bounds.m_maxs.y, 0.f);
    Vector3 br = Vector3(bounds.m_maxs.x, bounds.m_mins.y, 0.f);
    Vector3 tr = Vector3(bounds.m_maxs.x, bounds.m_maxs.y, 0.f);

    Vertex3 vertices[] = {
        Vertex3(bl, color, uvBottomLeft),
        Vertex3(tl, color, Vector2(uvBottomLeft.x, uvTopRight.y)),
        Vertex3(tr, color, uvTopRight),
        Vertex3(br, color, Vector2(uvTopRight.x, uvBottomLeft.y)),
    };

    unsigned int indices[] = { 0, 3, 2, 0, 2, 1 };

    DrawIndexArrays(ePrimitiveType::PRIMITIVE_TRIANGLES, vertices, 4, indices, 6);
}

//*******************************************************************
void SimpleRenderer::DrawSprite2DCentered(Texture2D* texture, Vector2 uvBottomLeft, Vector2 uvTopRight, Vector2 position, float width, float height, Rgba color)
{
    float halfWidth = width / 2.f;
    float halfHeight = height / 2.f;

    if (texture != nullptr)
    {
        SetTexture(texture);
    }


    Vector2 bl = Vector2(position.x - halfWidth, position.y - halfHeight);
    Vector2 tr = Vector2(position.x + halfWidth, position.y + halfHeight);

    Vector2 uvBL = uvBottomLeft;
    Vector2 uvTR = uvTopRight;

    Vertex3 vertices[] = {
        Vertex3(Vector3(bl.x, bl.y, 0.0f), color, Vector2(uvBL.x, uvBL.y)), //0
        Vertex3(Vector3(tr.x, tr.y, 0.0f), color, Vector2(uvTR.x, uvTR.y)), //2
        Vertex3(Vector3(bl.x, tr.y, 0.0f), color, Vector2(uvBL.x, uvTR.y)), //1
        Vertex3(Vector3(bl.x, bl.y, 0.0f), color, Vector2(uvBL.x, uvBL.y)), //0
        Vertex3(Vector3(tr.x, bl.y, 0.0f), color, Vector2(uvTR.x, uvBL.y)), //3
        Vertex3(Vector3(tr.x, tr.y, 0.0f), color, Vector2(uvTR.x, uvTR.y)),
    };

    //UpdateVBO(vertices, 6);
    DrawVertexArrays(vertices, 6);

    /*Vector3 bl = Vector3(position.x - halfWidth, position.y - halfHeight, 0.f);
    Vector3 tl = Vector3(position.x - halfWidth, position.y + halfHeight, 0.f);
    Vector3 br = Vector3(position.x + halfWidth, position.y - halfHeight, 0.f);
    Vector3 tr = Vector3(position.x + halfWidth, position.y + halfHeight, 0.f);

    Vertex3 vertices[] = {
        Vertex3(bl, color, uvBottomLeft),
        Vertex3(tl, color, Vector2(uvBottomLeft.x, uvTopRight.y)),
        Vertex3(tr, color, uvTopRight),
        Vertex3(br, color, Vector2(uvTopRight.x, uvBottomLeft.y)),
    };

    unsigned int indices[] = { 0, 3, 2, 0, 2, 1 };

    DrawIndexArrays(ePrimitiveType::PRIMITIVE_TRIANGLES, vertices, 4, indices, 6);*/
}

//*******************************************************************
void SimpleRenderer::DrawSprite2DCentered(Texture2D* texture, Vector2 uvBottomLeft, Vector2 uvTopRight, Vector2 position, float orientationDegrees, float width, float height, Rgba color)
{
    EnableBlend(BLEND_SRC_ALPHA, BLEND_INV_SRC_ALPHA);

    float halfWidth = width / 2.f;
    float halfHeight = height / 2.f;

    if (texture != nullptr)
    {
        SetTexture(texture);
    }

    //Vector2 bl = Vector2(position.x - (halfWidth * CosDegrees(orientationDegrees)), position.y - (halfHeight * SinDegrees(orientationDegrees)));
    //Vector2 tr = Vector2(position.x + (halfWidth * CosDegrees(orientationDegrees)), position.y + (halfHeight * SinDegrees(orientationDegrees)));
    Vector2 bl = Vector2(0.0f - halfWidth, 0.0f - halfHeight);
    Vector2 tr = Vector2(0.0f + halfWidth, 0.0f + halfHeight);
    
    Vector2 uvBL = uvBottomLeft;
    Vector2 uvTR = uvTopRight;

    Vertex3 vertices[] = {
        Vertex3(Vector3(bl.x, bl.y, 0.0f), color, Vector2(uvBL.x, uvBL.y)), //0
        Vertex3(Vector3(tr.x, tr.y, 0.0f), color, Vector2(uvTR.x, uvTR.y)), //2
        Vertex3(Vector3(bl.x, tr.y, 0.0f), color, Vector2(uvBL.x, uvTR.y)), //1
        Vertex3(Vector3(bl.x, bl.y, 0.0f), color, Vector2(uvBL.x, uvBL.y)), //0
        Vertex3(Vector3(tr.x, bl.y, 0.0f), color, Vector2(uvTR.x, uvBL.y)), //3
        Vertex3(Vector3(tr.x, tr.y, 0.0f), color, Vector2(uvTR.x, uvTR.y)),
    };
    
    Matrix4 newModel = Matrix4();
    newModel.RotateDegreesAboutZ(orientationDegrees - 90.0f);
    newModel.Translate(position);
    
    SetModelMatrix(newModel);
    DrawVertexArrays(vertices, 6);
    SetModelMatrix(Matrix4());

    DisableBlend();
}


//*******************************************************************
void SimpleRenderer::DrawSprite2DCentered(Texture2D* texture, Vector2 uvBottomLeft, Vector2 uvTopRight, Vector2 position, float width, float height, Rgba color, std::vector<Vertex3>& vertexes)
{
    float halfWidth = width / 2.f;
    float halfHeight = height / 2.f;

    SetTexture(texture);

    Vector2 bl = Vector2(position.x - halfWidth, position.y - halfHeight);
    Vector2 tr = Vector2(position.x + halfWidth, position.y + halfHeight);

    Vector2 uvBL = uvBottomLeft;
    Vector2 uvTR = uvTopRight;

    Vertex3 vertices[] = {
        Vertex3(Vector3(bl.x, bl.y, 0.0f), color, Vector2(uvBL.x, uvBL.y)), //0
        Vertex3(Vector3(tr.x, tr.y, 0.0f), color, Vector2(uvTR.x, uvTR.y)), //2
        Vertex3(Vector3(bl.x, tr.y, 0.0f), color, Vector2(uvBL.x, uvTR.y)), //1
        Vertex3(Vector3(bl.x, bl.y, 0.0f), color, Vector2(uvBL.x, uvBL.y)), //0
        Vertex3(Vector3(tr.x, bl.y, 0.0f), color, Vector2(uvTR.x, uvBL.y)), //3
        Vertex3(Vector3(tr.x, tr.y, 0.0f), color, Vector2(uvTR.x, uvTR.y)),
    };

    for (int verticesIndex = 0; verticesIndex < 6; verticesIndex++)
    {
        vertexes.push_back(vertices[verticesIndex]);
    }
    //vbo->Update(m_context, vertices, 6);
    //UpdateVBO(vertices, 6);
    //DrawVertexArrays(vertices, 6, vbo);

    /*Vector3 bl = Vector3(position.x - halfWidth, position.y - halfHeight, 0.f);
    Vector3 tl = Vector3(position.x - halfWidth, position.y + halfHeight, 0.f);
    Vector3 br = Vector3(position.x + halfWidth, position.y - halfHeight, 0.f);
    Vector3 tr = Vector3(position.x + halfWidth, position.y + halfHeight, 0.f);

    Vertex3 vertices[] = {
    Vertex3(bl, color, uvBottomLeft),
    Vertex3(tl, color, Vector2(uvBottomLeft.x, uvTopRight.y)),
    Vertex3(tr, color, uvTopRight),
    Vertex3(br, color, Vector2(uvTopRight.x, uvBottomLeft.y)),
    };

    unsigned int indices[] = { 0, 3, 2, 0, 2, 1 };

    DrawIndexArrays(ePrimitiveType::PRIMITIVE_TRIANGLES, vertices, 4, indices, 6);*/
}

//*******************************************************************
void SimpleRenderer::DrawLine3D(const Vector3& startPosition, const Vector3& endPosition, Rgba const &color)
{
    Vertex3 vertices[2] = {
        Vertex3(startPosition, color, Vector2(0.f, 0.f)),
        Vertex3(endPosition, color, Vector2(1.0f, 1.0f)),
    };

    DrawVertexArrays(ePrimitiveType::PRIMITIVE_LINES, vertices, 2);
}

//*******************************************************************
void SimpleRenderer::DrawCubeAxisLine(const Vector3& startPosition, const Vector3& endPosition, float thickness, Rgba const &color /*= Rgba::WHITE*/)
{
    Vector3 right = Vector3(thickness, 0.f, 0.f);
    Vector3 up = Vector3(0.f, thickness, 0.f);
    Vector3 towards = Vector3(0.f, 0.f, thickness);
    float negXExtents = -1.f;
    float negYExtents = -1.f;
    float posXExtents = endPosition.x;
    float posYExtents = endPosition.y;
    float posZExtents = endPosition.z;

    //Forward Face, 1
    Vector3 bl = startPosition + negXExtents * right + negYExtents * up;
    Vector3 tl = startPosition + negXExtents * right + posYExtents * up;
    Vector3 br = startPosition + posXExtents * right + negYExtents * up;
    Vector3 tr = startPosition + posXExtents * right + posYExtents * up;

    Vertex3 vertices[] = {
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
        Vertex3(tl, color, Vector2(0.0f, 0.0f)),
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(br, color, Vector2(1.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
    };
    DrawVertexArrays(vertices, 6);

    //Bottom-Side Face, 2
    bl = startPosition + negXExtents * right + negYExtents * up + posZExtents * towards;
    tl = startPosition + negXExtents * right + negYExtents * up;
    br = startPosition + posXExtents * right + negYExtents * up + posZExtents * towards;
    tr = startPosition + posXExtents * right + negYExtents * up;

    Vertex3 vertices2[] = {
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
        Vertex3(tl, color, Vector2(0.0f, 0.0f)),
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(br, color, Vector2(1.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
    };
    DrawVertexArrays(vertices2, 6);

    //Right-Side Face, 3
    bl = startPosition + posXExtents * right + negYExtents * up;
    tl = startPosition + posXExtents * right + posYExtents * up;
    br = startPosition + posXExtents * right + negYExtents * up + posZExtents * towards;
    tr = startPosition + posXExtents * right + posYExtents * up + posZExtents * towards;

    Vertex3 vertices3[] = {
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
        Vertex3(tl, color, Vector2(0.0f, 0.0f)),
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(br, color, Vector2(1.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
    };
    DrawVertexArrays(vertices3, 6);

    //Top-Side Face, 4
    bl = startPosition + posXExtents * right + posYExtents * up + posZExtents * towards;
    tl = startPosition + posXExtents * right + posYExtents * up;
    br = startPosition + negXExtents * right + posYExtents * up + posZExtents * towards;
    tr = startPosition + negXExtents * right + posYExtents * up;

    Vertex3 vertices4[] = {
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
        Vertex3(tl, color, Vector2(0.0f, 0.0f)),
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(br, color, Vector2(1.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
    };
    DrawVertexArrays(vertices4, 6);

    //Left-Side Face, 5
    bl = startPosition + negXExtents * right + negYExtents * up + posZExtents * towards;
    tl = startPosition + negXExtents * right + posYExtents * up + posZExtents * towards;
    br = startPosition + negXExtents * right + negYExtents * up;
    tr = startPosition + negXExtents * right + posYExtents * up;

    Vertex3 vertices5[] = {
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
        Vertex3(tl, color, Vector2(0.0f, 0.0f)),
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(br, color, Vector2(1.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
    };
    DrawVertexArrays(vertices5, 6);

    //Back Face, 6
    bl = startPosition + posXExtents * right + negYExtents * up + posZExtents * towards;
    tl = startPosition + posXExtents * right + posYExtents * up + posZExtents * towards;
    br = startPosition + negXExtents * right + negYExtents * up + posZExtents * towards;
    tr = startPosition + negXExtents * right + posYExtents * up + posZExtents * towards;

    Vertex3 vertices6[] = {
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
        Vertex3(tl, color, Vector2(0.0f, 0.0f)),
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(br, color, Vector2(1.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
    };
    DrawVertexArrays(vertices6, 6);
}

//*******************************************************************
void SimpleRenderer::DrawCubeLine(const Vector3& startPosition, const Vector3& endPosition, float thickness, Rgba const &color /*= Rgba::WHITE*/)
{
    Vector3 right = Vector3(thickness, 0.f, 0.f);
    Vector3 up = Vector3(0.f, thickness, 0.f);
    Vector3 towards = Vector3(0.f, 0.f, thickness);
    float negXExtents = -endPosition.x;
    float negYExtents = -endPosition.y;
    float negZExtents = -endPosition.z;
    float posXExtents = endPosition.x;
    float posYExtents = endPosition.y;
    float posZExtents = endPosition.z;

    //Forward Face, 1
    Vector3 bl = startPosition + negXExtents * right + negYExtents * up;
    Vector3 tl = startPosition + negXExtents * right + posYExtents * up;
    Vector3 br = startPosition + posXExtents * right + negYExtents * up;
    Vector3 tr = startPosition + posXExtents * right + posYExtents * up;

    Vertex3 vertices[] = {
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
        Vertex3(tl, color, Vector2(0.0f, 0.0f)),
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(br, color, Vector2(1.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
    };
    DrawVertexArrays(vertices, 6);

    //Bottom-Side Face, 2
    bl = startPosition + negXExtents * right + negYExtents * up + posZExtents * towards;
    tl = startPosition + negXExtents * right + negYExtents * up + negZExtents * towards;
    br = startPosition + posXExtents * right + negYExtents * up + posZExtents * towards;
    tr = startPosition + posXExtents * right + negYExtents * up + negZExtents * towards;

    Vertex3 vertices2[] = {
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
        Vertex3(tl, color, Vector2(0.0f, 0.0f)),
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(br, color, Vector2(1.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
    };
    DrawVertexArrays(vertices2, 6);

    //Right-Side Face, 3
    bl = startPosition + posXExtents * right + negYExtents * up + negZExtents * towards;
    tl = startPosition + posXExtents * right + posYExtents * up + negZExtents * towards;
    br = startPosition + posXExtents * right + negYExtents * up + posZExtents * towards;
    tr = startPosition + posXExtents * right + posYExtents * up + posZExtents * towards;

    Vertex3 vertices3[] = {
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
        Vertex3(tl, color, Vector2(0.0f, 0.0f)),
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(br, color, Vector2(1.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
    };
    DrawVertexArrays(vertices3, 6);

    //Top-Side Face, 4
    bl = startPosition + posXExtents * right + posYExtents * up + posZExtents * towards;
    tl = startPosition + posXExtents * right + posYExtents * up + negZExtents * towards;
    br = startPosition + negXExtents * right + posYExtents * up + posZExtents * towards;
    tr = startPosition + negXExtents * right + posYExtents * up + negZExtents * towards;

    Vertex3 vertices4[] = {
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
        Vertex3(tl, color, Vector2(0.0f, 0.0f)),
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(br, color, Vector2(1.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
    };
    DrawVertexArrays(vertices4, 6);

    //Left-Side Face, 5
    bl = startPosition + negXExtents * right + negYExtents * up + posZExtents * towards;
    tl = startPosition + negXExtents * right + posYExtents * up + posZExtents * towards;
    br = startPosition + negXExtents * right + negYExtents * up + negZExtents * towards;
    tr = startPosition + negXExtents * right + posYExtents * up + negZExtents * towards;

    Vertex3 vertices5[] = {
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
        Vertex3(tl, color, Vector2(0.0f, 0.0f)),
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(br, color, Vector2(1.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
    };
    DrawVertexArrays(vertices5, 6);

    //Back Face, 6
    bl = startPosition + posXExtents * right + negYExtents * up + posZExtents * towards;
    tl = startPosition + posXExtents * right + posYExtents * up + posZExtents * towards;
    br = startPosition + negXExtents * right + negYExtents * up + posZExtents * towards;
    tr = startPosition + negXExtents * right + posYExtents * up + posZExtents * towards;

    Vertex3 vertices6[] = {
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
        Vertex3(tl, color, Vector2(0.0f, 0.0f)),
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(br, color, Vector2(1.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
    };
    DrawVertexArrays(vertices6, 6);
}

//*******************************************************************
void SimpleRenderer::DrawXZPlane()
{
    float unitThickness = 0.001f;
    float size = 10000.f;

    int minimumXUnit = -10;
    int maximumXUnit = 10;
    for (int xUnit = minimumXUnit; xUnit <= maximumXUnit; xUnit++)
    {
        Vector3 startPosition = Vector3(0.f, -0.025f, (float)xUnit);
        Vector3 endPosition = Vector3(size, -0.025f, (float)maximumXUnit);
        DrawCubeLine(startPosition, endPosition, unitThickness, Rgba(255, 0, 255, 255));

        float fifthUnit = 1.0f / 5.0f;
        float fifthUnitThickness = 0.0001f;
        float fifthSize = 100000.f;
        if (xUnit < maximumXUnit)
        {
            for (float unitIndex = fifthUnit; unitIndex < 1.0f; unitIndex += fifthUnit)
            {
                Vector3 startPosition2 = Vector3(0.f, -0.025f, (float)xUnit + unitIndex);
                Vector3 endPosition2 = Vector3(fifthSize, -0.025f, (float)maximumXUnit);
                DrawCubeLine(startPosition2, endPosition2, fifthUnitThickness, Rgba(255, 255, 255, 75));
            }
        }
    }

    int minimumZUnit = -10;
    int maximumZUnit = 10;
    for (int zUnit = minimumZUnit; zUnit <= maximumZUnit; zUnit++)
    {
        Vector3 startPosition = Vector3((float)zUnit, -0.025f, 0.f);
        Vector3 endPosition = Vector3((float)maximumZUnit, -0.025f, size);
        DrawCubeLine(startPosition, endPosition, unitThickness, Rgba(0, 255, 255, 255));

        float fifthUnit = 1.0f / 5.0f;
        float fifthUnitThickness = 0.0001f;
        float fifthSize = 100000.f;
        if (zUnit < maximumZUnit)
        {
            for (float unitIndex = fifthUnit; unitIndex < 1.0f; unitIndex += fifthUnit)
            {
                Vector3 startPosition2 = Vector3((float)zUnit + unitIndex, -0.025f, 0.f);
                Vector3 endPosition2 = Vector3((float)maximumZUnit, -0.025f, fifthSize);
                DrawCubeLine(startPosition2, endPosition2, fifthUnitThickness, Rgba(255, 255, 255, 75));
            }
        }
    }
}

//*******************************************************************
void SimpleRenderer::DrawQuad(const Vector3& position, const Vector3& extension)
{
    Vertex3 vertices[6] =
    {
        Vertex3(position + Vector3(-extension.x, -extension.y, extension.z),    Rgba(255, 255, 255, 255), Vector2(0.0f, 1.0f)), //0
        Vertex3(position + Vector3(extension.x, -extension.y, extension.z),     Rgba(255, 255, 255, 255), Vector2(1.0f, 1.0f)), //3
        Vertex3(position + Vector3(extension.x, extension.y, extension.z),      Rgba(255, 255, 255, 255), Vector2(1.0f, 0.0f)), //2
        Vertex3(position + Vector3(-extension.x, -extension.y, extension.z),    Rgba(255, 255, 255, 255), Vector2(0.0f, 1.0f)), //0
        Vertex3(position + Vector3(extension.x, extension.y, extension.z),      Rgba(255, 255, 255, 255), Vector2(1.0f, 0.0f)), //2
        Vertex3(position + Vector3(-extension.x, extension.y, extension.z),     Rgba(255, 255, 255, 255), Vector2(0.0f, 0.0f)), //1 
    };


    DrawVertexArrays(vertices, 6);
}

//*******************************************************************
void SimpleRenderer::DrawQuad(const Vector3& position, const Vector3& extension, Rgba color)
{
    Vertex3 vertices[6] =
    {
        Vertex3(position + Vector3(-extension.x, -extension.y, extension.z),    color, Vector2(0.0f, 1.0f)), //0
        Vertex3(position + Vector3(extension.x, -extension.y, extension.z),     color, Vector2(1.0f, 1.0f)), //3
        Vertex3(position + Vector3(extension.x, extension.y, extension.z),      color, Vector2(1.0f, 0.0f)), //2
        Vertex3(position + Vector3(-extension.x, -extension.y, extension.z),    color, Vector2(0.0f, 1.0f)), //0
        Vertex3(position + Vector3(extension.x, extension.y, extension.z),      color, Vector2(1.0f, 0.0f)), //2
        Vertex3(position + Vector3(-extension.x, extension.y, extension.z),     color, Vector2(0.0f, 0.0f)), //1 
    };

    DrawVertexArrays(vertices, 6);
}

//*******************************************************************
void SimpleRenderer::DrawQuad(Vector2 bottomLeft, Vector2 topRight, Vector2 uvBottomLeft, Vector2 uvTopRight, Rgba color)
{
    Vector2 bl = bottomLeft;
    Vector2 tr = topRight;

    Vector2 uvBL = uvBottomLeft;
    Vector2 uvTR = uvTopRight;

    Vertex3 vertices[] = {
        Vertex3(Vector3(bl.x, bl.y, 0.0f), color, Vector2(uvBL.x, uvBL.y)), //0
        Vertex3(Vector3(tr.x, tr.y, 0.0f), color, Vector2(uvTR.x, uvTR.y)), //2
        Vertex3(Vector3(bl.x, tr.y, 0.0f), color, Vector2(uvBL.x, uvTR.y)), //1
        Vertex3(Vector3(bl.x, bl.y, 0.0f), color, Vector2(uvBL.x, uvBL.y)), //0
        Vertex3(Vector3(tr.x, bl.y, 0.0f), color, Vector2(uvTR.x, uvBL.y)), //3
        Vertex3(Vector3(tr.x, tr.y, 0.0f), color, Vector2(uvTR.x, uvTR.y)),
    };

    //UpdateVBO(vertices, 6);
    DrawVertexArrays(vertices, 6);
}

//*******************************************************************
void SimpleRenderer::DrawQuad(Vector2 bottomLeft, Vector2 topRight, Vector2 uvBottomLeft, Vector2 uvTopRight, Rgba color, std::vector<Vertex3>& vertexes)
{
    Vector2 bl = bottomLeft;
    Vector2 tr = topRight;

    Vector2 uvBL = uvBottomLeft;
    Vector2 uvTR = uvTopRight;

    Vertex3 vertices[] = {
        Vertex3(Vector3(bl.x, bl.y, 0.0f), color, Vector2(uvBL.x, uvBL.y)), //0
        Vertex3(Vector3(tr.x, tr.y, 0.0f), color, Vector2(uvTR.x, uvTR.y)), //2
        Vertex3(Vector3(bl.x, tr.y, 0.0f), color, Vector2(uvBL.x, uvTR.y)), //1
        Vertex3(Vector3(bl.x, bl.y, 0.0f), color, Vector2(uvBL.x, uvBL.y)), //0
        Vertex3(Vector3(tr.x, bl.y, 0.0f), color, Vector2(uvTR.x, uvBL.y)), //3
        Vertex3(Vector3(tr.x, tr.y, 0.0f), color, Vector2(uvTR.x, uvTR.y)),
    };

    for (int verticesIndex = 0; verticesIndex < 6; verticesIndex++)
    {
        vertexes.push_back(vertices[verticesIndex]);
    }

    //vbo->Update(m_context, vertices, 6);
    //UpdateVBO(vertices, 6);
    //DrawVertexArrays(vertices, 6, vbo);
}

//*******************************************************************
void SimpleRenderer::DrawCircle(const Disc2D& discMembers, const Rgba& color, float orientationDegrees)
{    
    UNUSED(orientationDegrees);

    std::vector<Vertex3> vertices;
    const float DEGREES_PER_VERTEX = 360.0f / 100.0f;
    Vertex3 centerVertex = Vertex3(Vector3(discMembers.center.x, discMembers.center.y, 0.0f), color, Vector2());
    for (int vertexIndex = 0; vertexIndex <= 100; ++vertexIndex)
    {
        float degrees = DEGREES_PER_VERTEX * (float)vertexIndex;
        float x = discMembers.center.x + (discMembers.radius * CosDegrees(degrees));
        float y = discMembers.center.y + (discMembers.radius * SinDegrees(degrees));

        Vertex3 currentVertex;
        currentVertex.m_position = Vector3(x, y, 0.0f);
        currentVertex.m_rgba = color;
        currentVertex.m_uv = Vector2();

        vertices.push_back(currentVertex);
    }
    
    DrawVertexArrays(ePrimitiveType::PRIMITIVE_LINE_STRIP, &vertices[0], (int)vertices.size());
}

//*******************************************************************
void SimpleRenderer::DrawSimpleLine2D(Vector2 startPoint, Vector2 endPoint, Rgba color)
{

    std::vector<Vertex3> vertices;
    Vertex3 sPnt;
    Vertex3 ePnt;

    sPnt.m_position = Vector3(startPoint.x, startPoint.y, 0.0f);
    sPnt.m_rgba = color;
    sPnt.m_uv = Vector2();
    vertices.push_back(sPnt);

    ePnt.m_position = Vector3(endPoint.x, endPoint.y, 0.0f);
    ePnt.m_rgba = color;
    ePnt.m_uv = Vector2();
    vertices.push_back(ePnt);

    DrawVertexArrays(ePrimitiveType::PRIMITIVE_LINE_STRIP, &vertices[0], (int)vertices.size());
    SetModelMatrix(Matrix4());
}

//*******************************************************************
void SimpleRenderer::DrawQuad3D(Vector3 const &origin, Vector3 const &right, float negXExtents, float posXExtents, Vector3 const &up, float negYExtents, float posYExtents, Rgba const &color)
{
    Vector3 bl = origin + negXExtents * right + negYExtents * up;
    Vector3 tl = origin + negXExtents * right + posYExtents * up;
    Vector3 br = origin + posXExtents * right + negYExtents * up;
    Vector3 tr = origin + posXExtents * right + posYExtents * up;

    Vertex3 vertices[] = {
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
        Vertex3(tl, color, Vector2(0.0f, 0.0f)),
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(br, color, Vector2(1.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
    };

    DrawVertexArrays(vertices, 6);
}

//*******************************************************************
void SimpleRenderer::DrawIndexedQuad3D(Vector3 const &origin, Vector3 const &right, float negXExtents, float posXExtents, Vector3 const &up, float negYExtents, float posYExtents, Rgba const &color)
{
    Vector3 bl = origin + negXExtents * right + negYExtents * up;
    Vector3 tl = origin + negXExtents * right + posYExtents * up;
    Vector3 br = origin + posXExtents * right + negYExtents * up;
    Vector3 tr = origin + posXExtents * right + posYExtents * up;

    Vertex3 vertices[] = {
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(tl, color, Vector2(0.0f, 0.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
        Vertex3(br, color, Vector2(1.0f, 1.0f)),
    };

    unsigned int indices[] = { 0, 3, 2, 0, 2, 1 };

    DrawIndexArrays(ePrimitiveType::PRIMITIVE_TRIANGLES, vertices, 4, indices, 6);
}

//*******************************************************************
void SimpleRenderer::DrawTwoSidedQuad3D(Vector3 const &origin, Vector3 const &right, float negXExtents, float posXExtents, Vector3 const &up, float negYExtents, float posYExtents, Rgba const &color)
{
    Vector3 bl = origin + negXExtents * right + negYExtents * up;
    Vector3 tl = origin + negXExtents * right + posYExtents * up;
    Vector3 br = origin + posXExtents * right + negYExtents * up;
    Vector3 tr = origin + posXExtents * right + posYExtents * up;

    Vector3 bl2 = origin + posXExtents * right + negYExtents * up;
    Vector3 tl2 = origin + posXExtents * right + posYExtents * up;
    Vector3 br2 = origin + negXExtents * right + negYExtents * up;
    Vector3 tr2 = origin + negXExtents * right + posYExtents * up;

    Vertex3 vertices[] = {
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),
        Vertex3(tl, color, Vector2(0.0f, 0.0f)),
        Vertex3(bl, color, Vector2(0.0f, 1.0f)),
        Vertex3(br, color, Vector2(1.0f, 1.0f)),
        Vertex3(tr, color, Vector2(1.0f, 0.0f)),

        Vertex3(bl2, color, Vector2(0.0f, 1.0f)),
        Vertex3(tr2, color, Vector2(1.0f, 0.0f)),
        Vertex3(tl2, color, Vector2(0.0f, 0.0f)),
        Vertex3(bl2, color, Vector2(0.0f, 1.0f)),
        Vertex3(br2, color, Vector2(1.0f, 1.0f)),
        Vertex3(tr2, color, Vector2(1.0f, 0.0f)),
    };

    DrawVertexArrays(vertices, 12);
}

//*******************************************************************
void SimpleRenderer::DrawCube(Vector3 const &origin, Vector3 const &right, float negXExtents, float posXExtents, Vector3 const &up, float negYExtents, float posYExtents, Vector3 const &towards, float negZExtents, float posZExtents, Rgba const &color /*= Rgba::WHITE*/)
{
    UNUSED(negZExtents);

    //Forward Face, 1
    Vector3 bl = origin + negXExtents * right + negYExtents * up;
    Vector3 tl = origin + negXExtents * right + posYExtents * up;
    Vector3 br = origin + posXExtents * right + negYExtents * up;
    Vector3 tr = origin + posXExtents * right + posYExtents * up;

    Vector3 f_right = right;
    Vector3 f_up = up;
    Vector3 normal = f_up.CrossProduct(f_right);
    Vector3 tangent = f_right;
    Vector3 bitangent = tangent.CrossProduct(normal);

    Vertex3 vertices[] = {
        Vertex3(bl, normal, tangent, bitangent, color, Vector2(0.0f, 1.0f)),
        Vertex3(tr, normal, tangent, bitangent, color, Vector2(1.0f, 0.0f)),
        Vertex3(tl, normal, tangent, bitangent, color, Vector2(0.0f, 0.0f)),
        Vertex3(bl, normal, tangent, bitangent, color, Vector2(0.0f, 1.0f)),
        Vertex3(br, normal, tangent, bitangent, color, Vector2(1.0f, 1.0f)),
        Vertex3(tr, normal, tangent, bitangent, color, Vector2(1.0f, 0.0f)),
    };
    DrawVertexArrays(vertices, 6);

    //Bottom-Side Face, 2
    bl = origin + negXExtents * right + negYExtents * up + posZExtents * towards;
    tl = origin + negXExtents * right + negYExtents * up;
    br = origin + posXExtents * right + negYExtents * up + posZExtents * towards;
    tr = origin + posXExtents * right + negYExtents * up;

    f_right = right;
    f_up = (-1) * towards;
    normal = f_up.CrossProduct(f_right);
    tangent = f_right;
    bitangent = tangent.CrossProduct(normal);

    Vertex3 vertices2[] = {
        Vertex3(bl, normal, tangent, bitangent, color, Vector2(0.0f, 1.0f)),
        Vertex3(tr, normal, tangent, bitangent, color, Vector2(1.0f, 0.0f)),
        Vertex3(tl, normal, tangent, bitangent, color, Vector2(0.0f, 0.0f)),
        Vertex3(bl, normal, tangent, bitangent, color, Vector2(0.0f, 1.0f)),
        Vertex3(br, normal, tangent, bitangent, color, Vector2(1.0f, 1.0f)),
        Vertex3(tr, normal, tangent, bitangent, color, Vector2(1.0f, 0.0f)),
    };
    DrawVertexArrays(vertices2, 6);

    //Right-Side Face, 3
    bl = origin + posXExtents * right + negYExtents * up;
    tl = origin + posXExtents * right + posYExtents * up;
    br = origin + posXExtents * right + negYExtents * up + posZExtents * towards;
    tr = origin + posXExtents * right + posYExtents * up + posZExtents * towards;

    f_right = towards;
    f_up = up;
    normal = f_up.CrossProduct(f_right);
    tangent = f_right;
    bitangent = tangent.CrossProduct(normal);

    Vertex3 vertices3[] = {
        Vertex3(bl, normal, tangent, bitangent, color, Vector2(0.0f, 1.0f)),
        Vertex3(tr, normal, tangent, bitangent, color, Vector2(1.0f, 0.0f)),
        Vertex3(tl, normal, tangent, bitangent, color, Vector2(0.0f, 0.0f)),
        Vertex3(bl, normal, tangent, bitangent, color, Vector2(0.0f, 1.0f)),
        Vertex3(br, normal, tangent, bitangent, color, Vector2(1.0f, 1.0f)),
        Vertex3(tr, normal, tangent, bitangent, color, Vector2(1.0f, 0.0f)),
    };
    DrawVertexArrays(vertices3, 6);

    //Top-Side Face, 4
    bl = origin + posXExtents * right + posYExtents * up + posZExtents * towards;
    tl = origin + posXExtents * right + posYExtents * up;
    br = origin + negXExtents * right + posYExtents * up + posZExtents * towards;
    tr = origin + negXExtents * right + posYExtents * up;

    f_right = (-1) * right;
    f_up = (-1) * towards;
    normal = f_up.CrossProduct(f_right);
    tangent = f_right;
    bitangent = tangent.CrossProduct(normal);

    Vertex3 vertices4[] = {
        Vertex3(bl, normal, tangent, bitangent, color, Vector2(0.0f, 1.0f)),
        Vertex3(tr, normal, tangent, bitangent, color, Vector2(1.0f, 0.0f)),
        Vertex3(tl, normal, tangent, bitangent, color, Vector2(0.0f, 0.0f)),
        Vertex3(bl, normal, tangent, bitangent, color, Vector2(0.0f, 1.0f)),
        Vertex3(br, normal, tangent, bitangent, color, Vector2(1.0f, 1.0f)),
        Vertex3(tr, normal, tangent, bitangent, color, Vector2(1.0f, 0.0f)),
    };
    DrawVertexArrays(vertices4, 6);

    //Left-Side Face, 5
    bl = origin + negXExtents * right + negYExtents * up + posZExtents * towards;
    tl = origin + negXExtents * right + posYExtents * up + posZExtents * towards;
    br = origin + negXExtents * right + negYExtents * up;
    tr = origin + negXExtents * right + posYExtents * up;

    f_right = (-1) * towards;
    f_up = up;
    normal = f_up.CrossProduct(f_right);
    tangent = f_right;
    bitangent = tangent.CrossProduct(normal);

    Vertex3 vertices5[] = {
        Vertex3(bl, normal, tangent, bitangent, color, Vector2(0.0f, 1.0f)),
        Vertex3(tr, normal, tangent, bitangent, color, Vector2(1.0f, 0.0f)),
        Vertex3(tl, normal, tangent, bitangent, color, Vector2(0.0f, 0.0f)),
        Vertex3(bl, normal, tangent, bitangent, color, Vector2(0.0f, 1.0f)),
        Vertex3(br, normal, tangent, bitangent, color, Vector2(1.0f, 1.0f)),
        Vertex3(tr, normal, tangent, bitangent, color, Vector2(1.0f, 0.0f)),
    };
    DrawVertexArrays(vertices5, 6);

    //Back Face, 6
    bl = origin + posXExtents * right + negYExtents * up + posZExtents * towards;
    tl = origin + posXExtents * right + posYExtents * up + posZExtents * towards;
    br = origin + negXExtents * right + negYExtents * up + posZExtents * towards;
    tr = origin + negXExtents * right + posYExtents * up + posZExtents * towards;

    f_right = (-1) * right;
    f_up = up;
    normal = f_up.CrossProduct(f_right);
    tangent = f_right;
    bitangent = tangent.CrossProduct(normal);

    Vertex3 vertices6[] = {
        Vertex3(bl, normal, tangent, bitangent, color, Vector2(0.0f, 1.0f)),
        Vertex3(tr, normal, tangent, bitangent, color, Vector2(1.0f, 0.0f)),
        Vertex3(tl, normal, tangent, bitangent, color, Vector2(0.0f, 0.0f)),
        Vertex3(bl, normal, tangent, bitangent, color, Vector2(0.0f, 1.0f)),
        Vertex3(br, normal, tangent, bitangent, color, Vector2(1.0f, 1.0f)),
        Vertex3(tr, normal, tangent, bitangent, color, Vector2(1.0f, 0.0f)),
    };
    DrawVertexArrays(vertices6, 6);
}

//*******************************************************************
void SimpleRenderer::DrawCubeMeshFromOBJ()
{
    OBJLoader objLoader = OBJLoader("Data/OBJ/cube.obj");

    Vertex3 vertices[36];
    for (int vertexIndex = 0; vertexIndex < (int)objLoader.m_outTriangleVertices.size(); vertexIndex++)
    {
        vertices[vertexIndex] = objLoader.m_outTriangleVertices[vertexIndex];
    }
    DrawVertexArrays(vertices, 36);
    //DrawIndexArrays(ePrimitiveType::PRIMITIVE_TRIANGLES, vertices, 36);
}

//*******************************************************************
void SimpleRenderer::DrawDiamondFromOBJ()
{
    OBJLoader objLoader = OBJLoader("Data/OBJ/diamond.obj");

    Vertex3 vertices[27];
    for (int vertexIndex = 0; vertexIndex < (int)objLoader.m_outTriangleVertices.size(); vertexIndex++)
    {
        vertices[26 - vertexIndex] = objLoader.m_outTriangleVertices[vertexIndex];
    }
    //DrawIndexArrays(vertices, 27);
    DrawVertexArrays(ePrimitiveType::PRIMITIVE_LINES, vertices, 27);
}

//*******************************************************************
void SimpleRenderer::DrawTeapotFromOBJ()
{
    OBJLoader objLoader = OBJLoader("Data/OBJ/teapot.obj");

    Vertex3 vertices[3072];
    for (int vertexIndex = 0; vertexIndex < (int)objLoader.m_outTriangleVertices.size(); vertexIndex++)
    {
        vertices[3071 - vertexIndex] = objLoader.m_outTriangleVertices[vertexIndex];
    }
    //DrawIndexArrays(vertices, 3072);
    DrawVertexArrays(ePrimitiveType::PRIMITIVE_LINES, vertices, 27);
}

//*******************************************************************
void SimpleRenderer::DrawSpaceshipFromOBJ()
{
    //Vertex3 vertices[81408];
    /*for (int vertexIndex = 0; vertexIndex < (int)m_objSpaceship.m_outTriangleVertices.size(); vertexIndex++)
    {
        vertices[81407 - vertexIndex] = m_objSpaceship.m_outTriangleVertices[vertexIndex];
    }*/
    //DrawIndexArrays(vertices, 81408);
    DrawVertexArrays(ePrimitiveType::PRIMITIVE_TRIANGLE_STRIP, m_spaceShipVertices, 81408);
}

//*******************************************************************
void SimpleRenderer::DrawSphere(Vector3 const &position, float const &radius, int const &sliceCount, int const &stackCount, Rgba const &color /*= Rgba::WHITE*/)
{
    UNUSED(position);

    //Vertex(x, y, z, normal, tangent, uv.x, uv.y, r, g, b, a)
    //Vertex(0,radius,0, 0,      1,      0,    1,  0, 0, 0, 0)

    //(sliceCount + 1) * (stackCount - 2)
    int const NUM_VERTEXES = (21 * 19) + 2;
    Vertex3 vertices[NUM_VERTEXES];

    vertices[0] = Vertex3(Vector3(0.f, radius, 0.f), color, Vector2(0.f, 1.f));
    float phiStep = fPI / (float)stackCount;
    float thetaStep = (2.f * fPI) / (float)sliceCount;

    int currentIndex = 1;
    for (int i = 1; i <= stackCount - 1; i++)
    {
        float phi = (float)i * phiStep;
        for (int j = 0; j <= sliceCount; j++)
        {
            float theta = (float)j * thetaStep;
            Vector3 p = Vector3(radius * sin(phi) * cos(theta), radius * cos(phi), radius * sin(phi) * sin(theta));
            Vector3 t = Vector3(-radius * sin(phi) * sin(theta), 0, radius * sin(phi) * cos(theta));
            t.Normalize();
            Vector3 n = p;
            n.Normalize();
            Vector2 uv = Vector2(theta / (fPI * 2), phi / fPI);

            vertices[currentIndex] = Vertex3(p, color, uv);
            currentIndex++;
        }
    }
    vertices[currentIndex] = Vertex3(Vector3(0.f, -radius, 0.f), color, Vector2(0.f, 1.f));

    DrawVertexArrays(vertices, NUM_VERTEXES);
}

//*******************************************************************
void SimpleRenderer::DrawMesh()
{
    std::vector<Vertex3>& vertices = m_meshBuilderCube->m_vertices;
    DrawVertexArrays(&vertices[0], (int)vertices.size());
}

//*******************************************************************
void SimpleRenderer::DrawMeshSkinned()
{
    std::vector<Vertex3>& vertices = m_meshBuilderCube->m_vertices;
    DrawVertexArrays(&vertices[0], (int)vertices.size());
}

//*******************************************************************
void SimpleRenderer::DrawSkeleton()
{
    for (int jointTransformIndex = 0; jointTransformIndex < (int)m_skeletonCube->m_jointTransforms.size(); jointTransformIndex++)
    {
        std::string parentName = m_skeletonCube->GetParentJointName(jointTransformIndex);
        if (parentName != "")
        {
            Matrix4 jointTransform = m_skeletonCube->GetJointTransform(jointTransformIndex);
            Vector3 myPosition = Vector3(jointTransform.m_translation.x, jointTransform.m_translation.y, jointTransform.m_translation.z);
            Vertex3 myVertex = Vertex3(myPosition, Rgba::WHITE, Vector2(0.f, 0.f));

            Matrix4 parentjointTransform = m_skeletonCube->GetParentJointTransform(jointTransformIndex);
            Vector3 parentPosition = Vector3(parentjointTransform.m_translation.x, parentjointTransform.m_translation.y, parentjointTransform.m_translation.z);
            Vertex3 parentVertex = Vertex3(parentPosition, Rgba::WHITE, Vector2(0.f, 0.f));

            //Draw a line;
            Vertex3 vertices[] = {
                myVertex,
                parentVertex
            };

            DrawVertexArrays(ePrimitiveType::PRIMITIVE_LINES, vertices, 2);
        }
    }
}

//*******************************************************************
void SimpleRenderer::DrawCubeAnimation(float currentTimeInSeconds)
{
    float durationTime = m_motionCube->GetDuration();
    float frameRate = m_motionCube->m_framerate;
    float currentPoseTime = fmod(currentTimeInSeconds, durationTime);
    unsigned int poseIndex = (unsigned int)floor(currentPoseTime * frameRate);

    for (int jointTransformIndex = 0; jointTransformIndex < (int)m_motionCube->m_poses[poseIndex]->m_localTranforms.size(); jointTransformIndex++)
    {
        std::string parentName = m_skeletonCube->GetParentJointName(jointTransformIndex);
        if (parentName != "")
        {
            Matrix4 jointTransformLocal = m_motionCube->m_poses[poseIndex]->m_localTranforms[jointTransformIndex];//m_skeletonCube->GetJointTransform(jointTransformIndex);
            Matrix4 jointTransformWorld = jointTransformLocal;
            unsigned int currentJointIndex = jointTransformIndex;
            while (parentName != "")
            {
                unsigned int parentJointTransformIndex = m_skeletonCube->GetParentIndex(currentJointIndex);
                Matrix4 parentJointTransformLocal = m_motionCube->m_poses[poseIndex]->m_localTranforms[parentJointTransformIndex];
                jointTransformWorld = MultiplyMatrix4ByMatrix4(jointTransformWorld, parentJointTransformLocal);
                parentName = m_skeletonCube->GetParentJointName(parentJointTransformIndex);
                currentJointIndex = parentJointTransformIndex;
            }
            Vector3 myPosition = Vector3(jointTransformWorld.m_translation.x, jointTransformWorld.m_translation.y, jointTransformWorld.m_translation.z);
            Vertex3 myVertex = Vertex3(myPosition, Rgba::WHITE, Vector2(0.f, 0.f));

            unsigned int parentJointTransformIndex = m_skeletonCube->GetParentIndex(jointTransformIndex);
            Matrix4 parentJointTransformLocal = m_motionCube->m_poses[poseIndex]->m_localTranforms[parentJointTransformIndex];
            Matrix4 parentJointTransformWorld = parentJointTransformLocal;
            unsigned int currentParentJointIndex = parentJointTransformIndex;
            parentName = m_skeletonCube->GetParentJointName(currentParentJointIndex);
            while (parentName != "")
            {
                parentJointTransformIndex = m_skeletonCube->GetParentIndex(currentParentJointIndex);
                parentJointTransformLocal = m_motionCube->m_poses[poseIndex]->m_localTranforms[parentJointTransformIndex];
                parentJointTransformWorld = MultiplyMatrix4ByMatrix4(parentJointTransformWorld, parentJointTransformLocal);
                parentName = m_skeletonCube->GetParentJointName(parentJointTransformIndex);
                currentParentJointIndex = parentJointTransformIndex;
            }
            Vector3 parentPosition = Vector3(parentJointTransformWorld.m_translation.x, parentJointTransformWorld.m_translation.y, parentJointTransformWorld.m_translation.z);
            Vertex3 parentVertex = Vertex3(parentPosition, Rgba::WHITE, Vector2(0.f, 0.f));

            Vertex3 vertices[] = {
                myVertex,
                parentVertex
            };

            DrawVertexArrays(ePrimitiveType::PRIMITIVE_LINES, vertices, 2);
        }
    }

    //for (unsigned int poseIndex = 0; poseIndex < m_motionCube->m_poses.size();  poseIndex++)
    //{
    //    for (int jointTransformIndex = 0; jointTransformIndex < (int)m_motionCube->m_poses[poseIndex]->m_localTranforms.size(); jointTransformIndex++)
    //    {
    //        std::string parentName = m_skeletonCube->GetParentJointName(jointTransformIndex);
    //        if (parentName != "")
    //        {
    //            Matrix4 jointTransform = m_skeletonCube->GetJointTransform(jointTransformIndex);
    //            Vector3 myPosition = Vector3(jointTransform.m_translation.x, jointTransform.m_translation.y, jointTransform.m_translation.z);
    //            Vertex3 myVertex = Vertex3(myPosition, Rgba::WHITE, Vector2(0.f, 0.f));

    //            Matrix4 parentjointTransform = m_skeletonCube->GetParentJointTransform(jointTransformIndex);
    //            Vector3 parentPosition = Vector3(parentjointTransform.m_translation.x, parentjointTransform.m_translation.y, parentjointTransform.m_translation.z);
    //            Vertex3 parentVertex = Vertex3(parentPosition, Rgba::WHITE, Vector2(0.f, 0.f));

    //            //Draw a line;
    //            Vertex3 vertices[] = {
    //                myVertex,
    //                parentVertex
    //            };

    //            DrawVertexArrays(ePrimitiveType::PRIMITIVE_LINES, vertices, 2);
    //        }
    //    }
    //}

}

//*******************************************************************
void SimpleRenderer::DrawSkinnedCubeAnimation(float currentTimeInSeconds)
{
    float durationTime = m_motionCube->GetDuration();
    float frameRate = m_motionCube->m_framerate;
    float currentPoseTime = fmod(currentTimeInSeconds, durationTime);
    unsigned int poseIndex = (unsigned int)floor(currentPoseTime * frameRate);

    Pose pose;
    m_motionCube->Evaluate(pose, currentTimeInSeconds);

    CalculateSkinMatrices(&pose, poseIndex);//m_skeletonInstances[poseIndex]->m_skinTransforms;


    for (int jointTransformIndex = 0; jointTransformIndex < (int)m_motionCube->m_poses[poseIndex]->m_localTranforms.size(); jointTransformIndex++)
    {
        std::string parentName = m_skeletonCube->GetParentJointName(jointTransformIndex);
        if (parentName != "")
        {
            Matrix4 jointTransformLocal = m_motionCube->m_poses[poseIndex]->m_localTranforms[jointTransformIndex];//m_skeletonCube->GetJointTransform(jointTransformIndex);
            Matrix4 jointTransformWorld = jointTransformLocal;
            unsigned int currentJointIndex = jointTransformIndex;
            while (parentName != "")
            {
                unsigned int parentJointTransformIndex = m_skeletonCube->GetParentIndex(currentJointIndex);
                Matrix4 parentJointTransformLocal = m_motionCube->m_poses[poseIndex]->m_localTranforms[parentJointTransformIndex];
                jointTransformWorld = MultiplyMatrix4ByMatrix4(jointTransformWorld, parentJointTransformLocal);
                parentName = m_skeletonCube->GetParentJointName(parentJointTransformIndex);
                currentJointIndex = parentJointTransformIndex;
            }
            Vector3 myPosition = Vector3(jointTransformWorld.m_translation.x, jointTransformWorld.m_translation.y, jointTransformWorld.m_translation.z);
            Vertex3 myVertex = Vertex3(myPosition, Rgba::WHITE, Vector2(0.f, 0.f));

            unsigned int parentJointTransformIndex = m_skeletonCube->GetParentIndex(jointTransformIndex);
            Matrix4 parentJointTransformLocal = m_motionCube->m_poses[poseIndex]->m_localTranforms[parentJointTransformIndex];
            Matrix4 parentJointTransformWorld = parentJointTransformLocal;
            unsigned int currentParentJointIndex = parentJointTransformIndex;
            parentName = m_skeletonCube->GetParentJointName(currentParentJointIndex);
            while (parentName != "")
            {
                parentJointTransformIndex = m_skeletonCube->GetParentIndex(currentParentJointIndex);
                parentJointTransformLocal = m_motionCube->m_poses[poseIndex]->m_localTranforms[parentJointTransformIndex];
                parentJointTransformWorld = MultiplyMatrix4ByMatrix4(parentJointTransformWorld, parentJointTransformLocal);
                parentName = m_skeletonCube->GetParentJointName(parentJointTransformIndex);
                currentParentJointIndex = parentJointTransformIndex;
            }
            Vector3 parentPosition = Vector3(parentJointTransformWorld.m_translation.x, parentJointTransformWorld.m_translation.y, parentJointTransformWorld.m_translation.z);
            Vertex3 parentVertex = Vertex3(parentPosition, Rgba::WHITE, Vector2(0.f, 0.f));

            Vertex3 vertices[] = {
                myVertex,
                parentVertex
            };

            DrawVertexArrays(ePrimitiveType::PRIMITIVE_LINES, vertices, 2);
        }
    }
}

//*******************************************************************
void SimpleRenderer::DrawPoint(Vector3 const &position, Rgba const &color /*= Rgba::WHITE*/)
{
    Vector3 r = Vector3(1.0f, 0.0f, 0.0f);
    Vector3 u = Vector3(0.0f, 1.0f, 0.0f);
    Vector3 f = Vector3(0.0f, 0.0f, 1.0f);

    Vertex3 vertices[] = {
        Vertex3(position - r, color, Vector2(0.0f, 0.0f)),
        Vertex3(position + r, color, Vector2(0.0f, 0.0f)),
        Vertex3(position - u, color, Vector2(0.0f, 0.0f)),
        Vertex3(position + u, color, Vector2(0.0f, 0.0f)),
        Vertex3(position - f, color, Vector2(0.0f, 0.0f)),
        Vertex3(position + f, color, Vector2(0.0f, 0.0f)),
    };

    DrawVertexArrays(ePrimitiveType::PRIMITIVE_LINES, vertices, ARRAYSIZE(vertices));
}

//*******************************************************************
void SimpleRenderer::SetConstantBuffer(unsigned int idx, ConstantBuffer *cb)
{
    m_context->SetConstantBuffer(idx, cb);
}

//*******************************************************************
int SimpleRenderer::GetTextWidth(Font *font, char const *text)
{
    unsigned int cursor = 0;

    char const *c = text;
    int prevCharCode = -1;

    while (*c != NULL)
    {
        glyph_info_t tempGlyph = font->GetGlyph(*c);
        glyph_info_t const *glyph = &tempGlyph;

        if (glyph == nullptr)
        {
            tempGlyph = font->GetInvalidGlyph();
            glyph = &tempGlyph;
            //or skip
        }

        cursor += glyph->m_xAdvance;
        prevCharCode = *c;
        ++c;

        int kerning = font->GetKerningOffset((char)prevCharCode, (char)*c);
        cursor += kerning;
    }

    return cursor;
}

//*******************************************************************
int SimpleRenderer::GetTextHeight(Font *font, char const *text)
{
    unsigned int cursor = 0;

    char const *c = text;
    int prevCharCode = -1;

    while (*c != NULL)
    {
        glyph_info_t tempGlyph = font->GetGlyph(*c);
        glyph_info_t const *glyph = &tempGlyph;

        if (glyph == nullptr)
        {
            tempGlyph = font->GetInvalidGlyph();
            glyph = &tempGlyph;
            //or skip
        }

        if ((signed int)glyph->m_height > (signed int)cursor)
        {
            cursor = glyph->m_height;
        }
        prevCharCode = *c;
        ++c;
    }

    return cursor;
}

//*******************************************************************
void SimpleRenderer::DrawText(Font* font, int sx, int sy, char const *text, Rgba color)
{
    Vector2 cursor = Vector2((float)sx, (float)sy);

    char const *c = text;
    int prevCharCode = -1;

    //assume one texture
    SetTexture(m_fontTexture);

    //std::vector<Vertex3> vertices;
    //vertices.reserve(10000);
    while ((*c != NULL))
    {
        glyph_info_t tempGlyph = font->GetGlyph(*c);
        glyph_info_t const *glyph = &tempGlyph;

        if (glyph == nullptr)
        {
            tempGlyph = font->GetInvalidGlyph();
            glyph = &tempGlyph;
            //or skip
            //continue;
        }

        Vector2 bottomLeft;
        Vector2 topRight;

        Vector2 topLeft = cursor + Vector2((float)glyph->m_xOffset, (float)-glyph->m_yOffset);
        bottomLeft = topLeft + Vector2(0, (float)-glyph->m_height);
        topRight = topLeft + Vector2((float)glyph->m_width, 0);
        //Vector2 bottomRight = bottomLeft + Vector2(glyph->m_width, 0);

        Vector2 uvTopLeft = Vector2((float)glyph->m_x / (float)font->m_scaleW, (float)glyph->m_y / (float)font->m_scaleH);
        Vector2 uvBottomLeft = uvTopLeft + Vector2(0, (float)glyph->m_height / (float)font->m_scaleH);
        Vector2 uvTopRight = uvTopLeft + Vector2((float)glyph->m_width / (float)font->m_scaleW, 0);

        //add_quad_to_vertices(vertices, glyph), optimization
        DrawQuad(bottomLeft, topRight, uvBottomLeft, uvTopRight, color);


        cursor += Vector2((float)glyph->m_xAdvance, 0);
        prevCharCode = *c;
        ++c;

        int kerning = font->GetKerningOffset((char)prevCharCode, (char)*c);
        cursor += Vector2((float)kerning, 0);
    }
};

//*******************************************************************
void SimpleRenderer::DrawText(Font* font, int sx, int sy, int cellHeight, char const *text, Rgba color)
{
    Vector2 cursor = Vector2((float)sx, (float)sy);

    char const *c = text;
    int prevCharCode = -1;

    //assume one texture
    SetTexture(m_fontTexture);

    //std::vector<Vertex3> vertices;
    //vertices.reserve(10000);

    while ((*c != NULL))
    {
        glyph_info_t tempGlyph = font->GetGlyph(*c);
        glyph_info_t const *glyph = &tempGlyph;

        if (glyph == nullptr)
        {
            tempGlyph = font->GetInvalidGlyph();
            glyph = &tempGlyph;
            //or skip
        }

        Vector2 bottomLeft;
        Vector2 topRight;

        float xOffset = ((float)glyph->m_xOffset * (float)cellHeight) / (float)font->m_size;
        float yOffset = ((float)glyph->m_yOffset * (float)cellHeight) / (float)font->m_size;
        float height = ((float)glyph->m_height * (float)cellHeight) / (float)font->m_size;
        float width = ((float)glyph->m_width * (float)cellHeight) / (float)font->m_size;

        Vector2 topLeft = cursor + Vector2(xOffset, -yOffset);
        bottomLeft = topLeft + Vector2(0, -height);
        topRight = topLeft + Vector2(width, 0);
        //Vector2 bottomRight = bottomLeft + Vector2(glyph->m_width, 0);

        Vector2 uvTopLeft = Vector2((float)glyph->m_x / (float)font->m_scaleW, (float)glyph->m_y / (float)font->m_scaleH);
        Vector2 uvBottomLeft = uvTopLeft + Vector2(0, (float)glyph->m_height / (float)font->m_scaleH);
        Vector2 uvTopRight = uvTopLeft + Vector2((float)glyph->m_width / (float)font->m_scaleW, 0);

        //add_quad_to_vertices(vertices, glyph), optimization
        DrawQuad(bottomLeft, topRight, uvBottomLeft, uvTopRight, color);

        float xAdvance = ((float)glyph->m_xAdvance * (float)cellHeight) / (float)font->m_size;
        cursor += Vector2(xAdvance, 0);
        prevCharCode = *c;
        ++c;

        int kerning = font->GetKerningOffset((char)prevCharCode, (char)*c);
        float scaledKerning = ((float)kerning * (float)cellHeight) / (float)font->m_size;
        cursor += Vector2(scaledKerning, 0);
    }
    DisableBlend();
}

//*******************************************************************
void SimpleRenderer::DrawText(Font* font, int sx, int sy, int h, int w, char const *text, Rgba color)
{
    Vector2 cursor = Vector2((float)sx, (float)sy);

    char const *c = text;
    int prevCharCode = -1;

    //assume one texture
    SetTexture(m_fontTexture);

    //std::vector<Vertex3> vertices;
    //vertices.reserve(10000);

    while ((*c != NULL))
    {
        glyph_info_t tempGlyph = font->GetGlyph(*c);
        glyph_info_t const *glyph = &tempGlyph;

        if (glyph == nullptr)
        {
            tempGlyph = font->GetInvalidGlyph();
            glyph = &tempGlyph;
            //or skip
        }

        Vector2 bottomLeft;
        Vector2 topRight;

        float xOffset = ((float)glyph->m_xOffset * (float)w) / (float)font->m_size;
        float yOffset = ((float)glyph->m_yOffset * (float)h) / (float)font->m_size;
        float height = ((float)glyph->m_height * (float)h) / (float)font->m_size;
        float width = ((float)glyph->m_width * (float)w) / (float)font->m_size;

        Vector2 topLeft = cursor + Vector2(xOffset, -yOffset);
        bottomLeft = topLeft + Vector2(0, -height);
        topRight = topLeft + Vector2(width, 0);
        //Vector2 bottomRight = bottomLeft + Vector2(glyph->m_width, 0);

        Vector2 uvTopLeft = Vector2((float)glyph->m_x / (float)font->m_scaleW, (float)glyph->m_y / (float)font->m_scaleH);
        Vector2 uvBottomLeft = uvTopLeft + Vector2(0, (float)glyph->m_height / (float)font->m_scaleH);
        Vector2 uvTopRight = uvTopLeft + Vector2((float)glyph->m_width / (float)font->m_scaleW, 0);

        //add_quad_to_vertices(vertices, glyph), optimization
        DrawQuad(bottomLeft, topRight, uvBottomLeft, uvTopRight, color);

        float xAdvance = ((float)glyph->m_xAdvance * (float)w) / (float)font->m_size;
        cursor += Vector2(xAdvance, 0);
        prevCharCode = *c;
        ++c;

        int kerning = font->GetKerningOffset((char)prevCharCode, (char)*c);
        float scaledKerning = ((float)kerning * (float)w) / (float)font->m_size;
        cursor += Vector2(scaledKerning, 0);
    }

}

//*******************************************************************
void SimpleRenderer::DrawText(Font* font, int sx, int sy, int h, int w, char const *text, Rgba color, std::vector<Vertex3>& vertexes)
{
    Vector2 cursor = Vector2((float)sx, (float)sy);

    char const *c = text;
    int prevCharCode = -1;

    //assume one texture
    SetTexture(m_fontTexture);

    //std::vector<Vertex3> vertices;
    //vertices.reserve(10000);

    while ((*c != NULL))
    {
        glyph_info_t tempGlyph = font->GetGlyph(*c);
        glyph_info_t const *glyph = &tempGlyph;

        if (glyph == nullptr)
        {
            tempGlyph = font->GetInvalidGlyph();
            glyph = &tempGlyph;
            //or skip
        }

        Vector2 bottomLeft;
        Vector2 topRight;

        float xOffset = ((float)glyph->m_xOffset * (float)w) / (float)font->m_size;
        float yOffset = ((float)glyph->m_yOffset * (float)h) / (float)font->m_size;
        float height = ((float)glyph->m_height * (float)h) / (float)font->m_size;
        float width = ((float)glyph->m_width * (float)w) / (float)font->m_size;

        Vector2 topLeft = cursor + Vector2(xOffset, -yOffset);
        bottomLeft = topLeft + Vector2(0, -height);
        topRight = topLeft + Vector2(width, 0);
        //Vector2 bottomRight = bottomLeft + Vector2(glyph->m_width, 0);

        Vector2 uvTopLeft = Vector2((float)glyph->m_x / (float)font->m_scaleW, (float)glyph->m_y / (float)font->m_scaleH);
        Vector2 uvBottomLeft = uvTopLeft + Vector2(0, (float)glyph->m_height / (float)font->m_scaleH);
        Vector2 uvTopRight = uvTopLeft + Vector2((float)glyph->m_width / (float)font->m_scaleW, 0);

        //add_quad_to_vertices(vertices, glyph), optimization
        DrawQuad(bottomLeft, topRight, uvBottomLeft, uvTopRight, color, vertexes);

        float xAdvance = ((float)glyph->m_xAdvance * (float)w) / (float)font->m_size;
        cursor += Vector2(xAdvance, 0);
        prevCharCode = *c;
        ++c;

        int kerning = font->GetKerningOffset((char)prevCharCode, (char)*c);
        float scaledKerning = ((float)kerning * (float)w) / (float)font->m_size;
        cursor += Vector2(scaledKerning, 0);
    }

}

//*******************************************************************
void SimpleRenderer::DrawTextCentered(Font* font, Vector2 position, char const *text, Rgba color)
{
    int halfTextHeight = GetTextHeight(font, text) / 2;
    int halfTextWidth = GetTextWidth(font, text) / 2;

    char const c = text[0];
    glyph_info_t tempGlyph = font->GetGlyph(c);
    glyph_info_t const *glyph = &tempGlyph;

    if (glyph == nullptr)
    {
        tempGlyph = font->GetInvalidGlyph();
        glyph = &tempGlyph;
        //or skip
    }

    int sx = (int)position.x - halfTextWidth;
    int sy = (int)(position.y + (glyph->m_yOffset) + (halfTextHeight));

    DrawText(font, sx, sy, text, color);
}

//*******************************************************************
void SimpleRenderer::DrawTextCenteredAndScaledToFitAABB2D(Font* font, AABB2D aabb2d, char const *text, Rgba color)
{
    int aabb2dWidth = (int)(aabb2d.m_maxs.x - aabb2d.m_mins.x);
    int aabb2dHeight = (int)(aabb2d.m_maxs.y - aabb2d.m_mins.y);

    int sx = (int)aabb2d.m_mins.x;
    int sy = (int)aabb2d.m_mins.y;

    DrawText(font, sx, sy, aabb2dHeight, aabb2dWidth, text, color);
}

//*******************************************************************
void SimpleRenderer::DrawTextCenteredAndScaledToFitAABB2D(Font* font, AABB2D aabb2d, char const *text, Rgba color, std::vector<Vertex3>& vertexes)
{
    //int halfTextHeight = GetTextHeight(font, text) / 2;
    //int halfTextWidth = GetTextWidth(font, text) / 2;

    char const c = text[0];
    glyph_info_t tempGlyph = font->GetGlyph(c);
    glyph_info_t const *glyph = &tempGlyph;

    if (glyph == nullptr)
    {
        tempGlyph = font->GetInvalidGlyph();
        glyph = &tempGlyph;
    }

    int positionX = (int)((aabb2d.m_maxs.x - aabb2d.m_mins.x) / 2.f) + (int)aabb2d.m_mins.x;
    int positionY = (int)((aabb2d.m_maxs.y - aabb2d.m_mins.y) / 2.f) + (int)aabb2d.m_mins.y;

    int aabb2dWidth = (int)(aabb2d.m_maxs.x - aabb2d.m_mins.x);
    int aabb2dHeight = (int)(aabb2d.m_maxs.y - aabb2d.m_mins.y);

    //float yOffset = ((float)glyph->m_yOffset * (float)aabb2dHeight) / (float)font->m_size;
    //float h = ((float)glyph->m_height * (float)aabb2dHeight) / (float)font->m_size;
    //float w = ((float)glyph->m_width * (float)aabb2dWidth) / (float)font->m_size;

    int sx = positionX - (int)(aabb2dWidth / 2);
    int sy = positionY + (int)(aabb2dHeight / 2);

    //EnableBlend(BLEND_SRC_ALPHA, BLEND_INV_SRC_ALPHA);
    DrawText(font, sx, sy, aabb2dHeight, aabb2dWidth, text, color, vertexes);
    //DisableBlend();
}

//*******************************************************************
void SimpleRenderer::LoadSkinMatrices(Motion *motion, Skeleton *skeleton)
{
    for (int poseIndex = 0; poseIndex < (int)motion->m_poses.size(); poseIndex++)
    {
        SkeletonInstance* newSkeletonInstance = new SkeletonInstance(skeleton);
        newSkeletonInstance->m_currentPose = *motion->m_poses[poseIndex];
        std::vector<Matrix4>& poseLocalTransforms = motion->m_poses[poseIndex]->m_localTranforms;

        std::vector<Matrix4> skinMatrices;
        for (int localTransformsIndex = 0; localTransformsIndex < (int)poseLocalTransforms.size(); localTransformsIndex++)
        {
            Matrix4 initialTransform = Matrix4();
            skinMatrices.push_back(initialTransform);
        }
        StructuredBuffer* newSkinTransform = new StructuredBuffer(m_device, &skinMatrices, sizeof(Matrix4), (unsigned int)skinMatrices.size());
        newSkeletonInstance->m_skinTransforms = newSkinTransform;
        m_skeletonInstances.push_back(newSkeletonInstance);

        if (poseIndex == 0)
        {
            m_structureBuffer = new StructuredBuffer(m_device, &skinMatrices, sizeof(Matrix4), (unsigned int)skinMatrices.size());
            SetStructureBuffer(m_structureBuffer);
        }
    }
}

//*******************************************************************
//void SimpleRenderer::CalculateSkinMatrices()
//{
//    for (int skeletonInstanceIndex = 0; skeletonInstanceIndex < (int)m_skeletonInstances.size(); skeletonInstanceIndex++)
//    {
//        SkeletonInstance* currentSkeletonInstance = m_skeletonInstances[skeletonInstanceIndex];
//        std::vector<Matrix4>& poseLocalTransforms = currentSkeletonInstance->m_currentPose.m_localTranforms;
//
//        std::vector<Matrix4> updatedSkinMatrices;
//        for (int localTransformsIndex = 0; localTransformsIndex < (int)poseLocalTransforms.size(); localTransformsIndex++)
//        {
//            Matrix4 bindPoseWorldPosition = currentSkeletonInstance->GetJointBindPoseGlobalTransform(localTransformsIndex);
//            Matrix4 currentPoseWorldPosition = currentSkeletonInstance->GetJointCurrentPoseGlobalTransform(localTransformsIndex);
//
//            Matrix4 updatedSkinTransform = MultiplyMatrix4ByMatrix4(bindPoseWorldPosition.GetInverse(), currentPoseWorldPosition);
//            updatedSkinTransform.transpose();
//            updatedSkinMatrices.push_back(updatedSkinTransform);
//        }
//
//        currentSkeletonInstance->m_skinTransforms->Update(m_context, updatedSkinMatrices.data());
//    }
//    
//}


//*******************************************************************
void SimpleRenderer::CalculateSkinMatrices(Pose* pose, unsigned int poseIndex)
{
    UNUSED(poseIndex);

    //#THIS_Fs_CRAP_UP
    //SkeletonInstance* currentSkeletonInstance = m_skeletonInstances[skeletonInstanceIndex];
    //std::vector<Matrix4>& poseLocalTransforms = pose->m_localTranforms;//m_motionCube->m_poses[skeletonInstanceIndex]->m_localTranforms;

    std::vector<Matrix4> updatedSkinMatrices;
    unsigned int jointCount = m_skeletonCube->GetJointCount();
    updatedSkinMatrices.reserve(jointCount);

    for (unsigned int index = 0; index < jointCount; index++)
    {
        Matrix4 currentPoseWorldPosition = pose->GetJointCurrentPoseGlobalTransform(index, m_skeletonCube);
        Matrix4 initialPose = m_skeletonCube->m_jointTransforms[index];

        Matrix4 invInitialPose = initialPose.GetInverse();

        Matrix4 bindPose = MultiplyMatrix4ByMatrix4(invInitialPose, currentPoseWorldPosition);
        bindPose.transpose();
        updatedSkinMatrices.push_back(bindPose);
    }

    m_structureBuffer->Update(m_context, updatedSkinMatrices.data());
    //SetStructureBuffer(m_structureBuffer);
}


//*******************************************************************
Texture2D* SimpleRenderer::LoadTextureAsync(char const* filename, on_texture_loaded_cb cb /*= nullptr*/, void* userArg /*= nullptr*/)
{
    //PROFILE_LOG_SCOPE("LoadTextureAsync");

    Texture2D *tex = new Texture2D(m_device, filename);
    if (tex == nullptr)
        return nullptr;

    //always assume texture is not loaded
    load_texture_job_t* jobData = new load_texture_job_t();
    jobData->m_filename = filename;
    jobData->m_image = nullptr;
    jobData->m_texture = tex;

    Job* loadImage = JobCreateLoadImage(JOB_GENERIC, LoadImageFromFileJob, jobData->m_image, &jobData->m_filename);
    Job* imageToTexture = JobCreateTexture(JOB_RENDER, LoadTextureFromImageJob, tex, jobData->m_image);
    imageToTexture->dependent_on(loadImage);

    Job* cleanup = JobCreate(JOB_GENERIC, CleanupJob, jobData);
    cleanup->dependent_on(imageToTexture);
    JobDispatchAndRelease(cleanup);

    if (cb != nullptr)
    {
        Job* cbJob = JobCreate(JOB_MAIN, CallLoadTextureCallbackJob, cb, tex, userArg);
        cbJob->dependent_on(imageToTexture);

        JobDispatchAndRelease(cbJob);
    }

    JobDispatchAndRelease(loadImage);
    JobDispatchAndRelease(imageToTexture);

    return tex;
}

//*******************************************************************
ComputeShader* SimpleRenderer::CreateOrGetComputeShader(std::string computeShaderFilePath)
{
    ComputeShader* computeShader = GetComputeShader(computeShaderFilePath);
    if (computeShader)
        return computeShader;

    computeShader = CreateComputeShader(computeShaderFilePath);
    return computeShader;
}

//*******************************************************************
ComputeShader* SimpleRenderer::GetComputeShader(std::string computeShaderFilePath)
{
    return s_computeShaders[computeShaderFilePath];
}

//*******************************************************************
ComputeShader* SimpleRenderer::CreateComputeShader(std::string computeShaderFilePath)
{
    //check for failures on the filepath
    ComputeShader *computeShader = m_device->CreateComputeShaderFromHlslFile(computeShaderFilePath.c_str());
    s_computeShaders[computeShaderFilePath] = computeShader;
    return s_computeShaders[computeShaderFilePath];
}
