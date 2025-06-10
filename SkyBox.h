#pragma once
#include "myhead.h"
#include "Object.h"
struct SkyboxVERTEX
{
	D3DXVECTOR3 pos;
	DWORD color;
	D3DXVECTOR2 texpos;//��������
};

class CSkyBox :public CObject
{
public:
	CSkyBox();
	~CSkyBox();
	//                 
	HRESULT InitData();
	//������պ�

	virtual void Update(float fElapsedTime);

	void Render();
	
private:

	LPDIRECT3DDEVICE9	m_pDev;		//�豸�ӿ�
	D3DSURFACE_DESC desc;
	LPDIRECT3DTEXTURE9 m_pTexColor[6];	//���ֵ�����ɫֵ��ͼƬ

	SkyboxVERTEX* m_pVertex4;		//�����õ�
	WORD* m_pIndex;//���Ի�������
	DWORD* m_pAttri;//���Ի�������

	string TexUp;
	string TexDown;
	string TexForworld;
	string TexBack;
	string TexLeft;
	string TexRight;

	float m_fSize=10;

	//���ģ��
	LPD3DXMESH m_pSkyMesh;
};

