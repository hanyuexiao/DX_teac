#include "Object.h"


CObject::CObject(void) :
	m_pNode(NULL)
{
	m_pNode = new CNode;
}

CObject::~CObject(void)
{
}

HRESULT CObject::InitObject(TSTRING name)
{
	string ansiName;
#ifdef _UNICODE
	ansiName = wide_char_to_multi_byte(name, -1);
#else
	ansiName = name;
#endif
	TiXmlDocument doc(ansiName.c_str());
	if (!doc.LoadFile())
		return E_FAIL;
	TiXmlDocument* pDoc = &doc;	
	m_pRootElem = pDoc->FirstChildElement();	

	InitData();
}

HRESULT CObject::InitData()
{
	return S_OK;
}

TSTRING CObject::GetName()
{
	return m_strObjectName;
}

bool CObject::IsAttached()
{
	return (m_pNode != NULL);
}

void CObject::SetNode(CNode* pNode)
{
	m_pNode = pNode;
}

CNode* CObject::GetNode()
{
	return m_pNode;
}
