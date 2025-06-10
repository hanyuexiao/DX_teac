#include "Camera.h"

CCamera::CCamera(void)
{
	
}

CCamera::~CCamera(void)
{

}

void CCamera::ProceeEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_LBUTTONDOWN://鼠标左键按下事件
		{
			mousePos.x = LOWORD(lParam);
			mousePos.y = HIWORD(lParam);

			break;
		}
		case WM_LBUTTONUP://鼠标左键抬起事件
		{

			break;
		}
		case WM_MOUSEMOVE://鼠标移动事件
		{
			//计算2帧之间的鼠标偏移量
			if (wParam == MK_LBUTTON)//鼠标移动时，同时按下了鼠标左键
			{
				offsetPosX = LOWORD(lParam) - mousePos.x;
				offsetPosY = HIWORD(lParam) - mousePos.y;

				mousePos.x = LOWORD(lParam);
				mousePos.y = HIWORD(lParam);
			}

			break;
		}
	}
}

void CCamera::Update(float fElapsedTime)
{

//方向键（上下左右）控制相机的移动
	D3DXMATRIX rotMatrix;

	//根据鼠标的偏移量实现相机的旋转
	if (offsetPosY)
	{
		float angle=offsetPosY*0.01;//0.01代表鼠标的灵敏度值，把偏移量计算成（弧度）

		D3DXMatrixRotationAxis(&rotMatrix, &CameraAxisX, angle);
		//D3DXMatrixRotationX 该X轴，代表世界的X轴，永远固定的（1，0，0）；所以不能使用固定的轴旋转
		//改Y，Z
		D3DXVec3TransformNormal(&CameraAxisY, &CameraAxisY, &rotMatrix);
		D3DXVec3TransformNormal(&CameraAxisZ, &CameraAxisZ, &rotMatrix);


		offsetPosY=0;
	}

	if (offsetPosX)
	{
		float angle = offsetPosX * 0.01;//0.01代表鼠标的灵敏度值，把偏移量计算成（弧度）

		D3DXMatrixRotationAxis(&rotMatrix, &CameraAxisY, angle);
		//改X，Z
		D3DXVec3TransformNormal(&CameraAxisX, &CameraAxisX, &rotMatrix);
		D3DXVec3TransformNormal(&CameraAxisZ, &CameraAxisZ, &rotMatrix);

		offsetPosX=0;
	}
	D3DXMATRIX matView, matCameraAxisMatrix;

	//D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
	//手写相机坐标系矩阵
	matCameraAxisMatrix._11 = CameraAxisX.x; matCameraAxisMatrix._12 = CameraAxisX.y; matCameraAxisMatrix._13 = CameraAxisX.z; matCameraAxisMatrix._14 = 0;
	matCameraAxisMatrix._21 = CameraAxisY.x; matCameraAxisMatrix._22 = CameraAxisY.y; matCameraAxisMatrix._23 = CameraAxisY.z; matCameraAxisMatrix._24 = 0;
	matCameraAxisMatrix._31 = CameraAxisZ.x; matCameraAxisMatrix._32 = CameraAxisZ.y; matCameraAxisMatrix._33 = CameraAxisZ.z; matCameraAxisMatrix._34 = 0;
	matCameraAxisMatrix._41 = CameraPos.x;	 matCameraAxisMatrix._42 = CameraPos.y; matCameraAxisMatrix._43 = CameraPos.z;   matCameraAxisMatrix._44 = 1;
	//相机的矩阵是相机坐标系矩阵的逆矩阵
	D3DXMatrixInverse(&matView, nullptr, &matCameraAxisMatrix);

	//手写相机矩阵

	CGraphic::GetSingleObjPtr()->m_pDevice->SetTransform(D3DTS_VIEW, &matView);//应用矩阵，实现相机变换（世界-》相机）

	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 800.0f / 600.0f, 1.0f, 10000);
	CGraphic::GetSingleObjPtr()->m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);//应用矩阵，实现投影变换（相机-》投影空间）

	////5、显示（窗口坐标系）(投影空间坐标系-》窗口坐标系)
	D3DVIEWPORT9 viewPort;
	viewPort.X = 0;
	viewPort.Y = 0;
	viewPort.Width = 800;
	viewPort.Height = 600;
	viewPort.MinZ = 0;
	viewPort.MaxZ = 1;
	CGraphic::GetSingleObjPtr()->m_pDevice->SetViewport(&viewPort);
}

void CCameraManager::ChangeCamera(ECameraType type)
{
	if(!m_pActiveCamera)
	{
		m_pActiveCamera = m_mapCamera[type];
	}
	else
	{
		CCamera* oldCamera=m_pActiveCamera;

		m_pActiveCamera = m_mapCamera[type];

		//实现平滑移动
		//todo....
	}
	


}

HRESULT CCameraManager::InitData()
{
	TiXmlElement* pElem=m_pRootElem->FirstChildElement("FPS");

	CCamera* pCamera=new CFPSCamera;
	pCamera->InitCamera(pElem);
	m_mapCamera[EType_FPS]=pCamera;

	pElem = m_pRootElem->FirstChildElement("TPS");
	pCamera = new CTPSCamera;
	pCamera->InitCamera(pElem);
	m_mapCamera[EType_TPS] = pCamera;

	pElem = m_pRootElem->FirstChildElement("Free");
	pCamera = new CFreeCamera;
	pCamera->InitCamera(pElem);
	m_mapCamera[EType_Free] = pCamera;

	return S_OK;
}


void CCameraManager::Update(float fElapsedTime)
{
	m_pActiveCamera->Update(fElapsedTime);
}

void CCameraManager::ProceeEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_pActiveCamera->ProceeEvent(uMsg,wParam,lParam);
}

CCameraManager::CCameraManager()
{
	m_pNode=new CNode;
}

CCameraManager::~CCameraManager()
{

}

HRESULT CFPSCamera::InitCamera(TiXmlElement* cameraElem)
{
	return S_OK;
}

HRESULT CTPSCamera::InitCamera(TiXmlElement* cameraElem)
{
	return S_OK;
}
HRESULT CFreeCamera::InitCamera(TiXmlElement* cameraElem)
{
		string str1, str2, str3;
		TiXmlElement* pElem = cameraElem->FirstChildElement("Position");
		if (pElem)
		{
			str1 = pElem->Attribute("x");
			str2 = pElem->Attribute("y");
			str3 = pElem->Attribute("z");

			float x = (float)atof(str1.c_str());
			float y = (float)atof(str2.c_str());
			float z = (float)atof(str3.c_str());

			CameraPos = D3DXVECTOR3(x, y, z);
		}
		//读取Z轴信息
		pElem = cameraElem->FirstChildElement("Forward");
		if (pElem)
		{
			str1 = pElem->Attribute("x");
			str2 = pElem->Attribute("y");
			str3 = pElem->Attribute("z");

			float x = (float)atof(str1.c_str());
			float y = (float)atof(str2.c_str());
			float z = (float)atof(str3.c_str());

			CameraAxisZ = D3DXVECTOR3(x, y, z);
		}
		//读取X轴信息
		pElem = cameraElem->FirstChildElement("Right");
		if (pElem)
		{
			str1 = pElem->Attribute("x");
			str2 = pElem->Attribute("y");
			str3 = pElem->Attribute("z");

			float x = (float)atof(str1.c_str());
			float y = (float)atof(str2.c_str());
			float z = (float)atof(str3.c_str());

			CameraAxisX = D3DXVECTOR3(x, y, z);
		}
		//读取Z轴信息
		pElem = cameraElem->FirstChildElement("Up");
		if (pElem)
		{
			str1 = pElem->Attribute("x");
			str2 = pElem->Attribute("y");
			str3 = pElem->Attribute("z");

			float x = (float)atof(str1.c_str());
			float y = (float)atof(str2.c_str());
			float z = (float)atof(str3.c_str());

			CameraAxisY = D3DXVECTOR3(x, y, z);
		}

		return S_OK;

}

void CFreeCamera::Update(float fElapsedTime)
{
	if (GetAsyncKeyState(VK_LEFT))
	{
		//相机向左移动
		CameraPos -= CameraAxisX * CameraMoveSpeed;

	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		//相机向右移动
		CameraPos += CameraAxisX * CameraMoveSpeed;
	}
	if (GetAsyncKeyState(VK_UP))
	{
		//相机向前移动
		CameraPos += CameraAxisZ * CameraMoveSpeed;

	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		//相机向后移动
		CameraPos -= CameraAxisZ * CameraMoveSpeed;
	}

	__super::Update(fElapsedTime);
}

