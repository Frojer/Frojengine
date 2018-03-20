#pragma once

#include "FJDefine.h"

VECTOR3 operator+ (const VECTOR3& lhs, const VECTOR3& rhs);
VECTOR3 operator- (const VECTOR3& lhs, const VECTOR3& rhs);
void operator+= (VECTOR3& lhs, const VECTOR3& rhs);
void operator-= (VECTOR3& lhs, const VECTOR3& rhs);
VECTOR3 operator* (const VECTOR3& lhs, const float& rhs);
VECTOR3 operator/ (const VECTOR3& lhs, const float& rhs);
VECTOR3 operator- (const VECTOR3& lhs);
void QuaternionToPitchYawRoll(VECTOR3* pEular, const VECTOR* pQ);