#pragma once
#include "myHead.h"
#include "Object.h"
//相机的类型
enum ECameraType
{
	EType_FPS,//第一人称相机
	EType_TPS,//第三人称相机
	EType_Free,//自由相机
};

class CCamera
{

protected:
	D3DXMATRIX matCameraAxisMatrix;//相机的坐标系矩阵
	D3DXMATRIX m_matView;//绘制模型的矩阵
	D3DXMATRIX	matProj;
	POINT mousePos;//鼠标的位置
	int offsetPosX=0;//偏移量X
	int offsetPosY=0;//偏移量Y

	ECameraType m_eType;
	//相机的设置
	D3DXVECTOR3 vEye=D3DXVECTOR3(0, 20, -30);//相机位置
	D3DXVECTOR3 vAt = D3DXVECTOR3(0, 0, 0);//相机目标位置
	D3DXVECTOR3 vUp = D3DXVECTOR3(0, 1, 0);//相机的上方向

	//定义相机的坐标系参数
	D3DXVECTOR3 CameraPos = D3DXVECTOR3(0, 20, -10);//相机位置
	D3DXVECTOR3 CameraAxisX = D3DXVECTOR3(1, 0, 0);//相机的X轴
	D3DXVECTOR3 CameraAxisY = D3DXVECTOR3(0, 1, 0);//相机的Y轴
	D3DXVECTOR3 CameraAxisZ = D3DXVECTOR3(0, 0, 1);//相机的Z轴

	

	float CameraXAxisAngle = 0.0f;//单位是弧度   3.14  =180度
	float CameraYAxisAngle = 0.0f;
	//float zAxisAngle = 0.0f;
	float CameraMoveSpeed = 1.0f;

	public:
		CCamera(void);
		~CCamera(void);
		virtual HRESULT InitCamera(TiXmlElement* cameraElem)=0;

		void ProceeEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);

		virtual void Update(float fElapsedTime) ;

		D3DXMATRIX GetCameraViewMatrix() { return m_matView; } //获取相机的视图矩阵
};


class CFPSCamera:public CCamera
{
public:
	virtual HRESULT InitCamera(TiXmlElement* cameraElem);
	CFPSCamera(){m_eType=EType_FPS;}
};

class CTPSCamera :public CCamera
{
public:
	virtual HRESULT InitCamera(TiXmlElement* cameraElem);
	CTPSCamera(){m_eType=EType_TPS;}
};

class CFreeCamera :public CCamera
{
public:
	CFreeCamera(){m_eType=EType_Free;}


	public:
		HRESULT InitCamera(TiXmlElement* cameraElem);
	void Update(float fElapsedTime);
};



//相机管理类
class CCameraManager:public CObject
{
public:
	map<ECameraType,CCamera*> m_mapCamera;

	CCamera* m_pActiveCamera;

	void ChangeCamera(ECameraType type);//切换相机类型
public:
		//加载具体的数据
	HRESULT InitData();
	void Update(float fElapsedTime);
	void ProceeEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	CCameraManager();
	~CCameraManager();
};