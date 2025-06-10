#include "SkyBox.h"



CSkyBox::CSkyBox()
{
	m_pNode=new CNode;
}
CSkyBox::~CSkyBox()
{

}

HRESULT CSkyBox::InitData()//(TSTRING UPname, TSTRING Downname, TSTRING Forworldname, TSTRING Backname, TSTRING Leftname, TSTRING Rightname)
{
	TiXmlElement* pElem=m_pRootElem->FirstChildElement("UpTex");
	TexUp=pElem->GetText();

	pElem = m_pRootElem->FirstChildElement("DownTex");
	TexDown = pElem->GetText();

	pElem = m_pRootElem->FirstChildElement("LeftTex");
	TexLeft = pElem->GetText();

	pElem = m_pRootElem->FirstChildElement("FrontTex");
	TexForworld = pElem->GetText();

	pElem = m_pRootElem->FirstChildElement("RightTex");
	TexRight = pElem->GetText();

	pElem = m_pRootElem->FirstChildElement("BackTex");
	TexBack = pElem->GetText();

	pElem = m_pRootElem->FirstChildElement("Size");
	m_fSize = atof(pElem->GetText());

	m_pDev = CGraphic::GetSingleObjPtr()->m_pDevice;
	//����������ͼ
	//D3DXCreateTextureFromFile(m_pDev, colorName.c_str(), &m_pTexColor);
	D3DXCreateTextureFromFileA(m_pDev, TexUp.c_str(), &m_pTexColor[0]);
	D3DXCreateTextureFromFileA(m_pDev, TexForworld.c_str(), &m_pTexColor[1]);
	D3DXCreateTextureFromFileA(m_pDev, TexBack.c_str(), &m_pTexColor[2]);
	D3DXCreateTextureFromFileA(m_pDev, TexLeft.c_str(), &m_pTexColor[3]);
	D3DXCreateTextureFromFileA(m_pDev, TexRight.c_str(), &m_pTexColor[4]);
	D3DXCreateTextureFromFileA(m_pDev, TexDown.c_str(), &m_pTexColor[5]);

	//�������ģ��
	D3DXCreateMeshFVF(12, 24, 0, D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE, m_pDev, &m_pSkyMesh);


	//���㻺�渳ֵ 
	m_pSkyMesh->LockVertexBuffer(0, (void**)&m_pVertex4);

	//��
	m_pVertex4[0] = { D3DXVECTOR3(m_fSize, m_fSize, m_fSize),0xffffffff,D3DXVECTOR2(1, 0) };//5
	m_pVertex4[1] = { D3DXVECTOR3(-m_fSize, m_fSize, m_fSize),0xffffffff,D3DXVECTOR2(0, 0) };//4
	m_pVertex4[2] = { D3DXVECTOR3(m_fSize, m_fSize,-m_fSize),0xffffffff,D3DXVECTOR2(1, 1) };//1
	m_pVertex4[3] = { D3DXVECTOR3(-m_fSize, m_fSize,-m_fSize),0xffffffff,D3DXVECTOR2(0, 1) };//0
	//ǰ			  
	m_pVertex4[4] = { D3DXVECTOR3(m_fSize, m_fSize,-m_fSize),0xffffffff,D3DXVECTOR2(1, 0) };//1
	m_pVertex4[5] = { D3DXVECTOR3(-m_fSize, m_fSize,-m_fSize),0xffffffff,D3DXVECTOR2(0, 0) };//0
	m_pVertex4[6] = { D3DXVECTOR3(m_fSize,-m_fSize,-m_fSize),0xffffffff,D3DXVECTOR2(1, 1) };//2
	m_pVertex4[7] = { D3DXVECTOR3(-m_fSize,-m_fSize,-m_fSize),0xffffffff,D3DXVECTOR2(0, 1) };//3
	//��
	m_pVertex4[8] = { D3DXVECTOR3(-m_fSize, m_fSize, m_fSize),0xffffffff,D3DXVECTOR2(1, 0) };//4
	m_pVertex4[9] = { D3DXVECTOR3(m_fSize, m_fSize, m_fSize),0xffffffff,D3DXVECTOR2(0, 0) };//5
	m_pVertex4[10] = { D3DXVECTOR3(-m_fSize,-m_fSize, m_fSize),0xffffffff,D3DXVECTOR2(1, 1) };//7
	m_pVertex4[11] = { D3DXVECTOR3(m_fSize,-m_fSize, m_fSize),0xffffffff,D3DXVECTOR2(0, 1) };//6
	//��
	m_pVertex4[12] = { D3DXVECTOR3(-m_fSize, m_fSize,-m_fSize),0xffffffff,D3DXVECTOR2(1, 0) };//0
	m_pVertex4[13] = { D3DXVECTOR3(-m_fSize, m_fSize, m_fSize),0xffffffff,D3DXVECTOR2(0, 0) };//4
	m_pVertex4[14] = { D3DXVECTOR3(-m_fSize,-m_fSize,-m_fSize),0xffffffff,D3DXVECTOR2(1, 1) };//3
	m_pVertex4[15] = { D3DXVECTOR3(-m_fSize,-m_fSize, m_fSize),0xffffffff,D3DXVECTOR2(0, 1) };//7
	//��
	m_pVertex4[16] = { D3DXVECTOR3(m_fSize, m_fSize, m_fSize),0xffffffff,D3DXVECTOR2(1, 0) };//5
	m_pVertex4[17] = { D3DXVECTOR3(m_fSize, m_fSize,-m_fSize),0xffffffff,D3DXVECTOR2(0, 0) };//1
	m_pVertex4[18] = { D3DXVECTOR3(m_fSize,-m_fSize, m_fSize),0xffffffff,D3DXVECTOR2(1, 1) };//6
	m_pVertex4[19] = { D3DXVECTOR3(m_fSize,-m_fSize,-m_fSize),0xffffffff,D3DXVECTOR2(0, 1) };//2
	//��
	m_pVertex4[20] = { D3DXVECTOR3(-m_fSize,-m_fSize, m_fSize),0xffffffff,D3DXVECTOR2(0, 1) };//7
	m_pVertex4[21] = { D3DXVECTOR3(m_fSize,-m_fSize, m_fSize),0xffffffff,D3DXVECTOR2(1, 1) };//6
	m_pVertex4[22] = { D3DXVECTOR3(-m_fSize,-m_fSize,-m_fSize),0xffffffff,D3DXVECTOR2(0, 0) };//3
	m_pVertex4[23] = { D3DXVECTOR3(m_fSize,-m_fSize,-m_fSize),0xffffffff,D3DXVECTOR2(1, 0) };//2
	m_pSkyMesh->UnlockVertexBuffer();

	//�������渳ֵ
	WORD index[36] = { 0,3,2,0,1,3,
						4,7,6,4,5,7,
						8,11,10,8,9,11,
						12,15,14,12,13,15,
						16,19,18,16,17,19,
						20,23,22,20,21,23
	};
	m_pSkyMesh->LockIndexBuffer(0, (void**)&m_pIndex);
	memcpy(m_pIndex, index, sizeof(index));
	m_pSkyMesh->UnlockIndexBuffer();

	//���ģ����6���Ӽ�
	//���Ի��渳ֵ
	DWORD attribute[12] = { 0,0,1,1,2,2,3,3,4,4,5,5 };
	m_pSkyMesh->LockAttributeBuffer(0, &m_pAttri);

	memcpy(m_pAttri, attribute, sizeof(attribute));

	m_pSkyMesh->UnlockAttributeBuffer();

	return S_OK;
}

void CSkyBox::Update(float fElapsedTime)
{
	D3DXMATRIX matView, matCameraAxis;
	m_pDev->GetTransform(D3DTS_VIEW, &matView);//�õ�����ľ���

	//������ľ����������===���������ϵ����
	D3DXMatrixInverse(&matCameraAxis, 0, &matView);
	D3DXVECTOR3 cameraPos(matCameraAxis._41, matCameraAxis._42, matCameraAxis._43);

	m_pNode->SetRelativePosition(cameraPos);

	m_pNode->Update(fElapsedTime);
}

void CSkyBox::Render()
{	

	D3DXMATRIX worldMatrix;
	m_pNode->GetWorldTransform(worldMatrix);
	m_pDev->SetTransform(D3DTS_WORLD, &worldMatrix);
	//��Ⱦ״̬�����ã���Ӱ��ȫ�֣�

	//�ر���ȼ���
	//m_pDev->SetRenderState(D3DRS_ZENABLE,false);//�ر���ȼ���
	m_pDev->SetRenderState(D3DRS_ZWRITEENABLE, false);//�ر�д���ֵ

	for (int i = 0; i < 6; i++)
	{
		m_pDev->SetTexture(0, m_pTexColor[i]);


		//ʹ��Ĭ�ϵĹ��˷���
		//m_pDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);//ͼƬ��Сʱ����ȡ�ķ���
		//m_pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);//ͼƬ�Ŵ�ʱ����ȡ�ķ���
		//m_pDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);//�༶����ѡ����ʵĴ�С����ͼ

		m_pDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		m_pDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pSkyMesh->DrawSubset(i);
	}

	m_pDev->SetRenderState(D3DRS_ZWRITEENABLE, true);//�ر�д���ֵ
}
