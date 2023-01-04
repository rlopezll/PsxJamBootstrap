#pragma once

#include <string.h>
#include <fbxsdk.h>
#include <fbxsdk/core/math/fbxaffinematrix.h>

enum ResultParserFBX {
  OK,
  FBX_MUST_BE_TRIANGLES,
  CAN_OPEN_SCENE
};

struct SVertexInfo {
  float x,y,z;
  float nx,ny,nz;
  float u,v;
  SVertexInfo() {
    memset(this, 0x0, sizeof(SVertexInfo));
  }
  static bool compareVertex(const SVertexInfo& v1, const SVertexInfo& v2) {
    return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
  }
	static bool compareVertexUVs(const SVertexInfo& v1, const SVertexInfo& v2) {
		return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.u == v2.u && v1.v == v2.v;
	}
	static bool compareVertexUVsNormals(const SVertexInfo& v1, const SVertexInfo& v2) {
		return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.nx == v2.nx && v1.ny == v2.ny && v1.nz == v2.nz && v1.u == v2.u && v1.v == v2.v;
	}
	static bool compareVertexNormals(const SVertexInfo& v1, const SVertexInfo& v2) {
		return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.nx == v2.nx && v1.ny == v2.ny && v1.nz == v2.nz;
	}
};

typedef struct TImporterContext TImporterContext;

typedef struct {

  int  (*getVertexIndex)(TImporterContext* context, const SVertexInfo& vertex);
  void (*setNumVertices)(TImporterContext *context, int nvertices);
  void (*addMesh)(TImporterContext *context, const char *name, FbxAMatrix &mtx);
  void (*setFormatVertex)(TImporterContext *context, bool has_uvs, bool has_color, bool has_normals, bool has_tangents, bool has_binormals);

  void (*setPosition)(TImporterContext *context, int idx, float x, float y, float z);
  void (*setNormal)(TImporterContext *context, int idx, float x, float y, float z);
  void (*setColor)(TImporterContext *context, int idx, float r, float g, float b, float a);
  void (*setUV)(TImporterContext *context, int idx, float u, float v);
  void (*setTangent)(TImporterContext *context, int idx, float x, float y, float z, float w);
  void (*setBinormal)(TImporterContext *context, int idx, float x, float y, float z);

  void (*setTexture)(TImporterContext *context, const char *filename, const char *channel_name);
  
  void (*addAnimationTransform)(TImporterContext *context, const char *mesh_name, const char *name, int nframes);
  void (*setTransform)(TImporterContext *context, int frame, float time, float *mtx);

  void (*addMeshAttribute)(TImporterContext *context, const char *path, const char *value);
  void (*addMaterialAttribute)(TImporterContext *context, const char *material_path, const char *value);
  void (*addEntity)(TImporterContext *context, const char *name, FbxAMatrix &mtx);

} SFBXParserInterface;

struct TImporterContext
{
	SFBXParserInterface *inter;
	void * user_data;
};

ResultParserFBX parserFBX( const char *filename, TImporterContext *context );
