#pragma once

enum ResultParserFBX {
  OK,
  FBX_MUST_BE_TRIANGLES,
  CAN_OPEN_SCENE
};

typedef struct TImporterContext TImporterContext;

typedef struct {

  void (*setNumVertices)(TImporterContext *context, int nvertices);
  void (*addMesh)(TImporterContext *context, const char *name, float *mtx);
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
  void (*addEntity)(TImporterContext *context, const char *name, float *mtx);

} SFBXParserInterface;

struct TImporterContext
{
	SFBXParserInterface *inter;
	void * user_data;
};

ResultParserFBX parserFBX( const char *filename, TImporterContext *context );
