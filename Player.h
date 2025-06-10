#pragma once
#include "myHead.h"
#include "Object.h"
enum enMotionType { MT_STAY, MT_WALK, MT_ATTACK, MT_BEHIT, MT_DIEING, MT_DEAD };

class CCStaticMesh;


class CPlayer :public CSingleton<CPlayer>, public CObject
{
	

	friend class CSingleton<CPlayer>;
public:
	LPDIRECT3DDEVICE9			m_pDev;
	CCStaticMesh* m_pPlayerMesh;

	//��ҵĵ�ǰλ��
	D3DXVECTOR3		m_vRelativePos;
	// ������ײ����ָ�Ϊ��ײ״̬λ��
	D3DXVECTOR3		m_vNextPos;
	float			m_fRotAngleY = 0;//Y�����ת�Ƕ�
	float			m_fMoveSpeed;		// �ƶ��ٶ�

	D3DXVECTOR3 m_vecUP;
	D3DXVECTOR3 m_vecRight;
	D3DXVECTOR3 m_vecForward;


	D3DXMATRIX m_matWorld;//����ģ�͵ľ���4x4)

	

	//��������
	enMotionType	m_enMotionType;	// ��������
public:
	CPlayer(void);
	~CPlayer(void);

	HRESULT InitData();
	void Render();
	void Update(float fElapsedTime);

	D3DXVECTOR3 GetPosition() { return m_vRelativePos; }
	void SetPosition(const D3DXVECTOR3& vPos) { m_vRelativePos = vPos; }

	CCStaticMesh* GetSkinMesh() { return m_pPlayerMesh; }
	// �л���������
	void ChangeMotionType(enMotionType enType);
	// ��������
	void ProcessKey(UINT message, WPARAM wParam, LPARAM lParam);


	//����Ķ����������������ϵ��������ʾ

	//1 0 0 0    m_vecAxisX=(1,0,0)   0
	//0 1 0 0	 m_vecAxisY=(0,1,0)   0
	//0 0 1 0	 m_vecAxisZ=(0,0,1)   0
	//0 0 0 1	 m_vecPos  =(0,0,0)   1

	//���Ĭ������������ϵԭ��

	//�������ת10�ȣ�X��Z�ᶼ�����
};


