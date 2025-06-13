#include "CFrustum.h"

void CFrustum::MakeFrustumPlanes()
{
	//������׶���3��ƽ��
	//ǰƽ��
	D3DXPlaneFromPoints(&m_Planes[0], &m_FrustumVertexsInWorld[5], &m_FrustumVertexsInWorld[4], &m_FrustumVertexsInWorld[6]);
	//��ƽ��
	D3DXPlaneFromPoints(&m_Planes[1], &m_FrustumVertexsInWorld[1], &m_FrustumVertexsInWorld[5], &m_FrustumVertexsInWorld[7]);
	//��ƽ��
	D3DXPlaneFromPoints(&m_Planes[2], &m_FrustumVertexsInWorld[4], &m_FrustumVertexsInWorld[0], &m_FrustumVertexsInWorld[2]);
}

bool CFrustum::PointInFrustum(const D3DXVECTOR3& point)
{
	for (int i = 0; i < 3; i++)
	{
		//����㵽ƽ��ľ���
		float distance = D3DXPlaneDotCoord(&m_Planes[i], &point);
		if (distance > 10)
		{
			return false;
		}
		return true; // ������׶����
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
			return true; // ��һ���㲻����׶����
	}
	return false; // ���е㶼����׶����
}

bool CFrustum::SphereInFrustum(const D3DXVECTOR3& center, float radius)
{
	for (int i = 0; i < 3; i++)
	{
		float distance = D3DXPlaneDotCoord(&m_Planes[i], &center);
		if (distance > radius + 10)
		{
			return true; // ������׶����
		}
	}
	return false; // ������׶����
}

void CFrustum::InitData()
{
	//�ȼ���һ����ʼ���λ�õ���׶����
	//UpdateFrustum();
	//����һ���յ�ģ��
	D3DXCreateMeshFVF(12,24,D3DXMESH_MANAGED,FRUSTUMVERTEXFVF, CGraphic::GetSingleObjPtr()->m_pDevice,&m_pFrustumMesh);

	

	//36������ֵ
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

	//����������º���Ҫ����ȥ��ģ�͸�ֵ
	m_pFrustumMesh->LockVertexBuffer(0, (void**)&m_pArrayFrustumVertex);
	//24���㸳ֵ
	//ǰƽ��4����
	m_pArrayFrustumVertex[0] = { m_FrustumVertexsInWorld[0],0x88ff0000 };
	m_pArrayFrustumVertex[1] = { m_FrustumVertexsInWorld[1],0x88ff0000 };
	m_pArrayFrustumVertex[2] = { m_FrustumVertexsInWorld[2],0x88ff0000 };
	m_pArrayFrustumVertex[3] = { m_FrustumVertexsInWorld[3],0x88ff0000 };

	//��ƽ��4����
	m_pArrayFrustumVertex[4] = { m_FrustumVertexsInWorld[4],0x880000ff };
	m_pArrayFrustumVertex[5] = { m_FrustumVertexsInWorld[0],0x880000ff };
	m_pArrayFrustumVertex[6] = { m_FrustumVertexsInWorld[6],0x880000ff };
	m_pArrayFrustumVertex[7] = { m_FrustumVertexsInWorld[2],0x880000ff };

	//��ƽ��4����
	m_pArrayFrustumVertex[8] = { m_FrustumVertexsInWorld[1],0x880000ff };
	m_pArrayFrustumVertex[9] = { m_FrustumVertexsInWorld[5],0x880000ff };
	m_pArrayFrustumVertex[10] = { m_FrustumVertexsInWorld[3],0x880000ff };
	m_pArrayFrustumVertex[11] = { m_FrustumVertexsInWorld[7],0x880000ff };

	//��ƽ��4����
	m_pArrayFrustumVertex[12] = { m_FrustumVertexsInWorld[4],0x8800ff00 };
	m_pArrayFrustumVertex[13] = { m_FrustumVertexsInWorld[5],0x8800ff00 };
	m_pArrayFrustumVertex[14] = { m_FrustumVertexsInWorld[0],0x8800ff00 };
	m_pArrayFrustumVertex[15] = { m_FrustumVertexsInWorld[1],0x8800ff00 };

	//��ƽ��4����
	m_pArrayFrustumVertex[16] = { m_FrustumVertexsInWorld[7],0x8800ff00 };
	m_pArrayFrustumVertex[17] = { m_FrustumVertexsInWorld[6],0x8800ff00 };
	m_pArrayFrustumVertex[18] = { m_FrustumVertexsInWorld[3],0x8800ff00 };
	m_pArrayFrustumVertex[19] = { m_FrustumVertexsInWorld[2],0x8800ff00 };

	//��ƽ��4����
	m_pArrayFrustumVertex[20] = { m_FrustumVertexsInWorld[5],0x880000ff };
	m_pArrayFrustumVertex[21] = { m_FrustumVertexsInWorld[4],0x880000ff };
	m_pArrayFrustumVertex[22] = { m_FrustumVertexsInWorld[7],0x880000ff };
	m_pArrayFrustumVertex[23] = { m_FrustumVertexsInWorld[6],0x880000ff };

	m_pFrustumMesh->UnlockVertexBuffer();


	//�����ü���3��ƽ��
	MakeFrustumPlanes();
}

void CFrustum::DrawFrustum()
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	//����任
	CGraphic::GetSingleObjPtr()->m_pDevice->SetTransform(D3DTS_WORLD,&matWorld);

	//alpha��Ͽ���
	CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);//����alpha���
	CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);//SRC������Ҫ���Ƶ�����
	CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);//dest����������֪������
	//����alphaֵ�����ڶ���
	CGraphic::GetSingleObjPtr()->m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);//alpha�����ڶ�����ɫ
	CGraphic::GetSingleObjPtr()->m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);//��ͼ����ɫ
	CGraphic::GetSingleObjPtr()->m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);//������ɫ
	
	//û����ͼ
	CGraphic::GetSingleObjPtr()->m_pDevice->SetTexture(0, NULL);
	//����ģ��
	m_pFrustumMesh->DrawSubset(0);

	//�ر�alpha���
	CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);//����alpha���
}
