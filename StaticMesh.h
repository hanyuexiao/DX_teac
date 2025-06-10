#pragma once

#include "myHead.h"
#include "Object.h"

class CModelData;
class CAxisAlignedBox;
//���泡����ĳһ�������ģ��
//�������������
class CStaticMesh :public CObject
{
public:
	CStaticMesh();
	~CStaticMesh();
	HRESULT InitObject(TiXmlElement* root){m_pRootElem=root;return S_OK;}
	HRESULT InitData();
	void Render() ;

private:
	
	TSTRING m_strModelName;
	//ģ����������
	CModelData* m_pModelData;

	CAxisAlignedBox* m_pAABB; //AABB��Χ��
	CAxisAlignedBox* m_pSelfBox;

	
};

