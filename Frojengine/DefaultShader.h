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

		VECTOR mtrlDiffuse;  //�� ����(Ȯ�걤) �� �ݻ���(%) 
		VECTOR mtrlAmbient;  //���� ����(�ֺ���) �� �ݻ���(%) 
	};
};