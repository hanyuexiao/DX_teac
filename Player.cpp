#include "Player.h"
#include "StaticMesh.h"
#include "ModelManager.h"

CPlayer::CPlayer(void) :
	m_pPlayerMesh(NULL),
	m_vRelativePos(0.0f, 0.0f, 0.0f),
	m_enMotionType(MT_STAY),
	m_fMoveSpeed(0.0f),
	m_fRotAngleY(0.0f)
{
	

}

CPlayer::~CPlayer(void)
{
}

HRESULT CPlayer::InitData()
{
	
	return S_OK;
}

void CPlayer::Render()
{
	

}

void CPlayer::Update(float fElapsedTime)
{
	
}

void CPlayer::ChangeMotionType(enMotionType enType)
{
	m_enMotionType = enType;

	switch (m_enMotionType)
	{
	case MT_STAY:
		
		break;

	case MT_WALK:
		
		break;

	case MT_ATTACK:
		
		break;

	case MT_BEHIT:
		
		break;

	case MT_DIEING:
		
		break;

	case MT_DEAD:
		
		break;
	}
}

void CPlayer::ProcessKey(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == 'A')
			m_fRotAngleY -= -0.05;
		if (wParam == 'D')
			m_fRotAngleY += 0.05;
		if (wParam == 'W')
		{
			ChangeMotionType(MT_WALK);
			m_fMoveSpeed = 100;
		}
		if (wParam == 'S')
		{
			ChangeMotionType(MT_WALK);
			m_fMoveSpeed = -100;
		}
		break;
	case WM_KEYUP:
		if (wParam == 'W' || wParam == 'S')
			m_fMoveSpeed = 0;
	}
}