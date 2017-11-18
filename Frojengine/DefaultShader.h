#pragma once

#include "Shader.h"

class DefaultShader : public CShader
{
	struct CB_Default
	{
		MATRIXA mWorld;
		MATRIXA mView;
		MATRIXA mWV;
		MATRIXA mProj;

		VECTOR mtrlDiffuse;  //주 광량(확산광) 의 반사율(%) 
		VECTOR mtrlAmbient;  //보조 광량(주변광) 의 반사율(%) 
	};
};