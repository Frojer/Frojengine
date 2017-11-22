#pragma once

#include "Frojengine.h"

class Renderer : public Component, private Device
{
public:
	CMesh* m_pMesh;
	CMaterial* m_pMaterial;

private:
	void BufferUpdate();

	virtual void Render() override;

public:
	Renderer();
	virtual ~Renderer();

	void ChangeMesh(CMesh* i_pMesh);
	void ChangeMaterial(CMaterial* i_pMaterial);
};