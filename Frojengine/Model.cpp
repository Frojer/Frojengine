#include "Model.h"

CModel::CModel()
	: m_pMesh(nullptr), m_pMaterial(nullptr)
{

}


CModel::CModel(CMesh* i_pMesh, CMaterial* i_pMaterial)
	: m_pMesh(i_pMesh), m_pMaterial(i_pMaterial)
{

}


CModel::~CModel()
{

}