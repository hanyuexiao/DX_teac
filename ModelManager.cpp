#include "ModelManager.h"
#include "Node.h"
HRESULT CModelData::LoadMesh(TSTRING name,TSTRING pathName)
{
	this->m_strName=name;
	this->m_strPath=pathName;

	LPD3DXBUFFER materialsBuff=nullptr;
	//D3DXCreateBox() 
	//����ģ��FBX  ��
	//D3DXLoadMeshFromX(_T("./resource/3D/binglong.x"),D3DXMESH_MANAGED, CGraphic::GetSingleObjPtr()->m_pDevice,0, &materialsBuff,0, &numMaterials,&g_pMesh);
	if(FAILED(D3DXLoadMeshFromX(pathName.c_str(), D3DXMESH_MANAGED, CGraphic::GetSingleObjPtr()->m_pDevice, 0, &materialsBuff, 0, &m_dwNumMaterials, &m_pMesh)))
		return S_FALSE;

	D3DXMATERIAL* pMaterials = (D3DXMATERIAL*)materialsBuff->GetBufferPointer();//�õ�����������Ϣ�Ľṹ�������ָ��

	for (int i = 0; i < m_dwNumMaterials; i++)
	{
		if (pMaterials[i].pTextureFilename)
		{
			//������ͼ
			LPDIRECT3DTEXTURE9 pTexture;

			string path = "./resource/3D/";
			path += pMaterials[i].pTextureFilename;

			if (FAILED(D3DXCreateTextureFromFileA(CGraphic::GetSingleObjPtr()->m_pDevice, path.c_str(), &pTexture)))
				m_vecTextures.push_back(nullptr);//û����ͼ
			m_vecTextures.push_back(pTexture);//������ͼ
		}
		else
		{
			m_vecTextures.push_back(nullptr);//û����ͼ
		}
	}


	return S_OK;
}

void CModelData::RenderMesh(D3DXMATRIX outMatrix)
{
	

	CGraphic::GetSingleObjPtr()->m_pDevice->SetTransform(D3DTS_WORLD, &outMatrix);//Ӧ�þ���ʵ������任������-�����磩
	//6������

	for (int i = 0; i < m_dwNumMaterials; i++)
	{

		CGraphic::GetSingleObjPtr()->m_pDevice->SetTexture(0, m_vecTextures[i]);//������ͼ
		m_pMesh->DrawSubset(i);//��������	

	}
}

//������
HRESULT CModelManager::Init(TSTRING fileName)
{

	string name;
#ifdef _UNICODE
	name=wide_char_to_multi_byte(fileName,-1);
#else
	name=fileName;
#endif
	TiXmlDocument doc("./resource/3D/config/models.xml");
	if (!doc.LoadFile())	//��֤�ļ��Ƿ���ڣ���xml�﷨�Ƿ���ȷ
		return E_FAIL;
	TiXmlDocument* pDoc = &doc;

	TiXmlElement* pRoot = NULL;//�ڵ��Ա
	TiXmlElement* pElem = NULL;
	

	pRoot = pDoc->FirstChildElement();	//�õ����ڵ�
	
	TSTRING nodeName;
	for(pElem = pRoot->FirstChildElement(); pElem; pElem = pElem->NextSiblingElement())
	{
		string name = pElem->Attribute("name");//��ȡ�ڵ������ֵ
		string path = pElem->GetText();			//��ȡ�ڵ������ֵ

		TSTRING tName,tPath;
#ifdef _UNICODE
		tName = multi_byte_to_wide_char(name, 0);
		tPath = multi_byte_to_wide_char(path, 0);
#else
		tName = name;
		tPath = path;
#endif
		CModelData *pData=new CModelData;
		if (FAILED(pData->LoadMesh(tName,tPath)))
		{
			break;
		}
		nodeName = pData->m_strName;
		m_mapAllMeshs[nodeName] = pData;
	}
	
	return S_OK;
}

CModelData* CModelManager::GetModelData(TSTRING name)
{
	auto it = m_mapAllMeshs.find(name);
	if (it != m_mapAllMeshs.end())
	{
		return it->second;
	}
	return nullptr;
}
