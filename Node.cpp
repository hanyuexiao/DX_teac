#include "Node.h"
#include "Object.h"


D3DXVECTOR3 Vec3Mutiply(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2)
{
	D3DXVECTOR3 vResult;
	vResult.x = v1.x * v2.x;
	vResult.y = v1.y * v2.y;
	vResult.z = v1.z * v2.z;
	return vResult;
}


CNode::CNode() :
	m_bCachedWorldTransOutOfDate(true),
	m_vRelativePos(0.f, 0.f, 0.f),//��Ը��׵�ƽ������
	m_vRelativeScale(1.f, 1.f, 1.f),//��Ը��׵���������
	m_vWorldPos(0.f, 0.f, 0.f),//��������ƽ��
	m_vWorldScale(1.f, 1.f, 1.f),//������������
	m_pParent(NULL),//���׽ڵ�
	m_bUpdateFromParent(false),//�Ƿ�Ӹ��ռ���±任��Ϣ
	m_bRotateFromParent(true),//�Ƿ�̳и��ռ�任�������ת��Ϣ
	m_bScaleFromParent(true)//�Ƿ�̳и��ռ�任�����������Ϣ
{
	D3DXMatrixIdentity(&m_matRelativeRotation);//��Ը��׵���ת����
	D3DXMatrixIdentity(&m_matWorldRotation);//����������ת����
	m_Children.clear();
}

CNode::~CNode()
{
	m_Children.clear();
}

//�õ����������µľ���
void CNode::GetWorldTransform(D3DXMATRIX& world)
{
	//�Ƿ���Ҫ����
	if (m_bCachedWorldTransOutOfDate)
	{
		// Scaling, Rotate, Translate
		D3DXMATRIX matScale, matRot, matTranslate;
		D3DXMatrixScaling(&matScale, m_vWorldScale.x, m_vWorldScale.y, m_vWorldScale.z);
		D3DXMatrixTranslation(&matTranslate, m_vWorldPos.x, m_vWorldPos.y, m_vWorldPos.z);
		m_CachedWorldTrans = matScale * m_matWorldRotation * matTranslate;
		m_bCachedWorldTransOutOfDate = false;
	}

	world = m_CachedWorldTrans;
}

//����λ�ã�Ȼ�����¸���������Ϣ
void CNode::SetRelativePosition(const D3DXVECTOR3& pos)
{
	m_vRelativePos = pos;
	NeedUpdate();
}

//�������ţ�Ȼ�����¸���������Ϣ
void CNode::SetRelativeScale(const D3DXVECTOR3& scale)
{
	m_vRelativeScale = scale;
	NeedUpdate();
}

//������ת��Ȼ�����¸���������Ϣ
void CNode::SetRelativeRotation(const D3DXMATRIX& rot)
{
	m_matRelativeRotation = rot;
	NeedUpdate();
}

//�õ�����ռ��µ�ƽ����Ϣ
const D3DXVECTOR3& CNode::GetWorldPos()
{
	if (m_bUpdateFromParent)
	{
		UpdateFromParent();
		m_bUpdateFromParent = false;
	}
	return m_vWorldPos;
}

//�õ�����ռ��µ���ת��Ϣ
const D3DXMATRIX& CNode::GetWorldRotation()
{
	if (m_bUpdateFromParent)
	{
		UpdateFromParent();
		m_bUpdateFromParent = false;
	}
	return m_matWorldRotation;
}

//�õ�����ռ��µ�������Ϣ
const D3DXVECTOR3& CNode::GetWorldScale()
{
	if (m_bUpdateFromParent)
	{
		UpdateFromParent();
		m_bUpdateFromParent = false;
	}
	return m_vWorldScale;
}

//���ĳ���ռ���ƽ��ʵ�֣������Ը��ռ��µ�ƽ��
void CNode::Translate(const D3DXVECTOR3& d, TransformSpace relativeTo)
{
	switch (relativeTo)
	{
		//���ؿռ�
	case TS_LOCAL:
	{
		D3DXVECTOR3 pos;
		D3DXVec3TransformCoord(&pos, &d, &m_matRelativeRotation);
		m_vRelativePos += pos;
	}
	break;
	case TS_PARENT:
		m_vRelativePos += d;
		break;
	case TS_WORLD:
	{
		if (m_pParent == NULL)
		{
			m_vRelativePos += d;
		}
		else
		{
			D3DXVECTOR3 deltaWorld;
			D3DXMATRIX parentRotInverse;
			D3DXMatrixInverse(&parentRotInverse, NULL, &m_pParent->GetWorldRotation());
			D3DXVec3TransformCoord(&deltaWorld, &d, &parentRotInverse);
			const D3DXVECTOR3& parentScale = m_pParent->GetWorldScale();
			deltaWorld = Vec3Mutiply(deltaWorld, D3DXVECTOR3(1.f / parentScale.x, 1.f / parentScale.y, 1.f / parentScale.z));

			m_vRelativePos += deltaWorld;
		}
	}
	break;
	}
	NeedUpdate();
}

void CNode::Rotate(const D3DXVECTOR3& axis, float angle, TransformSpace relativeTo)
{
	D3DXMATRIX matRot;
	D3DXMatrixRotationAxis(&matRot, &axis, angle);

	switch (relativeTo)
	{
	case TS_LOCAL:
		m_matRelativeRotation = matRot * m_matRelativeRotation;
		break;
	case TS_PARENT:
		m_matRelativeRotation = m_matRelativeRotation * matRot;
		break;
	case TS_WORLD:
		const D3DXMATRIX& derivedRot = GetWorldRotation();
		D3DXMATRIX derivedRotInverse;
		D3DXMatrixInverse(&derivedRotInverse, NULL, &derivedRot);
		m_matRelativeRotation = derivedRot * matRot * derivedRotInverse * m_matRelativeRotation;
		break;
	}
	NeedUpdate();

}
void CNode::Yaw(float fYaw, TransformSpace relativeTo)
{
	Rotate(D3DXVECTOR3(0.f, 1.f, 0.f), fYaw, relativeTo);
	NeedUpdate();
}
void CNode::Pitch(float fPitch, TransformSpace relativeTo)
{
	Rotate(D3DXVECTOR3(1.f, 0.f, 0.f), fPitch, relativeTo);
	NeedUpdate();
}
void CNode::Roll(float fRoll, TransformSpace relativeTo)
{
	Rotate(D3DXVECTOR3(0.f, 0.f, 1.f), fRoll, relativeTo);
	NeedUpdate();
}

//�޸���������
void CNode::UpdateFromParent()
{
	if (m_pParent)
	{
		D3DXMATRIX parentOrientation;
		D3DXMatrixIdentity(&parentOrientation);
		D3DXVECTOR3	parentScale(1.f, 1.f, 1.f);
		// orientation
		if (m_bRotateFromParent)
		{
			parentOrientation = m_pParent->GetWorldRotation();
			m_matWorldRotation = m_matRelativeRotation * parentOrientation;
		}
		else
		{
			m_matWorldRotation = m_matRelativeRotation;
		}

		// scale
		if (m_bScaleFromParent)
		{
			parentScale = m_pParent->GetWorldScale();
			m_vWorldScale = Vec3Mutiply(parentScale, m_vRelativeScale);
		}
		else
		{
			m_vWorldScale = m_vRelativeScale;
		}

		// position
		if (m_bRotateFromParent || m_bScaleFromParent)
		{
			D3DXVECTOR3 v;
			v=Vec3Mutiply(parentScale, m_vRelativePos);
			D3DXVec3TransformCoord(&m_vWorldPos, &v,
				&parentOrientation);
		}
		else
		{
			m_vWorldPos = m_vRelativePos;
		}
		m_vWorldPos += m_pParent->GetWorldPos();
	}
	else
	{
		m_vWorldPos = m_vRelativePos;
		m_matWorldRotation = m_matRelativeRotation;
		m_vWorldScale = m_vRelativeScale;
	}
	//�޸�������ռ��µ���Ϣ֮����Ҫ���������������
	m_bCachedWorldTransOutOfDate = true;
}

void CNode::Update(float fElapsedTime)
{
	UpdateFromParent();

	ChildVector::iterator i, iEnd;
	i = m_Children.begin();
	iEnd = m_Children.end();

	for (; i != iEnd; ++i)
	{
		(*i)->Update(fElapsedTime);
	}
}


void CNode::addChild(CNode* child)
{
	assert(child);
	child->m_pParent = this;
	m_Children.push_back(child);
}

void CNode::removeChild(CNode* child)
{
	assert(child);
	ChildVector::iterator i, iEnd;
	i = m_Children.begin();
	iEnd = m_Children.end();

	for (; i != iEnd; ++i)
	{
		if (*i == child)
		{
			m_Children.erase(i);
			break;
		}
	}
}

void CNode::NeedUpdate()
{
	m_bCachedWorldTransOutOfDate = true;
	m_bUpdateFromParent = true;
}

void CNode::AttachObject(CObject* pObject)
{
	if (pObject->IsAttached())
		return;

	m_Objects.insert(ObjectMap::value_type(pObject->GetName(), pObject));
	pObject->SetNode(this);
}
UINT CNode::GetNumAttachedObjects()
{
	return static_cast<UINT>(m_Objects.size());
}
CObject* CNode::GetAttachedObject(UINT uiIndex)
{
	if (uiIndex < m_Objects.size())
	{
		ObjectMap::iterator iter = m_Objects.begin();

		while (uiIndex--) iter++;

		return iter->second;
	}
	return NULL;
}
CObject* CNode::GetAttachedObject(TSTRING name)
{
	ObjectMap::iterator iter = m_Objects.find(name);
	if (iter == m_Objects.end())
	{
		return NULL;
	}
	return iter->second;
}
CObject* CNode::DetachAttachedObject(UINT uiIndex)
{
	if (uiIndex < m_Objects.size())
	{
		ObjectMap::iterator iter = m_Objects.begin();
		while (uiIndex--) iter++;

		CObject* pObj = iter->second;
		m_Objects.erase(iter);
		if (pObj)
		{
			pObj->SetNode(NULL);
		}

		return pObj;
	}

	return NULL;
}
void CNode::DetachAttachedObject(CObject* pObject)
{
	ObjectMap::iterator itBegin = m_Objects.begin();
	ObjectMap::iterator itEnd = m_Objects.end();

	CObject* pObj = NULL;
	for (; itBegin != itEnd; ++itBegin)
	{
		pObj = itBegin->second;
		if (pObj == pObject)
		{
			m_Objects.erase(itBegin);
			if (pObj)
			{
				pObj->SetNode(NULL);
			}
			break;
		}
	}
}
CObject* CNode::DetachAttachedObject(TSTRING name)
{
	ObjectMap::iterator iter = m_Objects.find(name);
	if (iter == m_Objects.end())
	{
		return NULL;
	}
	CObject* pObj = iter->second;
	m_Objects.erase(iter);
	if (pObj)
	{
		pObj->SetNode(NULL);
	}

	return pObj;
}
void CNode::DetachAllObjects()
{
	ObjectMap::iterator itBegin = m_Objects.begin();
	ObjectMap::iterator itEnd = m_Objects.end();

	CObject* pObj = NULL;
	for (; itBegin != itEnd; ++itBegin)
	{
		pObj = itBegin->second;
		if (pObj)
		{
			pObj->SetNode(NULL);
		}
	}

	m_Objects.clear();
}

void CNode::ResetRelativeRotation(void)
{
	D3DXMatrixIdentity(&m_matRelativeRotation);
}
