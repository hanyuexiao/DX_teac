#pragma once
#include "myHead.h"

struct FRUSTUMVERTEX
{
	D3DXVECTOR3 pos;
	DWORD color;//��͸����ɫ
};

#define FRUSTUMVERTEXFVF (D3DFVF_XYZ|D3DFVF_DIFFUSE)
class CFrustum
{
	private:
	//�̶��Ĳü��ռ�8��������
	D3DXVECTOR3 m_vecClip[8]={
		D3DXVECTOR3(-1,1,0),
		D3DXVECTOR3(1, 1, 0),
		D3DXVECTOR3(-1, -1, 0),
		D3DXVECTOR3(1, -1, 0),

		D3DXVECTOR3(-1,1,1),
		D3DXVECTOR3(1, 1, 1),
		D3DXVECTOR3(-1, -1, 1),
		D3DXVECTOR3(1, -1, 1),
	};
	D3DXVECTOR3 m_FrustumVertexsInWorld[8]={};
	D3DXMATRIX InvViewProj;//�������*ͶӰ����������
	bool m_bLockedView=false;

	LPD3DXMESH m_pFrustumMesh;//��׶ģ��

	FRUSTUMVERTEX* m_pArrayFrustumVertex;
	WORD * m_arrayFrustumIndex;

	//������׶��ü�ʱ��3��ƽ��
	D3DXPLANE m_Planes[3];

public:
	//������׶���3��ƽ��
	void MakeFrustumPlanes();

	//����㵽��ľ��룬�жϵ��Ƿ�����׶����
	bool PointInFrustum(const D3DXVECTOR3& point);

	//�����Χ���Ƿ�����׶����
	bool BoxInFrustum(const D3DXVECTOR3* arrayCorner);

	//�������Ƿ�����׶����
	bool SphereInFrustum(const D3DXVECTOR3& center, float radius);

	
		void InitData();//������׶ģ�ͣ���䶥�����������
		void UpdateFrustum(D3DXMATRIX* View=nullptr);//��һ������ʱ�̵����������Ҫ����ģ�͵Ķ�������
		void DrawFrustum();//������׶�壬Ҫ������͸��
		bool GetLocked(){return m_bLockedView;}
		void SetLockeed(){m_bLockedView=!m_bLockedView;}
};

