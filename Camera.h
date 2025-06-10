#pragma once
#include "myHead.h"
#include "Object.h"
//���������
enum ECameraType
{
	EType_FPS,//��һ�˳����
	EType_TPS,//�����˳����
	EType_Free,//�������
};

class CCamera
{

protected:
	D3DXMATRIX matCameraAxisMatrix;//���������ϵ����
	D3DXMATRIX m_matView;//����ģ�͵ľ���
	D3DXMATRIX	matProj;
	POINT mousePos;//����λ��
	int offsetPosX=0;//ƫ����X
	int offsetPosY=0;//ƫ����Y

	ECameraType m_eType;
	//���������
	D3DXVECTOR3 vEye=D3DXVECTOR3(0, 20, -30);//���λ��
	D3DXVECTOR3 vAt = D3DXVECTOR3(0, 0, 0);//���Ŀ��λ��
	D3DXVECTOR3 vUp = D3DXVECTOR3(0, 1, 0);//������Ϸ���

	//�������������ϵ����
	D3DXVECTOR3 CameraPos = D3DXVECTOR3(0, 20, -10);//���λ��
	D3DXVECTOR3 CameraAxisX = D3DXVECTOR3(1, 0, 0);//�����X��
	D3DXVECTOR3 CameraAxisY = D3DXVECTOR3(0, 1, 0);//�����Y��
	D3DXVECTOR3 CameraAxisZ = D3DXVECTOR3(0, 0, 1);//�����Z��

	

	float CameraXAxisAngle = 0.0f;//��λ�ǻ���   3.14  =180��
	float CameraYAxisAngle = 0.0f;
	//float zAxisAngle = 0.0f;
	float CameraMoveSpeed = 1.0f;

	public:
		CCamera(void);
		~CCamera(void);
		virtual HRESULT InitCamera(TiXmlElement* cameraElem)=0;

		void ProceeEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);

		virtual void Update(float fElapsedTime) ;

		D3DXMATRIX GetCameraViewMatrix() { return m_matView; } //��ȡ�������ͼ����
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



//���������
class CCameraManager:public CObject
{
public:
	map<ECameraType,CCamera*> m_mapCamera;

	CCamera* m_pActiveCamera;

	void ChangeCamera(ECameraType type);//�л��������
public:
		//���ؾ��������
	HRESULT InitData();
	void Update(float fElapsedTime);
	void ProceeEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	CCameraManager();
	~CCameraManager();
};