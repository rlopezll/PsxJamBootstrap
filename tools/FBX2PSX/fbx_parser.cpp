#include "fbx_parser.h"
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <set>
#include "fbxsdk.h"

#define IOS_CUSTOM_REF (*pManager->GetIOSettings())

void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene);
void DestroySdkObjects(FbxManager* pManager, bool pExitStatus);
bool LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename);
ResultParserFBX processNode(FbxNode* node, bool is_root);
ResultParserFBX processMesh(FbxMesh* pMesh);
FbxAMatrix GetGlobalDefaultPosition(FbxNode* pNode);

TImporterContext *g_context = nullptr;

ResultParserFBX parserFBX(const char *filename, TImporterContext *context)
{
  FbxManager* lSdkManager = NULL;
  FbxScene* lScene = NULL;
  bool lResult;

  g_context = context;
  // Prepare the FBX SDK.
  InitializeSdkObjects(lSdkManager, lScene);

  lResult = LoadScene(lSdkManager, lScene, filename);
  assert(lResult);

  if (lScene)
  {
    //get root node of the fbx scene
    FbxNode* lRootNode = lScene->GetRootNode();
    ResultParserFBX res = processNode(lRootNode, true);

    /*FbxAnimStack *anim_stack = lScene->GetCurrentAnimationStack();
    if (anim_stack) {
      int a = 0;
      a = 3;
      int nbAnimLayers = anim_stack->GetMemberCount<FbxAnimLayer>();
      for (int l = 0; l < nbAnimLayers; l++)
      {
        FbxAnimLayer* anim_layer = anim_stack->GetMember<FbxAnimLayer>(l);
        for (auto &n : g_nodes) {
          FbxAnimCurve* lAnimCurve = n->LclRotation.GetCurve(anim_layer, FBXSDK_CURVENODE_COMPONENT_X);
          if (lAnimCurve) {
            printf("Node '%s' has anim\n", n->GetName());
            a = 5;
          }
        }
      }

    }*/
    return res;
  }
  else {
    return ResultParserFBX::CAN_OPEN_SCENE;
  }

}

void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene)
{
  //The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
  pManager = FbxManager::Create();
  if (!pManager)
  {
    FBXSDK_printf("Error: Unable to create FBX Manager!\n");
    exit(1);
  }
  else FBXSDK_printf("Autodesk FBX SDK version %s\n", pManager->GetVersion());

  //Create an IOSettings object. This object holds all import/export settings.
  FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
  pManager->SetIOSettings(ios);

  //Load plugins from the executable directory (optional)
  FbxString lPath = FbxGetApplicationDirectory();
  pManager->LoadPluginsDirectory(lPath.Buffer());

  //Create an FBX scene. This object holds most objects imported/exported from/to files.
  pScene = FbxScene::Create(pManager, "My Scene");
  if (!pScene)
  {
    FBXSDK_printf("Error: Unable to create FBX scene!\n");
    exit(1);
  }
}

void DestroySdkObjects(FbxManager* pManager, bool pExitStatus)
{
  //Delete the FBX Manager. All the objects that have been allocated using the FBX Manager and that haven't been explicitly destroyed are also automatically destroyed.
  if (pManager) pManager->Destroy();
  if (pExitStatus) FBXSDK_printf("Program Success!\n");
}

bool LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename)
{
  int lFileMajor, lFileMinor, lFileRevision;
  int lSDKMajor, lSDKMinor, lSDKRevision;
  //int lFileFormat = -1;
  int i, lAnimStackCount;
  bool lStatus;
  char lPassword[1024];

  // Get the file version number generate by the FBX SDK.
  FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

  // Create an importer.
  FbxImporter* lImporter = FbxImporter::Create(pManager, "");

  // Initialize the importer by providing a filename.
  const bool lImportStatus = lImporter->Initialize(pFilename, -1, pManager->GetIOSettings());
  lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

  if (!lImportStatus)
  {
    FbxString error = lImporter->GetStatus().GetErrorString();
    FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
    FBXSDK_printf("Error returned: %s\n\n", error.Buffer());

    if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
    {
      FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
      FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);
    }

    return false;
  }

  FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);

  if (lImporter->IsFBX())
  {
    FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);

    // From this point, it is possible to access animation stack information without
    // the expense of loading the entire file.

    FBXSDK_printf("Animation Stack Information\n");

    lAnimStackCount = lImporter->GetAnimStackCount();

    FBXSDK_printf("    Number of Animation Stacks: %d\n", lAnimStackCount);
    FBXSDK_printf("    Current Animation Stack: \"%s\"\n", lImporter->GetActiveAnimStackName().Buffer());
    FBXSDK_printf("\n");

    for (i = 0; i < lAnimStackCount; i++)
    {
      FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);

      FBXSDK_printf("    Animation Stack %d\n", i);
      FBXSDK_printf("         Name: \"%s\"\n", lTakeInfo->mName.Buffer());
      FBXSDK_printf("         Description: \"%s\"\n", lTakeInfo->mDescription.Buffer());

      // Change the value of the import name if the animation stack should be imported 
      // under a different name.
      FBXSDK_printf("         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer());

      // Set the value of the import state to false if the animation stack should be not
      // be imported. 
      FBXSDK_printf("         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false");
      FBXSDK_printf("\n");
    }

    // Set the import states. By default, the import states are always set to 
    // true. The code below shows how to change these states.
    IOS_CUSTOM_REF.SetBoolProp(IMP_FBX_MATERIAL, true);
    IOS_CUSTOM_REF.SetBoolProp(IMP_FBX_TEXTURE, true);
    IOS_CUSTOM_REF.SetBoolProp(IMP_FBX_LINK, true);
    IOS_CUSTOM_REF.SetBoolProp(IMP_FBX_SHAPE, true);
    IOS_CUSTOM_REF.SetBoolProp(IMP_FBX_GOBO, true);
    IOS_CUSTOM_REF.SetBoolProp(IMP_FBX_ANIMATION, true);
    IOS_CUSTOM_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
  }

  // Import the scene.
  lStatus = lImporter->Import(pScene);

  if (lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
  {
    FBXSDK_printf("Please enter password: ");

    lPassword[0] = '\0';

    FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
      scanf("%s", lPassword);
    FBXSDK_CRT_SECURE_NO_WARNING_END

      FbxString lString(lPassword);

    IOS_CUSTOM_REF.SetStringProp(IMP_FBX_PASSWORD, lString);
    IOS_CUSTOM_REF.SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);

    lStatus = lImporter->Import(pScene);

    if (lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
    {
      FBXSDK_printf("\nPassword is wrong, import aborted.\n");
    }
  }

  // Destroy the importer.
  lImporter->Destroy();

  return lStatus;
}

// Recursive function to get a node's global default position.
// As a prerequisite, parent node's default local position must be already set.
FbxAMatrix GetGlobalDefaultPosition(FbxNode* pNode)
{
  FbxAMatrix lLocalPosition;
  FbxAMatrix lGlobalPosition;
  FbxAMatrix lParentGlobalPosition;

  lLocalPosition.SetT(pNode->LclTranslation.Get());
  lLocalPosition.SetR(pNode->LclRotation.Get());
  lLocalPosition.SetS(pNode->LclScaling.Get());

  if (pNode->GetParent())
  {
    lParentGlobalPosition = GetGlobalDefaultPosition(pNode->GetParent());
    lGlobalPosition = lParentGlobalPosition * lLocalPosition;
  }
  else
  {
    lGlobalPosition = lLocalPosition;
  }

  return lGlobalPosition;
}

ResultParserFBX processNode(FbxNode* node, bool is_root_node) {
  ResultParserFBX ret;
  const char *name = node->GetName();
  FbxAMatrix matrix = GetGlobalDefaultPosition(node);
  float mtx[16];
  FbxDouble *values = &matrix.mData[0].mData[0];
  for (size_t i = 0; i < 16; ++i) {
    mtx[i] = static_cast<float>(values[i]);
  }
  bool processed = false;

  if (g_context->inter->addEntity && !is_root_node) {
    g_context->inter->addEntity(g_context, name, mtx);
  }
  FbxVector4 node_pre_rotation_euler = node->GetPreRotation(FbxNode::eSourcePivot);

  if (node->GetMesh()) {
    if (g_context->inter->addMesh) {
      g_context->inter->addMesh(g_context, name, mtx);
    }
    FbxMesh *mesh = node->GetMesh();
    //FbxQuaternion q;
    //q.SetAxisAngle(FbxVector4(1.0f, 0.0f, 0.0f, 0.0f), -90.0f);
    FbxAMatrix rot_mtx;
    rot_mtx.SetTRS(FbxVector4(0.0f, 0.0f, 0.0f, 0.0f), node_pre_rotation_euler, FbxVector4(1.0f, 1.0f, 1.0f, 1.0f));
    //rot_mtx.SetTQS(FbxVector4(0.0f, 0.0f, 0.0f, 0.0f), q, FbxVector4(1.0f, 1.0f, 1.0f, 1.0f));
    mesh->SetPivot(rot_mtx);
    mesh->ApplyPivot();
    ret = processMesh(mesh);
    processed = true;
  }

  /*
  if (processed) {
    int material_count = node->GetMaterialCount();
    for (int lCount = 0; lCount < material_count; lCount++)
    {
      const FbxSurfaceMaterial *lMaterial = node->GetMaterial(lCount);
      //FbxProperty lFbxProp = lMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
      FbxProperty lFbxProp = lMaterial->GetFirstProperty();
      while (lFbxProp.IsValid())
      {
        if (lFbxProp.GetSrcObjectCount<FbxTexture>() > 0)
        {
          for (int j = 0; j<lFbxProp.GetSrcObjectCount<FbxFileTexture>(); ++j)
          {
            const FbxFileTexture *lTex = lFbxProp.GetSrcObject<FbxFileTexture>(j);
            const char *filename = lTex->GetFileName();
            if (g_context->inter->setTexture) {
              FbxString prop_name = lFbxProp.GetName();
              g_context->inter->setTexture(g_context, filename, prop_name.Buffer());
            }
          }
        }
        lFbxProp = lMaterial->GetNextProperty(lFbxProp);
      }
    }
  }
  */

  for (int i = 0; i < node->GetChildCount(); ++i) {
    FbxNode *node_child = node->GetChild(i);
    ret = processNode(node_child, false);
    if (ret != ResultParserFBX::OK)
      return ret;
  }
  return ret;
}

ResultParserFBX processMesh(FbxMesh* pMesh)
{
  int i, j, lPolygonCount = pMesh->GetPolygonCount();
  FbxVector4* lControlPoints = pMesh->GetControlPoints();

  if (g_context->inter->setFormatVertex) {
    bool has_uvs = pMesh->GetElementUVCount() > 0;
    bool has_color = pMesh->GetElementVertexColorCount() > 0;
    bool has_normals = pMesh->GetElementNormalCount() > 0;
    bool has_tangents = pMesh->GetElementTangentCount() > 0;
    bool has_binormals = pMesh->GetElementBinormalCount() > 0;
    g_context->inter->setFormatVertex(g_context, has_uvs, has_color, has_normals, has_tangents, has_binormals );
  }
  if (g_context->inter->setNumVertices) {
    g_context->inter->setNumVertices(g_context, lPolygonCount*3);
  }

  struct SVertex {
    float x, y, z;
    void set(float _x, float _y, float _z) {
			x = _x;
      y = _y;
      z = _z;
    }
    SVertex(float _x, float _y, float _z) {
      set(_x, _y, _z);
    }
    bool operator==(const SVertex& vertex) {
      return x == vertex.x && y == vertex.y && z == vertex.z;
    }
  };

  std::vector<SVertex> allVertexs;

  int vertexId = 0;
  for (i = 0; i < lPolygonCount; i++)
  {
    int l;
    int lPolygonSize = pMesh->GetPolygonSize(i);
    if (lPolygonSize != 3) {
      return ResultParserFBX::FBX_MUST_BE_TRIANGLES;
    }
    for (j = 0; j < lPolygonSize; j++)
    {
      int lControlPointIndex = pMesh->GetPolygonVertex(i, j);
      FbxVector4 pos = lControlPoints[lControlPointIndex];
      if (g_context->inter->setPosition) {
        SVertex curr_vertex(static_cast<float>(pos.mData[0]), static_cast<float>(pos.mData[1]), static_cast<float>(pos.mData[2]));
        int index = 0;
        bool bFound = false;
        for (auto c : allVertexs) {
          if (c == curr_vertex) {
            bFound = true;
						break;
					}
          ++index;
        }
				if (bFound) {
					vertexId = index;
				}
				else {
					allVertexs.push_back(curr_vertex);
					vertexId = allVertexs.size() - 1;
				}
        g_context->inter->setPosition(g_context, vertexId, static_cast<float>(pos.mData[0]), static_cast<float>(pos.mData[1]), static_cast<float>(pos.mData[2]));
        //++vertexId;
      }

      for (l = 0; l < pMesh->GetElementVertexColorCount(); l++)
      {
        FbxGeometryElementVertexColor* leVtxc = pMesh->GetElementVertexColor(l);
        if (leVtxc->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
          switch (leVtxc->GetReferenceMode())
          {
          case FbxGeometryElement::eDirect:
          {
            FbxColor color = leVtxc->GetDirectArray().GetAt(vertexId);
            if (g_context->inter->setColor) {
              g_context->inter->setColor(g_context, vertexId, static_cast<float>(color.mRed), static_cast<float>(color.mGreen), static_cast<float>(color.mBlue), static_cast<float>(color.mAlpha));
            }
            break;
          }
          case FbxGeometryElement::eIndexToDirect:
          {
            int id = leVtxc->GetIndexArray().GetAt(vertexId);
            FbxColor color = leVtxc->GetDirectArray().GetAt(id);
            if (g_context->inter->setColor) {
              g_context->inter->setColor(g_context, vertexId, static_cast<float>(color.mRed), static_cast<float>(color.mGreen), static_cast<float>(color.mBlue), static_cast<float>(color.mAlpha));
            }
          }
          break;
          default:
            break; // other reference modes not shown here!
          }
        }
      }
      for (l = 0; l < pMesh->GetElementUVCount(); ++l)
      {
        FbxGeometryElementUV* leUV = pMesh->GetElementUV(l);
        if (leUV->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
          int lTextureUVIndex = pMesh->GetTextureUVIndex(i, j);
          switch (leUV->GetReferenceMode())
          {
          case FbxGeometryElement::eDirect:
          case FbxGeometryElement::eIndexToDirect:
          {
            FbxVector2 vec2 = leUV->GetDirectArray().GetAt(lTextureUVIndex);
            if (g_context->inter->setUV) {
              g_context->inter->setUV(g_context, vertexId, static_cast<float>(vec2.mData[0]), static_cast<float>(vec2.mData[1]));
            }
          }
          break;
          default:
            break; // other reference modes not shown here!
          }
        }
      }

      for (l = 0; l < pMesh->GetElementNormalCount(); ++l)
      {
        FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(l);

        if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
        {
          switch (leNormal->GetReferenceMode())
          {
          case FbxGeometryElement::eDirect:
          {
            FbxVector4 vtx = leNormal->GetDirectArray().GetAt(vertexId);
            if (g_context->inter->setNormal) {
              g_context->inter->setNormal(g_context, vertexId, static_cast<float>(vtx.mData[0]), static_cast<float>(vtx.mData[1]), static_cast<float>(vtx.mData[2]));
            }
            break;
          }
          case FbxGeometryElement::eIndexToDirect:
          {
            int id = leNormal->GetIndexArray().GetAt(vertexId);
            FbxVector4 vtx = leNormal->GetDirectArray().GetAt(id);
            if (g_context->inter->setNormal) {
              g_context->inter->setNormal(g_context, vertexId, static_cast<float>(vtx.mData[0]), static_cast<float>(vtx.mData[1]), static_cast<float>(vtx.mData[2]));
            }
          }
          break;
          default:
            break; // other reference modes not shown here!
          }
        }

      }

      for (l = 0; l < pMesh->GetElementTangentCount(); ++l)
      {
        FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent(l);

        if (leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
        {
          switch (leTangent->GetReferenceMode())
          {
          case FbxGeometryElement::eDirect:
          {
            FbxVector4 vtx = leTangent->GetDirectArray().GetAt(vertexId);
            if (g_context->inter->setTangent) {
              g_context->inter->setTangent(g_context, vertexId, static_cast<float>(vtx.mData[0]), static_cast<float>(vtx.mData[1]), static_cast<float>(vtx.mData[2]), static_cast<float>(vtx.mData[3]));
            }
            break;
          }
          case FbxGeometryElement::eIndexToDirect:
          {
            int id = leTangent->GetIndexArray().GetAt(vertexId);
            FbxVector4 vtx = leTangent->GetDirectArray().GetAt(id);
            if (g_context->inter->setTangent) {
              g_context->inter->setTangent(g_context, vertexId, static_cast<float>(vtx.mData[0]), static_cast<float>(vtx.mData[1]), static_cast<float>(vtx.mData[2]), static_cast<float>(vtx.mData[3]));
            }
          }
          break;
          default:
            break; // other reference modes not shown here!
          }
        }

      }

      for (l = 0; l < pMesh->GetElementBinormalCount(); ++l)
      {

        FbxGeometryElementBinormal* leBinormal = pMesh->GetElementBinormal(l);

        if (leBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
        {
          switch (leBinormal->GetReferenceMode())
          {
          case FbxGeometryElement::eDirect:
          {
            FbxVector4 vtx = leBinormal->GetDirectArray().GetAt(vertexId);
            if (g_context->inter->setBinormal) {
              g_context->inter->setBinormal(g_context, vertexId, static_cast<float>(vtx.mData[0]), static_cast<float>(vtx.mData[1]), static_cast<float>(vtx.mData[2]));
            }
            break;
          }
          case FbxGeometryElement::eIndexToDirect:
          {
            int id = leBinormal->GetIndexArray().GetAt(vertexId);
            FbxVector4 vtx = leBinormal->GetDirectArray().GetAt(id);
            if (g_context->inter->setBinormal) {
              g_context->inter->setBinormal(g_context, vertexId, static_cast<float>(vtx.mData[0]), static_cast<float>(vtx.mData[1]), static_cast<float>(vtx.mData[2]));
            }
          }
          break;
          default:
            break; // other reference modes not shown here!
          }
        }
      }
      //vertexId++;
    } // for polygonSize
  } // for polygonCount

  return ResultParserFBX::OK;
}

