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

	TiXmlElement* pElem=m_pRootElem->FirstChildElement("ColorTex1");
	if(pElem)
	{
		string colorName=pElem->GetText();	
		//加载颜色图片
		D3DXCreateTextureFromFileA(m_pDev, colorName.c_str(), &m_pTexColor1);
	}

	pElem = m_pRootElem->FirstChildElement("ColorTex2");
	if (pElem)
	{
		string colorName = pElem->GetText();
		//加载颜色图片
		D3DXCreateTextureFromFileA(m_pDev, colorName.c_str(), &m_pTexColor2);
	}

	pElem = m_pRootElem->FirstChildElement("HeightTex");
	if (pElem)
	{
		string heightName = pElem->GetText();
		//加载高度图片
		D3DXCreateTextureFromFileA(m_pDev, heightName.c_str(), &m_pTexHeight);

		D3DLOCKED_RECT locked;
		m_pTexHeight->LockRect(0, &locked, 0, 0);
		m_pTexHeight->UnlockRect(0);

		m_pColorData = (DWORD*)locked.pBits; //获取高度图片的颜色数据

		D3DSURFACE_DESC desc;
		m_pTexHeight->GetLevelDesc(0, &desc);

		m_iWidth = desc.Width;	//获取高度图片的宽度
		m_iHeight = desc.Height; //获取高度图片的高度
	}

	pElem = m_pRootElem->FirstChildElement("Size");
	if (pElem)
	{
		/*string str = pElem->Attribute("width");
		m_iWidth=atof(str.c_str());

		str = pElem->Attribute("height");
		m_iHeight = atof(str.c_str());*/

		string str = pElem->Attribute("Len");
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

			m_pTerrainVertex[j + i * m_iWidth].pos.y =(*(m_pColorData + (m_iHeight - i) * m_iWidth + j)&0xff)/2.0f;//高度值

			//m_pTerrainVertex[j + i * m_iWidth].pos.y = 0.0f; //高度值
			if (m_pTerrainVertex[j + i * m_iWidth].pos.y >50)
				m_pTerrainVertex[j + i * m_iWidth].color = 0x00ffffff;
			else
				m_pTerrainVertex[j + i * m_iWidth].color = 0xffffffff;
			
			//保证图片的纹理是0~1的范围
			m_pTerrainVertex[j + i * m_iWidth].texpos1.x = 10.0f * i / (m_iHeight - 1);
			m_pTerrainVertex[j + i * m_iWidth].texpos1.y = 10.0f * j / (m_iWidth - 1);

			m_pTerrainVertex[j + i * m_iWidth].texpos2.x = 5.0f * i / (m_iHeight - 1);
			m_pTerrainVertex[j + i * m_iWidth].texpos2.y = 5.0f * j / (m_iWidth - 1);
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

	
	m_pDev->SetTexture(0, m_pTexColor1);
	
	//设置纹理阶段状态
	m_pDev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0); //纹理坐标索引
	m_pDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);


	m_pDev->SetTexture(1, m_pTexColor2);
	m_pDev->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1); //纹理坐标索引
	m_pDev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BLENDDIFFUSEALPHA);
	m_pDev->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDev->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);

	m_pTerrainMesh->DrawSubset(0);

	m_pDev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
}


float CTerrain::GetHeight(float x, float z)
{
	float fWidth = (m_iWidth - 1) * m_fLen;	//总大小
	if (x > fWidth || x < 0 ||
		z > fWidth || z < 0)
	{
		return 9999.f;
	}

	//将连续的点转为离散的个个顶点，定位当前点所在区域的左顶点--》方便对其他顶点进行计算，都是进行+1操作
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


	D3DXVECTOR3 point0 = m_pTerrainVertex[uiZTileIndex * m_iWidth + uiXTileIndex].pos;   ////左上角点
	D3DXVECTOR3 point1 = m_pTerrainVertex[(uiZTileIndex + 1) * m_iWidth + uiXTileIndex].pos;  ////左下角点
	D3DXVECTOR3 point2 = m_pTerrainVertex[uiZTileIndex * m_iWidth + uiXTileIndex + 1].pos;   /////右上角点
	D3DXVECTOR3 point3 = m_pTerrainVertex[(uiZTileIndex + 1) * m_iWidth + uiXTileIndex + 1].pos; ////右下角点

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

