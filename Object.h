#pragma once

#include "myHead.h"

#include "Node.h"

//一切逻辑对象都继承该类
class CObject
{

public:
	CObject(void);
	virtual ~CObject(void);
	//加载脚本文件
	HRESULT InitObject(TSTRING name);
	//加载具体的数据
	virtual HRESULT InitData();
	/// 得到物体名字
	TSTRING  GetName();
	/// 判断此物体是否挂接在节点上
	bool IsAttached();
	/// 设置此物体挂接的节点
	void SetNode(CNode* pNode);
	/// 得到此物体挂接的节点
	CNode* GetNode();

	virtual void Update(float fElapsedTime) {};
	/** 渲染不透明部分
		@remarks
			为了实现正确的渲染结果，应该先渲染不透明物体再渲染透明物体。
			这里做了简单的处理，将不透明物体和透明物体的渲染分开。
	*/
	virtual void Render() {};
	/** 渲染透明部分
	*/
	virtual void RenderWithAlpha() {};
protected:
	/// 此物体挂接到的节点

	TiXmlElement* m_pRootElem = NULL;

	CNode* m_pNode;//节点类保存的是逻辑数据
	/// 此物体的名字，要求在所有物体中是惟一的
	TSTRING m_strObjectName;
};

//月亮的动画
	//月亮自身的动画*地球的动画
					//地球的自身动画*太阳的动画
