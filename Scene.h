#pragma once
#include "myHead.h"

class CPlayer;
class CStaticMesh;
class CCameraManager;
class CTerrain;
class CSkyBox;
class CFrustum;

class CScene:public CSingleton<CScene>
{
	friend CSingleton<CScene>;
private:
	//CPlayer* m_pPlayer;
	vector<CStaticMesh*> m_vecMesh;	//场景中有若干个模型
	CCameraManager* m_pCameraManager;
	CTerrain* m_pTerrain;	//地形
	CSkyBox* m_pSkyBox;	//天空盒
	CFrustum* m_pFrustum;//视锥体


	D3DXMATRIX m_matLockedView;
	public:
	//加载脚本文件
	HRESULT InitObject(TSTRING name);
	HRESULT InitScene(TSTRING name);

	HRESULT LoadStaticMeshFile(TSTRING name);

	void EventProcess(UINT msg, WPARAM wParam, LPARAM lParam);

	void Update(float fElapsedTime);

	void Render();

	void RenderAllMeshes();

	void RenderTerrain();
	
};

