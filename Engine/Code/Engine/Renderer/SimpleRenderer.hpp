#pragma once
#include "Engine/RHI/RHITypes.hpp"
#include "Engine/Core/Rgba.hpp"
#include "Engine/RHI/RHIDeviceContext.hpp"
#include "Engine/RHI/RHIInstance.hpp"
#include "Engine/RHI/ShaderProgram.hpp"
#include "Engine/RHI/RasterState.hpp"
#include "Engine/RHI/BlendState.hpp"
#include "Engine/RHI/ConstantBuffer.hpp"
#include "Engine/MeshBuilder/StructuredBuffer.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/Matrix4.hpp"
#include "Engine/Tools/fbx.h"
#include "Engine/Font/Font.hpp"
#include "Engine/RHI/IndexBuffer.hpp"
#include "Engine/MeshBuilder/OBJLoader.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Job/Job.hpp"
#include "Engine/RHI/ComputeJob.hpp"
#include "Engine/Math/Disc2D.hpp"


class Sampler;

#define MATRIX_BUFFER_INDEX (0)
#define TIME_BUFFER_INDEX (1)
#define LIGHT_BUFFER_INDEX (2)
#define STRUCTURE_BUFFER_INDEX (3)
#define MOUSE_BUFFER_INDEX (5)

//Acts as a go between for the RHI and Game.
//Take out RHI in game solution, and replace with simplerenderer equivalents

typedef bool(*WindowMessageHandler)(HWND, UINT, WPARAM, LPARAM);

struct time_constant_t
{
    float gameTime;
    float systemTime;
    float gameDeltaTime;
    float systemDeltaTime;
};

struct matrix_buffer_t
{
    matrix_buffer_t()
        : eyePosition(Vector4())
    {}

    Matrix4 model;
    Matrix4 view;
    Matrix4 projection;

    Vector4 eyePosition;
};

struct blend_state_t
{
    bool enabled;
    eBlendFactor srcFactor;
    eBlendFactor destFactor;
};

struct light_parts
{
    light_parts()
        : m_lightColor(255, 255, 255, 0)
        , m_lightPosition(0, 0, 0, 1)
        , m_attenuation(1, 0, 0, 0)
        , m_specAttenuation(1, 0, 0, 0)
    {}

    Vector4 m_lightColor;
    Vector4 m_lightPosition;
    Vector4 m_attenuation;
    Vector4 m_specAttenuation;
};

struct light_buffer_t
{
    light_buffer_t()
        : m_ambient(255, 255, 255, 255)
        , m_lightColor(255, 255, 255, 0)
        , m_lightPosition(0, 0, 0, 1)
        , m_attenuation(1, 0, 0, 0)
        , m_specAttenuation(1, 0, 0, 0)
        , m_specFactor(0.0f)
        , m_specPower(1.0f)
    {
        for (int lightIndex = 0; lightIndex < 8; lightIndex++)
        {
            m_lights[lightIndex] = light_parts();
        }
    }

    Vector4 m_ambient;

    light_parts m_lights[8];
    Vector4 m_lightColor;
    Vector4 m_lightPosition;
    Vector4 m_attenuation;
    Vector4 m_specAttenuation;

    float m_specFactor;
    float m_specPower;
    Vector2 m_unused;
};

struct structure_buffer_t
{

};

struct mouse_position_t
{
    Vector4 m_mousePosition;
};


class SimpleRenderer
{
public:
public:
	SimpleRenderer();
	~SimpleRenderer();

	void SetUp(WindowMessageHandler windowMessageHandler);
	void SetTitle(char const* title);
	void Destroy();
    void SetViewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h);

    void Update(float deltaSeconds);
	void ProcessMessages();
	bool IsClosed() const;

    void LoadTextures();
    void LoadShaders();

    SpriteSheet CreateSpriteSheet(const std::string& imageFilePath, int tilesWide, int tilesHigh);
    SpriteSheet* CreateSpriteSheetPointer(const std::string& imageFilePath, int tilesWide, int tilesHigh);
    BitmapFont* GetFont(const std::string& bitmapFontName);
    BitmapFont* CreateOrGetFont(const std::string& bitmapFontName);
    void DrawText2D(const Vector2& startBottomLeft, const std::string& asciiText, float cellHeight, const Rgba& tint = Rgba::WHITE, const BitmapFont* font = nullptr);
    
    void SetModelMatrix(Matrix4 const &model);
    void SetViewMatrix(Matrix4 const &view);
    void SetProjectionMatrix(Matrix4 const &projection);

    void SetCameraMatrix(Matrix4 const &camera);
    void SetEyePosition(Vector3 const &eyePosition);

    void SetOrthoProjection(Vector2 const &bottomLeft, Vector2 const &topRight);
    void SetPerspectiveProjection(float const fovRadians, float aspectRatio, float const nz, float const fz);

    void EnableBlend(eBlendFactor src, eBlendFactor dest);
    void DisableBlend();

    void EnableDepthTest(bool enable);
    void EnableDepthWrite(bool enable);
    void EnableDepth(bool test, bool write);

    void ClearColor(Rgba const &color);
    void ClearDepth(float depth = 1.0f, uint8_t stencil = 0);

    void ClearTargetColor(Texture2D *target, Rgba const &color);

	void Present();
    Texture2D* CreateRenderTarget();
    void SetRenderTarget(Texture2D *color_target, Texture2D *depthStencil = nullptr);
    
	// [A02: Optional]
	// Set the viewport using percentages of the size of the current render target.
	void SetViewportAsPercent(float /*x*/, float /*y*/, float /*w*/, float /*h*/) {}

	void SetShader(ShaderProgram *shader);
	void SetTexture(unsigned int texture_index, Texture2D *texture);
	inline void SetTexture(Texture2D *texture) { SetTexture(0, texture); }

    inline void SetDiffuse(Texture2D *texture) { SetTexture(0, (texture == nullptr) ? m_whiteSample : texture); }
    inline void SetNormal(Texture2D *texture) { SetTexture(1, (texture == nullptr) ? flatNormalTexture : texture); }
    inline void SetSpec(Texture2D *texture) { SetTexture(2, (texture == nullptr) ? m_whiteSample : texture); }

    void SetStructureBuffer(unsigned int structureBufferID, StructuredBuffer *structureBuffer);
    inline void SetStructureBuffer(StructuredBuffer *structureBuffer) { SetStructureBuffer(3, structureBuffer); }

	void SetSampler(unsigned int sampler_index, Sampler *sampler);
	inline void SetSampler(Sampler *sampler) { SetSampler(0, sampler); }

    void SetAmbientLight(float intensity, Rgba const &color = Rgba::WHITE);
    void SetSpecularConstants(float const specPower, float const specFactor = 1.0f);

    void EnablePointLight(int const &lightIndex, Vector3 const &position, Rgba const &color, float intesity = 1.0f, Vector3 const &attentuation = Vector3(0, 0, 1), Vector3 const &specAttentuation = Vector3(0, 0, 1));

    void SetPointLight(int const &lightIndex, Vector3 const &position, Rgba const &color, float intensity = 1.0f, Vector3 const &attenuation = Vector3(0, 0, 1));
    void SetMousePosition(Vector2 position);
    inline void DisablePointLight() { EnablePointLight(0, Vector3(), Rgba::WHITE, 0.0f, Vector3(1.0f, 0.0f, 0.0f)); };

	// [A02]
	// Indexed is optional - though highly encouraged to do 
	// during this assignment as it is the easiest assignment to 
	// implement them with
	void Draw(const ePrimitiveType& topology, VertexBuffer *vbo, unsigned int const vertex_count);
	void DrawIndexed(const ePrimitiveType& topology, VertexBuffer* vbo, IndexBuffer* ibo, unsigned int const indexCount);

    void UpdateVBO(Vertex3 *vertices, unsigned int vertexCount);
    void DrawVertexArrays(Vertex3 *vertices, unsigned int vertexCount);
    void DrawVertexArrays(Vertex3 *vertices, unsigned int vertexCount, VertexBuffer* vbo);
    void DrawVertexArrays(ePrimitiveType type, Vertex3 *vertices, unsigned int vertexCount);
    void DrawVertexArrays(unsigned int vertexCount, VertexBuffer* vbo);
    void DrawIndexArrays(Vertex3 *vertices, unsigned int vertexCount, unsigned int *indices, unsigned int indexCount);
    void DrawIndexArrays(ePrimitiveType type, Vertex3 *vertices, unsigned int vertexCount, unsigned int *indices, unsigned int indexCount);

    void DrawLine2D(float thickness, Vector3 startPosition, Vector3 endPosition, Rgba startColor, Rgba endColor);
    void DrawSprite2D(Texture2D* texture, Vector2 uvBottomLeft, Vector2 uvTopRight, AABB2D bounds, Rgba color = Rgba::WHITE);
    void DrawSprite2DCentered(Texture2D* texture, Vector2 uvBottomLeft, Vector2 uvTopRight, Vector2 position, float width, float height, Rgba color);

    void DrawSprite2DCentered(Texture2D* texture, Vector2 uvBottomLeft, Vector2 uvTopRight, Vector2 position, float width, float height, Rgba color, std::vector<Vertex3>& vertexes);
    void DrawSprite2DCentered(Texture2D* texture, Vector2 uvBottomLeft, Vector2 uvTopRight, Vector2 position, float orientationDegrees, float width, float height, Rgba color);
    void DrawLine3D(const Vector3& startPosition, const Vector3& endPosition, Rgba const &color = Rgba::WHITE);
    void DrawCubeAxisLine(const Vector3& startPosition, const Vector3& endPosition, float thickness, Rgba const &color = Rgba::WHITE);
    void DrawCubeLine(const Vector3& startPosition, const Vector3& endPosition, float thickness, Rgba const &color = Rgba::WHITE);
    void DrawXZPlane();
    void DrawQuad(const Vector3& position, const Vector3& extension);
    void DrawQuad(Vector2 bottomLeft, Vector2 topRight, Vector2 uvBottomLeft, Vector2 uvTopRight, Rgba color);
    void DrawQuad(const Vector3& position, const Vector3& extension, Rgba color);
    void DrawQuad(Vector2 bottomLeft, Vector2 topRight, Vector2 uvBottomLeft, Vector2 uvTopRight, Rgba color, std::vector<Vertex3>& vertexes);
    void DrawCircle(const Disc2D& discMembers, const Rgba& color, float orientationDegrees);
    void DrawSimpleLine2D(Vector2 startPoint, Vector2 endPoint, Rgba color);
    void DrawQuad3D(Vector3 const &origin, Vector3 const &right, float negXExtents, float posXExtents, Vector3 const &up, float negYExtents, float posYExtents, Rgba const &color = Rgba::WHITE);
    void DrawIndexedQuad3D(Vector3 const &origin, Vector3 const &right, float negXExtents, float posXExtents, Vector3 const &up, float negYExtents, float posYExtents, Rgba const &color = Rgba::WHITE);
    void DrawTwoSidedQuad3D(Vector3 const &origin, Vector3 const &right, float negXExtents, float posXExtents, Vector3 const &up, float negYExtents, float posYExtents, Rgba const &color = Rgba::WHITE);
    void DrawCube(Vector3 const &origin, Vector3 const &right, float negXExtents, float posXExtents, Vector3 const &up, float negYExtents, float posYExtents, Vector3 const &towards, float negZExtents, float posZExtents, Rgba const &color = Rgba::WHITE);
    void DrawCubeMeshFromOBJ();
    void DrawDiamondFromOBJ();
    void DrawTeapotFromOBJ();
    void DrawSpaceshipFromOBJ();
    void DrawSphere(Vector3 const &position, float const &radius, int const &sliceCount = 20, int const &stackCount = 20, Rgba const &color = Rgba::WHITE);
    void DrawMesh();
    void DrawMeshSkinned();
    void DrawSkeleton();
    void DrawCubeAnimation(float currentTimeInSeconds);
    void DrawSkinnedCubeAnimation(float currentTimeInSeconds);
    void DrawPoint(Vector3 const &position, Rgba const &color = Rgba::WHITE);

    void SetConstantBuffer(unsigned int idx, ConstantBuffer *cb);

    int GetTextWidth(Font *font, char const *text);
    int GetTextHeight(Font *font, char const *text);
    void DrawText(Font* font, int sx, int sy, char const *text, Rgba color);
    void DrawText(Font* font, int sx, int sy, int cellHeight, char const *text, Rgba color);
    void DrawText(Font* font, int sx, int sy, int h, int w, char const *text, Rgba color);
    void DrawText(Font* font, int sx, int sy, int h, int w, char const *text, Rgba color, std::vector<Vertex3>& vertexes);
    void DrawTextCentered(Font* font, Vector2 position, char const *text, Rgba color);
    void DrawTextCenteredAndScaledToFitAABB2D(Font* font, AABB2D aabb2d, char const *text, Rgba color);

    void DrawTextCenteredAndScaledToFitAABB2D(Font* font, AABB2D aabb2d, char const *text, Rgba color, std::vector<Vertex3>& vertexes);
    void LoadSkinMatrices(Motion *motion, Skeleton *skeleton);
    void CalculateSkinMatrices(Pose* pose, unsigned int poseIndex);

    Texture2D* LoadTextureAsync(char const* filename, on_texture_loaded_cb cb = nullptr, void* userArg = nullptr);

    ComputeShader* CreateOrGetComputeShader(std::string computeShaderFilePath);
    ComputeShader* GetComputeShader(std::string computeShaderFilePath);
    ComputeShader* CreateComputeShader(std::string computeShaderFilePath);

public:
	RHIDeviceContext *m_context;
	RHIDevice *m_device;
	RHIOutput *m_output; 

    Sampler *m_pointSampler;
    Sampler *m_linearSampler;
	
    Texture2D *m_currentTarget;
    Texture2D *m_defaultColorTarget;
    Texture2D *m_currentColorTarget;

    Texture2D *m_defaultDepthStencil;
    Texture2D *m_currentDepthStencil;        

    Texture2D *m_whiteSample;
    Texture2D *m_diffuseTexture;
    Texture2D *m_spaceShipDiffuse;
    Texture2D *m_normalTexture;
    Texture2D *m_spaceShipNormal;
    Texture2D *flatNormalTexture;
    Texture2D *texSample;
    Texture2D *m_uavSample;
    std::vector<Texture2D*> texSamples;

    ShaderProgram *shaderProgram;
    ShaderProgram *m_colorShader;
    ShaderProgram *m_unlitShader;
    ShaderProgram *m_lightShader;
    ShaderProgram *m_skinningShader;
    ShaderProgram *m_skinnedBoneWeightsShader;
    std::vector<ShaderProgram*> shaderPrograms;

    time_constant_t time;
    ConstantBuffer *timeConstants;

    depth_stencil_desc_t m_depthStencilDesc;
    DepthStencilState *m_defaultDepthStencilState;

    RasterState *m_rasterState;
    VertexBuffer *m_tempVbo;
    int m_vertexCount;
    int m_indexCount;
    IndexBuffer *m_tempIbo;

    blend_state_t m_blendState;
    BlendState *m_currentBlendState;

    time_constant_t time_data;
    ConstantBuffer *time_cb;

    matrix_buffer_t matrix_data;
    ConstantBuffer *matrix_cb;

    light_buffer_t light_data;
    ConstantBuffer *light_cb;

    Font *m_font;
    Texture2D *m_fontTexture;

    OBJLoader m_objSpaceship;
    Vertex3 m_spaceShipVertices[81408];

    std::map<std::string, BitmapFont*> m_loadedBitmapFonts;

    MeshBuilder* m_meshBuilderCube;
    Skeleton* m_skeletonCube;
    Motion* m_motionCube;    

    StructuredBuffer* m_structureBuffer;
    std::vector<SkeletonInstance*> m_skeletonInstances;

    ComputeJob* m_job;


    mouse_position_t m_mouse_position_data;
    ConstantBuffer *m_mouse_position_cb;
};

static std::map<std::string, ComputeShader*> s_computeShaders;