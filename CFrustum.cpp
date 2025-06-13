#include "CFrustum.h"

void CFrustum::MakeFrustumPlanes()
{
	//计算视锥体的3个平面
	//前平面
	D3DXPlaneFromPoints(&m_Planes[0], &m_FrustumVertexsInWorld[5], &m_FrustumVertexsInWorld[4], &m_FrustumVertexsInWorld[6]);
	//左平面
	D3DXPlaneFromPoints(&m_Planes[1], &m_FrustumVertexsInWorld[1], &m_FrustumVertexsInWorld[5], &m_FrustumVertexsInWorld[7]);
	//右平面
	D3DXPlaneFromPoints(&m_Planes[2], &m_FrustumVertexsInWorld[4], &m_FrustumVertexsInWorld[0], &m_FrustumVertexsInWorld[2]);
}

bool CFrustum::PointInFrustum(const D3DXVECTOR3& point)
{
	for (int i = 0; i < 3; i++)
	{
		//计算点到平面的距离
		float distance = D3DXPlaneDotCoord(&m_Planes[i], &point);
		if (distance > 10)
		{
			return false;
		}
		return true; // 点在视锥体内
	}
}

bool CFrustum::BoxInFrustum(const D3DXVECTOR3* arrayCorner) {
	for (int i = 0; i < 3; i++)
	{
		bool bInFrustum = true;
		for (int j = 0; j < 8; j++)
		{
			if (D3DXPlaneDotCoord(&m_Planes[i], &arrayCorner[j]) < 0.0f) 
			{
				bInFrustum = false;
				break;
			}
		}
		if (bInFrustum)
			return true; // 有一个点不在视锥体内
	}
	return false; // 所有点都在视锥体内
}

bool CFrustum::SphereInFrustum(const D3DXVECTOR3& center, float radius)
{
	for (int i = 0; i < 3; i++)
	{
		float distance = D3DXPlaneDotCoord(&m_Planes[i], &center);
		if (distance > radius + 10)
		{
			return true; // 球不在视锥体内
		}
	}
	return false; // 球在视锥体内
}

void CFrustum::InitData()
{
	//先计算一个初始相机位置的视锥坐标
	//UpdateFrustum();
	//创建一个空的模型
	D3DXCreateMeshFVF(12,24,D3DXMESH_MANAGED,FRUSTUMVERTEXFVF, CGraphic::GetSingleObjPtr()->m_pDevice,&m_pFrustumMesh);

	

	//36个索引值
	WORD arrayIndex[36] = {
					0,1,3,0,3,2,
					4,5,7,4,7,6,
					8,9,11,8,11,10,
					12,13,15,12,15,14,
					16,17,19,16,19,18,
					20,21,23,20,23,21,
	};
	m_pFrustumMesh->LockIndexBuffer(0,(void**)&m_arrayFrustumIndex);
	memcpy(m_arrayFrustumIndex, arrayIndex, 36 * sizeof(WORD));
	m_pFrustumMesh->UnlockIndexBuffer();
}

void CFrustum::UpdateFrustum(D3DXMATRIX* View/*=nullptr*/)
{
	/*for (int i = 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(&m_vecWorldPos[i], &m_vecClip[i], &InvViewProj);
	}*/
	D3DXMATRIX matView,matProj;

	if(!m_bLockedView)
		CGraphic::GetSingleObjPtr()->m_pDevice->GetTransform(D3DTS_VIEW,&matView);
	else
		matView=*View;



	CGraphic::GetSingleObjPtr()->m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMATRIX matViewProj= matView * matProj;
	D3DXMatrixInverse(&InvViewProj,NULL,&matViewProj);

	D3DXVec3TransformCoordArray(m_FrustumVertexsInWorld,sizeof(D3DXVECTOR3),m_vecClip,sizeof(D3DXVECTOR3),&InvViewProj,8);

	//世界坐标更新后，需要重新去给模型赋值
	m_pFrustumMesh->LockVertexBuffer(0, (void**)&m_pArrayFrustumVertex);
	//24个点赋值
	//前平面4个点
	m_pArrayFrustumVertex[0] = { m_FrustumVertexsInWorld[0],0x88ff0000 };
	m_pArrayFrustumVertex[1] = { m_FrustumVertexsInWorld[1],0x88ff0000 };
	m_pArrayFrustumVertex[2] = { m_FrustumVertexsInWorld[2],0x88ff0000 };
	m_pArrayFrustumVertex[3] = { m_FrustumVertexsInWorld[3],0x88ff0000 };

	//左平面4个点
	m_pArrayFrustumVertex[4] = { m_FrustumVertexsInWorld[4],0x880000ff };
	m_pArrayFrustumVertex[5] = { m_FrustumVertexsInWorld[0],0x880000ff };
	m_pArrayFrustumVertex[6] = { m_FrustumVertexsInWorld[6],0x880000ff };
	m_pArrayFrustumVertex[7] = { m_FrustumVertexsInWorld[2],0x880000ff };

	//右平面4个点
	m_pArrayFrustumVertex[8] = { m_FrustumVertexsInWorld[1],0x880000ff };
	m_pArrayFrustumVertex[9] = { m_FrustumVertexsInWorld[5],0x880000ff };
	m_pArrayFrustumVertex[10] = { m_FrustumVertexsInWorld[3],0x880000ff };
	m_pArrayFrustumVertex[11] = { m_FrustumVertexsInWorld[7],0x880000ff };

	//上平面4个点
	m_pArrayFrustumVertex[12] = { m_FrustumVertexsInWorld[4],0x8800ff00 };
	m_pArrayFrustumVertex[13] = { m_FrustumVertexsInWorld[5],0x8800ff00 };
	m_pArrayFrustumVertex[14] = { m_FrustumVertexsInWorld[0],0x8800ff00 };
	m_pArrayFrustumVertex[15] = { m_FrustumVertexsInWorld[1],0x8800ff00 };

	//下平面4个点
	m_pArrayFrustumVertex[16] = { m_FrustumVertexsInWorld[7],0x8800ff00 };
	m_pArrayFrustumVertex[17] = { m_FrustumVertexsInWorld[6],0x8800ff00 };
	m_pArrayFrustumVertex[18] = { m_FrustumVertexsInWorld[3],0x8800ff00 };
	m_pArrayFrustumVertex[19] = { m_FrustumVertexsInWorld[2],0x8800ff00 };

	//后平面4个点
	m_pArrayFrustumVertex[20] = { m_FrustumVertexsInWorld[5],0x880000ff };
	m_pArrayFrustumVertex[21] = { m_FrustumVertexsInWorld[4],0x880000ff };
	m_pArrayFrustumVertex[22] = { m_FrustumVertexsInWorld[7],0x880000ff };
	m_pArrayFrustumVertex[23] = { m_FrustumVertexsInWorld[6],0x880000ff };

	m_pFrustumMesh->UnlockVertexBuffer();


	//构建裁剪的3个平面
	MakeFrustumPlanes();
}

void CFrustum::DrawFrustum()
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	//矩阵变换
	CGraphic::GetSingleObjPtr()->m_pDevice->SetTransform(D3DTS_WORLD,&matWorld);

	//alpha混合开启
	CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);//开启alpha混合
	CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);//SRC代表即将要绘制的物体
	CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);//dest代表场景中已知的物体
	//设置alpha值来自于顶点
	CGraphic::GetSingleObjPtr()->m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);//alpha来自于顶点颜色
	CGraphic::GetSingleObjPtr()->m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);//贴图的颜色
	CGraphic::GetSingleObjPtr()->m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);//顶点颜色
	
	//没有贴图
	CGraphic::GetSingleObjPtr()->m_pDevice->SetTexture(0, NULL);
	//绘制模型
	m_pFrustumMesh->DrawSubset(0);

	//关闭alpha混合
	CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);//开启alpha混合
}
