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

	//玩家的当前位置
	D3DXVECTOR3		m_vRelativePos;
	// 用于碰撞检测后恢复为碰撞状态位置
	D3DXVECTOR3		m_vNextPos;
	float			m_fRotAngleY = 0;//Y轴的旋转角度
	float			m_fMoveSpeed;		// 移动速度

	D3DXVECTOR3 m_vecUP;
	D3DXVECTOR3 m_vecRight;
	D3DXVECTOR3 m_vecForward;


	D3DXMATRIX m_matWorld;//绘制模型的矩阵（4x4)

	

	//动画名称
	enMotionType	m_enMotionType;	// 动作类型
public:
	CPlayer(void);
	~CPlayer(void);

	HRESULT InitData();
	void Render();
	void Update(float fElapsedTime);

	D3DXVECTOR3 GetPosition() { return m_vRelativePos; }
	void SetPosition(const D3DXVECTOR3& vPos) { m_vRelativePos = vPos; }

	CCStaticMesh* GetSkinMesh() { return m_pPlayerMesh; }
	// 切换动作类型
	void ChangeMotionType(enMotionType enType);
	// 按键处理
	void ProcessKey(UINT message, WPARAM wParam, LPARAM lParam);


	//物体的动画矩阵可以用坐标系矩阵来表示

	//1 0 0 0    m_vecAxisX=(1,0,0)   0
	//0 1 0 0	 m_vecAxisY=(0,1,0)   0
	//0 0 1 0	 m_vecAxisZ=(0,0,1)   0
	//0 0 0 1	 m_vecPos  =(0,0,0)   1

	//玩家默认在世界坐标系原点

	//玩家向左转10度，X和Z轴都会更新
};


