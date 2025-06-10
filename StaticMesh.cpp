
#include "StaticMesh.h"
#include "ModelManager.h"
#include "Node.h"
#include "AxisAlignedBox.h"

CStaticMesh::CStaticMesh()
{
	m_pModelData=nullptr;

	m_pNode=new CNode;
}

CStaticMesh::~CStaticMesh()
{
	
}

HRESULT CStaticMesh::InitData()
{	
	TiXmlElement* pElem=m_pRootElem;
	string strName = pElem->Attribute("name");

#ifdef _UNICODE
	m_strObjectName = multi_byte_to_wide_char(strName, 0);
#else
	m_strObjectName = strName;
#endif

	//读取位置信息
	if (m_pNode)
	{
		string strName = pElem->Attribute("ModelName");
#ifdef _UNICODE
		m_strModelName = multi_byte_to_wide_char(strName, 0);

#else
		m_strModelName = strName;
#endif
		m_pModelData = CModelManager::GetSingleObjPtr()->GetModelData(m_strModelName);
		if (!m_pModelData)
			return E_FAIL;

		string str1, str2, str3;
		TiXmlElement* pModelElem = pElem->FirstChildElement("Position");
		if (pModelElem)
		{
			str1 = pModelElem->Attribute("x");
			str2 = pModelElem->Attribute("y");
			str3 = pModelElem->Attribute("z");

			float x = (float)atof(str1.c_str());
			float y = (float)atof(str2.c_str());
			float z = (float)atof(str3.c_str());

			m_pNode->SetRelativePosition(D3DXVECTOR3(x, y, z));
		}
		//读取旋转信息
		pModelElem = pElem->FirstChildElement("Rot");
		if (pModelElem)
		{
			str1 = pModelElem->Attribute("x");
			str2 = pModelElem->Attribute("y");
			str3 = pModelElem->Attribute("z");

			float x = (float)atof(str1.c_str());
			float y = (float)atof(str2.c_str());
			float z = (float)atof(str3.c_str());

			D3DXMATRIX matRot;
			D3DXMatrixRotationYawPitchRoll(&matRot, D3DXToRadian(y), D3DXToRadian(x), D3DXToRadian(z));
			m_pNode->SetRelativeRotation(matRot);
		}
		//读取缩放信息
		pModelElem = pElem->FirstChildElement("Scale");
		if (pModelElem)
		{
			str1 = pModelElem->Attribute("x");
			str2 = pModelElem->Attribute("y");
			str3 = pModelElem->Attribute("z");

			float x = (float)atof(str1.c_str());
			float y = (float)atof(str2.c_str());
			float z = (float)atof(str3.c_str());

			m_pNode->SetRelativeScale(D3DXVECTOR3(x, y, z));
		}

	}
	//计算包围盒
	void* pVertex;
	m_pModelData->m_pMesh->LockVertexBuffer(0, &pVertex);
	m_pModelData->m_pMesh->UnlockVertexBuffer();

	//取出模型的顶点数
	int numVertex = m_pModelData->m_pMesh->GetNumVertices();

	int vertexSize = m_pModelData->m_pMesh->GetNumBytesPerVertex();

	D3DXVECTOR3 MinVertex, MaxVertex;
	D3DXComputeBoundingBox((D3DXVECTOR3*)pVertex, numVertex, vertexSize, &MinVertex, &MaxVertex);

	m_pAABB = new CAxisAlignedBox;
	m_pAABB->SetExtents(MinVertex, MaxVertex);

	m_pSelfBox = new CAxisAlignedBox;
	m_pSelfBox->SetExtents(MinVertex, MaxVertex);

	m_pAABB->m_bRenderAABB = true;
	m_pSelfBox->m_bRenderBox = true;
	return S_OK;
}



void CStaticMesh::Render()
{
	D3DXMATRIX worldMatrix;
	m_pNode->GetWorldTransform(worldMatrix);
	m_pModelData->RenderMesh(worldMatrix);


	if (m_pAABB) {
		m_pAABB->Transform(worldMatrix);
		m_pAABB->RenderAABB();
	}

	if (m_pSelfBox && m_pSelfBox->m_bRenderAABB) {
		m_pSelfBox->Transform(worldMatrix);
		m_pSelfBox->RenderBox();
	}
	
}


