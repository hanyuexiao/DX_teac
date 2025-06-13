#pragma once
#include "myHead.h"
#include "Object.h"
#include "CFrustum.h"
#define D3DFVF_TERRIANVERTEXFORAMT  D3DFVF_XYZ |D3DFVF_DIFFUSE | D3DFVF_TEX2
struct TerrainVERTEX
{
	D3DXVECTOR3 pos;
	DWORD color;//0xffff0000
	D3DXVECTOR2 texpos1;//纹理坐标1
	D3DXVECTOR2 texpos2;//纹理坐标2
};


class CTerrain :public CObject
{
public:
	CTerrain();
	~CTerrain();
	//设备接口、			
	HRESULT InitData();
	void SetLength(float len) { m_fLen = len; }
	void Update(CFrustum* pFrustum);
	void Render();

	LPD3DXMESH GetTerrainMesh() { return m_pTerrainMesh; }

	D3DXMATRIX GetTerrainMatrix() { return matWorld; }

	float GetHeight(float x, float y);

	bool IsInFrustum(CFrustum* pFrustum,WORD index1,WORD index2,WORD index3)
	{
		D3DXVECTOR3 vecPoints[3];
		vecPoints[0] = m_pTerrainVertex[index1].pos;
		vecPoints[1] = m_pTerrainVertex[index2].pos;
		vecPoints[2] = m_pTerrainVertex[index3].pos;

		if (pFrustum->PointInFrustum(vecPoints[0]) ||
			pFrustum->PointInFrustum(vecPoints[1]) ||
			pFrustum->PointInFrustum(vecPoints[2]))
		{
			return true; //三角形的三个点都在视锥体内
		}
		return false;
	}
private:
	TerrainVERTEX* m_pTerrainVertex;	//点
	void* m_pIndexPalne;//WORD   DWORD

	LPDIRECT3DDEVICE9	m_pDev;		//设备接口
	float m_fLen = 10;
	WORD m_iWidth;
	WORD m_iHeight;
	
	LPDIRECT3DTEXTURE9 m_pTexColor1;	//保持地形颜色值的图片
	LPDIRECT3DTEXTURE9 m_pTexColor2;	//保持地形颜色值的图片
	LPDIRECT3DTEXTURE9 m_pTexHeight;	//保持地形高度值的图片
	
	DWORD* m_pColorData;	//地形高度值的颜色数据
	//模型方法绘制地形
	LPD3DXMESH		m_pTerrainMesh;
	D3DXMATRIX matWorld;

	int						m_nTriangles;	/// 输出三角形的个数
};



