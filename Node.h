#pragma once
#include "myHead.h"


class CObject;
//模型的逻辑数据
//D3DXVECTOR3 m_vecPos;//模型的位置
//D3DXVECTOR3 m_vecRot;//模型的旋转
//D3DXVECTOR3 m_vecScale;//模型的缩放

//D3DXMATRIX m_matMatrix;//绘制模型的矩阵

/// 此物体的名字，要求在所有物体中是惟一的
//TSTRING m_Name;

//DECLARE_MEMBER(D3DXVECTOR3, vecPos, ObjectPos);
//DECLARE_MEMBER(D3DXVECTOR3, vecRot, ObjectRot);
//DECLARE_MEMBER(D3DXVECTOR3, vecScale, ObjectScale);
//
//DECLARE_MEMBER(D3DXVECTOR3, vecForward, ForwardDir);
//DECLARE_MEMBER(D3DXVECTOR3, vecRight, RightDir);
//DECLARE_MEMBER(D3DXVECTOR3, vecUP, UPDir);
//
//DECLARE_MEMBER(D3DXMATRIX, matMatrix, ObjectMatrix);
//
//DECLARE_MEMBER(TSTRING, strName, ObjectName);
//
//DECLARE_MEMBER(BOOL, bIsAnim, IsAnim);

/** 坐标变换的相对空间
	*/
enum TransformSpace
{
	/// 本地空间
	TS_LOCAL,
	/// 父空间
	TS_PARENT,
	/// 世界空间
	TS_WORLD
};

/** 向量各分量相乘
*/
D3DXVECTOR3 Vec3Mutiply(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2);


class CNode
{
protected:

	/// 父空间变换矩阵的位置分量
	D3DXVECTOR3			m_vRelativePos;
	/// 父空间变换矩阵的旋转分量
	D3DXMATRIX			m_matRelativeRotation;
	/// 父空间变换矩阵的缩放分量
	D3DXVECTOR3			m_vRelativeScale;

	/// 世界空间变换矩阵的位置分量
	D3DXVECTOR3			m_vWorldPos;
	/// 世界空间变换矩阵的旋转分量
	D3DXMATRIX			m_matWorldRotation;
	/// 世界空间变换矩阵的缩放分量
	D3DXVECTOR3			m_vWorldScale;

	/// 是否从父空间更新变换信息
	bool				m_bUpdateFromParent;
	/// 是否继承父空间变换矩阵的旋转信息
	bool				m_bRotateFromParent;
	/// 是否继承父空间变换矩阵的缩放信息
	bool				m_bScaleFromParent;

	/// 相对世界空间的矩阵
	D3DXMATRIX			m_CachedWorldTrans;
	/// 是否需要计算m_CachedWorldTrans。为效率考虑，只有当有改变时才需要重新计算m_CachedWorldTrans
	bool				m_bCachedWorldTransOutOfDate;

	/// 父节点
	CNode* m_pParent;
	typedef std::vector<CNode*>	ChildVector;
	/// 子节点
	ChildVector			m_Children;

	typedef std::map<TSTRING, CObject*> ObjectMap;
	/** 节点上挂接的物体列表，
		@remarks
			考虑到用户可能经常通过名字查找物体、插入删除物体，使用map容器进行搜索
			、插入、删除的速度相对更快
	*/
	ObjectMap		m_Objects;


public:
	CNode();
	virtual ~CNode();

	/** 得到相对于世界空间的矩阵，如果需要将分量组合成矩阵则做计算后再返回
		@remarks
			因为矩阵相乘不满足交换率，将位置、缩放、旋转三个分量组合成矩阵
			有多种组合方式，这里我们使用 Scale * Rotation * Translate 的顺序，
			保证节点的原点不变。
	*/
	void GetWorldTransform(D3DXMATRIX& world);

	/// 设置相对于父空间的位置分量
	void SetRelativePosition(const D3DXVECTOR3& pos);
	/// 设置相对于父空间的缩放分量
	void SetRelativeScale(const D3DXVECTOR3& scale);
	/// 设置相对于父空间的旋转分量
	void SetRelativeRotation(const D3DXMATRIX& rot);

	/// 获得相对于世界空间的位置分量
	const D3DXVECTOR3& GetWorldPos();
	/// 获得相对于世界空间的旋转分量
	const D3DXMATRIX& GetWorldRotation();
	/// 获得相对于世界空间的缩放分量
	const D3DXVECTOR3& GetWorldScale();

	/** 平移节点
		@remarks
			此处重点推导并讲解：相对不同空间的不同计算公式
		@param
			d 节点的偏移量
		@param
			relativeTo 指出相对哪个空间来平移节点，
			默认值 TS_PARENT, 代表相对父空间平移，
			当值为 TS_LOCAL 时，则相对本地空间平移，
			当值为 TS_WORLD 时，则相对世界空间平移。
	*/
	void Translate(const D3DXVECTOR3& d, TransformSpace relativeTo = TS_PARENT);

	/** 使用轴－角对的形式旋转节点
	@remarks
		此处重点推导并讲解：相对不同空间的不同计算公式
	@param
		axis 指定相对于哪个轴做旋转
	@param
		angle 旋转弧度
	@param
		relativeTo 指出相对哪个空间来旋转节点，
		默认值 TS_LOCAL，代表相对本地空间旋转，
		当值为 TS_PARENT时, 则相对父空间旋转，
		当值为 TS_WORLD 时，则相对世界空间旋转。
	*/
	void Rotate(const D3DXVECTOR3& axis, float angle, TransformSpace relativeTo = TS_LOCAL);
	/// 以欧拉角的形式旋转节点
	void Yaw(float fYaw, TransformSpace relativeTo = TS_LOCAL);
	/// 以欧拉角的形式旋转节点
	void Pitch(float fPitch, TransformSpace relativeTo = TS_LOCAL);
	/// 以欧拉角的形式旋转节点
	void Roll(float fRoll, TransformSpace relativeTo = TS_LOCAL);

	/// 通过父节点的分量，计算此节点相对世界空间的分量
	void UpdateFromParent();
	/// 递归调用本节点和所有子节点的UpdateFromParent()，计算相对世界空间的分量
	void Update(float fElapsedTime);

	/// 增加子节点
	void addChild(CNode* child);
	/// 删除子节点
	void removeChild(CNode* child);

	/// 标识此节点为需要重新计算
	void NeedUpdate();

	/// 挂接一个物体
	void AttachObject(CObject* pObject);
	/// 得到此节点挂接的物体数量
	UINT GetNumAttachedObjects();
	/** 通过索引得到某个挂接物体
		@param
			uiIndex 物体列表中的索引
	*/
	CObject* GetAttachedObject(UINT uiIndex);
	/** 通过名字得到某个挂接物体
		@param
			name 物体名字
	*/
	CObject* GetAttachedObject(TSTRING name);
	/** 取消某个物体和节点之间的挂接关系
		@param
			uiIndex 物体列表中的索引
	*/
	CObject* DetachAttachedObject(UINT uiIndex);
	/** 取消某个物体和节点之间的挂接关系
		@param
			pObject 物体指针
	*/
	void DetachAttachedObject(CObject* pObject);
	/** 取消某个物体和节点之间的挂接关系
		@param
			name 物体名字
	*/
	CObject* DetachAttachedObject(TSTRING name);
	/** 取消所有物体与本节点间的挂接关系
	*/
	void DetachAllObjects();
	/** 重置旋转矩阵
	*/
	void ResetRelativeRotation(void);
};

