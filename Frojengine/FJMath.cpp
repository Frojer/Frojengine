#include "FJMath.h"


VECTOR3 operator+ (const VECTOR3& lhs, const VECTOR3& rhs)
{
	VECTOR3 vec;
	vec.x = lhs.x + rhs.x;
	vec.y = lhs.y + rhs.y;
	vec.z = lhs.z + rhs.z;
	return vec;
}


VECTOR3 operator- (const VECTOR3& lhs, const VECTOR3& rhs)
{
	VECTOR3 vec;
	vec.x = lhs.x - rhs.x;
	vec.y = lhs.y - rhs.y;
	vec.z = lhs.z - rhs.z;
	return vec;
}


void operator+= (VECTOR3& lhs, const VECTOR3& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
}


VECTOR3 operator* (const VECTOR3& lhs, const float& rhs)
{
	VECTOR3 vec;
	vec.x = lhs.x * rhs;
	vec.y = lhs.y * rhs;
	vec.z = lhs.z * rhs;
	return vec;
}


VECTOR3 operator/ (const VECTOR3& lhs, const float& rhs)
{
	VECTOR3 vec;
	vec.x = lhs.x / rhs;
	vec.y = lhs.y / rhs;
	vec.z = lhs.z / rhs;
	return vec;
}


VECTOR3 operator- (const VECTOR3& lhs)
{
	VECTOR3 vec;
	vec.x = -lhs.x;
	vec.y = -lhs.y;
	vec.z = -lhs.z;
	return vec;
}


void QuaternionToPitchYawRoll(VECTOR3* pEular, const VECTOR* pQ)
{
	VECTOR4 q, sq;
	XMStoreFloat4(&q, *pQ);
	XMStoreFloat4(&sq, XMVectorMultiply(*pQ, *pQ));

	pEular->x = XMConvertToDegrees(asinf(2.0f * (q.w * q.x - q.y * q.z)));
	pEular->y = XMConvertToDegrees(atan2f(2.0f * (q.x * q.z + q.w * q.y), (-sq.x - sq.y + sq.z + sq.w)));
	pEular->z = XMConvertToDegrees(atan2f(2.0f * (q.x * q.y + q.w * q.z), (-sq.x + sq.y - sq.z + sq.w)));
}