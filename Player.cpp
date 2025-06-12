#include "Player.h"
#include "ModelManager.h" // ��Ҫ������ͷ�ļ�����ȡģ��
#include "Node.h"         // ��Ҫ������ͷ�ļ��������ڵ�

// CPlayer ���캯��, �������ʼ��Ĭ��ֵ
CPlayer::CPlayer(void) :
	m_pPlayerData(nullptr),
	// ���޸ġ������� main.cpp �зɻ���ʼ״̬һ�µ�Ĭ��ֵ
	m_vRelativePos(0.0f, 20.0f, 0.0f), // ��ʼλ�ã����������ڿ���
	m_vecRight(-1.0f, 0.0f, 0.0f),     // ��ʼX�ᣬ��main��һ��
	m_vecUP(0.0f, 1.0f, 0.0f),         // ��ʼY��
	m_vecForward(0.0f, 0.0f, -1.0f),    // ��ʼZ��
	m_fMoveSpeed(25.0f),               // ����һ�����ʵ��ƶ��ٶ�
	m_fRotSpeed(2.0f)                  // ����һ�����ʵ���ת�ٶ�
{
	// ȷ���ڵ㱻������CObject�Ĺ��캯���Ѿ����������
}

CPlayer::~CPlayer(void)
{
	// δ�������������ͷ���Դ
}

HRESULT CPlayer::InitData()
{
	// 1. ��ģ�͹�������ȡģ������
	//    ע�⣺��ȷ����� "models.xml" ����һ����Ϊ "airplane" ��ģ���ָ�� "airplane2.x" �ļ�
	m_pPlayerData = CModelManager::GetSingleObjPtr()->GetModelData(_T("����"));
	if (!m_pPlayerData)
	{
		// �����ȡʧ�ܣ����Ե�������־��ʾ�������ش���
		MessageBox(NULL, _T("δ���ҵ���Ϊ 'airplane' ��ģ��!"), _T("����"), MB_OK);
		return E_FAIL;
	}

	// 2. ����ʼλ�����õ��ڵ���
	m_pNode->SetRelativePosition(m_vRelativePos);

	return S_OK;
}

void CPlayer::Update(float fElapsedTime)
{
	// --- 1. ��������͸�����̬ ---
	// ʹ�� GetAsyncKeyState ��ʵ��ƽ�����ƶ�����ת
	D3DXMATRIX rotMatrix;
	D3DXMatrixIdentity(&rotMatrix); // ÿ�θ���ǰ��������ת����

	// WS�� - ���¸��� (�� Right ����ת)
	if (GetAsyncKeyState('W') & 0x8000)
		D3DXMatrixRotationAxis(&rotMatrix, &m_vecRight, -m_fRotSpeed * fElapsedTime);
	if (GetAsyncKeyState('S') & 0x8000)
		D3DXMatrixRotationAxis(&rotMatrix, &m_vecRight, m_fRotSpeed * fElapsedTime);

	// AD�� - ����ƫ�� (�� UP ����ת)
	if (GetAsyncKeyState('A') & 0x8000)
		D3DXMatrixRotationAxis(&rotMatrix, &m_vecUP, m_fRotSpeed * fElapsedTime);
	if (GetAsyncKeyState('D') & 0x8000)
		D3DXMatrixRotationAxis(&rotMatrix, &m_vecUP, -m_fRotSpeed * fElapsedTime);

	// QE�� - ���ҷ��� (�� Forward ����ת)
	if (GetAsyncKeyState('Q') & 0x8000)
		D3DXMatrixRotationAxis(&rotMatrix, &m_vecForward, m_fRotSpeed * fElapsedTime);
	if (GetAsyncKeyState('E') & 0x8000)
		D3DXMatrixRotationAxis(&rotMatrix, &m_vecForward, -m_fRotSpeed * fElapsedTime);

	// Ӧ����ת���������·ɻ�������������
	D3DXVec3TransformNormal(&m_vecRight, &m_vecRight, &rotMatrix);
	D3DXVec3TransformNormal(&m_vecForward, &m_vecForward, &rotMatrix);
	// Y��Ҳ��Ҫһ����ת���Ա�������ϵ�Ĵ�ֱ
	D3DXVec3Cross(&m_vecUP, &m_vecForward, &m_vecRight);

	// --- 2. �����ƶ� ---
	// F�� - ����ǰ�� (�� Forward ��)
	if (GetAsyncKeyState('F') & 0x8000)
		m_vRelativePos += m_vecForward * m_fMoveSpeed * fElapsedTime;
	// V�� - ���ٺ���
	if (GetAsyncKeyState('V') & 0x8000)
		m_vRelativePos -= m_vecForward * m_fMoveSpeed * fElapsedTime;

	// --- 3. ���½ڵ���Ϣ ---
	// �����������λ�ú���תӦ�õ������ڵ�(CNode)��
	m_pNode->SetRelativePosition(m_vRelativePos);

	// ���ݸ��º�����������������ֶ�������ת�������ø�Node
	D3DXMATRIX matFinalRot;
	D3DXMatrixIdentity(&matFinalRot);
	matFinalRot._11 = m_vecRight.x;   matFinalRot._12 = m_vecRight.y;   matFinalRot._13 = m_vecRight.z;
	matFinalRot._21 = m_vecUP.x;      matFinalRot._22 = m_vecUP.y;      matFinalRot._23 = m_vecUP.z;
	matFinalRot._31 = m_vecForward.x; matFinalRot._32 = m_vecForward.y; matFinalRot._33 = m_vecForward.z;
	m_pNode->SetRelativeRotation(matFinalRot);
}

void CPlayer::Render()
{
	// ���û��ģ�����ݣ���ִ����Ⱦ
	if (!m_pPlayerData)
		return;

	// 1. �ӽڵ��ȡ���յ�����任����
	D3DXMATRIX worldMatrix;
	m_pNode->GetWorldTransform(worldMatrix);

	// 2. ����ģ�����ݵ���Ⱦ�������������õ��������
	m_pPlayerData->RenderMesh(worldMatrix);
}
