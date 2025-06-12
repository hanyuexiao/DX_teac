#pragma once
#include "myHead.h"
#include "Object.h"

// ǰ������ (Forward Declaration)������ͷ�ļ�ѭ������
class CModelData;

// �� CPlayer ��Ϊ�ɻ���������
class CPlayer : public CSingleton<CPlayer>, public CObject
{
	friend class CSingleton<CPlayer>;
private:
	//ʹ�� CModelData ָ�����洢ģ������
	CModelData* m_pPlayerData;

	D3DXVECTOR3 m_vRelativePos;   // λ�� (�൱�� main ��� planePos)
	D3DXVECTOR3 m_vecRight;       // X��/�ҷ��� 
	D3DXVECTOR3 m_vecUP;          // Y��/�Ϸ��� 
	D3DXVECTOR3 m_vecForward;     // Z��/ǰ���� 

	// ���ڿ����ƶ�����ת���ٶ�
	float m_fMoveSpeed;
	float m_fRotSpeed;

public:
	CPlayer(void);
	~CPlayer(void);

	// ��д������麯��
	virtual HRESULT InitData() override;
	virtual void Update(float fElapsedTime) override;
	virtual void Render() override;

};
