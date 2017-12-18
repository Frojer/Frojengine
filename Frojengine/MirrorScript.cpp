#include "MirrorScript.h"
#include "FJMath.h"

void MirrorScript::Initialize()
{

}

void SettingRenderer(list<CObject*> children)
{
	FOR_STL(children)
	{
		if ((*iter)->m_pRenderer != nullptr)
		{
			(*iter)->m_pRenderer->SetCounterClockwise(true);
			(*iter)->m_pRenderer->SetStencilEnable(true);
			(*iter)->m_pRenderer->m_stencilRef = 1;
			(*iter)->m_pRenderer->SetStencilFuncFront(COMPARISON_EQUAL);
			(*iter)->m_pRenderer->SetStencilFuncBack(COMPARISON_EQUAL);
		}

		SettingRenderer((*iter)->GetChildren());
	}
}


void MirrorScript::Update()
{
	VECTOR pos, rot, scale;
	VECTOR3 v;

	//평면 생성.
	XMVECTOR planePos = XMLoadFloat3(&VECTOR3(0, 0, 0));
	XMVECTOR planeDir = XMLoadFloat3(&VECTOR3(0, 1, 0));
	VECTOR plane = XMPlaneFromPointNormal(planePos, planeDir);
	MATRIXA mReflect = XMMatrixReflect(plane);

	MATRIXA mTMReflect = pModelTr->GetWorldMatrix() * mReflect;

	XMMatrixDecompose(&scale, &rot, &pos, mTMReflect);
	XMStoreFloat3(&v, pos);
	GetMyObject()->m_pTransform->SetPositionWorld(v);
	QuaternionToPitchYawRoll(&v, &rot);
	GetMyObject()->m_pTransform->SetRotationDegree(v);
	XMStoreFloat3(&v, scale);
	GetMyObject()->m_pTransform->m_vScale = v;

	SettingRenderer(GetMyObject()->GetChildren());
}
