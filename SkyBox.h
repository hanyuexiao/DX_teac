#pragma once
#include "myhead.h"
#include "Object.h"
struct SkyboxVERTEX
{
	D3DXVECTOR3 pos;
	DWORD color;
	D3DXVECTOR2 texpos;//纹理坐标
};

class CSkyBox :public CObject
{
public:
	CSkyBox();
	~CSkyBox();
	//                 
	HRESULT InitData();
	//绘制天空盒

	virtual void Update(float fElapsedTime);

	void Render();
	
private:

	LPDIRECT3DDEVICE9	m_pDev;		//设备接口
	D3DSURFACE_DESC desc;
	LPDIRECT3DTEXTURE9 m_pTexColor[6];	//保持地形颜色值的图片

	SkyboxVERTEX* m_pVertex4;		//绘制用点
	WORD* m_pIndex;//所以缓存数组
	DWORD* m_pAttri;//属性缓存数组

	string TexUp;
	string TexDown;
	string TexForworld;
	string TexBack;
	string TexLeft;
	string TexRight;

	float m_fSize=10;

	//天空模型
	LPD3DXMESH m_pSkyMesh;
};

