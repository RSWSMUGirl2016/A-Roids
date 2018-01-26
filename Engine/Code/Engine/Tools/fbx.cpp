#include "Engine/Tools/fbx.h"
#include <string>
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Console.hpp"
#include "Engine/Math/MatrixStack.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/Matrix4.hpp"
#include "Engine/Core/StringUtils.hpp"

#include <algorithm>

#if defined(TOOLS_BUILD) 
// If we have FBX IMPORTING ENABLED, make sure to include the library and header files.
#define __PLACEMENT_NEW_INLINE 
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk-md.lib")

//------------------------------------------------------------------------
// helper struct for loading
struct fbx_skin_weight_t
{
    fbx_skin_weight_t() :
        indices(UnsignedIntVector4()),
        weights(Vector4())
    {}

    void reset()
    {
        indices = UnsignedIntVector4();
        weights = Vector4();
    }

    UnsignedIntVector4 indices;
    Vector4 weights;
};

fbxsdk::FbxScene* FbxLoadScene(char const *filename, bool import_embedded);
void FbxUnloadScene(fbxsdk::FbxScene *scene);
void FbxPrintNode(fbxsdk::FbxNode *node, int depth);
void FbxListScene(fbxsdk::FbxScene *scene);
void FbxPrintAttribute(fbxsdk::FbxNodeAttribute* pAttribute, int depth);
char const* GetAttributeTypeName(fbxsdk::FbxNodeAttribute::EType type);

void ConvertSceneToEngineBasis(fbxsdk::FbxScene *scene);
void TriangulateScene(fbxsdk::FbxScene *scene);
void FlipX(MeshBuilder *mb);
void FlipX(Skeleton *skeleton);
void ImportMeshes(MeshBuilder *out, fbxsdk::FbxNode *node, MatrixStack &matrixStack);
Matrix4 GetNodeWorldTransform(fbxsdk::FbxNode *node);
Matrix4 ToEngineMatrix(fbxsdk::FbxMatrix const &fbxMatrix);
Matrix4 ToEngineMatrix2(fbxsdk::FbxMatrix const &fbxMatrix);
static void ImportMesh(MeshBuilder *out, MatrixStack &matrixStack, fbxsdk::FbxMesh *mesh);
Matrix4 GetGeometricTransform(fbxsdk::FbxNode *node);
void FbxImportVertex(MeshBuilder *out, Matrix4 const &transform, fbxsdk::FbxMesh *mesh, int32_t polyIdx, int32_t vertIdx);
bool GetNormal(Vector3 *out, Matrix4 const &transform, fbxsdk::FbxMesh *mesh, int poly_idx, int vert_idx);
bool FbxGetPosition(Vector3 *outPos, Matrix4 const &transform, fbxsdk::FbxMesh *mesh, int polyIdx, int vertIdx);
bool FbxGetTexCoords(Vector2 *out, Matrix4 const &transform, fbxsdk::FbxMesh *mesh, int poly_idx, int vert_idx);
fbxsdk::FbxPose* GetBindPose(fbxsdk::FbxScene *scene);
void ImportSkeleton(Skeleton *out, fbxsdk::FbxNode *node, fbxsdk::FbxSkeleton *root_bone, fbxsdk::FbxSkeleton *parent_bone, fbxsdk::FbxPose *pose);
static char const* GetNodeName(fbxsdk::FbxNode const *node);
static char const* GetBoneName(fbxsdk::FbxSkeleton const *skel);
static float GetNativeFramefrate(fbxsdk::FbxScene const *scene);
static bool FbxImportMotionSkeleton(Motion *motion, Skeleton const *skeleton, fbxsdk::FbxScene *scene, fbxsdk::FbxAnimStack *anim, float framerate);
Matrix4 GetNodeWorldTransformAtTime(fbxsdk::FbxNode *node, fbxsdk::FbxTime time);
Matrix4 GetNodeWorldTransform2(fbxsdk::FbxNode *node);
void ImportMeshes(MeshBuilder *out, fbxsdk::FbxNode *node, MatrixStack *matriStack, Skeleton const *skeleton);
bool ImportMesh(MeshBuilder *out, MatrixStack *matrixStack, fbxsdk::FbxMesh *mesh, Skeleton const *skeleton);
void FbxImportVertex(MeshBuilder *out, Matrix4 const &transform, fbxsdk::FbxMesh *mesh, int32_t polyIdx, int32_t vertIdx, fbx_skin_weight_t *skin_weight);
void CalculateSkinWeights(std::vector<fbx_skin_weight_t> *skin_weights, fbxsdk::FbxMesh const *mesh, Skeleton const *skeleton);
void AddHighestWeight(fbx_skin_weight_t *skin_weight, uint32_t bone_idx, float weight);
bool HasSkinWeights(fbxsdk::FbxMesh const *mesh);
static bool FbxImportMotion(Motion *motion, Skeleton const *skeleton, fbxsdk::FbxScene *scene, fbxsdk::FbxAnimStack *anim, float framerate);
void WriteMeshOutToFile(char const *fbx_filename, char const *root_output_name, float impor_scale /*= 1.0f*/);
void WriteSkeletonOutToFile(char const *fbx_filename, char const *root_output_name, float impor_scale /*= 1.0f*/);
void WriteMotionOutToFile(char const *fbx_filename, char const *root_output_name, float impor_scale /*= 1.0f*/);

Vector2 ToVec2(fbxsdk::FbxVector2 fbx_pos);
Vector3 ToVec3(fbxsdk::FbxVector4 fbx_pos);
Vector4 ToVec4(fbxsdk::FbxVector4 fbx_pos);
float Max(float floatOne, float floatTwo);

/** FbxListFile **/

//------------------------------------------------------------------------
void FbxListFile(char const *filename)
{
    fbxsdk::FbxScene *scene = FbxLoadScene(filename, false);
    if (nullptr == scene) {
        DebuggerPrintf("Failed to load scene: %s\n", filename);
        std::string temp = "Failed to load scene: ";
        std::string consoleString = temp + filename;
        g_theConsole->AddConsoleMessage(consoleString);
        return;
    }

    DebuggerPrintf("Listing File: %s\n", filename);
    std::string temp = "Listing File: ";
    std::string consoleString = temp + filename;
    g_theConsole->AddConsoleMessage(consoleString);

    FbxListScene(scene);

    FbxUnloadScene(scene);
}

//------------------------------------------------------------------------
// Reference:
//    $(FBXSDK_DIR)\samples\Common\Common.cxx
fbxsdk::FbxScene* FbxLoadScene(char const *filename, bool import_embedded)
{
    // Manager - think of it as a running process of the FbxSdk - you could only ever load one of these
    // and call it good - though I usually just keep one around per import.
    fbxsdk::FbxManager *fbx_manager = fbxsdk::FbxManager::Create();
    if (nullptr == fbx_manager) {
        DebuggerPrintf("Could not create FBX Manager.\n");
        std::string temp = "Could not create FBX Manager.";
        std::string consoleString = temp;
        g_theConsole->AddConsoleMessage(consoleString);
        return nullptr;
    }

    // Next, set the IO settings - this is how we configure what we're importing
    // By default, we import everything, but if you wanted to explicitly not import some parts
    // you could do that here)
    fbxsdk::FbxIOSettings *io_settings = fbxsdk::FbxIOSettings::Create(fbx_manager, IOSROOT);
    io_settings->SetBoolProp(IMP_FBX_EXTRACT_EMBEDDED_DATA, import_embedded);

    fbx_manager->SetIOSettings(io_settings);


    // Next, create our importer (since we're loading an FBX, not saving one)
    // Second argument is the name - we don't need to name it.
    fbxsdk::FbxImporter *importer = fbxsdk::FbxImporter::Create(fbx_manager, "");

    bool result = importer->Initialize(filename,
        -1, // File format, -1 will let the program figure it out
        io_settings);

    if (result) {
        // First, create a scene to be imported into
        // Doesn't need a name, we're not going to do anything with it
        fbxsdk::FbxScene *scene = fbxsdk::FbxScene::Create(fbx_manager, "");

        // Import into the scene
        result = importer->Import(scene);
        importer->Destroy();
        importer = nullptr;

        if (result) {
            return scene;
        }
    }

    // Failed somewhere, so clean up after ourselves.
    FBX_SAFE_DESTROY(io_settings);
    FBX_SAFE_DESTROY(importer);
    FBX_SAFE_DESTROY(fbx_manager);

    return nullptr;
}

//------------------------------------------------------------------------
void FbxUnloadScene(fbxsdk::FbxScene *scene)
{
    if (nullptr == scene) {
        return;
    }

    fbxsdk::FbxManager *manager = scene->GetFbxManager();
    fbxsdk::FbxIOSettings *io_settings = manager->GetIOSettings();

    FBX_SAFE_DESTROY(scene);
    FBX_SAFE_DESTROY(io_settings);
    FBX_SAFE_DESTROY(manager);
}

//------------------------------------------------------------------------
void FbxPrintNode(fbxsdk::FbxNode *node, int depth)
{
    // Print the node's attributes.
    //DebuggerPrintf("%*sNode [%s]\n", depth, node->GetName());
    std::string temp1 = "Node [";
    std::string temp2 = "]\n";
    std::string name = node->GetName();
    std::string consoleString = temp1 + name + temp2;
    g_theConsole->AddConsoleMessage(consoleString);

    for (int i = 0; i < node->GetNodeAttributeCount(); i++) {
        FbxPrintAttribute(node->GetNodeAttributeByIndex(i), depth);
    }

    // Print the nodes children
    for (int32_t i = 0; i < node->GetChildCount(); ++i) {
        FbxPrintNode(node->GetChild(i), depth + 1);
    }
}

//------------------------------------------------------------------------
void FbxListScene(fbxsdk::FbxScene *scene)
{
    // Print the node
    fbxsdk::FbxNode *root = scene->GetRootNode();
    FbxPrintNode(root, 0);
}

//------------------------------------------------------------------------
void FbxPrintAttribute(fbxsdk::FbxNodeAttribute* pAttribute, int depth)
{
    if (nullptr == pAttribute) {
        return;
    }

    fbxsdk::FbxNodeAttribute::EType type = pAttribute->GetAttributeType();

    char const* typeName = GetAttributeTypeName(type);
    char const* attrName = pAttribute->GetName();

    //DebuggerPrintf("%*s- type='%s' name='%s'\n", depth, typeName, attrName);
    std::string temp1 = "- type='";
    std::string temp2 = "' name='";
    std::string temp3 = "'";
    std::string typeNameStr = typeName;
    std::string attrNameStr = attrName;
    std::string consoleString = temp1 + typeNameStr + temp2 + attrNameStr + temp3;
    g_theConsole->AddConsoleMessage(consoleString);
}

//------------------------------------------------------------------------
char const* GetAttributeTypeName(fbxsdk::FbxNodeAttribute::EType type)
{
    switch (type) {
    case fbxsdk::FbxNodeAttribute::eUnknown: return "unidentified";
    case fbxsdk::FbxNodeAttribute::eNull: return "null";
    case fbxsdk::FbxNodeAttribute::eMarker: return "marker";
    case fbxsdk::FbxNodeAttribute::eSkeleton: return "skeleton";
    case fbxsdk::FbxNodeAttribute::eMesh: return "mesh";
    case fbxsdk::FbxNodeAttribute::eNurbs: return "nurbs";
    case fbxsdk::FbxNodeAttribute::ePatch: return "patch";
    case fbxsdk::FbxNodeAttribute::eCamera: return "camera";
    case fbxsdk::FbxNodeAttribute::eCameraStereo: return "stereo";
    case fbxsdk::FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
    case fbxsdk::FbxNodeAttribute::eLight: return "light";
    case fbxsdk::FbxNodeAttribute::eOpticalReference: return "optical reference";
    case fbxsdk::FbxNodeAttribute::eOpticalMarker: return "marker";
    case fbxsdk::FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
    case fbxsdk::FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
    case fbxsdk::FbxNodeAttribute::eBoundary: return "boundary";
    case fbxsdk::FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
    case fbxsdk::FbxNodeAttribute::eShape: return "shape";
    case fbxsdk::FbxNodeAttribute::eLODGroup: return "lodgroup";
    case fbxsdk::FbxNodeAttribute::eSubDiv: return "subdiv";
    default: return "unknown";
    }
}


// Loading a Mesh

//------------------------------------------------------------------------
//------------------------------------------------------------------------
bool FbxLoadMesh(MeshBuilder *mb, char const *filename)
{
    // First - load the scene - code is identical to when you 
    // listed the file
    fbxsdk::FbxScene *scene = FbxLoadScene(filename, false);
    if (nullptr == scene)
    {
        return false;
    }

    // Second, we want to convert the basis for consitancy
    ConvertSceneToEngineBasis(scene);

    // Third, our engine only deals with triangles, 
    // but authoring programs tend to deal with a lot of 
    // different surface patch types.  For we triangulate
    // (the process of converting every non-triangle polygon to triangles )
    TriangulateScene(scene);


    // Alright, now we can start importing data, for this we'll need a matrix stack 
    // [well, it helps, you can do this without since we'll mostly be deailing with 
    // global transforms outside of what is called the "geometric transform" which is
    // always local]

    // clear all old data before importing
    mb->Clear();

    // My matrix stack treats the top as being IDENTITY
    MatrixStack matrixStack;

    // Next, get the root node, and start importing
    FbxNode *root = scene->GetRootNode();

    // import data into our builder
    ImportMeshes(mb, root, matrixStack);

    // Clean up after ourself
    FbxUnloadScene(scene);


    // [HACK] After loading, I flip everything across the X axis
    // for conistancy.  Would love this to be part of ConvertSceneToEngineBasis,
    // but no mattter the transform I sent, it was always flipped on X [at best]
    //
    // Doing this causes it to be match Unity/Unreal/Editor in terms of orientation
    FlipX(mb);
    return true;
}

// So, new functions and the Fbx Code that makes them up

// Conver scene to a consistant basis.
// The name is misleading, as this actually converts to a 
// right-handed system (Y-UP, X-RIGHT, Z-BACK)
// but any attempt to convert to a left-handed
// system caused it to flip across Y, which is not what I 
// want - so I handle the final basis flip as a final step after
// loading
void ConvertSceneToEngineBasis(fbxsdk::FbxScene *scene)
{
    fbxsdk::FbxAxisSystem local_system(
        (fbxsdk::FbxAxisSystem::EUpVector) fbxsdk::FbxAxisSystem::EUpVector::eYAxis,
        (fbxsdk::FbxAxisSystem::EFrontVector) fbxsdk::FbxAxisSystem::EFrontVector::eParityOdd,
        (fbxsdk::FbxAxisSystem::ECoordSystem) fbxsdk::FbxAxisSystem::ECoordSystem::eRightHanded);

    fbxsdk::FbxAxisSystem scene_system = scene->GetGlobalSettings().GetAxisSystem();
    if (scene_system != local_system)
    {
        local_system.ConvertScene(scene);
    }
}

// Converts the scene to only use triangles for the meshes.
void TriangulateScene(fbxsdk::FbxScene *scene)
{
    fbxsdk::FbxGeometryConverter converter(scene->GetFbxManager());

    // Replace being true means it will destroy the old 
    // representation of the mesh after conversion.  Setting this to 
    // false would leave the old representations in.
    converter.Triangulate(scene, /*replace=*/true);
}

// Flips all geoemetry across the X access
void FlipX(MeshBuilder *mb)
{
    // LEFT AS AN EXCERCISE (mostly as it depends on how your implementation of MeshBuilder/Mesh)

    // You just want to flip the X coordinate of all vertex
    // data (positions,normals/tangents/bitangents)
    //#IMPLEMENT_ME;
    mb->FlipX();
}

//------------------------------------------------------------------------
void FlipX(Skeleton *skeleton)
{
    skeleton->FlipX();
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
// The meat of the problem - Importing a mesh
// This walking of a list, loading specific attributes will be a pretty common task
// in this class.
void ImportMeshes(MeshBuilder *out, fbxsdk::FbxNode *node, MatrixStack &matriStack)
{
    // first, we're traversing the graph, keep track of our current world transform
    // (I will do this by by pushing directly the world transform of this node to my stack)
    // (that is, this transform becomes the new top exactly)
    Matrix4 transform = GetNodeWorldTransform(node);
    matriStack.Push(transform);

    // First, traverse the scene graph, looking for FbxMesh node attributes.
    int attrib_count = node->GetNodeAttributeCount();
    for (int ai = 0; ai < attrib_count; ++ai)
    {
        fbxsdk::FbxNodeAttribute *attrib = node->GetNodeAttributeByIndex(ai);
        if (attrib->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMesh)
        {
            ImportMesh(out, matriStack, (fbxsdk::FbxMesh*)attrib);
        }
    }

    // Try to load any meshes that are children of this node
    int child_count = node->GetChildCount();
    for (int ci = 0; ci < child_count; ++ci)
    {
        fbxsdk::FbxNode *child = node->GetChild(ci);
        ImportMeshes(out, child, matriStack);
    }

    // we're done with this node, so its transform off the stack
    matriStack.Pop();

}

// This gets the world transform of a specific node
// and converts it to OUR engine matrix format
Matrix4 GetNodeWorldTransform(fbxsdk::FbxNode *node)
{
    fbxsdk::FbxAMatrix fbx_mat = node->EvaluateGlobalTransform();
    return ToEngineMatrix(fbx_mat);
}

Matrix4 ToEngineMatrix(fbxsdk::FbxMatrix const &fbxMatrix)
{
    // EXCERCISE TO READER - convert their matrix to your matrix
    // format.

    /** For reference, fbx_mat is stored Row Major, and can be accessed...
    FbxDouble4 row0 = fbx_mat.mData[0];
    FbxDouble4 row1 = fbx_mat.mData[1];
    FbxDouble4 row2 = fbx_mat.mData[2];
    FbxDouble4 row3 = fbx_mat.mData[3];

    // An FbxDouble4 has it's valued stored in an mData array as well,
    // it's x, y, z, w can be accessed as seen below converting it to my engines type
    vec4 my_row0 = vec4(
    (float) row0.mData[0],
    (float) row0.mData[1],
    (float) row0.mData[2],
    (float) row0.mData[3],
    );
    */

    fbxsdk::FbxDouble4 row0 = fbxMatrix.mData[0];
    fbxsdk::FbxDouble4 row1 = fbxMatrix.mData[1];
    fbxsdk::FbxDouble4 row2 = fbxMatrix.mData[2];
    fbxsdk::FbxDouble4 row3 = fbxMatrix.mData[3];

    Matrix4 matrix = Matrix4();
    matrix.m_iBasis = Vector4((float)row0.mData[0], (float)row0.mData[1], (float)row0.mData[2], (float)row0.mData[3]);
    matrix.m_jBasis = Vector4((float)row1.mData[0], (float)row1.mData[1], (float)row1.mData[2], (float)row1.mData[3]);
    matrix.m_kBasis = Vector4((float)row2.mData[0], (float)row2.mData[1], (float)row2.mData[2], (float)row2.mData[3]);
    matrix.m_translation = Vector4((float)row3.mData[0], (float)row3.mData[1], (float)row3.mData[2], (float)row3.mData[3]);

    //#IMPLEMENT_ME;
    return matrix;
}

Matrix4 ToEngineMatrix2(fbxsdk::FbxMatrix const &fbxMatrix)
{
    /** For reference, fbx_mat is stored Row Major, and can be accessed...
    mat44 const flip_x = mat44::Basis(
      -vec3::RIGHT,
      vec3::UP,
      vec3::FORWARD );

   mat44 ret = mat44::FromRows(
      ToVec4( m.mData[0] ),
      ToVec4( m.mData[1] ),
      ToVec4( m.mData[2] ),
      ToVec4( m.mData[3] )
   );

   return flip_x * ret * flip_x;
    */

    Matrix4 flip_x = Matrix4();
    flip_x.m_iBasis = Vector4(-1.f, 0.f, 0.f, 0.f);
    flip_x.m_jBasis = Vector4(0.f, 1.f, 0.f, 0.f);
    flip_x.m_kBasis = Vector4(0.f, 0.f, 1.f, 0.f);
    flip_x.m_translation = Vector4(0.f, 0.f, 0.f, 1.f);

    fbxsdk::FbxDouble4 row0 = fbxMatrix.mData[0];
    fbxsdk::FbxDouble4 row1 = fbxMatrix.mData[1];
    fbxsdk::FbxDouble4 row2 = fbxMatrix.mData[2];
    fbxsdk::FbxDouble4 row3 = fbxMatrix.mData[3];

    Matrix4 ret = Matrix4();
    ret.m_iBasis = Vector4((float)row0.mData[0], (float)row0.mData[1], (float)row0.mData[2], (float)row0.mData[3]);
    ret.m_jBasis = Vector4((float)row1.mData[0], (float)row1.mData[1], (float)row1.mData[2], (float)row1.mData[3]);
    ret.m_kBasis = Vector4((float)row2.mData[0], (float)row2.mData[1], (float)row2.mData[2], (float)row2.mData[3]);
    ret.m_translation = Vector4((float)row3.mData[0], (float)row3.mData[1], (float)row3.mData[2], (float)row3.mData[3]);

    return MultiplyMatrix4ByMatrix4(MultiplyMatrix4ByMatrix4(flip_x, ret), flip_x);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------

// Import a single mesh attribute
static void ImportMesh(MeshBuilder *out, MatrixStack &matrixStack, fbxsdk::FbxMesh *mesh)
{
    // Should have been triangulated before this - sort of a late check 
    // [treat this as an ASSERT_OR_DIE]
    //ASSERT_RETURN(mesh->IsTriangleMesh());
    ASSERT_OR_DIE(mesh->IsTriangleMesh(), "Is not a triangle mesh!");

    // Geometric Transformations only apply to the current node
    // http://download.autodesk.com/us/fbx/20112/FBX_SDK_HELP/index.html?url=WS1a9193826455f5ff1f92379812724681e696651.htm,topicNumber=d0e7429
    Matrix4 geo_trans = GetGeometricTransform(mesh->GetNode());
    matrixStack.PushMultiply(geo_trans);

    // import the mesh data.
    Matrix4 transform = matrixStack.m_top;

    // Starting a draw call.
    out->Begin(ePrimitiveType::PRIMITIVE_TRIANGLES, false);

    // Load in the mesh - first, figure out how many polygons there are
    signed int poly_count = mesh->GetPolygonCount();

    signed int total_vert_count = 0;

    for (signed int poly_idx = 0; poly_idx < poly_count; ++poly_idx) {

        // For each polygon - get the number of vertices that make it up (should always be 3 for a triangulated mesh)
        signed int vert_count = mesh->GetPolygonSize(poly_idx);
        total_vert_count += vert_count;
        //ASSERT(vert_count == 3); // Triangle Meshes should ALWAYS have 3 verts per poly
        ASSERT_OR_DIE(vert_count == 3, "Triangle Meshes should ALWAYS have 3 verts per poly");

        // Finally, import all the data for this vertex (for now, just position)
        for (signed int vert_idx = 0; vert_idx < vert_count; ++vert_idx) {
            FbxImportVertex(out, transform, mesh, poly_idx, vert_idx);
        }
    }

    out->End((unsigned int)total_vert_count);

    // pop the geometric transform
    matrixStack.Pop();
}

// This gets a local transform that is applied ONLY to data stored at this node,
// and is always considered local to the node.
Matrix4 GetGeometricTransform(fbxsdk::FbxNode *node)
{
    Matrix4 ret = Matrix4();

    if ((node != nullptr) && (node->GetNodeAttribute() != nullptr)) {
        fbxsdk::FbxEuler::EOrder order;

        node->GetRotationOrder(fbxsdk::FbxNode::eSourcePivot, order);

        fbxsdk::FbxVector4 const geo_trans = node->GetGeometricTranslation(FbxNode::eSourcePivot);
        fbxsdk::FbxVector4 const geo_rot = node->GetGeometricRotation(FbxNode::eSourcePivot);
        fbxsdk::FbxVector4 const geo_scale = node->GetGeometricScaling(FbxNode::eSourcePivot);

        fbxsdk::FbxAMatrix geo_mat;
        fbxsdk::FbxAMatrix mat_rot;
        mat_rot.SetR(geo_rot, order);
        geo_mat.SetTRS(geo_trans, mat_rot, geo_scale);

        ret = ToEngineMatrix2(geo_mat);
    }

    return ret;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
// This loads in all the information for a specific vertex as defined by a 
// poly_idx and vert_idx.  For now, will only worry about position.
void FbxImportVertex(MeshBuilder *out, Matrix4 const &transform, fbxsdk::FbxMesh *mesh, int32_t poly_idx, int32_t vert_idx)
{
    /** NEW BITS **/

    // Will demonstarate with normal, but this process is repeated for
    // uvs, colors, tangents, bitangents (called binormals in the SDK)
    Vector3 normal;
    if (GetNormal(&normal, transform, mesh, poly_idx, vert_idx)) {
        out->SetNormal(normal);
    }

    Vector2 uvCoords;
    if (FbxGetTexCoords(&uvCoords, transform, mesh, poly_idx, vert_idx)) {
        out->SetUV(uvCoords);
    }


    // same process for UVs, Tangents, Bitangents

    /** END NEW BITS **/

    // If we get a position- push it.
    // This actually should always succeed.
    Vector3 position;
    if (FbxGetPosition(&position, transform, mesh, poly_idx, vert_idx)) {
        position.x *= -1.f;
        out->AddVertex(position);
    }
}

//------------------------------------------------------------------------
bool GetNormal(Vector3 *out, Matrix4 const &transform, fbxsdk::FbxMesh *mesh, int poly_idx, int vert_idx)
{
    // First, we need to get the geometry element we care about...
    // you can this of this as a "layer" of data for a mesh.  In this case
    // is is the normal data.

    // Note:  0 Index - meshes can potentially contain multiple layers
    // of the same type (UVs are the most common exmaple of this).  
    // For Normals, I just use the first one and continue on.
    fbxsdk::FbxGeometryElementNormal *element = mesh->GetElementNormal(0);
    if (element == nullptr) {
        // no layer - no data
        return false;
    }

    // Next, we need to figure out how to pull the normal for this particular vertex out
    // of this layer. 
    // 
    // So, vnormal is just a packed array of normals, or potentially normals, and
    // and index buffer into those normals.
    //
    // Either way, you can think of vnormal as a packed collection of normals.
    // and we're trying to figure out which element of this array we want (elem_idx)

    // So first, get the element index.
    // (Is the data tied to the control point, or the polygon vertex)
    int elem_idx = 0;
    switch (element->GetMappingMode()) {
    case fbxsdk::FbxGeometryElement::eByControlPoint: {
        // Get the normal by control point - most direct way.
        elem_idx = mesh->GetPolygonVertex(poly_idx, vert_idx);
    } break; // case: eByControlPoint

    case fbxsdk::FbxGeometryElement::eByPolygonVertex: {
        // array is packed by polygon vertex - so each polygon is unique
        // (think sharp edges in the case of normals)
        elem_idx = mesh->GetPolygonVertexIndex(poly_idx) + vert_idx;
    } break; // case: eByPolygonVertex

    default:
        //ASSERT(0); // Unknown mapping mode
        ASSERT_OR_DIE(false, "Unknown mapping mode");
        return false;
    }

    // next, now that we have the element index, we figure out how this is used.
    // Is this a direct array, or an indexed array.
    // If we got an element index, fetch the data based on how it's referenced
    // (Directly, or by an index into an array);
    switch (element->GetReferenceMode()) {
    case fbxsdk::FbxGeometryElement::eDirect: {
        // this is just an array - use elem index as an index into this array
        if (elem_idx < element->GetDirectArray().GetCount()) {
            *out = ToVec3(element->GetDirectArray().GetAt(elem_idx));
            return true;
        }
    } break;

    case fbxsdk::FbxGeometryElement::eIndexToDirect: {
        // This is an indexed array, so elem_idx is our offset into the 
        // index buffer.  We use that to get our index into the direct array.
        if (elem_idx < element->GetIndexArray().GetCount()) {
            int index = element->GetIndexArray().GetAt(elem_idx);
            *out = ToVec3(element->GetDirectArray().GetAt(index));
            return true;
        }
    } break;

    default:
        //ASSERT(0); // Unknown reference type
        ASSERT_OR_DIE(false, "Unknown reference type");
        return false;
    }
}

// This code can be templatized out btw, to help remove some code duplication, 
// since the process is the same for most data, and the only thing that changes
// is the out data type.

// Gets the position from a FbxMesh value.
// Every <poly_idx, vert_idx> pair will evaluate to a unique
// "control point" - that is, a position, or vertex that can be 
// manipulate by the program - which makes importing positions pretty easy

//------------------------------------------------------------------------
bool FbxGetPosition(Vector3 *out_pos, Matrix4 const &transform, fbxsdk::FbxMesh *mesh, int polyIdx, int vertIdx)
{
    // First, get the control point index for this poly/vert pair.
    int ctrl_idx = mesh->GetPolygonVertex(polyIdx, vertIdx);
    if (ctrl_idx < 0) {
        return false;
    }

    // Get the position for the control point.
    fbxsdk::FbxVector4 fbx_pos = mesh->GetControlPointAt(ctrl_idx);

    // convert to an engine vertex, and save it to the out variable
    Vector4 temp = ToVec4(fbx_pos);
    Vector3 pos = Vector3(temp.x, temp.y, temp.z);
    *out_pos = MultiplyVector3ByMatrix4(pos, transform);
    out_pos->x *= -1.f;

    // return success.
    return true;
}

bool FbxGetTexCoords(Vector2 *out, Matrix4 const &transform, fbxsdk::FbxMesh *mesh, int poly_idx, int vert_idx)
{
    // First, we need to get the geometry element we care about...
    // you can this of this as a "layer" of data for a mesh.  In this case
    // is is the normal data.

    // Note:  0 Index - meshes can potentially contain multiple layers
    // of the same type (UVs are the most common exmaple of this).  
    // For Normals, I just use the first one and continue on.
    fbxsdk::FbxGeometryElementUV *element = mesh->GetElementUV(0);
    if (element == nullptr) {
        // no layer - no data
        return false;
    }

    // Next, we need to figure out how to pull the normal for this particular vertex out
    // of this layer. 
    // 
    // So, vnormal is just a packed array of normals, or potentially normals, and
    // and index buffer into those normals.
    //
    // Either way, you can think of vnormal as a packed collection of normals.
    // and we're trying to figure out which element of this array we want (elem_idx)

    // So first, get the element index.
    // (Is the data tied to the control point, or the polygon vertex)
    int elem_idx = 0;
    switch (element->GetMappingMode()) {
    case fbxsdk::FbxGeometryElement::eByControlPoint: {
        // Get the normal by control point - most direct way.
        elem_idx = mesh->GetPolygonVertex(poly_idx, vert_idx);
    } break; // case: eByControlPoint

    case fbxsdk::FbxGeometryElement::eByPolygonVertex: {
        // array is packed by polygon vertex - so each polygon is unique
        // (think sharp edges in the case of normals)
        elem_idx = mesh->GetPolygonVertexIndex(poly_idx) + vert_idx;
    } break; // case: eByPolygonVertex

    default:
        //ASSERT(0); // Unknown mapping mode
        ASSERT_OR_DIE(false, "Unknown mapping mode");
        return false;
    }

    // next, now that we have the element index, we figure out how this is used.
    // Is this a direct array, or an indexed array.
    // If we got an element index, fetch the data based on how it's referenced
    // (Directly, or by an index into an array);
    switch (element->GetReferenceMode()) {
    case fbxsdk::FbxGeometryElement::eDirect: {
        // this is just an array - use elem index as an index into this array
        if (elem_idx < element->GetDirectArray().GetCount()) {
            Vector2 temp = ToVec2(element->GetDirectArray().GetAt(elem_idx));
            *out = Vector2(temp.x, temp.y);
            return true;
        }
    } break;

    case fbxsdk::FbxGeometryElement::eIndexToDirect: {
        // This is an indexed array, so elem_idx is our offset into the 
        // index buffer.  We use that to get our index into the direct array.
        if (elem_idx < element->GetIndexArray().GetCount()) {
            int index = element->GetIndexArray().GetAt(elem_idx);
            Vector2 temp = ToVec2(element->GetDirectArray().GetAt(index));
            *out = Vector2(temp.x, temp.y);
            return true;
        }
    } break;

    default:
        //ASSERT(0); // Unknown reference type
        ASSERT_OR_DIE(false, "Unknown reference type");
        return false;
    }
}

//------------------------------------------------------------------------
bool FbxLoadSkeleton(Skeleton *skel, char const *filename)
{
    // Same as you did for Meshes.
    fbxsdk::FbxScene *fbx_scene = FbxLoadScene(filename, false);
    if (nullptr == fbx_scene) {
        return false;
    }

    ConvertSceneToEngineBasis(fbx_scene);

    // First, get the bind post for the scene
    skel->Clear();


    // Now, traverse the scene, and build the skeleton out with 
    // the bind pose positions
    fbxsdk::FbxNode *root = fbx_scene->GetRootNode();

    // Bind pose - the pose which assets are authored for.
    fbxsdk::FbxPose *pose = GetBindPose(fbx_scene);

    // Import the skeleton by traversing the scene.
    ImportSkeleton(skel, root, nullptr, nullptr, pose);

    // Flip the Axis 
    FlipX(skel);

    FbxUnloadScene(fbx_scene);

    // Success if I've loaded in at least ONE bone/joint.
    return (skel->GetJointCount() > 0U);
}

// Get the bind pose - that is, the pose that the resources are authored
// for. If none exists - we'll assume ONLY the bind pose exists, and use
// the global transformations.
fbxsdk::FbxPose* GetBindPose(FbxScene *scene)
{
    int pose_count = scene->GetPoseCount();
    for (int i = 0; i < pose_count; ++i) {
        fbxsdk::FbxPose *pose = scene->GetPose(i);
        if (pose->IsBindPose()) {
            // DEBUG - Print all nodes part of this pose
            /*
            int count = pose->GetCount();
            for (int j = 0; j < count; ++j) {
            FbxNode *node = pose->GetNode(j);
            Trace( "fbx", "Pose Node: %s", node->GetName() );
            }
            */

            return pose;
        }
    }

    return nullptr;
}

// Imports the skeleton.
void ImportSkeleton(Skeleton *out, fbxsdk::FbxNode *node, fbxsdk::FbxSkeleton *root_bone, fbxsdk::FbxSkeleton *parent_bone, fbxsdk::FbxPose *pose)
{
    for (int i = 0; i < node->GetNodeAttributeCount(); ++i) {

        // If this node is a skeleton node (a joint), 
        // then it should have a skeleton attribute. 
        fbxsdk::FbxNodeAttribute *na = node->GetNodeAttributeByIndex(i);
        if (na->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton) {

            // Cast it, and figure out the type of bone. 
            fbxsdk::FbxSkeleton *skel = (fbxsdk::FbxSkeleton*)na;
            fbxsdk::FbxSkeleton::EType type = skel->GetSkeletonType();

            // From FBXSDK Documentation.
            // eRoot,			/*!< First element of a chain. */
            // eLimb,			/*!< Chain element. */
            // eLimbNode,		/*!< Chain element. */
            // eEffector		/*!< Last element of a chain. */

            // If this is a root - we better have not have another root higher 
            // up (for now).  This is just here to catch this happening so
            // I know if I should support it in the future - you could cut this.
            if ((type == fbxsdk::FbxSkeleton::eRoot) || (type == fbxsdk::FbxSkeleton::eEffector)) {
                // this is a root bone - so can treat it as the root
                // of a skeleton.
                // TODO: no support for nested skeletons yet.
                //ASSERT(root_bone == nullptr);
                ASSERT_OR_DIE(root_bone == nullptr, "Root bone should not be null");
                root_bone = skel;
            } // else, it is a limb or limb node, which we treat the same

              // See if this bone exists int he pose, and if so, 
              // get its index in the pose.
            int pose_node_idx = -1;
            if (pose != nullptr) {
                pose_node_idx = pose->Find(node);
            }

            // Get the global transform for this bone
            Matrix4 bone_transform = Matrix4();
            if (pose_node_idx != -1) {
                // If it exists in the pose - use the poses transform.
                // Since we're only doing the bind pose now, the pose should ONLY
                // be storing global transforms (as per the documentation).
                //
                // When we update this code to load in animated poses, this willc hange.
                bool is_local = pose->IsLocalMatrix(pose_node_idx);
                ASSERT_OR_DIE(false == is_local, "It shouldn't be the local");
                //ASSERT(false == is_local); // for now, should always be global (pose transforms always are)

                fbxsdk::FbxMatrix fbx_matrix = pose->GetMatrix(pose_node_idx);
                bone_transform = ToEngineMatrix(fbx_matrix);
            }
            else
            {
                // Not in the pose - so we have two options
                if (parent_bone == nullptr) {
                    // We have no parent [first in the skeleton], so just use 
                    // the global transform of the node.
                    bone_transform = ToEngineMatrix(node->EvaluateGlobalTransform());
                }
                else {
                    // Otherwise, calculate my world based on my parent transform
                    // and my nodes local.
                    //
                    // The reason behind this is EvaluateGlobalTransform actually
                    // gets the animated transform, while getting the Pose transform
                    // will get an unanimated transform.  So if we're a child of a transform
                    // that was gotten from a pose, I should be evaluating off that.
                    //
                    // TODO: Evaluate Local from Parent - just in case their is siblings between this
                    Matrix4 local = ToEngineMatrix(node->EvaluateLocalTransform());
                    Matrix4 ptrans = out->GetJointTransform(GetBoneName(parent_bone));
                    //bone_transform = local * ptrans;
                    bone_transform = MultiplyMatrix4ByMatrix4(local, ptrans);
                }
            }

            // Add a joint.
            out->AddJoint(GetBoneName(skel), (parent_bone != nullptr) ? GetBoneName(parent_bone) : nullptr, bone_transform);

            // set this as the next nodes parent, and continue down the chain
            parent_bone = skel;
            break;
        }
    }

    // import the rest
    for (int i = 0; i < node->GetChildCount(); ++i) {
        ImportSkeleton(out, node->GetChild(i), root_bone, parent_bone, pose);
    }
}

//------------------------------------------------------------------------
// Just a helper - checks for null - but otherwise gets the node name
static char const* GetNodeName(fbxsdk::FbxNode const *node)
{
    if (nullptr == node) {
        return "";
    }
    else {
        return node->GetName();
    }
}

//------------------------------------------------------------------------
// Get the name to use for this bone (ideally, use the node, but if the bone
// doesn't exist, use the skeleton attributes name)
static char const* GetBoneName(fbxsdk::FbxSkeleton const *skel)
{
    if (nullptr == skel) {
        return nullptr;
    }

    char const *node_name = GetNodeName(skel->GetNode());
    if ((node_name != nullptr) || (node_name != "")) {
        return skel->GetNode()->GetName();
    }
    else {
        return skel->GetName();
    }
}

//------------------------------------------------------------------------
// Just as an extra - if you want to use the FBX files frame rate, this
// is how you can fetch it.
static float GetNativeFramefrate(fbxsdk::FbxScene const *scene)
{
    // Get the scenes authored framerate
    const fbxsdk::FbxGlobalSettings &settings = scene->GetGlobalSettings();
    fbxsdk::FbxTime::EMode time_mode = settings.GetTimeMode();
    double framerate;
    if (time_mode == fbxsdk::FbxTime::eCustom) {
        framerate = settings.GetCustomFrameRate();
    }
    else {
        framerate = fbxsdk::FbxTime::GetFrameRate(time_mode);
    }
    return framerate;
}

//------------------------------------------------------------------------
static bool FbxImportMotionSkeleton(Motion *motion,
    Skeleton const *skeleton,
    fbxsdk::FbxScene *scene,
    fbxsdk::FbxAnimStack *anim,
    float framerate)
{
    // First, set the scene to use this animation - helps all the evaluation functions
    scene->SetCurrentAnimationStack(anim);

    // how far do we move through this
    fbxsdk::FbxTime advance;
    advance.SetSecondDouble((double)(1.0f / framerate));

    // Get the start, end, and duration of this animation.
    fbxsdk::FbxTime local_start = anim->LocalStart;
    fbxsdk::FbxTime local_end = anim->LocalStop;
    float local_start_fl = local_start.GetSecondDouble();
    float local_end_fl = local_end.GetSecondDouble();

    // Okay, so local start will sometimes start BEFORE the first frame - probably to give it something to T pose?  This was adding a jerky frame movement to UnityChan's walk cycle. Whatever, we'll always start at least 0.
    local_start_fl = Max(0.0f, local_start_fl);
    float duration_fl = local_end_fl - local_start_fl;

    // no duration, no animation!
    if (duration_fl <= 0.0f) {
        return false;
    }

    // Alright, we have a motion, we start getting data from it
    char const *motion_name = anim->GetName();
    float time_span = duration_fl;

    // set some identifying information
    motion->m_name = motion_name;
    motion->SetDuration(duration_fl);

    // number of frames should encompasses the duration, so say we are at 10hz,
    // but have an animation that is 0.21f seconds.  We need at least...
    //    3 intervals (Ceiling(10 * 0.21) = Ceiling(2.1) = 3)
    //    4 frames (0.0, 0.1, 0.2, 0.3)
    unsigned int frame_count = (unsigned int)ceilf(duration_fl * framerate) + 1;


    // Now, for each joint in our skeleton
    // not the most efficient way to go about this, but whatever - tool step
    // and it is not slow enough to matter.
    unsigned int joint_count = skeleton->GetJointCount();
    for (unsigned int joint_idx = 0; joint_idx < joint_count; ++joint_idx) {
        std::string stringName = const_cast<Skeleton&>(*skeleton).GetJointName(joint_idx);
        char const *name = stringName.c_str();

        // get the node associated with this bone
        // I rely on the names to find this - so author of animations
        // should be sure to be using the same rig/rig names.
        // (this is only important for the import process, after that
        // everything is assuming matching indices)
        fbxsdk::FbxNode *node = scene->FindNodeByName(name);
        fbxsdk::FbxNode *parent = nullptr;
        int parent_idx = skeleton->GetParentIndex(joint_idx);
        ASSERT_OR_DIE(node != nullptr, "Node cannot be a nullptr");

        // get the parent node
        if (parent_idx != -1) {
            std::string stringParentName = const_cast<Skeleton&>(*skeleton).GetJointName(parent_idx);
            char const *parent_name = stringParentName.c_str();
            parent = scene->FindNodeByName(parent_name);
            ASSERT_OR_DIE(parent != nullptr, "Parent cannot be equal to nullptr");
        }

        // Now, for this entire animation, evaluate the local transform for this bone at every interval
        // number of frames is 
        fbxsdk::FbxTime eval_time = fbxsdk::FbxTime(0);
        for (unsigned int frame_idx = 0; frame_idx < frame_count; ++frame_idx) {
            // Okay, get the pose we want
            Pose *pose = motion->GetPose(frame_idx);
            Matrix4 joint_world = GetNodeWorldTransformAtTime(node, eval_time);
            Matrix4 joint_local = joint_world;
            if (nullptr != parent) {
                Matrix4 parent_world = GetNodeWorldTransformAtTime(parent, eval_time);
                joint_local = MultiplyMatrix4ByMatrix4(joint_world, parent_world.GetInverse());
            }

            pose->m_localTranforms.insert(pose->m_localTranforms.begin() + joint_idx, joint_local);

            //------------------------------------------------------------------------
            // Missed code - make sure to advanced the clock
            eval_time += advance;
            //------------------------------------------------------------------------
        }
    }

    return true;
}

//------------------------------------------------------------------------
bool FbxLoadMotionSkeleton(Motion *motion, Skeleton const *skeleton, char const *filename, unsigned int framerate /*= 10*/)
{

    // Normal things
    fbxsdk::FbxScene *scene = FbxLoadScene(filename, false);
    if (nullptr == scene) {
        return false;
    }
    ConvertSceneToEngineBasis(scene);

    // Framerate - how often are we sampling this
    float fr = (float)framerate;
    motion->m_framerate = fr;


    // Next get the animation count.
    int anim_count = scene->GetSrcObjectCount<fbxsdk::FbxAnimStack>();
    if (anim_count > 0U) {
        // great, we have a motion
        // NOTE:  Multiple motions may be embedded in this file, you could update to extract all 
        fbxsdk::FbxAnimStack *anim = scene->GetSrcObject<fbxsdk::FbxAnimStack>(0);
        FbxImportMotionSkeleton(motion, skeleton, scene, anim, fr);
    }

    FbxUnloadScene(scene);

    /*Matrix4 transform = Matrix4();
    transform.m_iBasis = Vector4(-1.f, 0.f, 0.f, 0.f);

    unsigned int frame_count = motion->GetFrameCount();
    for (unsigned int frame_idx = 0U; frame_idx < frame_count; ++frame_idx) {
        Pose *pose = motion->GetPose(frame_idx);
        for (unsigned int i = 0; i < pose->m_localTranforms.size(); ++i) {
            pose->m_localTranforms[i] = MultiplyMatrix4ByMatrix4(pose->m_localTranforms[i], transform);
        }
    }*/

    return (motion->GetDuration() > 0.0f);
}

//------------------------------------------------------------------------
// Missed Functions
//------------------------------------------------------------------------
Matrix4 GetNodeWorldTransformAtTime(fbxsdk::FbxNode *node, fbxsdk::FbxTime time)
{
    if (nullptr == node) {
        return Matrix4();
    }

    fbxsdk::FbxMatrix fbx_mat = node->EvaluateGlobalTransform(time);
    return ToEngineMatrix(fbx_mat);
}

Matrix4 GetNodeWorldTransform2(fbxsdk::FbxNode *node)
{
    fbxsdk::FbxAMatrix fbx_mat = node->EvaluateGlobalTransform();
    return ToEngineMatrix(fbx_mat);
}

bool FbxLoadMesh(MeshBuilder *mb, char const *filename, Skeleton const *skeleton)
{
    // First - load the scene - code is identical to when you 
    // listed the file
    fbxsdk::FbxScene *scene = FbxLoadScene(filename, false);
    if (nullptr == scene)
    {
        return false;
    }

    // Second, we want to convert the basis for consitancy
    ConvertSceneToEngineBasis(scene);

    // Third, our engine only deals with triangles, 
    // but authoring programs tend to deal with a lot of 
    // different surface patch types.  For we triangulate
    // (the process of converting every non-triangle polygon to triangles )
    TriangulateScene(scene);


    // Alright, now we can start importing data, for this we'll need a matrix stack 
    // [well, it helps, you can do this without since we'll mostly be deailing with 
    // global transforms outside of what is called the "geometric transform" which is
    // always local]

    // clear all old data before importing
    mb->Clear();

    // My matrix stack treats the top as being IDENTITY
    MatrixStack* matrixStack = new MatrixStack();

    // Next, get the root node, and start importing
    fbxsdk::FbxNode *root = scene->GetRootNode();

    // import data into our builder
    ImportMeshes(mb, root, matrixStack, skeleton);

    // Clean up after ourself
    FbxUnloadScene(scene);


    // [HACK] After loading, I flip everything across the X axis
    // for conistancy.  Would love this to be part of ConvertSceneToEngineBasis,
    // but no mattter the transform I sent, it was always flipped on X [at best]
    //
    // Doing this causes it to be match Unity/Unreal/Editor in terms of orientation
    //FlipX(mb);

    return true;
}

void ImportMeshes(MeshBuilder *out, fbxsdk::FbxNode *node, MatrixStack *matriStack, Skeleton const *skeleton)
{
    // first, we're traversing the graph, keep track of our current world transform
    // (I will do this by by pushing directly the world transform of this node to my stack)
    // (that is, this transform becomes the new top exactly)
    Matrix4 transform = GetNodeWorldTransform2(node);
    matriStack->Push(transform);

    // First, traverse the scene graph, looking for FbxMesh node attributes.
    int attrib_count = node->GetNodeAttributeCount();
    for (int ai = 0; ai < attrib_count; ++ai)
    {
        fbxsdk::FbxNodeAttribute *attrib = node->GetNodeAttributeByIndex(ai);
        if (attrib->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMesh)
        {
            ImportMesh(out, matriStack, (fbxsdk::FbxMesh*)attrib, skeleton);
        }
    }

    // Try to load any meshes that are children of this node
    int child_count = node->GetChildCount();
    for (int ci = 0; ci < child_count; ++ci)
    {
        fbxsdk::FbxNode *child = node->GetChild(ci);
        ImportMeshes(out, child, matriStack, skeleton);
    }

    // we're done with this node, so its transform off the stack
    matriStack->Pop();

}

bool ImportMesh(MeshBuilder *out, MatrixStack *matrixStack, fbxsdk::FbxMesh *mesh, Skeleton const *skeleton)
{
    ASSERT_OR_DIE(mesh->IsTriangleMesh(), "Is not a triangle mesh!");

    signed int poly_count = mesh->GetPolygonCount();
    if (poly_count <= 0) {
        return false;
    }

    std::vector<fbx_skin_weight_t> skin_weights;
    if (nullptr != skeleton) {
        CalculateSkinWeights(&skin_weights, mesh, skeleton);
    }

    // Geometric Transformations only apply to the current node
    // http://download.autodesk.com/us/fbx/20112/FBX_SDK_HELP/index.html?url=WS1a9193826455f5ff1f92379812724681e696651.htm,topicNumber=d0e7429
    Matrix4 geo_trans = GetGeometricTransform(mesh->GetNode());
    matrixStack->PushMultiply(geo_trans);

    // import the mesh data.
    Matrix4 transform = matrixStack->m_top;

    char const *mat_name = "";//fbxsdk::FbxGetMaterialName(mesh);
    out->Begin(mat_name, ePrimitiveType::PRIMITIVE_TRIANGLES, false);

    signed int total_vert_count = 0;

    // Load in the mesh
    for (signed int poly_idx = 0; poly_idx < poly_count; ++poly_idx) {
        signed int vert_count = mesh->GetPolygonSize(poly_idx);
        total_vert_count += vert_count;
        //ASSERT(vert_count == 3); // Triangle Meshes should ALWAYS have 3 verts per poly
        ASSERT_OR_DIE(vert_count == 3, "Is not a triangle mesh!");
        for (signed int vert_idx = 0; vert_idx < vert_count; ++vert_idx) {

            int skinWeightIndex = mesh->GetPolygonVertex(poly_idx, vert_idx);
            if (skinWeightIndex >= 0 && !skin_weights.empty())
            {
                fbx_skin_weight_t skinWeight = skin_weights[skinWeightIndex];

                float& x = skinWeight.weights.x;
                float& y = skinWeight.weights.y;
                float& z = skinWeight.weights.z;
                float& w = skinWeight.weights.w;

                float totalWeight = x + y + z + w;

                float invTotalWeight = (1.0f / totalWeight);
                x *= invTotalWeight;
                y *= invTotalWeight;
                z *= invTotalWeight;
                w *= invTotalWeight;

                FbxImportVertex(out, transform, mesh, poly_idx, vert_idx, &skinWeight);
            }
            else
            {
                FbxImportVertex(out, transform, mesh, poly_idx, vert_idx, nullptr);
            }
        }
        Vertex3 vertexOne = out->m_vertices[out->m_vertices.size() - 3];
        int positionOne = out->m_vertices.size() - 3;
        Vertex3 vertexTwo = out->m_vertices[out->m_vertices.size() - 2];
        int positionTwo = out->m_vertices.size() - 2;
        Vertex3 vertexThree = out->m_vertices[out->m_vertices.size() - 1];
        int positionThree = out->m_vertices.size() - 1;
        std::iter_swap(out->m_vertices.begin() + positionTwo, out->m_vertices.begin() + positionThree);
        //1 2 3
        //1 3 2
    }

    out->End((unsigned int)total_vert_count);

    matrixStack->Pop();
    return true;
}

//------------------------------------------------------------------------
void FbxImportVertex(MeshBuilder *out, Matrix4 const &transform, fbxsdk::FbxMesh *mesh, int32_t polyIdx, int32_t vertIdx, fbx_skin_weight_t *skin_weight)
{
    /** NEW BITS **/

    // Will demonstarate with normal, but this process is repeated for
    // uvs, colors, tangents, bitangents (called binormals in the SDK)
    Vector3 normal;
    if (GetNormal(&normal, transform, mesh, polyIdx, vertIdx)) {
        out->SetNormal(normal);
    }

    Vector2 uvCoords;
    if (FbxGetTexCoords(&uvCoords, transform, mesh, polyIdx, vertIdx)) {
        float y = 1 - uvCoords.y;
        uvCoords.y = y;
        out->SetUV(uvCoords);
    }

    // same process for UVs, Tangents, Bitangents


    /** END NEW BITS **/

    out->SetBoneIndices(skin_weight->indices);
    out->SetBoneWeights(skin_weight->weights);

    // If we get a position- push it.
    // This actually should always succeed.
    Vector3 position;
    if (FbxGetPosition(&position, transform, mesh, polyIdx, vertIdx)) {
        position.x *= -1.f;
        out->AddVertex(position);
    }
}

//------------------------------------------------------------------------
// So skin weights are by control point - so we'll start off
// by collecting them all up front before we even start building our mesh
void CalculateSkinWeights(std::vector<fbx_skin_weight_t> *skin_weights,
    fbxsdk::FbxMesh const *mesh,
    Skeleton const *skeleton)
{
    // default them all to zero
    unsigned int ctrl_count = (unsigned int)mesh->GetControlPointsCount();
    //skin_weights->set_capacity(ctrl_count);
    //skin_weights->set_count(ctrl_count);
    skin_weights->reserve(ctrl_count);
    skin_weights->resize(ctrl_count);

    for (unsigned int i = 0; i < ctrl_count; ++i) {
        //skin_weights->get_pointer(i)->reset();
        (&skin_weights->at(i))->reset();
    }

    if (skeleton == nullptr) {
        skin_weights->clear();
        return;
    }

    // for each deformer [probably will only be one] - figure out how it affects
    // my vertices
    int deformer_count = mesh->GetDeformerCount(fbxsdk::FbxDeformer::eSkin);
    for (int didx = 0; didx < deformer_count; ++didx) {
        fbxsdk::FbxSkin *skin = (fbxsdk::FbxSkin*)mesh->GetDeformer(didx, fbxsdk::FbxDeformer::eSkin);
        if (nullptr == skin) {
            continue;
        }

        // cluster is how a single joint affects the mesh, 
        // so go over the vertices it affects and keep track of how much!
        int cluster_count = skin->GetClusterCount();
        for (int cidx = 0; cidx < cluster_count; ++cidx) {
            fbxsdk::FbxCluster *cluster = skin->GetCluster(cidx);
            fbxsdk::FbxNode const *link_node = cluster->GetLink();

            // Can't use it without a link node [shouldn't happen!]
            if (nullptr == link_node) {
                continue;
            }

            // Find the joint - if this skeleton doesn't have a joint
            // then we skip this cluster!
            unsigned int joint_idx = const_cast<Skeleton&>(*skeleton).GetJointIndex(link_node->GetName());
            if (joint_idx == -1) {
                continue;
            }

            // cluster stores things in an index buffer again,
            // it will store indices to the control points it affects, 
            // as well as how much weight we apply
            int *indices = cluster->GetControlPointIndices();
            int index_count = cluster->GetControlPointIndicesCount();
            double *weights = cluster->GetControlPointWeights();

            for (int i = 0; i < index_count; ++i) {
                int control_idx = indices[i];
                double weight = weights[i];

                fbx_skin_weight_t *skin_weight = &skin_weights->at(control_idx);
                AddHighestWeight(skin_weight, (uint32_t)joint_idx, (float)weight);
            }
        }
    }
}

//------------------------------------------------------------------------
// Keep track of this influencer if it influences more than an already existing
// bone. [all bones influence at 0 at the start]
void AddHighestWeight(fbx_skin_weight_t *skin_weight, uint32_t bone_idx, float weight)
{
    // LEFT AS EXCERCISE TO READER
    /*so skin_weight has a vec4 of influencing weights,
      you check each of these against the calc weight
      and replace the lowest value if the calc - ed one is higher
      along with the appropriate index*/
    Vector4& currentBoneWeights = skin_weight->weights;
    UnsignedIntVector4& currentBoneIndices = skin_weight->indices;

    unsigned int needToChangeThisBone = -1;
    float currentWeights[] = { currentBoneWeights.x, currentBoneWeights.y, currentBoneWeights.z, currentBoneWeights.w };
    float lowestWeight = MAX_FLOAT;
    for (int currentWeightsIndex = 0; currentWeightsIndex < 4; currentWeightsIndex++)
    {
        if (currentWeights[currentWeightsIndex] < weight)
        {
            if (currentWeights[currentWeightsIndex] < lowestWeight)
            {
                lowestWeight = currentWeights[currentWeightsIndex];
                needToChangeThisBone = currentWeightsIndex;
            }
        }
    }

    switch (needToChangeThisBone)
    {
    case 0: currentBoneWeights.x = weight; currentBoneIndices.x = bone_idx; return;
    case 1: currentBoneWeights.y = weight; currentBoneIndices.y = bone_idx; return;
    case 2: currentBoneWeights.z = weight; currentBoneIndices.z = bone_idx; return;
    case 3: currentBoneWeights.w = weight; currentBoneIndices.z = bone_idx; return;
    default: return;
    }

}

//------------------------------------------------------------------------
bool HasSkinWeights(fbxsdk::FbxMesh const *mesh)
{
    int deformer_count = mesh->GetDeformerCount(fbxsdk::FbxDeformer::eSkin);
    return deformer_count > 0;
}

/***
- TEST:  Write an hlsl file that can handle skinweights/skinindices.
- [ ] Vertex Shader just passes them onto the Fragment Shader
- [ ] Fragment Shader outputs weights as colour
return float4( input.bone_weights.xyz, 1.0f );
- [ ] Should look similar to this image for samplebox.fbx.  [will be posted]
NOTE:  Colours may not match (why might that be?), but pattern should!
/**/

/***
- Passing up bone transforms.  What are we passing up? For the bind pose, what does
this matrix end up being?
- [ ] Calculate Skinning Transforms for current skeletal instance
- [ ] Put into a StructuredBuffer
[ ] Implement a StructuredBuffer [example code]
[ ] Give your SkeletalInstance a structured buffer for storing skin transforms.
- [ ] Bind structured buffer to pipeline
[ ] These use the same slots as texture resources in DX,
so maake sure you use an index that isn't used for a texture!
- [ ] Write a skinning shader [See example that assumes two bone weights - extend to four]
- [ ] TEST!  On a TPose, it should do nothing.  Once you start animating, it should skin
/**/

//------------------------------------------------------------------------
static bool FbxImportMotion(Motion *motion,
    Skeleton const *skeleton,
    fbxsdk::FbxScene *scene,
    fbxsdk::FbxAnimStack *anim,
    float framerate)
{
    // First, set the scene to use this animation - helps all the evaluation functions
    scene->SetCurrentAnimationStack(anim);

    // how far do we move through this
    fbxsdk::FbxTime advance;
    advance.SetSecondDouble((double)(1.0f / framerate));

    // Get the start, end, and duration of this animation.
    fbxsdk::FbxTime local_start = anim->LocalStart;
    fbxsdk::FbxTime local_end = anim->LocalStop;
    float local_start_fl = local_start.GetSecondDouble();
    float local_end_fl = local_end.GetSecondDouble();

    // Okay, so local start will sometimes start BEFORE the first frame - probably to give it something to T pose?  This was adding a jerky frame movement to UnityChan's walk cycle. Whatever, we'll always start at least 0.
    local_start_fl = Max(0.0f, local_start_fl);
    float duration_fl = local_end_fl - local_start_fl;

    // no duration, no animation!
    if (duration_fl <= 0.0f) {
        return false;
    }

    // Alright, we have a motion, we start getting data from it
    char const *motion_name = anim->GetName();
    float time_span = duration_fl;

    // set some identifying information
    motion->m_name = motion_name;
    motion->SetDuration(duration_fl);

    // number of frames should encompasses the duration, so say we are at 10hz,
    // but have an animation that is 0.21f seconds.  We need at least...
    //    3 intervals (Ceiling(10 * 0.21) = Ceiling(2.1) = 3)
    //    4 frames (0.0, 0.1, 0.2, 0.3)
    unsigned int frame_count = (unsigned int)ceilf(duration_fl * framerate) + 1;


    // Now, for each joint in our skeleton
    // not the most efficient way to go about this, but whatever - tool step
    // and it is not slow enough to matter.
    unsigned int joint_count = skeleton->GetJointCount();

    for (unsigned int joint_idx = 0; joint_idx < joint_count; ++joint_idx) {
        std::string stringName = const_cast<Skeleton&>(*skeleton).GetJointName(joint_idx);
        char const *name = stringName.c_str();

        // get the node associated with this bone
        // I rely on the names to find this - so author of animations
        // should be sure to be using the same rig/rig names.
        // (this is only important for the import process, after that
        // everything is assuming matching indices)
        fbxsdk::FbxNode *node = scene->FindNodeByName(name);
        fbxsdk::FbxNode *parent = nullptr;
        int parent_idx = skeleton->GetParentIndex(joint_idx);
        ASSERT_OR_DIE(node != nullptr, "Node cannot be a nullptr");

        // get the parent node
        if (parent_idx != -1) {
            std::string stringParentName = const_cast<Skeleton&>(*skeleton).GetJointName(parent_idx);
            char const *parent_name = stringParentName.c_str();
            parent = scene->FindNodeByName(parent_name);
            ASSERT_OR_DIE(parent != nullptr, "Parent cannot be equal to nullptr");
        }

        // Now, for this entire animation, evaluate the local transform for this bone at every interval
        // number of frames is 
        fbxsdk::FbxTime eval_time = fbxsdk::FbxTime(0);
        for (unsigned int frame_idx = 0; frame_idx < frame_count; ++frame_idx) {
            // Okay, get the pose we want
            Pose *pose = motion->GetPose(frame_idx);
            Matrix4 joint_world = GetNodeWorldTransformAtTime(node, eval_time);
            Matrix4 joint_local = joint_world;
            if (nullptr != parent) {
                Matrix4 parent_world = GetNodeWorldTransformAtTime(parent, eval_time);
                joint_local = MultiplyMatrix4ByMatrix4(joint_world, parent_world.GetInverse());
            }

            pose->m_localTranforms.insert(pose->m_localTranforms.begin() + joint_idx, joint_local);

            //------------------------------------------------------------------------
            // Missed code - make sure to advanced the clock
            eval_time += advance;
            //------------------------------------------------------------------------
        }
    }

    return true;
}

bool FbxLoadMotion(Motion *motion, Skeleton const *skeleton, char const *filename, unsigned int framerate /*= 10*/)
{
    // Normal things
    fbxsdk::FbxScene *scene = FbxLoadScene(filename, false);
    if (nullptr == scene) {
        return false;
    }
    ConvertSceneToEngineBasis(scene);

    // Framerate - how often are we sampling this
    float fr = (float)framerate;
    motion->m_framerate = fr;


    // Next get the animation count.
    int anim_count = scene->GetSrcObjectCount<fbxsdk::FbxAnimStack>();
    if (anim_count > 0U) {
        // great, we have a motion
        // NOTE:  Multiple motions may be embedded in this file, you could update to extract all 
        fbxsdk::FbxAnimStack *anim = scene->GetSrcObject<fbxsdk::FbxAnimStack>(0);
        FbxImportMotion(motion, skeleton, scene, anim, fr);
    }

    FbxUnloadScene(scene);

    return (motion->GetDuration() > 0.0f);
}

//------------------------------------------------------------------------
unsigned int ConvertFBXFileToEngineFiles(char const *fbx_filename, char const *root_output_name, float impor_scale /*= 1.0f*/, unsigned int import_flags /*= IMPORT_ALL*/)
{

    if (import_flags == MESH)
    {
        WriteMeshOutToFile(fbx_filename, root_output_name, impor_scale);
    }
    if (import_flags == SKELETON)
    {
        WriteSkeletonOutToFile(fbx_filename, root_output_name, impor_scale);
    }
    if (import_flags == MOTION)
    {
        WriteMotionOutToFile(fbx_filename, root_output_name, impor_scale);
    }
    if (import_flags == IMPORT_ALL)
    {
        WriteMeshOutToFile(fbx_filename, root_output_name, impor_scale);
        WriteSkeletonOutToFile(fbx_filename, root_output_name, impor_scale);
        WriteMotionOutToFile(fbx_filename, root_output_name, impor_scale);
    }
    return 0;
}

void WriteMeshOutToFile(char const *fbx_filename, char const *root_output_name, float impor_scale /*= 1.0f*/)
{
    //*Vertices
    //* Draw Instructions

    FileBinaryStream fileBinaryStream = FileBinaryStream();
    std::string root_output_name_string = root_output_name;
    std::string newFileName = Stringf("Data/SavedFBXFiles/%s.mesh", root_output_name);
    fileBinaryStream.open_for_write(newFileName.c_str());
    std::string fbx_filename_string = fbx_filename;
    int fileNameLength = (int)fbx_filename_string.size();
    fileBinaryStream.write_bytes(&fileNameLength, sizeof(int));
    fileBinaryStream.write_bytes(fbx_filename_string.c_str(), fileNameLength);
    
    fileBinaryStream.write_bytes(&impor_scale, sizeof(float));

    MeshBuilder* meshBuilder = new MeshBuilder();
    FbxLoadMesh(meshBuilder, fbx_filename);

    int verticesSize = (int)meshBuilder->m_vertices.size();
    fileBinaryStream.write_bytes(&verticesSize, sizeof(int));
    for (int vertexIndex = 0; vertexIndex < (int)meshBuilder->m_vertices.size(); vertexIndex++)
    {
        Vector3 position = meshBuilder->m_vertices[vertexIndex].m_position;
        fileBinaryStream.write_bytes(&position.x, sizeof(float));
        fileBinaryStream.write_bytes(&position.y, sizeof(float));
        fileBinaryStream.write_bytes(&position.z, sizeof(float));

        Vector3 vecNormal = meshBuilder->m_vertices[vertexIndex].m_normal;
        fileBinaryStream.write_bytes(&vecNormal.x, sizeof(float));
        fileBinaryStream.write_bytes(&vecNormal.y, sizeof(float));
        fileBinaryStream.write_bytes(&vecNormal.z, sizeof(float));
    }

    int sizeOfDrawInstructions = (int)meshBuilder->m_drawInstructions.size();
    fileBinaryStream.write_bytes(&sizeOfDrawInstructions, sizeof(int));
    for (int drawInstructionIndex = 0; drawInstructionIndex < (int)meshBuilder->m_drawInstructions.size(); drawInstructionIndex++)
    {
        /*unsigned int    m_startIndex;
        unsigned int    m_count;
        ePrimitiveType  m_type;
        bool            m_usesIndexBuffer;*/

        drawInstruction currentDrawInstruction = meshBuilder->m_drawInstructions[drawInstructionIndex];
        fileBinaryStream.write_bytes(&currentDrawInstruction.m_startIndex, sizeof(unsigned int));
        int type = 0;
        switch (currentDrawInstruction.m_type)
        {
        case ePrimitiveType::PRIMITIVE_NONE: type = 0; break;
        case ePrimitiveType::PRIMITIVE_POINTS: type = 1; break;
        case ePrimitiveType::PRIMITIVE_LINES: type = 2; break;
        case ePrimitiveType::PRIMITIVE_TRIANGLES: type = 3; break;
        case ePrimitiveType::PRIMITIVE_TRIANGLE_STRIP: type = 4; break;
        }

        fileBinaryStream.write_bytes(&type, sizeof(int));
        fileBinaryStream.write_bytes(&currentDrawInstruction.m_startIndex, sizeof(unsigned int));
        int boolValue = 0;
        if (currentDrawInstruction.m_usesIndexBuffer)
        {
            boolValue = 1;
        }
        fileBinaryStream.write_bytes(&boolValue, sizeof(int));
    }

    fileBinaryStream.close();
}

void WriteSkeletonOutToFile(char const *fbx_filename, char const *root_output_name, float impor_scale /*= 1.0f*/)
{
    /*.skel:  These files will store the bine pose skeleton.Should just be an ID and an array
        of global transforms.*/

    FileBinaryStream fileBinaryStream = FileBinaryStream();
    std::string root_output_name_string = root_output_name;
    std::string newFileName = Stringf("Data/SavedFBXFiles/%s.skel", root_output_name);
    fileBinaryStream.open_for_write(newFileName.c_str());
    std::string fbx_filename_string = fbx_filename;
    fileBinaryStream.write_bytes(fbx_filename, fbx_filename_string.length());
    fileBinaryStream.write_byte(impor_scale * 255);

    Skeleton* skeleton = new Skeleton();
    FbxLoadSkeleton(skeleton, fbx_filename);

    //std::vector<Matrix4> m_jointTransforms;
    fileBinaryStream.write_byte((unsigned char)skeleton->m_jointTransforms.size());
    for (int jointIndex = 0; jointIndex < (int)skeleton->m_jointTransforms.size(); jointIndex++)
    {
        Vector4 translation = skeleton->m_jointTransforms[jointIndex].m_translation;
        fileBinaryStream.write_byte(translation.x * 255);
        fileBinaryStream.write_byte(translation.y * 255);
        fileBinaryStream.write_byte(translation.z * 255);
        fileBinaryStream.write_byte(translation.w * 255);
    }
}

void WriteMotionOutToFile(char const *fbx_filename, char const *root_output_name, float impor_scale /*= 1.0f*/)
{
    /*- *.motion:  A single motion.  At minimum should store off framerate, duration, frame count, and
  the array of poses.  Each pose is just an array of local transforms.  */

    FileBinaryStream fileBinaryStream = FileBinaryStream();
    std::string root_output_name_string = root_output_name;
    std::string newFileName = Stringf("Data/SavedFBXFiles/%s.motion", root_output_name);
    fileBinaryStream.open_for_write(newFileName.c_str());
    std::string fbx_filename_string = fbx_filename;
    fileBinaryStream.write_bytes(fbx_filename, fbx_filename_string.length());
    fileBinaryStream.write_byte(impor_scale * 255);

    Motion* motion = new Motion();
    Skeleton* skeleton = new Skeleton();

    FbxLoadSkeleton(skeleton, fbx_filename);
    FbxLoadMotion(motion, skeleton, fbx_filename);

    //std::vector<Matrix4> m_jointTransforms;
    fileBinaryStream.write_byte(motion->m_framerate * 255);
    fileBinaryStream.write_byte(motion->GetDuration() * 255);
    fileBinaryStream.write_byte((unsigned char)motion->GetFrameCount());

    fileBinaryStream.write_byte((unsigned char)motion->m_poses.size());
    for (int poseIndex = 0; poseIndex < (int)motion->m_poses.size(); poseIndex++)
    {
        fileBinaryStream.write_byte((unsigned char)motion->m_poses[poseIndex]->m_localTranforms.size());
        for (int localTransformIndex = 0; localTransformIndex < (int)motion->m_poses[poseIndex]->m_localTranforms.size(); localTransformIndex++)
        {
            Vector4 translation = motion->m_poses[poseIndex]->m_localTranforms[localTransformIndex].m_translation;
            fileBinaryStream.write_byte(translation.x * 255);
            fileBinaryStream.write_byte(translation.y * 255);
            fileBinaryStream.write_byte(translation.z * 255);
            fileBinaryStream.write_byte(translation.w * 255);
        }

    }
}



MeshBuilder* ConvertEngineMeshFileToFBXFile(char const *filename)
{
    FileBinaryStream fileBinaryStream = FileBinaryStream();
    fileBinaryStream.open_for_read(filename);

    float inverse255 = 1.f / 255.f;

    int stringSize;
    fileBinaryStream.read_bytes(&stringSize, sizeof(int));
    std::string fbx_filename_string;
    fileBinaryStream.read_bytes(&fbx_filename_string[0], stringSize);
    

    float import_scale;
    fileBinaryStream.read_bytes(&import_scale, sizeof(float));

    int numberOfVertices;
    fileBinaryStream.read_bytes(&numberOfVertices, sizeof(int));

    MeshBuilder* meshBuilder = new MeshBuilder();
    std::vector<Vertex3> m_vertices;
    for (int vertexIndex = 0; vertexIndex < numberOfVertices; vertexIndex++)
    {
        float positionX;
        fileBinaryStream.read_bytes(&positionX, sizeof(float));
        float positionY;
        fileBinaryStream.read_bytes(&positionY, sizeof(float));
        float positionZ;
        fileBinaryStream.read_bytes(&positionZ, sizeof(float));

        Vector3 position = Vector3(positionX, positionY, positionZ);

        float normalX;
        fileBinaryStream.read_bytes(&normalX, sizeof(float));
        float normalY;
        fileBinaryStream.read_bytes(&normalY, sizeof(float));
        float normalZ;
        fileBinaryStream.read_bytes(&normalZ, sizeof(float));

        Vector3 normal = Vector3(normalX, normalY, normalZ);

        meshBuilder->SetNormal(normal);
        meshBuilder->AddVertex(position);        
    }
    return meshBuilder;
}

Skeleton* ConvertEngineSkeletonFileToFBXFile(char const *filename)
{
    return nullptr;
}

Motion* ConvertEngineMotionFileToFBXFile(char const *filename)
{
    return nullptr;
}


/***
DEFINITIONS
- Skin Transform:  A tranform use to apply a bones change to a vertex.
*/

Vector2 ToVec2(fbxsdk::FbxVector2 fbx_pos)
{
    Vector2 vect2 = Vector2();
    vect2.x = (float)fbx_pos[0];
    vect2.y = (float)fbx_pos[1];
    return vect2;
}

//------------------------------------------------------------------------
Vector3 ToVec3(fbxsdk::FbxVector4 fbx_pos)
{
    Vector3 vect3 = Vector3();
    vect3.x = (float)fbx_pos[0];
    vect3.y = (float)fbx_pos[1];
    vect3.z = (float)fbx_pos[2];
    return vect3;
}

//------------------------------------------------------------------------
Vector4 ToVec4(fbxsdk::FbxVector4 fbx_pos)
{
    Vector4 vect4 = Vector4();
    vect4.x = (float)fbx_pos[0];
    vect4.y = (float)fbx_pos[1];
    vect4.z = (float)fbx_pos[2];
    vect4.w = (float)fbx_pos[3];
    return vect4;
}


//------------------------------------------------------------------------
float Max(float floatOne, float floatTwo)
{
    if (floatOne > floatTwo)
        return floatOne;
    return floatTwo;
}


#else 
// Stubs so that if we don't have the FBX SDK Available, our program does not crash
void FbxListFile(char const*) {}

#endif
