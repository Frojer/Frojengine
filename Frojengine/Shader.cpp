#include <d3dcompiler.h>				//DX ���̴� �����Ϸ� ���.
#pragma comment(lib, "d3dcompiler")		//DX ���̴� �����Ϸ� ���̺귯��.  D3DCompiler.dll �ʿ�.

#include "Shader.h"


unordered_map<UINT, CShader*> CShader::_shaderMap;
LPBUFFER CShader::_pWVP_CB;
LPBUFFER CShader::_pLight_CB;


CShader::CShader()
	: _pConstBuffer(nullptr), _pVS(nullptr), _pPS(nullptr), _pVSCode(nullptr), _pInputLayout(nullptr), _countTexture(0), _countMatrix(0), _countVector(0), _countScalar(0), _useLight(false)
{
	_shaderMap.insert(pair<UINT, CShader*>(GetID(), this));
}


CShader::~CShader()
{
	_shaderMap.erase(GetID());
	
	SAFE_RELEASE(_pInputLayout);
	SAFE_RELEASE(_pPS);
	SAFE_RELEASE(_pVS);
	SAFE_RELEASE(_pVSCode);
}


CShader* CShader::CreateShader(LPCWSTR i_fileName)
{
	CShader* pShader = new CShader();

	if (pShader == nullptr)
	{
		ErrMsgBox(L"Failed to Memory Allocate");
		return nullptr;
	}

	//--------------------------
	// ���� ���̴� ����.
	//--------------------------
	// ���� ���̴� ������ Compile the vertex shader
	HRESULT hr = ShaderCompile(i_fileName, "VS_Main", "vs_5_0", &pShader->_pVSCode);

	if (FAILED(hr))
	{
		FJError(hr, L"Failed to Vertex Shader Compile");
		//MessageBox(NULL, L"[����] ShaderLoad :: Vertex Shader ������ ����", L"Error", MB_OK);
		delete pShader;
		pShader = nullptr;

		return nullptr;
	}

	// ���� ���̴� ��ü ���� Create the vertex shader
	hr = _pDevice->CreateVertexShader(pShader->_pVSCode->GetBufferPointer(),
		pShader->_pVSCode->GetBufferSize(),
		nullptr,
		&pShader->_pVS
	);
	if (FAILED(hr))
	{
		FJError(hr, L"Failed to CreateVertexShader()");
		delete pShader;
		pShader = nullptr;

		return nullptr;
	}

	//--------------------------
	// �ȼ� ���̴� ����.
	//--------------------------
	// �ȼ� ���̴� ������ Compile the pixel shader
	ID3DBlob* pPSCode = nullptr;
	hr = ShaderCompile(i_fileName, "PS_Main", "ps_5_0", &pPSCode);

	if (FAILED(hr))
	{
		FJError(hr, L"Failed to Pixel Shader Compile");
		//MessageBox(NULL, L"[����] ShaderLoad :: Pixel Shader ������ ����", L"Error", MB_OK);

		delete pShader;
		pShader = nullptr;

		return nullptr;
	}
	// �ȼ� ���̴� ��ü ���� Create the pixel shader
	hr = _pDevice->CreatePixelShader(pPSCode->GetBufferPointer(),
		pPSCode->GetBufferSize(),
		nullptr,
		&pShader->_pPS
	);

	SAFE_RELEASE(pPSCode);				//�ӽ� ��ü ����.	

	if (FAILED(hr))
	{
		FJError(hr, L"Failed to CreatePixelShader()");

		delete pShader;
		pShader = nullptr;

		return nullptr;
	}

	// ���� �Է±��� Input layout ��
	// GPU �� ���޵� ���ϵ����� - ���� ������ ������ ������ �뵵���� ������ �����մϴ�.
	// ���� Vertex Format(DX7/8/9) �Ǵ� Vertex Declaration(DX9) �� ���� �������� �������� 
	// ���� ������ ����� �䱸�� ���߾� ������ �� ��������� Ȯ��Ǿ����ϴ�.
	//
	// �ٸ� ������ ����� ���ؼ��� �Ʒ��� ������ ���� �Ǵ� ȣȯ�Ǿ�� �մϴ�.
	// 1.���� ������ ������.  Vertex Buffer Data
	// 2.���� ���� Vertex Format (Input Layout)
	// 3.���̴� �Լ��� �Է±���.  Vertex Shader (Input Layout)
	//
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//  Sementic          format                       offset         classification             
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// ���� �Է±��� ��ü ���� Create the input layout
	hr = _pDevice->CreateInputLayout(layout,
		numElements,
		pShader->_pVSCode->GetBufferPointer(),
		pShader->_pVSCode->GetBufferSize(),
		&pShader->_pInputLayout
	);
	if (FAILED(hr))
		return nullptr;

	return pShader;
}






////////////////////////////////////////////////////////////////////////////// 
//
// ���̴� �ҽ� ������
//
HRESULT CShader::ShaderCompile(
						LPCWSTR FileName,		//�ҽ������̸�.
						char* EntryPoint,		//�����Լ� ������.
						char* ShaderModel,		//���̴� ��.
						ID3DBlob** ppCode		//[���] �����ϵ� ���̴� �ڵ�.
						)
{
	HRESULT hr = S_OK; 
	ID3DBlob* pError = nullptr;

	//������ �ɼ�1.
	UINT Flags = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;		//���켱 ��� ó��. ���� DX9 ���������� �������� ���. �ӵ��� �䱸�ȴٸ�, "��켱" ���� ó���� ��.
	//UINT Flags = D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;	//��켱 ��� ó��. �� �켱 ó������ �ӵ��� ����� ������, ����� ��ġ�Ѽ� GPU �� �����ؾ� �Ѵ�.
	//UINT Flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
	//Flags |=  D3DCOMPILE_DEBUG;		//����� ��� �߰�.


	//���̴� �ҽ� ������.
	hr = D3DCompileFromFile(FileName, 
							nullptr, nullptr, 
							EntryPoint, 
							ShaderModel,
							Flags,		//������ �ɼ�.1							 
							//Flags | D3DCOMPILE_DEBUG,	//������ �ɼ�1 + ����� ���� �߰�.
							0,					//������ �ɼ�2,  Effect ���� �����Ͻ� �����. �̿ܿ��� ���õ�.
							ppCode,				//[���] �����ϵ� ���̴� �ڵ�.
							&pError				//[���] ������ ���� �ڵ�.
							);
	if (FAILED(hr)) ShaderError(TRUE, L"���̴� ������ ����", hr, pError, FileName, EntryPoint, ShaderModel);
	
	SAFE_RELEASE(pError);
	return hr;
}



void CShader::ClearMap()
{
	auto i = _shaderMap.begin();

	while (i != _shaderMap.end())
		delete (i++)->second;
}


void CShader::Render()
{
	UINT i = 0;
	//�Է� ���̾ƿ� ����. Set the input layout
	_pDXDC->IASetInputLayout(_pInputLayout);

	//���̴� ����.
	_pDXDC->VSSetShader(_pVS, nullptr, 0);
	_pDXDC->PSSetShader(_pPS, nullptr, 0);

	_pDXDC->VSSetConstantBuffers(i++, 1, &_pWVP_CB);
	if (_useLight)
		_pDXDC->VSSetConstantBuffers(i++, 1, &_pLight_CB);
	if (_pConstBuffer != nullptr)
	{
		_pDXDC->VSSetConstantBuffers(i++, 1, &_pConstBuffer);
		_pDXDC->PSSetConstantBuffers(0, 1, &_pConstBuffer);
	}
}



HRESULT CShader::CreateDefaultBuffer()
{
	HRESULT hr = S_OK;

	//��� ���� ���� ����.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;				//���� �������� ����.
	bd.ByteWidth = sizeof(CMaterial::_WVPData);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//CPU ���� ����.

												//���긮�ҽ� ����.
	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = &CMaterial::_WVPData;							//(�ܺ�) ��� ������ ����.
	sd.SysMemPitch = 0;
	sd.SysMemSlicePitch = 0;

	//��� ���� ����.
	hr = _pDevice->CreateBuffer(&bd, &sd, &_pWVP_CB);
	if (FAILED(hr))
	{
		FJError(hr, L"CreateDefaultBuffer : �⺻���� ���� ����");
		return hr;
	}

	return hr;
}



////////////////////////////////////////////////////////////////////////////// 
//
// (����) ��� ���� ����
//
HRESULT CShader::CreateConstantBuffer(UINT size, ID3D11Buffer** ppCB)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* pCB = nullptr;

	//��� ���� ���� ����.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = size;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;


	//��� ���� ����.
	hr = _pDevice->CreateBuffer(&bd, nullptr, &pCB);
	if (FAILED(hr))
	{
		FJError(hr, L"CreateConstantBuffer : ������� ���� ����");
		return hr;
	}

	//�ܺη� ����.
	*ppCB = pCB;

	return hr;
}





////////////////////////////////////////////////////////////////////////////// 
//
// (����) ��� ���� ����
//
HRESULT CShader::CreateDynamicConstantBuffer(UINT size, LPVOID pData, ID3D11Buffer** ppCB)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* pCB = nullptr;

	//��� ���� ���� ����.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;				//���� �������� ����.
	bd.ByteWidth = size;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//CPU ���� ����.

												//���긮�ҽ� ����.
	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = pData;							//(�ܺ�) ��� ������ ����.
	sd.SysMemPitch = 0;
	sd.SysMemSlicePitch = 0;

	//��� ���� ����.
	hr = _pDevice->CreateBuffer(&bd, &sd, &pCB);
	if (FAILED(hr))
	{
		FJError(hr, L"CreateDynamicConstantBuffer : ���� ������� ���� ����");
		return hr;
	}

	//�ܺη� ����.
	*ppCB = pCB;

	return hr;
}




////////////////////////////////////////////////////////////////////////////// 
//
// ���� ���� ����.
//
HRESULT CShader::UpdateDynamicConstantBuffer(ID3D11Resource* pBuff, LPVOID pData, UINT size)
{
	HRESULT hr = S_OK;
	D3D11_MAPPED_SUBRESOURCE mr;
	ZeroMemory(&mr, sizeof(mr));

	//������� ����
	hr = _pDXDC->Map(pBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
	if (FAILED(hr))
	{
		FJError(hr, L"UpdateDynamicConstantBuffer : Map ����");
		return hr;
	}

	//��� ���� ����.
	memcpy(mr.pData, pData, size);

	//������� �ݱ�.
	_pDXDC->Unmap(pBuff, 0);


	return hr;
}


void CShader::UpdateConstantBuffer(LPVOID pCB, UINT size)
{
	UpdateDynamicConstantBuffer(_pWVP_CB, &CMaterial::_WVPData, sizeof(CMaterial::_WVPData));
	if (_useLight)
		UpdateDynamicConstantBuffer(_pLight_CB, &CMaterial::_LightData, sizeof(CMaterial::_LightData));
	if (size != 0)	UpdateDynamicConstantBuffer(_pConstBuffer, pCB, size);
}



CShader* CShader::Find(UINT id)
{
	if (_shaderMap.find(id) == _shaderMap.end())
		return nullptr;

	return _shaderMap[id];
}


CShader* CShader::Find(LPCWSTR name)
{
	FOR_STL(_shaderMap)
	{
		if (iter->second->m_name == name)
			return iter->second;
	}

	return nullptr;
}




////////////////////////////////////////////////////////////////////////////// 
//
// ���� �޼��� ó�� : ���̴� ���� ó����.
// 
void CShader::ShaderError(BOOL bMBox, TCHAR* msg, HRESULT hr, ID3DBlob* pBlob, LPCWSTR filename, char* EntryPoint, char* ShaderModel)
{

	/*//�����Ű����� ó��.
	TCHAR msgva[2048] = L"";
	va_list vl;
	va_start(vl, msg);
	_vstprintf(msgva, msg, vl);
	va_end(vl);
	*/
	//�Ķ����, �����ڵ�� ��ȯ.
	TCHAR func[80] = L"";
	size_t len = strlen(EntryPoint);
	::mbstowcs_s(&len, func, ARRAYSIZE(func), EntryPoint, strlen(EntryPoint));
	TCHAR sm[20] = L"";
	::mbstowcs_s(&len, sm, ARRAYSIZE(sm), ShaderModel, strlen(ShaderModel));


	//���̴� ���� �޼��� �б�.
	TCHAR errw[4096] = L"";
	::mbstowcs_s(&len, errw, ARRAYSIZE(errw), (char*)pBlob->GetBufferPointer(), pBlob->GetBufferSize());


	//HRESULT ���� ���� �޼��� ��� 
	//�ý������� ���� ��� ���̴� �����޼����� ����Ȯ�ϹǷ� ����.
	TCHAR herr[1024] = L"�Ʒ��� ������ Ȯ���Ͻʽÿ�.";
	/*FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, hr,
	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	herr, 1024, NULL);
	*/


	//���� �޼��� ���̱�.
	TCHAR errmsg[1024];
	_stprintf_s(errmsg, L"%s \nFile=%s  Entry=%s  Target=%s  \n�����ڵ�(0x%08X) : %s \n\n%s",
		msg, filename, func, sm,
		hr, herr, errw);


	//(����� ��) VS ���â���� ���..
	OutputDebugString(L"\n");
	OutputDebugString(errmsg);
	//OutputDebugString(errw);


	//�α����Ϸ� ���.
	//...


	//�޼��� â ���..
	if (bMBox)
	{
		MessageBox(NULL, errmsg, L"Yena::Error", MB_OK | MB_ICONERROR);
		//MessageBox(NULL, errw, L"Yena::Error", MB_OK | MB_ICONERROR);
	}
}