#pragma once
#define M_PI	3.14159265358979323846264338327950288419716939937510
class Vector2
{
public:
	// Constructors
	Vector2() : x(0.f), y(0.f)
	{

	}

	Vector2(float _x, float _y) : x(_x), y(_y)
	{

	}

	Vector2(float _v) : x(_v), y(_v)
	{

	}

	~Vector2()
	{

	}

	// Variables
	float x;
	float y;

	// Functions
	inline float Distance(Vector2 v)
	{
		return SpoofCall(sqrtf, (SpoofCall(powf, (v.x - x), 2.0f) + SpoofCall(powf, (v.y - y), 2.0f)));
	}

	inline ImVec2 ToImVec()
	{
		return ImVec2(x, y);
	}

	// Operators
	Vector2 operator+(Vector2 v)
	{
		return Vector2(x + v.x, y + v.y);
	}

	Vector2 operator-(Vector2 v)
	{
		return Vector2(x - v.x, y - v.y);
	}

	Vector2 operator*(float number) const {
		return Vector2(x * number, y * number);
	}

	Vector2 operator/(float number) const {
		return Vector2(x / number, y / number);
	}
};

class Vector4
{
public:
	// Constructors
	Vector4() : x(0.f), y(0.f), z(0.f), w(0.f)
	{

	}

	Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
	{

	}

	Vector4(float _v) : x(_v), y(_v), z(_v), w(_v)
	{

	}

	~Vector4()
	{

	}

	// Variables
	float x;
	float y;
	float z;
	float w;

	// Functions
	inline Vector4 ToColor(float fTransparant)
	{
		return Vector4(x, y, z, fTransparant);
	}

	inline ImVec4 ToImVec()
	{
		return ImVec4(x, y, z, w);
	}

	inline bool IsZero()
	{
		if (x == 0 && y == 0 && z == 0 && w == 0)
			return true;

		return false;
	}

	// Operators
	Vector4 operator+(Vector4 v)
	{
		return Vector4(x + v.x, y + v.y, z + v.z, y + v.w);
	}

	Vector4 operator-(Vector4 v)
	{
		return Vector4(x - v.x, y - v.y, z - v.z, y - v.w);
	}

	Vector4 operator*(float number) const {
		return Vector4(x * number, y * number, z * number, y * number);
	}

	Vector4 operator/(float number) const {
		return Vector4(x / number, y / number, z / number, y / number);
	}
};

typedef struct {
	float X, Y, Z;
} FVector;
class Vector3
{
public:
	// Constructors
	Vector3() : x(0.f), y(0.f), z(0.f)
	{

	}

	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
	{

	}

	Vector3(float _v) : x(_v), y(_v), z(_v)
	{

	}

	~Vector3()
	{

	}

	// Variables
	float x;
	float y;
	float z;

	// Functions
	inline float Distance(Vector3 v)
	{
		return SpoofCall(sqrtf, (SpoofCall(powf, (v.x - x), 2.0f) + SpoofCall(powf, (v.y - y), 2.0f) + SpoofCall(powf, (v.z - z), 2.0f))) / 100.0f;
	}

	inline Vector4 ToColor(float fTransparant = 1.0f)
	{
		return Vector4(x, y, z, fTransparant);
	}

	inline Vector2 ToVec2()
	{
		return Vector2(x, y);
	}

	inline ImVec3 ToImVec()
	{
		return ImVec3(x, y, z);
	}

	inline bool IsZero()
	{
		if (x == 0 && y == 0 && z == 0)
			return true;

		return false;
	}

	// Operators
	Vector3 operator+(Vector3 v)
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator-(Vector3 v)
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 operator*(float number) const {
		return Vector3(x * number, y * number, z * number);
	}

	Vector3 operator/(float number) const {
		return Vector3(x / number, y / number, z / number);
	}
};

class Matrix4
{
public:
	// Constructors
	Matrix4()
	{

	}

	Matrix4(Vector4 vPosition, Vector4 vRotation, Vector4 vScale)
	{
		_41 = vPosition.x;
		_42 = vPosition.y;
		_43 = vPosition.z;

		float x2 = vRotation.x + vRotation.x;
		float y2 = vRotation.y + vRotation.y;
		float z2 = vRotation.z + vRotation.z;

		float xx2 = vRotation.x * x2;
		float yy2 = vRotation.y * y2;
		float zz2 = vRotation.z * z2;
		_11 = (1.0f - (yy2 + zz2)) * vScale.x;
		_22 = (1.0f - (xx2 + zz2)) * vScale.y;
		_33 = (1.0f - (xx2 + yy2)) * vScale.z;

		float yz2 = vRotation.y * z2;
		float wx2 = vRotation.w * x2;
		_32 = (yz2 - wx2) * vScale.z;
		_23 = (yz2 + wx2) * vScale.y;

		float xy2 = vRotation.x * y2;
		float wz2 = vRotation.w * z2;
		_21 = (xy2 - wz2) * vScale.y;
		_12 = (xy2 + wz2) * vScale.x;

		float xz2 = vRotation.x * z2;
		float wy2 = vRotation.w * y2;
		_31 = (xz2 + wy2) * vScale.z;
		_13 = (xz2 - wy2) * vScale.x;

		_14 = 0.0f;
		_24 = 0.0f;
		_34 = 0.0f;
		_44 = 1.0f;
	}

	~Matrix4()
	{

	}

	// Variables
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;

		};
		float m[4][4];
	};

	// Operators
	Matrix4 operator*(Matrix4 mMatrix) const
	{
		Matrix4 pOut;
		pOut._11 = _11 * mMatrix._11 + _12 * mMatrix._21 + _13 * mMatrix._31 + _14 * mMatrix._41;
		pOut._12 = _11 * mMatrix._12 + _12 * mMatrix._22 + _13 * mMatrix._32 + _14 * mMatrix._42;
		pOut._13 = _11 * mMatrix._13 + _12 * mMatrix._23 + _13 * mMatrix._33 + _14 * mMatrix._43;
		pOut._14 = _11 * mMatrix._14 + _12 * mMatrix._24 + _13 * mMatrix._34 + _14 * mMatrix._44;
		pOut._21 = _21 * mMatrix._11 + _22 * mMatrix._21 + _23 * mMatrix._31 + _24 * mMatrix._41;
		pOut._22 = _21 * mMatrix._12 + _22 * mMatrix._22 + _23 * mMatrix._32 + _24 * mMatrix._42;
		pOut._23 = _21 * mMatrix._13 + _22 * mMatrix._23 + _23 * mMatrix._33 + _24 * mMatrix._43;
		pOut._24 = _21 * mMatrix._14 + _22 * mMatrix._24 + _23 * mMatrix._34 + _24 * mMatrix._44;
		pOut._31 = _31 * mMatrix._11 + _32 * mMatrix._21 + _33 * mMatrix._31 + _34 * mMatrix._41;
		pOut._32 = _31 * mMatrix._12 + _32 * mMatrix._22 + _33 * mMatrix._32 + _34 * mMatrix._42;
		pOut._33 = _31 * mMatrix._13 + _32 * mMatrix._23 + _33 * mMatrix._33 + _34 * mMatrix._43;
		pOut._34 = _31 * mMatrix._14 + _32 * mMatrix._24 + _33 * mMatrix._34 + _34 * mMatrix._44;
		pOut._41 = _41 * mMatrix._11 + _42 * mMatrix._21 + _43 * mMatrix._31 + _44 * mMatrix._41;
		pOut._42 = _41 * mMatrix._12 + _42 * mMatrix._22 + _43 * mMatrix._32 + _44 * mMatrix._42;
		pOut._43 = _41 * mMatrix._13 + _42 * mMatrix._23 + _43 * mMatrix._33 + _44 * mMatrix._43;
		pOut._44 = _41 * mMatrix._14 + _42 * mMatrix._24 + _43 * mMatrix._34 + _44 * mMatrix._44;
		return pOut;
	}
};