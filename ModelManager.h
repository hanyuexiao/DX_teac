#pragma once
#include "myHead.h"

class CNode;

struct CModelData
{
	TSTRING m_strName;
	TSTRING m_strPath;
	LPD3DXMESH m_pMesh;
	
	DWORD m_dwNumMaterials;
	vector<LPDIRECT3DTEXTURE9> m_vecTextures;

	HRESULT LoadMesh(TSTRING name, TSTRING strPath);
	void RenderMesh(D3DXMATRIX outMatrix);
	
	/*D3DXVECTOR3 m_vecLocalRot=D3DXVECTOR3(0,0,0);
	D3DXVECTOR3 m_vecLocalScale=D3DXVECTOR3(1,1,1);
	D3DXVECTOR3 m_vecLocalPos=D3DXVECTOR3(0,0,0);*/
};


class CModelManager:public CSingleton<CModelManager>
{
	friend CSingleton<CModelManager>;
private:
	map<TSTRING, CModelData*> m_mapAllMeshs;

public:
	HRESULT Init(TSTRING fileName);
	CModelData* GetModelData(TSTRING name);
	
};

