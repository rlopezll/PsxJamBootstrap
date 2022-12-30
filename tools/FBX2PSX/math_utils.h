#pragma once

struct Vector3f {
	float x, y, z;
	void Set(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}
	static const Vector3f Zero;
	static const Vector3f One;
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
	Vector4f operator+(const Vector4f& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}
};

struct Matrix44 {
	union {
		float m[16];
		Vector4f v[4];
		struct {
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
		} Fields;
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

	void SetScale(const Vector3f& scale) {
		v[0].Set(scale.x, 0.0f, 0.0f, 0.0f);
		v[1].Set(0.0f, scale.y, 0.0f, 0.0f);
		v[2].Set(0.0f, 0.0f, scale.z, 0.0f);
		v[3].Set(0.0f, 0.0f, 0.0f, 1.0f);
	}

	Matrix44 operator*(const Matrix44& r) {
		Fields.m00 = Fields.m00 * r.Fields.m00 + Fields.m01 * r.Fields.m10 + Fields.m02 * r.Fields.m20 + Fields.m03 * r.Fields.m30;
		Fields.m01 = Fields.m00 * r.Fields.m01 + Fields.m01 * r.Fields.m11 + Fields.m02 * r.Fields.m21 + Fields.m03 * r.Fields.m31;
		Fields.m02 = Fields.m00 * r.Fields.m02 + Fields.m01 * r.Fields.m12 + Fields.m02 * r.Fields.m22 + Fields.m03 * r.Fields.m32;
		Fields.m03 = Fields.m00 * r.Fields.m03 + Fields.m01 * r.Fields.m13 + Fields.m02 * r.Fields.m23 + Fields.m03 * r.Fields.m33;

		Fields.m10 = Fields.m10 * r.Fields.m00 + Fields.m11 * r.Fields.m10 + Fields.m12 * r.Fields.m20 + Fields.m13 * r.Fields.m30;
		Fields.m11 = Fields.m10 * r.Fields.m01 + Fields.m11 * r.Fields.m11 + Fields.m12 * r.Fields.m21 + Fields.m13 * r.Fields.m31;
		Fields.m12 = Fields.m10 * r.Fields.m02 + Fields.m11 * r.Fields.m12 + Fields.m12 * r.Fields.m22 + Fields.m13 * r.Fields.m32;
		Fields.m13 = Fields.m10 * r.Fields.m03 + Fields.m11 * r.Fields.m13 + Fields.m12 * r.Fields.m23 + Fields.m13 * r.Fields.m33;

		Fields.m20 = Fields.m20 * r.Fields.m00 + Fields.m21 * r.Fields.m10 + Fields.m22 * r.Fields.m20 + Fields.m23 * r.Fields.m30;
		Fields.m21 = Fields.m20 * r.Fields.m01 + Fields.m21 * r.Fields.m11 + Fields.m22 * r.Fields.m21 + Fields.m23 * r.Fields.m31;
		Fields.m22 = Fields.m20 * r.Fields.m02 + Fields.m21 * r.Fields.m12 + Fields.m22 * r.Fields.m22 + Fields.m23 * r.Fields.m32;
		Fields.m23 = Fields.m20 * r.Fields.m03 + Fields.m21 * r.Fields.m13 + Fields.m22 * r.Fields.m23 + Fields.m23 * r.Fields.m33;

		Fields.m30 = Fields.m30 * r.Fields.m00 + Fields.m31 * r.Fields.m10 + Fields.m32 * r.Fields.m20 + Fields.m33 * r.Fields.m30;
		Fields.m31 = Fields.m30 * r.Fields.m01 + Fields.m31 * r.Fields.m11 + Fields.m32 * r.Fields.m21 + Fields.m33 * r.Fields.m31;
		Fields.m32 = Fields.m30 * r.Fields.m02 + Fields.m31 * r.Fields.m12 + Fields.m32 * r.Fields.m22 + Fields.m33 * r.Fields.m32;
		Fields.m33 = Fields.m30 * r.Fields.m03 + Fields.m31 * r.Fields.m13 + Fields.m32 * r.Fields.m23 + Fields.m33 * r.Fields.m33;
		return *this;
	}

	Vector3f operator*(const Vector3f& vector)
	{
		Vector3f out;
		out.x = v[0].x * vector.x + v[0].y * vector.y + v[0].z * vector.z + v[3].x;
		out.y = v[1].x * vector.x + v[1].y * vector.y + v[1].z * vector.z + v[3].y; 
		out.z = v[2].x * vector.x + v[2].y * vector.y + v[2].z * vector.z + v[3].z;

		return out;
	}

	void SetYawPitchRoll(float yaw, float pitch, float roll) {
		roll *= -1.0f;
		v[0].x = cosf(yaw) * cosf(roll);
		v[0].y = cosf(yaw) * -sinf(roll) * cosf(pitch) + sinf(yaw) * sinf(pitch);
		v[0].z = cosf(yaw) * -sinf(roll) * -sinf(pitch) + sinf(yaw) * cosf(pitch);

		v[1].x = sinf(roll);
		v[1].y = cosf(roll) * cosf(pitch);
		v[1].z = cosf(roll) * -sinf(pitch);

		v[2].x = -sinf(yaw) * cosf(roll);
		v[2].y = -sinf(yaw) * -sinf(roll) * cosf(pitch) + cosf(yaw) * sinf(pitch);
		v[2].z = -sinf(yaw) * -sinf(roll) * -sinf(pitch) + cosf(yaw) * cosf(pitch);
	}
};
