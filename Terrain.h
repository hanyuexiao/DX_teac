#pragma once
#include "myHead.h"
#include "Object.h"
#include "CFrustum.h"
#define D3DFVF_TERRIANVERTEXFORAMT  D3DFVF_XYZ |D3DFVF_DIFFUSE | D3DFVF_TEX2
struct TerrainVERTEX
{
	D3DXVECTOR3 pos;
	DWORD color;//0xffff0000
	D3DXVECTOR2 texpos1;//��������1
	D3DXVECTOR2 texpos2;//��������2
};


class CTerrain :public CObject
{
public:
	CTerrain();
	~CTerrain();
	//�豸�ӿڡ�			
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
			return true; //�����ε������㶼����׶����
		}
		return false;
	}
private:
	TerrainVERTEX* m_pTerrainVertex;	//��
	void* m_pIndexPalne;//WORD   DWORD

	LPDIRECT3DDEVICE9	m_pDev;		//�豸�ӿ�
	float m_fLen = 10;
	WORD m_iWidth;
	WORD m_iHeight;
	
	LPDIRECT3DTEXTURE9 m_pTexColor1;	//���ֵ�����ɫֵ��ͼƬ
	LPDIRECT3DTEXTURE9 m_pTexColor2;	//���ֵ�����ɫֵ��ͼƬ
	LPDIRECT3DTEXTURE9 m_pTexHeight;	//���ֵ��θ߶�ֵ��ͼƬ
	
	DWORD* m_pColorData;	//���θ߶�ֵ����ɫ����
	//ģ�ͷ������Ƶ���
	LPD3DXMESH		m_pTerrainMesh;
	D3DXMATRIX matWorld;

	int						m_nTriangles;	/// ��������εĸ���
};



