#pragma once

#include <string>
#include <fbxsdk.h>
#include <fbxsdk/core/math/fbxaffinematrix.h>
#include <fbxsdk/core/arch/fbxtypes.h>

struct TImportParams {
  enum class EVertexFormatOutput {
		VERTEX,
		VERTEX_COLOR,
		VERTEX_UV, 
		VERTEX_COLOR_UV,
		VERTEX_NORMAL,
		VERTEX_NORMAL_COLOR,
		VERTEX_NORMAL_UV
  };

	enum class EFlipUVFlag {
    VERTICAL_FLIP = 1,
    HORIZONTAL_FLIP = 2,
	};

  std::string m_rootFolder;
  std::string m_outFolderMeshes;
  bool        m_overwriteMeshes;
  int         m_textureSize[2] = {32, 32};
  EVertexFormatOutput m_vertexFormatOutput = EVertexFormatOutput::VERTEX;
  int         m_flipUVFlags = (int)EFlipUVFlag::VERTICAL_FLIP;
	FbxAMatrix  m_matrix;

  std::string getRelativeFolder(const std::string &folder) const;
  std::string getAbsoluteFolder(const std::string &folder) const;
  void        refreshAbsolutePaths();
};

bool importMeshFromFBX(const char *filename, const TImportParams &params);
