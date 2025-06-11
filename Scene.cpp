#include "Scene.h"
#include "StaticMesh.h"
#include "Terrain.h"
#include "Camera.h"
#include "SkyBox.h"
#include "CFrustum.h"

HRESULT CScene::InitScene(TSTRING name)
{
	//解析和初始化地形
	TCHAR contextStr[100];
	GetPrivateProfileString(_T("Terrain"), _T("InfoName"), _T(""),contextStr, MAX_PATH, name.c_str());
	m_pTerrain = new CTerrain;
	if (FAILED(m_pTerrain->InitObject(contextStr)))
		return E_FAIL;

	//解析和初始化天空
	GetPrivateProfileString(_T("SkyBox"), _T("InfoName"), _T(""), contextStr, MAX_PATH, name.c_str());
	m_pSkyBox = new CSkyBox;
	m_pSkyBox->InitObject(contextStr);

	//解析和初始化模型数据
	GetPrivateProfileString(_T("StaticMeshs"), _T("InfoName"), _T(""), contextStr, MAX_PATH, name.c_str());
	if (FAILED(LoadStaticMeshFile(contextStr)))
		return E_FAIL;
	
	//解析和初始化相机
	GetPrivateProfileString(_T("Camera"), _T("InfoName"), _T(""), contextStr, MAX_PATH, name.c_str());
	m_pCameraManager=new CCameraManager;
	m_pCameraManager->InitObject(contextStr);
	GetPrivateProfileString(_T("Camera"), _T("Active"), _T(""), contextStr, MAX_PATH, name.c_str());
	m_pCameraManager->ChangeCamera((ECameraType)_ttoi(contextStr));


	//绑定节点关系
	for (CStaticMesh* pMesh : m_vecMesh)
	{
		if (pMesh)
		{
			m_pTerrain->GetNode()->addChild(pMesh->GetNode());
		}
	}
	

	//初始化视锥
	m_pFrustum=new CFrustum;
	m_pFrustum->InitData();

	return S_OK;
}

HRESULT CScene::LoadStaticMeshFile(TSTRING name){
	
	string ansiName;
#ifdef _UNICODE
	ansiName = wide_char_to_multi_byte(name, -1);
#else
	ansiName = name;
#endif
	TiXmlDocument doc(ansiName.c_str());
	if (!doc.LoadFile())
		return E_FAIL;
	TiXmlDocument* pDoc = &doc;
	TiXmlElement* pRoot = pDoc->FirstChildElement();

	TiXmlElement* pElem=pRoot->FirstChildElement();
	TSTRING nodeName;
	for (; pElem; pElem = pElem->NextSiblingElement())
	{
		string localname = pElem->Attribute("name");
		
		CStaticMesh* pMesh = new CStaticMesh;
		pMesh->InitObject(pElem);
		if (FAILED(pMesh->InitData()))
		{
			break;
		}

		D3DXVECTOR3 vPos = pMesh->GetNode()->GetWorldPos();
		vPos.y = m_pTerrain->GetHeight(vPos.x, vPos.z);

		pMesh->GetNode()->SetRelativePosition(vPos);
		m_vecMesh.push_back(pMesh);
	}
	return S_OK;
}

void CScene::EventProcess(UINT msg, WPARAM wParam, LPARAM lParam)
{
	//事件的中转
	if(m_pCameraManager)
		m_pCameraManager->ProceeEvent(msg,wParam,lParam);

	switch (msg)
	{
		case WM_KEYDOWN:
			if(wParam=='2')
			{
				//锁住相机
				m_pFrustum->SetLockeed();

				m_matLockedView = m_pCameraManager->m_pActiveCamera->GetCameraViewMatrix();
				break;
			}
		break;
	}
}

void CScene::Update(float fElapsedTime)
{
	m_pTerrain->GetNode()->Update(fElapsedTime);

	m_pSkyBox->Update(fElapsedTime);
	m_pCameraManager->Update(fElapsedTime);

	//判断是否锁住相机
	if (m_pFrustum->GetLocked())
	{
		//给锁住的相机矩阵赋值
		m_pFrustum->UpdateFrustum(&m_matLockedView);
	}
	else
	{
		m_pFrustum->UpdateFrustum();
	}
}

void CScene::Render()
{
	m_pSkyBox->Render();
	RenderTerrain();
	RenderAllMeshes();
	
	if (m_pFrustum->GetLocked())
		m_pFrustum->DrawFrustum();
	
}

void CScene::RenderAllMeshes()
{
	for	(CStaticMesh * pMesh : m_vecMesh)
	{
		if (pMesh)
		{
			pMesh->Render();
		}
	} 
}

void CScene::RenderTerrain()
{
	
	if (m_pTerrain)
		m_pTerrain->Render();
	
}

bool CScene::IsInFrustum()
{
	if (m_pFrustum)
	{
		for (CStaticMesh* pMesh : m_vecMesh)
		{
			if (pMesh && pMesh->GetBoundInfo()
			{
				if (!m_pFrustum->BoxInFrustum(pMesh->GetAABB()->GetCornerArray()))
				{
					pMesh->SetIsCulled(true);
				}
				else
				{
					pMesh->SetIsCulled(false);
				}
			}
		}
		return true;
	}
}
