#pragma once

#include "Frojengine.h"

class Renderer : public Component, private Device
{
public:
	CMesh* m_pMesh;
	CMaterial* m_pMaterial;

private:
	void BufferUpdate();

public:
	Renderer();
	~Renderer();

	virtual void Update() override;
	virtual void Render() override;

	void ChangeMesh(CMesh* i_pMesh);
	void ChangeMaterial(CMaterial* i_pMaterial);
};