#pragma once

#include <string>

struct Vector3f {
	float x, y, z;
	void Set(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}
};

struct Vector4f {
	float x, y, z, w;
	void Set(float _x, float _y, float _z, float _w) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
	Vector4f operator*(const Vector4f& v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}
};

struct Matrix44 {
	union {
		float m[16];
		Vector4f v[4];
	};

	Matrix44() {
		SetIdentity();
	}
	void SetIdentity()
	{
		v[0].Set(1.0f, 0.0f, 0.0f, 0.0f);
		v[1].Set(0.0f, 1.0f, 0.0f, 0.0f);
		v[2].Set(0.0f, 0.0f, 1.0f, 0.0f);
		v[3].Set(0.0f, 0.0f, 0.0f, 1.0f);
	}
	void SetScale(float scale) {
		v[0].Set(scale, 0.0f, 0.0f, 0.0f);
		v[1].Set(0.0f, scale, 0.0f, 0.0f);
		v[2].Set(0.0f, 0.0f, scale, 0.0f);
		v[3].Set(0.0f, 0.0f, 0.0f, 1.0f);
	}
	Matrix44 operator*(const Matrix44& m) {
		v[0] = v[0] * m.v[0];
		v[1] = v[1] * m.v[1];
		v[2] = v[2] * m.v[2];
		v[3] = v[3] * m.v[3];
	}

	Vector3f operator*(const Vector3f& vector)
	{
		Vector3f out;
		out.x = v[0].x * vector.x + v[0].y * vector.y + v[0].z * vector.z + v[0].w;
		out.y = v[1].x * vector.x + v[1].y * vector.y + v[1].z * vector.z + v[1].w;
		out.z = v[2].x * vector.x + v[2].y * vector.y + v[2].z * vector.z + v[2].w;
		return out;
	}
	void SetYawPitchRoll(float yaw, float pitch, float roll) {
		v[0].x = cosf(yaw) * cosf(pitch);
		v[0].y = cosf(yaw) * sinf(pitch) * sinf(roll) - sinf(yaw) * cosf(roll);
		v[0].z = cosf(yaw) * sinf(pitch) * cosf(roll) + sinf(yaw) * sinf(roll);

		v[1].x = sinf(yaw) * cosf(pitch);
		v[1].y = sinf(yaw) * sinf(pitch) * sinf(roll) + cosf(yaw) * cosf(roll);
		v[1].z = sinf(yaw) * sinf(pitch) * cosf(roll) - cosf(yaw) * sinf(roll);

		v[2].x = -sinf(pitch);
		v[2].y = cosf(pitch) * sinf(roll);
		v[2].z = cosf(pitch) * cosf(roll);
	}
};

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
  float       m_scalarVector[3] = { 1.0f, 1.0f, 1.0f };
  int         m_textureSize[2] = {32, 32};
  EVertexFormatOutput m_vertexFormatOutput = EVertexFormatOutput::VERTEX;
  int         m_flipUVFlags = (int)EFlipUVFlag::VERTICAL_FLIP;
	Matrix44    m_rotationMatrix;

  std::string getRelativeFolder(const std::string &folder) const;
  std::string getAbsoluteFolder(const std::string &folder) const;
  void        refreshAbsolutePaths();
};

bool importMeshFromFBX(const char *filename, const TImportParams &params);
