#pragma once

#include "myHead.h"
#include "Object.h"

class CModelData;
class CAxisAlignedBox;
//保存场景中某一个种类的模型
//该类仅用来绘制
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
	//模型自身数据
	CModelData* m_pModelData;

	CAxisAlignedBox* m_pAABB; //AABB包围盒
	CAxisAlignedBox* m_pSelfBox;

	
};

