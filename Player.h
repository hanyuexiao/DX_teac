#pragma once
#include "myHead.h"
#include "Object.h"

// 前向声明 (Forward Declaration)，避免头文件循环包含
class CModelData;

// 将 CPlayer 作为飞机类来完善
class CPlayer : public CSingleton<CPlayer>, public CObject
{
	friend class CSingleton<CPlayer>;
private:
	//使用 CModelData 指针来存储模型数据
	CModelData* m_pPlayerData;

	D3DXVECTOR3 m_vRelativePos;   // 位置 (相当于 main 里的 planePos)
	D3DXVECTOR3 m_vecRight;       // X轴/右方向 
	D3DXVECTOR3 m_vecUP;          // Y轴/上方向 
	D3DXVECTOR3 m_vecForward;     // Z轴/前方向 

	// 用于控制移动和旋转的速度
	float m_fMoveSpeed;
	float m_fRotSpeed;

public:
	CPlayer(void);
	~CPlayer(void);

	// 重写父类的虚函数
	virtual HRESULT InitData() override;
	virtual void Update(float fElapsedTime) override;
	virtual void Render() override;

};
