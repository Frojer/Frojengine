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