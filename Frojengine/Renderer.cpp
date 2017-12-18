#include "Renderer.h"

Renderer::Renderer()
	: _RSState(0), _DSState(0), m_stencilRef(0), m_pMesh(nullptr), m_pMaterial(nullptr)
{
	m_name = L"Renderer";
	_type = COMPONENT_TYPE_RENDER;
}

Renderer::~Renderer()
{

}


void Renderer::BufferUpdate()
{
	m_pMaterial->UpdateConstantBuffer(GetMyObject()->m_pTransform->GetWorldMatrix());
}


void Renderer::Render()
{
	if (m_pMesh != nullptr && m_pMaterial != nullptr)
	{
		BufferUpdate();

		m_pMesh->Render();
		m_pMaterial->Render();

		FJRenderingEngine::SetRSState(_RSState);
		FJRenderingEngine::SetDSState(_DSState, m_stencilRef);

		//±×¸®±â! Render a triangle ¡Ú
		_pDXDC->DrawIndexed(m_pMesh->m_indics.size() * 3, 0, 0);
	}
}


void Renderer::ChangeMesh(CMesh* i_pMesh)
{
	m_pMesh = i_pMesh;
}


void Renderer::ChangeMaterial(CMaterial* i_pMaterial)
{
	m_pMaterial = i_pMaterial;
}


void Renderer::SetWireFrame(bool i_bSet)
{
	_RSState &= 0x7F;
	_RSState |= i_bSet ? RS_WIREFRAME : RS_SOLID;
}

bool Renderer::GetWireFrame()
{
	return (_RSState & 0x80) == RS_WIREFRAME ? true : false;
}

void Renderer::SetSolidFrame(bool i_bSet)
{
	SetWireFrame(!i_bSet);
}

bool Renderer::GetSolidFrame()
{
	return !GetWireFrame();
}

void Renderer::SetCullMode(CULL_MODE mode)
{
	_RSState &= 0x9F;
	_RSState |= mode;
}

CULL_MODE Renderer::GetCullMode()
{
	return (CULL_MODE)(_RSState & 0x60);
}

void Renderer::SetClockwise(bool i_bSet)
{
	_RSState &= 0xEF;
	_RSState |= i_bSet ? RS_CLOCKWISE : RS_COUNTER_CLOCKWISE;
}

bool Renderer::GetClockwise()
{
	return _RSState & 0x10 == RS_CLOCKWISE;
}

void Renderer::SetCounterClockwise(bool i_bSet)
{
	SetClockwise(!i_bSet);
}

bool Renderer::GetCounterClockwise()
{
	return !GetClockwise();
}

void Renderer::SetDepthEnable(bool enable)
{
	_DSState &= 0x7FFFFFFF;
	_DSState |= enable ? DS_DEPTH_TEST_ON : DS_DEPTH_TEST_OFF;
}

void Renderer::SetDepthWrite(bool enable)
{
	_DSState &= 0xBFFFFFFF;
	_DSState |= enable ? DS_DEPTH_WRITE_ON : DS_DEPTH_WRITE_OFF;
}

void Renderer::SetDepthFunc(COMPARISON_FUNC func)
{
	_DSState &= 0xC7FFFFFF;
	switch (func)
	{
	case COMPARISON_NEVER:
		_DSState |= DS_DEPTH_COMPARISON_NEVER;
		break;
	case COMPARISON_LESS:
		_DSState |= DS_DEPTH_COMPARISON_LESS;
		break;
	case COMPARISON_EQUAL:
		_DSState |= DS_DEPTH_COMPARISON_EQUAL;
		break;
	case COMPARISON_LESS_EQUAL:
		_DSState |= DS_DEPTH_COMPARISON_LESS_EQUAL;
		break;
	case COMPARISON_GREATER:
		_DSState |= DS_DEPTH_COMPARISON_GREATER;
		break;
	case COMPARISON_NOT_EQUAL:
		_DSState |= DS_DEPTH_COMPARISON_NOT_EQUAL;
		break;
	case COMPARISON_GREATER_EQUAL:
		_DSState |= DS_DEPTH_COMPARISON_GREATER_EQUAL;
		break;
	case COMPARISON_ALWAYS:
		_DSState |= DS_DEPTH_COMPARISON_ALWAYS;
		break;
	}
}

void Renderer::SetStencilEnable(bool enable)
{
	_DSState &= 0xFBFFFFFF;
	_DSState |= enable ? DS_STENCIL_ON : DS_STENCIL_OFF;
}

void Renderer::SetStencilFailOpFront(STENCIL_OP op)
{
	_DSState &= 0xFC7FFFFF;
	switch (op)
	{
	case STENCIL_OP_KEEP:
		_DSState |= DS_STENCIL_FRONT_FAIL_OP_KEEP;
		break;
	case STENCIL_OP_ZERO:
		_DSState |= DS_STENCIL_FRONT_FAIL_OP_ZERO;
		break;
	case STENCIL_OP_REPLACE:
		_DSState |= DS_STENCIL_FRONT_FAIL_OP_REPLACE;
		break;
	case STENCIL_OP_INCR_SAT:
		_DSState |= DS_STENCIL_FRONT_FAIL_OP_INCR_SAT;
		break;
	case STENCIL_OP_DECR_SAT:
		_DSState |= DS_STENCIL_FRONT_FAIL_OP_DECR_SAT;
		break;
	case STENCIL_OP_INVERT:
		_DSState |= DS_STENCIL_FRONT_FAIL_OP_INVERT;
		break;
	case STENCIL_OP_INCR:
		_DSState |= DS_STENCIL_FRONT_FAIL_OP_INCR;
		break;
	case STENCIL_OP_DECR:
		_DSState |= DS_STENCIL_FRONT_FAIL_OP_DECR;
		break;
	}
}

void Renderer::SetStencilDepthFailOpFront(STENCIL_OP op)
{
	_DSState &= 0xFF8FFFFF;
	switch (op)
	{
	case STENCIL_OP_KEEP:
		_DSState |= DS_STENCIL_FRONT_DEPTH_FAIL_OP_KEEP;
		break;
	case STENCIL_OP_ZERO:
		_DSState |= DS_STENCIL_FRONT_DEPTH_FAIL_OP_ZERO;
		break;
	case STENCIL_OP_REPLACE:
		_DSState |= DS_STENCIL_FRONT_DEPTH_FAIL_OP_REPLACE;
		break;
	case STENCIL_OP_INCR_SAT:
		_DSState |= DS_STENCIL_FRONT_DEPTH_FAIL_OP_INCR_SAT;
		break;
	case STENCIL_OP_DECR_SAT:
		_DSState |= DS_STENCIL_FRONT_DEPTH_FAIL_OP_DECR_SAT;
		break;
	case STENCIL_OP_INVERT:
		_DSState |= DS_STENCIL_FRONT_DEPTH_FAIL_OP_INVERT;
		break;
	case STENCIL_OP_INCR:
		_DSState |= DS_STENCIL_FRONT_DEPTH_FAIL_OP_INCR;
		break;
	case STENCIL_OP_DECR:
		_DSState |= DS_STENCIL_FRONT_DEPTH_FAIL_OP_DECR;
		break;
	}
}

void Renderer::SetStencilPassOpFront(STENCIL_OP op)
{
	_DSState &= 0xFFF1FFFF;
	switch (op)
	{
	case STENCIL_OP_KEEP:
		_DSState |= DS_STENCIL_FRONT_PASS_OP_KEEP;
		break;
	case STENCIL_OP_ZERO:
		_DSState |= DS_STENCIL_FRONT_PASS_OP_ZERO;
		break;
	case STENCIL_OP_REPLACE:
		_DSState |= DS_STENCIL_FRONT_PASS_OP_REPLACE;
		break;
	case STENCIL_OP_INCR_SAT:
		_DSState |= DS_STENCIL_FRONT_PASS_OP_INCR_SAT;
		break;
	case STENCIL_OP_DECR_SAT:
		_DSState |= DS_STENCIL_FRONT_PASS_OP_DECR_SAT;
		break;
	case STENCIL_OP_INVERT:
		_DSState |= DS_STENCIL_FRONT_PASS_OP_INVERT;
		break;
	case STENCIL_OP_INCR:
		_DSState |= DS_STENCIL_FRONT_PASS_OP_INCR;
		break;
	case STENCIL_OP_DECR:
		_DSState |= DS_STENCIL_FRONT_PASS_OP_DECR;
		break;
	}
}

void Renderer::SetStencilFuncFront(COMPARISON_FUNC func)
{
	_DSState &= 0xFFFE3FFF;
	switch (func)
	{
	case COMPARISON_NEVER:
		_DSState |= DS_STENCIL_FRONT_COMPARISON_NEVER;
		break;
	case COMPARISON_LESS:
		_DSState |= DS_STENCIL_FRONT_COMPARISON_LESS;
		break;
	case COMPARISON_EQUAL:
		_DSState |= DS_STENCIL_FRONT_COMPARISON_EQUAL;
		break;
	case COMPARISON_LESS_EQUAL:
		_DSState |= DS_STENCIL_FRONT_COMPARISON_LESS_EQUAL;
		break;
	case COMPARISON_GREATER:
		_DSState |= DS_STENCIL_FRONT_COMPARISON_GREATER;
		break;
	case COMPARISON_NOT_EQUAL:
		_DSState |= DS_STENCIL_FRONT_COMPARISON_NOT_EQUAL;
		break;
	case COMPARISON_GREATER_EQUAL:
		_DSState |= DS_STENCIL_FRONT_COMPARISON_GREATER_EQUAL;
		break;
	case COMPARISON_ALWAYS:
		_DSState |= DS_STENCIL_FRONT_COMPARISON_ALWAYS;
		break;
	}
}

void Renderer::SetStencilFailOpBack(STENCIL_OP op)
{
	_DSState &= 0xFFFFC7FF;
	switch (op)
	{
	case STENCIL_OP_KEEP:
		_DSState |= DS_STENCIL_BACK_FAIL_OP_KEEP;
		break;
	case STENCIL_OP_ZERO:
		_DSState |= DS_STENCIL_BACK_FAIL_OP_ZERO;
		break;
	case STENCIL_OP_REPLACE:
		_DSState |= DS_STENCIL_BACK_FAIL_OP_REPLACE;
		break;
	case STENCIL_OP_INCR_SAT:
		_DSState |= DS_STENCIL_BACK_FAIL_OP_INCR_SAT;
		break;
	case STENCIL_OP_DECR_SAT:
		_DSState |= DS_STENCIL_BACK_FAIL_OP_DECR_SAT;
		break;
	case STENCIL_OP_INVERT:
		_DSState |= DS_STENCIL_BACK_FAIL_OP_INVERT;
		break;
	case STENCIL_OP_INCR:
		_DSState |= DS_STENCIL_BACK_FAIL_OP_INCR;
		break;
	case STENCIL_OP_DECR:
		_DSState |= DS_STENCIL_BACK_FAIL_OP_DECR;
		break;
	}
}

void Renderer::SetStencilDepthFailOpBack(STENCIL_OP op)
{
	_DSState &= 0xFFFFF8FF;
	switch (op)
	{
	case STENCIL_OP_KEEP:
		_DSState |= DS_STENCIL_BACK_DEPTH_FAIL_OP_KEEP;
		break;
	case STENCIL_OP_ZERO:
		_DSState |= DS_STENCIL_BACK_DEPTH_FAIL_OP_ZERO;
		break;
	case STENCIL_OP_REPLACE:
		_DSState |= DS_STENCIL_BACK_DEPTH_FAIL_OP_REPLACE;
		break;
	case STENCIL_OP_INCR_SAT:
		_DSState |= DS_STENCIL_BACK_DEPTH_FAIL_OP_INCR_SAT;
		break;
	case STENCIL_OP_DECR_SAT:
		_DSState |= DS_STENCIL_BACK_DEPTH_FAIL_OP_DECR_SAT;
		break;
	case STENCIL_OP_INVERT:
		_DSState |= DS_STENCIL_BACK_DEPTH_FAIL_OP_INVERT;
		break;
	case STENCIL_OP_INCR:
		_DSState |= DS_STENCIL_BACK_DEPTH_FAIL_OP_INCR;
		break;
	case STENCIL_OP_DECR:
		_DSState |= DS_STENCIL_BACK_DEPTH_FAIL_OP_DECR;
		break;
	}
}

void Renderer::SetStencilPassOpBack(STENCIL_OP op)
{
	_DSState &= 0xFFFFFF1F;
	switch (op)
	{
	case STENCIL_OP_KEEP:
		_DSState |= DS_STENCIL_BACK_PASS_OP_KEEP;
		break;
	case STENCIL_OP_ZERO:
		_DSState |= DS_STENCIL_BACK_PASS_OP_ZERO;
		break;
	case STENCIL_OP_REPLACE:
		_DSState |= DS_STENCIL_BACK_PASS_OP_REPLACE;
		break;
	case STENCIL_OP_INCR_SAT:
		_DSState |= DS_STENCIL_BACK_PASS_OP_INCR_SAT;
		break;
	case STENCIL_OP_DECR_SAT:
		_DSState |= DS_STENCIL_BACK_PASS_OP_DECR_SAT;
		break;
	case STENCIL_OP_INVERT:
		_DSState |= DS_STENCIL_BACK_PASS_OP_INVERT;
		break;
	case STENCIL_OP_INCR:
		_DSState |= DS_STENCIL_BACK_PASS_OP_INCR;
		break;
	case STENCIL_OP_DECR:
		_DSState |= DS_STENCIL_BACK_PASS_OP_DECR;
		break;
	}
}

void Renderer::SetStencilFuncBack(COMPARISON_FUNC func)
{
	_DSState &= 0xFFFFFFE3;
	switch (func)
	{
	case COMPARISON_NEVER:
		_DSState |= DS_STENCIL_BACK_COMPARISON_NEVER;
		break;
	case COMPARISON_LESS:
		_DSState |= DS_STENCIL_BACK_COMPARISON_LESS;
		break;
	case COMPARISON_EQUAL:
		_DSState |= DS_STENCIL_BACK_COMPARISON_EQUAL;
		break;
	case COMPARISON_LESS_EQUAL:
		_DSState |= DS_STENCIL_BACK_COMPARISON_LESS_EQUAL;
		break;
	case COMPARISON_GREATER:
		_DSState |= DS_STENCIL_BACK_COMPARISON_GREATER;
		break;
	case COMPARISON_NOT_EQUAL:
		_DSState |= DS_STENCIL_BACK_COMPARISON_NOT_EQUAL;
		break;
	case COMPARISON_GREATER_EQUAL:
		_DSState |= DS_STENCIL_BACK_COMPARISON_GREATER_EQUAL;
		break;
	case COMPARISON_ALWAYS:
		_DSState |= DS_STENCIL_BACK_COMPARISON_ALWAYS;
		break;
	}
}