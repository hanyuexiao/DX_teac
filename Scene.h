#pragma once
#include "myHead.h"
#include "Player.h"

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
	vector<CStaticMesh*> m_vecMesh;	//�����������ɸ�ģ��
	CCameraManager* m_pCameraManager;
	CTerrain* m_pTerrain;	//����
	CSkyBox* m_pSkyBox;	//��պ�
	CFrustum* m_pFrustum;//��׶��
	CPlayer* m_pPlayer;	//���

	D3DXMATRIX m_matLockedView;
	public:
	//���ؽű��ļ�
	HRESULT InitObject(TSTRING name);
	HRESULT InitScene(TSTRING name);

	HRESULT LoadStaticMeshFile(TSTRING name);

	void EventProcess(UINT msg, WPARAM wParam, LPARAM lParam);

	void Update(float fElapsedTime);

	void Render();

	void RenderAllMeshes();

	void RenderTerrain();

	//���ģ���Ƿ�����׶����
	bool IsInFrustum();
};

