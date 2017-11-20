#include "FileLoader.h"
#include "FJUtility.h"

wstring FileLoader::_fileName;

CObject* FileLoader::ObjectFileLoad(LPCWSTR i_fileName)
{
	WCHAR extention[64];
	CObject* pObj = nullptr;

	FileNameExtension(i_fileName, extention, ARRAYSIZE(extention));

	if (_wcsnicmp(extention, L".obj", ARRAYSIZE(extention)) == 0)
		pObj = LoadObjFile(i_fileName);

	else if (_wcsnicmp(extention, L".x", ARRAYSIZE(extention)) == 0)
		pObj = LoadXFile(i_fileName);

	return pObj;
}





CObject* FileLoader::LoadObjFile(LPCWSTR i_fileName)
{
	return nullptr;
}



CObject* FileLoader::LoadXFile(LPCWSTR i_fileName)
{
	CObject* pObj;
	CMaterial* pMtrl;

	wchar_t str[256];
	wchar_t c;

	_fileName = i_fileName;

	wifstream file(i_fileName);

	if (file.fail())
		return false;

	while (!file.eof())
	{
		file.get(c);

		switch (c)
		{
		case L'M':
			float power;
			pMtrl = new CMaterial(CShader::Find(L"Standard"));
			file >> str >> str;
			pMtrl->m_name = str;
			file >> str;

			// Diffuse 얻어오기
			file >> pMtrl->m_diffuse.x >> c >> pMtrl->m_diffuse.y >> c >> pMtrl->m_diffuse.z >> c >> pMtrl->m_diffuse.w >> str;

			// Power 얻어오기
			file >> power >> str;
			//file >> str;

			// Specular 얻어오기
			file >> pMtrl->m_specular.x >> c >> pMtrl->m_specular.y >> c >> pMtrl->m_specular.z >> str;

			// Emissive 얻어오기
			//file >> pMtrl->m_emissive.x >> c >> pMtrl->m_emissive.y >> c >> pMtrl->m_emissive.z >> str;
			file >> str;

			file >> str;
			if (_wcsnicmp(str, L"TextureFilename", 16) == 0)
			{
				file >> str;

				UINT i = 0;

				while (true)
				{
					file >> str;

					if (str[0] == L'}')
						break;

					// Load Texture;
					wstring ws = str;
					CTexture2D* pTex = new CTexture2D(ws.substr(1, ws.length() - 3).c_str());
					pTex->CreateTexture2D((_fileName.substr(0, _fileName.rfind(L"/", _fileName.length() - 1) + 1) + ws.substr(1, ws.length() - 3)).c_str());
					pMtrl->m_pTexture[i++] = pTex;
				}

				file >> str;
			}

			pMtrl->SetVector(0, pMtrl->m_diffuse);
			pMtrl->SetVector(1, XMFLOAT4(pMtrl->m_ambient.x, pMtrl->m_ambient.y, pMtrl->m_ambient.z, 1.0f));
			pMtrl->SetVector(2, XMFLOAT4(pMtrl->m_specular.x, pMtrl->m_specular.y, pMtrl->m_specular.z, 1.0f));
			pMtrl->SetScalar(0, power);

			break;

		case L'F':
			file >> str;
			if (_wcsnicmp(str, L"rame", 5) == 0)
			{
				pObj = XFileLoadFrame(nullptr, file);
			}

			break;

		default:
			file.getline(str, sizeof(str));
		}
	}

	return pObj;
}


CObject* FileLoader::XFileLoadFrame(CObject* i_pObj, wifstream& file)
{
	WCHAR str[256];
	CObject* pObj;
	CMesh* pMesh;
	CMaterial* pMtrl;

	vector<VF_PNT> vertics;
	vector<IndexFormat> indics;
	vector<VECTOR3> vecPos;
	vector<VECTOR3> vecNor;
	vector<VECTOR2> vecTex;

	UINT num;
	WCHAR c;

	file >> str;
	pObj = new CObject();
	pObj->m_name = str;

	if (i_pObj != nullptr)
	{
		pObj->SetParent(i_pObj);
	}

	file >> str;

	while (true)
	{
		file >> str;


		if (_wcsnicmp(str, L"FrameTransformMatrix", 21) == 0)
		{
			while (true)
			{
				file >> str;

				if (_wcsnicmp(str, L"}", 2) == 0)
					break;
			}
		}

		else if (_wcsnicmp(str, L"Frame", 6) == 0)
		{
			XFileLoadFrame(pObj, file);
		}

		else if (_wcsnicmp(str, L"Mesh", 5) == 0)
		{
			pObj->AddComponent(L"Renderer");
			pMesh = new CMesh;

			file >> str;
			pMesh->m_name = str;

			file >> str;
			file >> num >> str;

			vecPos.resize(num);

			for (UINT i = 0; i < num; i++)
			{
				file >> vecPos[i].x >> c >> vecPos[i].y >> c >> vecPos[i].z >> str;
			}

			file >> num >> str;

			indics.resize(num);

			for (UINT i = 0; i < num; i++)
			{
				file >> c >> c >> indics[i].x >> c >> indics[i].y >> c >> indics[i].z >> str;
			}

			while (true)
			{
				file >> str;

				if (_wcsnicmp(str, L"MeshNormals", 12) == 0)
				{
					file >> str;
					file >> num >> str;

					vecNor.resize(num);

					for (UINT i = 0; i < num; i++)
					{
						file >> vecNor[i].x >> c >> vecNor[i].y >> c >> vecNor[i].z >> str;
					}

					while (true)
					{
						file >> str;

						if (_wcsnicmp(str, L"}", 2) == 0)
							break;
					}
				}

				else if (_wcsnicmp(str, L"MeshMaterialList", 17) == 0)
				{
					int temp;

					file >> str;
					file >> num >> str;
					file >> temp >> str;
					file.getline(str, sizeof(str));

					for (int i = 0; i < temp; i++)
					{
						file.getline(str, sizeof(str));
					}

					file >> str;

					if (_wcsnicmp(str, L"{", 2) == 0)
					{
						while (true)
						{
							file >> str;

							if (_wcsnicmp(str, L"}", 2) == 0)
								break;

							// Material Setting
							((Renderer*)pObj->GetComponent(L"Renderer"))->m_pMaterial = CMaterial::Find(str);
						}
					}

					else if (_wcsnicmp(str, L"Material", 9) == 0)
					{
						file >> str;

						pMtrl = new CMaterial(CShader::Find(L"Standard"));
						pMtrl->m_name = pMesh->m_name + L" 1";

						// Material Setting
						((Renderer*)pObj->GetComponent(L"Renderer"))->m_pMaterial = pMtrl;

						// Diffuse 얻어오기
						file >> pMtrl->m_diffuse.x >> c >> pMtrl->m_diffuse.y >> c >> pMtrl->m_diffuse.z >> c >> pMtrl->m_diffuse.w >> str;

						// Power 얻어오기
						//file >> pMtrl->m_power >> str;
						file >> str;

						// Specular 얻어오기
						file >> pMtrl->m_specular.x >> c >> pMtrl->m_specular.y >> c >> pMtrl->m_specular.z >> str;

						// Emissive 얻어오기
						//file >> pMtrl->m_emissive.x >> c >> pMtrl->m_emissive.y >> c >> pMtrl->m_emissive.z >> str;
						file >> str;
						
						file >> str;
						if (_wcsnicmp(str, L"TextureFilename", 16) == 0)
						{
							file >> str;

							UINT i = 0;
							while (true)
							{
								file >> str;

								if (str[0] == L'}')
									break;

								// Load Texture;
								wstring ws = str;
								CTexture2D* pTex = new CTexture2D(ws.substr(1, ws.length() - 3).c_str());
								pTex->CreateTexture2D((_fileName.substr(0, _fileName.rfind(L"/", _fileName.length() - 1) + 1) + ws.substr(1, ws.length() - 3)).c_str());
								pMtrl->m_pTexture[i++] = pTex;
							}

							file >> str;
						}
					}

					file >> str;
				}

				else if (_wcsnicmp(str, L"MeshTextureCoords", 18) == 0)
				{
					file.getline(str, sizeof(str));

					file >> num >> str;

					vecTex.resize(num);

					for (UINT i = 0; i < num; i++)
					{
						file >> vecTex[i].x >> c >> vecTex[i].y >> str;
					}

					file >> str;
				}

				else if (_wcsnicmp(str, L"}", 2) == 0)
					break;
			}

			vertics.resize(vecPos.size());

			for (UINT i = 0; i < vertics.size(); i++)
			{
				vertics[i].pos = vecPos[i];
				vertics[i].nor = vecNor[i];
				vertics[i].uv = vecTex[i];
			}

			pMesh->CreateBuffer(vertics, indics);

			((Renderer*)pObj->GetComponent(L"Renderer"))->ChangeMesh(pMesh);
		}

		else if (_wcsnicmp(str, L"}", 2) == 0)
		{
			return pObj;
		}
	}
}
