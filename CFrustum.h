#pragma once
#include "myHead.h"

struct FRUSTUMVERTEX
{
	D3DXVECTOR3 pos;
	DWORD color;//半透明颜色
};

#define FRUSTUMVERTEXFVF (D3DFVF_XYZ|D3DFVF_DIFFUSE)
class CFrustum
{
	private:
	//固定的裁剪空间8个点坐标
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
	D3DXMATRIX InvViewProj;//相机矩阵*投影矩阵的逆矩阵
	bool m_bLockedView=false;

	LPD3DXMESH m_pFrustumMesh;//视锥模型

	FRUSTUMVERTEX* m_pArrayFrustumVertex;
	WORD * m_arrayFrustumIndex;

	//定义视锥体裁剪时的3个平面
	D3DXPLANE m_Planes[3];

public:
	//计算视锥体的3个平面
	void MakeFrustumPlanes();

	//计算点到面的距离，判断点是否在视锥体内
	bool PointInFrustum(const D3DXVECTOR3& point);

	//计算包围盒是否在视锥体内
	bool BoxInFrustum(const D3DXVECTOR3* arrayCorner);

	//计算球是否在视锥体内
	bool SphereInFrustum(const D3DXVECTOR3& center, float radius);

	
		void InitData();//创建视锥模型，填充顶点和索引数据
		void UpdateFrustum(D3DXMATRIX* View=nullptr);//传一个按键时刻的相机矩阵，需要更新模型的顶点数据
		void DrawFrustum();//绘制视锥体，要开启半透明
		bool GetLocked(){return m_bLockedView;}
		void SetLockeed(){m_bLockedView=!m_bLockedView;}
};

