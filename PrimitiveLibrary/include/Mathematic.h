#pragma once

struct Vector3
{
	float x = 0, y = 0, z = 0;
	Vector3() {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct Quaternion4
{
	float x = 0, y = 0, z = 0, w = 1;
	Quaternion4() {}
	Quaternion4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
};

Quaternion4 Multiply(Quaternion4 a, Quaternion4 b)
{
	return Quaternion4();
}

Quaternion4 Conjugate(Quaternion4 q)
{
	return Quaternion4();
}

Vector3 Rotate(Vector3 v, Quaternion4 q)
{
	Quaternion4 r = Multiply(Multiply(q, Quaternion4(v.x, v.y, v.z, 0)), Conjugate(q));
	return Vector3(r.x, r.y, r.z);
}