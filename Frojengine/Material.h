#pragma once

#include "Frojengine.h"

class CShader;
class CTexture2D;

class CMaterial : public IObject
{
private:
	struct WVP_Data
	{
		MATRIXA mWorld;
		MATRIXA mView;
		MATRIXA mWV;
		MATRIXA mProj;
	};

	struct Light_Data
	{
		VECTOR4 diffuse;
		VECTOR4 ambient;
		VECTOR3 position;
		VECTOR3 direction;
		float range;
	};

	static unordered_map<UINT, CMaterial*> _mtrlMap;
	
	CShader* _pShader;
	UINT _countTexture;
	static WVP_Data _WVPData;
	static Light_Data _LightData;
	vector<VECTOR> _constData;

	vector<VECTOR>	_vecScala;
	vector<VECTOR>	_vecVector;
	vector<MATRIXA>	_vecMatrix;

	bool _useLight;

public:
	VECTOR4 m_diffuse;
	VECTOR3 m_ambient;
	VECTOR3 m_specular;

	CTexture2D* m_pTexture[128];

private:
	static void ClearMap();
	void UpdateConstantBuffer(MATRIXA& mWorld);
	void Render();

public:
	CMaterial(CShader* shader);
	~CMaterial();

	void SetShader(CShader* shader);
	CShader* GetShader();

	void SetScalar(UINT id, float scala);
	void SetVector(UINT id, VECTOR4& vector);
	void SetMatrix(UINT id, MATRIX& matrix);

	static CMaterial* Find(UINT id);
	static CMaterial* Find(LPCWSTR name);

	// void Renderer::BufferUpdate()
	friend class Renderer;
	friend class CShader;
	friend class CScene;
	friend class FJSystemEngine;
	friend class Debug;
};