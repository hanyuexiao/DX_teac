#include "Terrain.h"


CTerrain::CTerrain()
{
	m_fLen = 10;
	m_iHeight = m_iWidth = 0;
	D3DXMatrixIdentity(&matWorld);

	m_pNode=new CNode;
}

CTerrain::~CTerrain()
{
	
}

HRESULT CTerrain::InitData()//(TSTRING colorName)
{
	this->m_strObjectName = _T("myTerrain");
	m_pDev = CGraphic::GetSingleObjPtr()->m_pDevice;
	//解析脚本数据
	TiXmlElement* pElem=m_pRootElem->FirstChildElement("ColorTex");
	if(pElem)
	{
		string colorName=pElem->GetText();	
		//加载颜色图片
		D3DXCreateTextureFromFileA(m_pDev, colorName.c_str(), &m_pTexColor);
	}
	pElem = m_pRootElem->FirstChildElement("Size");
	if (pElem)
	{
		string str = pElem->Attribute("width");
		m_iWidth=atof(str.c_str());

		str = pElem->Attribute("height");
		m_iHeight = atof(str.c_str());

		str = pElem->Attribute("Len");
		m_fLen = atof(str.c_str());
	}
	
	//创建地形网格模型
	m_nTriangles = (m_iWidth - 1) * (m_iHeight - 1) * 2;

	if (m_iWidth * m_iHeight <= 65535)
	{
		D3DXCreateMeshFVF(m_nTriangles, m_iWidth * m_iHeight,
			D3DXMESH_MANAGED, D3DFVF_TERRIANVERTEXFORAMT,
			m_pDev, &m_pTerrainMesh);

	}
	else
	{
		D3DXCreateMeshFVF(m_nTriangles, m_iWidth * m_iHeight,
		D3DXMESH_32BIT/*索引格式是32位 */|D3DXMESH_MANAGED, D3DFVF_TERRIANVERTEXFORAMT,
			m_pDev, &m_pTerrainMesh);
	}

	//定义地形的顶点坐标
	m_pTerrainMesh->LockVertexBuffer(0, (void**)&m_pTerrainVertex);
	for (int i = 0; i < m_iHeight; i++)
	{
		for (int j = 0; j < m_iWidth; j++)
		{
			m_pTerrainVertex[j + i * m_iWidth].pos = D3DXVECTOR3(j * m_fLen, 0.0f, i * m_fLen);

			
			m_pTerrainVertex[j + i * m_iWidth].color = 0xffffffff;
			
			//保证图片的纹理是0~1的范围
			m_pTerrainVertex[j + i * m_iWidth].texpos1.x = 10.0f * i / (m_iHeight - 1);
			m_pTerrainVertex[j + i * m_iWidth].texpos1.y = 10.0f * j / (m_iWidth - 1);


		}
	}
	m_pTerrainMesh->UnlockVertexBuffer();

	/////////////////////////////////////////////////////////////////////////

	m_pTerrainMesh->LockIndexBuffer(0, (void**)&m_pIndexPalne);
	if (m_iWidth * m_iHeight <= 65535)
	{
		DWORD num = 0;
		for (int i = 0; i < (m_iHeight - 1); i++)
		{
			for (int j = 0; j < (m_iWidth - 1); j++)
			{
				*((WORD*)m_pIndexPalne + num++) = j + m_iWidth * i;
				*((WORD*)m_pIndexPalne + num++) = j + m_iWidth * (i + 1);
				*((WORD*)m_pIndexPalne + num++) = j + 1 + m_iWidth * i;
				*((WORD*)m_pIndexPalne + num++) = j + m_iWidth * (i + 1);
				*((WORD*)m_pIndexPalne + num++) = j + m_iWidth * (i + 1) + 1;
				*((WORD*)m_pIndexPalne + num++) = j + 1 + m_iWidth * i;  //short
			}
		}
	}
	else
	{
		DWORD num = 0;
		for (int i = 0; i < (m_iHeight - 1); i++)
		{
			for (int j = 0; j < (m_iWidth - 1); j++)
			{
				*((DWORD*)m_pIndexPalne + num++) = j + m_iWidth * i;
				*((DWORD*)m_pIndexPalne + num++) = j + m_iWidth * (i + 1);
				*((DWORD*)m_pIndexPalne + num++) = j + 1 + m_iWidth * i;
				*((DWORD*)m_pIndexPalne + num++) = j + m_iWidth * (i + 1);
				*((DWORD*)m_pIndexPalne + num++) = j + m_iWidth * (i + 1) + 1;
				*((DWORD*)m_pIndexPalne + num++) = j + 1 + m_iWidth * i;
			}
		}
	}
	m_pTerrainMesh->UnlockIndexBuffer();

	//定义三角形的不同属性值（分子集）
	DWORD* attriBuffer;
	m_pTerrainMesh->LockAttributeBuffer(0, &attriBuffer);
	for (int i = 0; i < (m_iWidth - 1) * (m_iHeight - 1) * 2; i++)
		attriBuffer[i] = 0;
	m_pTerrainMesh->UnlockAttributeBuffer();


	return S_OK;
}

void CTerrain::Render()
{

	m_pDev->SetTransform(D3DTS_WORLD, &matWorld);


	//先定义材质，再设置纹理混合

	
	m_pDev->SetTexture(0, m_pTexColor);
	
	m_pDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);


	m_pTerrainMesh->DrawSubset(0);

}


float CTerrain::GetHeight(float x, float z)
{
	float fWidth = (m_iWidth - 1) * m_fLen;	//总大小
	if (x > fWidth || x < 0 ||
		z > fWidth || z < 0)
	{
		return 9999.f;
	}
	float fXTileIndex = x / m_fLen;//列号float
	float fZTileIndex = z / m_fLen;//行号float
	UINT uiXTileIndex = static_cast<UINT>(fXTileIndex);//列号转int
	UINT uiZTileIndex = static_cast<UINT>(fZTileIndex);//行号转int
	/*
	1-----3
	| \   |
	|  \  |
	|   \ |
	0-----2
	*/
	D3DXVECTOR3 point0 = m_pTerrainVertex[uiZTileIndex * m_iWidth + uiXTileIndex].pos;
	D3DXVECTOR3 point1 = m_pTerrainVertex[(uiZTileIndex + 1) * m_iWidth + uiXTileIndex].pos;
	D3DXVECTOR3 point2 = m_pTerrainVertex[uiZTileIndex * m_iWidth + uiXTileIndex + 1].pos;
	D3DXVECTOR3 point3 = m_pTerrainVertex[(uiZTileIndex + 1) * m_iWidth + uiXTileIndex + 1].pos;
	float x_pct = fXTileIndex - uiXTileIndex;//x偏移系数（0~1）
	float z_pct = fZTileIndex - uiZTileIndex;//z偏移系数（0~1）
	D3DXVECTOR3 restutPoint;
	//下三角（0号点为基准）
	if (x_pct + z_pct <= 1) {

		//V1 + f(V2-V1) + g(V3-V1). 
		D3DXVec3BaryCentric(&restutPoint, &point0, &point2, &point1, x_pct, z_pct);//三角形内插值
	}
	else //上三角3号点为基准
	{
		x_pct = 1 - x_pct;
		z_pct = 1 - z_pct;
		D3DXVec3BaryCentric(&restutPoint, &point3, &point1, &point2, x_pct, z_pct);
	}
	return restutPoint.y;
}

