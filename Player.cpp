#include "Player.h"
#include "ModelManager.h" // 需要包含此头文件来获取模型
#include "Node.h"         // 需要包含此头文件来操作节点

// CPlayer 构造函数, 在这里初始化默认值
CPlayer::CPlayer(void) :
	m_pPlayerData(nullptr),
	// 【修改】设置与 main.cpp 中飞机初始状态一致的默认值
	m_vRelativePos(0.0f, 20.0f, 0.0f), // 初始位置，可以设置在空中
	m_vecRight(-1.0f, 0.0f, 0.0f),     // 初始X轴，与main中一致
	m_vecUP(0.0f, 1.0f, 0.0f),         // 初始Y轴
	m_vecForward(0.0f, 0.0f, -1.0f),    // 初始Z轴
	m_fMoveSpeed(25.0f),               // 设置一个合适的移动速度
	m_fRotSpeed(2.0f)                  // 设置一个合适的旋转速度
{
	// 确保节点被创建，CObject的构造函数已经做了这件事
}

CPlayer::~CPlayer(void)
{
	// 未来可以在这里释放资源
}

HRESULT CPlayer::InitData()
{
	// 1. 从模型管理器获取模型数据
	//    注意：请确保你的 "models.xml" 中有一个名为 "airplane" 的模型项，指向 "airplane2.x" 文件
	m_pPlayerData = CModelManager::GetSingleObjPtr()->GetModelData(_T("冰龙"));
	if (!m_pPlayerData)
	{
		// 如果获取失败，可以弹窗或日志提示，并返回错误
		MessageBox(NULL, _T("未能找到名为 'airplane' 的模型!"), _T("错误"), MB_OK);
		return E_FAIL;
	}

	// 2. 将初始位置设置到节点上
	m_pNode->SetRelativePosition(m_vRelativePos);

	return S_OK;
}

void CPlayer::Update(float fElapsedTime)
{
	// --- 1. 处理输入和更新姿态 ---
	// 使用 GetAsyncKeyState 来实现平滑的移动和旋转
	D3DXMATRIX rotMatrix;
	D3DXMatrixIdentity(&rotMatrix); // 每次更新前先重置旋转矩阵

	// WS键 - 上下俯仰 (绕 Right 轴旋转)
	if (GetAsyncKeyState('W') & 0x8000)
		D3DXMatrixRotationAxis(&rotMatrix, &m_vecRight, -m_fRotSpeed * fElapsedTime);
	if (GetAsyncKeyState('S') & 0x8000)
		D3DXMatrixRotationAxis(&rotMatrix, &m_vecRight, m_fRotSpeed * fElapsedTime);

	// AD键 - 左右偏航 (绕 UP 轴旋转)
	if (GetAsyncKeyState('A') & 0x8000)
		D3DXMatrixRotationAxis(&rotMatrix, &m_vecUP, m_fRotSpeed * fElapsedTime);
	if (GetAsyncKeyState('D') & 0x8000)
		D3DXMatrixRotationAxis(&rotMatrix, &m_vecUP, -m_fRotSpeed * fElapsedTime);

	// QE键 - 左右翻滚 (绕 Forward 轴旋转)
	if (GetAsyncKeyState('Q') & 0x8000)
		D3DXMatrixRotationAxis(&rotMatrix, &m_vecForward, m_fRotSpeed * fElapsedTime);
	if (GetAsyncKeyState('E') & 0x8000)
		D3DXMatrixRotationAxis(&rotMatrix, &m_vecForward, -m_fRotSpeed * fElapsedTime);

	// 应用旋转矩阵来更新飞机的三个方向轴
	D3DXVec3TransformNormal(&m_vecRight, &m_vecRight, &rotMatrix);
	D3DXVec3TransformNormal(&m_vecForward, &m_vecForward, &rotMatrix);
	// Y轴也需要一起旋转，以保持坐标系的垂直
	D3DXVec3Cross(&m_vecUP, &m_vecForward, &m_vecRight);

	// --- 2. 处理移动 ---
	// F键 - 加速前进 (沿 Forward 轴)
	if (GetAsyncKeyState('F') & 0x8000)
		m_vRelativePos += m_vecForward * m_fMoveSpeed * fElapsedTime;
	// V键 - 减速后退
	if (GetAsyncKeyState('V') & 0x8000)
		m_vRelativePos -= m_vecForward * m_fMoveSpeed * fElapsedTime;

	// --- 3. 更新节点信息 ---
	// 将计算出的新位置和旋转应用到场景节点(CNode)上
	m_pNode->SetRelativePosition(m_vRelativePos);

	// 根据更新后的三个方向向量，手动构建旋转矩阵并设置给Node
	D3DXMATRIX matFinalRot;
	D3DXMatrixIdentity(&matFinalRot);
	matFinalRot._11 = m_vecRight.x;   matFinalRot._12 = m_vecRight.y;   matFinalRot._13 = m_vecRight.z;
	matFinalRot._21 = m_vecUP.x;      matFinalRot._22 = m_vecUP.y;      matFinalRot._23 = m_vecUP.z;
	matFinalRot._31 = m_vecForward.x; matFinalRot._32 = m_vecForward.y; matFinalRot._33 = m_vecForward.z;
	m_pNode->SetRelativeRotation(matFinalRot);
}

void CPlayer::Render()
{
	// 如果没有模型数据，则不执行渲染
	if (!m_pPlayerData)
		return;

	// 1. 从节点获取最终的世界变换矩阵
	D3DXMATRIX worldMatrix;
	m_pNode->GetWorldTransform(worldMatrix);

	// 2. 调用模型数据的渲染方法，传入计算好的世界矩阵
	m_pPlayerData->RenderMesh(worldMatrix);
}
