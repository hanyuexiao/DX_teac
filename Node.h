#pragma once
#include "myHead.h"


class CObject;
//ģ�͵��߼�����
//D3DXVECTOR3 m_vecPos;//ģ�͵�λ��
//D3DXVECTOR3 m_vecRot;//ģ�͵���ת
//D3DXVECTOR3 m_vecScale;//ģ�͵�����

//D3DXMATRIX m_matMatrix;//����ģ�͵ľ���

/// ����������֣�Ҫ����������������Ωһ��
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

/** ����任����Կռ�
	*/
enum TransformSpace
{
	/// ���ؿռ�
	TS_LOCAL,
	/// ���ռ�
	TS_PARENT,
	/// ����ռ�
	TS_WORLD
};

/** �������������
*/
D3DXVECTOR3 Vec3Mutiply(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2);


class CNode
{
protected:

	/// ���ռ�任�����λ�÷���
	D3DXVECTOR3			m_vRelativePos;
	/// ���ռ�任�������ת����
	D3DXMATRIX			m_matRelativeRotation;
	/// ���ռ�任��������ŷ���
	D3DXVECTOR3			m_vRelativeScale;

	/// ����ռ�任�����λ�÷���
	D3DXVECTOR3			m_vWorldPos;
	/// ����ռ�任�������ת����
	D3DXMATRIX			m_matWorldRotation;
	/// ����ռ�任��������ŷ���
	D3DXVECTOR3			m_vWorldScale;

	/// �Ƿ�Ӹ��ռ���±任��Ϣ
	bool				m_bUpdateFromParent;
	/// �Ƿ�̳и��ռ�任�������ת��Ϣ
	bool				m_bRotateFromParent;
	/// �Ƿ�̳и��ռ�任�����������Ϣ
	bool				m_bScaleFromParent;

	/// �������ռ�ľ���
	D3DXMATRIX			m_CachedWorldTrans;
	/// �Ƿ���Ҫ����m_CachedWorldTrans��ΪЧ�ʿ��ǣ�ֻ�е��иı�ʱ����Ҫ���¼���m_CachedWorldTrans
	bool				m_bCachedWorldTransOutOfDate;

	/// ���ڵ�
	CNode* m_pParent;
	typedef std::vector<CNode*>	ChildVector;
	/// �ӽڵ�
	ChildVector			m_Children;

	typedef std::map<TSTRING, CObject*> ObjectMap;
	/** �ڵ��Ϲҽӵ������б�
		@remarks
			���ǵ��û����ܾ���ͨ�����ֲ������塢����ɾ�����壬ʹ��map������������
			�����롢ɾ�����ٶ���Ը���
	*/
	ObjectMap		m_Objects;


public:
	CNode();
	virtual ~CNode();

	/** �õ����������ռ�ľ��������Ҫ��������ϳɾ�������������ٷ���
		@remarks
			��Ϊ������˲����㽻���ʣ���λ�á����š���ת����������ϳɾ���
			�ж�����Ϸ�ʽ����������ʹ�� Scale * Rotation * Translate ��˳��
			��֤�ڵ��ԭ�㲻�䡣
	*/
	void GetWorldTransform(D3DXMATRIX& world);

	/// ��������ڸ��ռ��λ�÷���
	void SetRelativePosition(const D3DXVECTOR3& pos);
	/// ��������ڸ��ռ�����ŷ���
	void SetRelativeScale(const D3DXVECTOR3& scale);
	/// ��������ڸ��ռ����ת����
	void SetRelativeRotation(const D3DXMATRIX& rot);

	/// ������������ռ��λ�÷���
	const D3DXVECTOR3& GetWorldPos();
	/// ������������ռ����ת����
	const D3DXMATRIX& GetWorldRotation();
	/// ������������ռ�����ŷ���
	const D3DXVECTOR3& GetWorldScale();

	/** ƽ�ƽڵ�
		@remarks
			�˴��ص��Ƶ������⣺��Բ�ͬ�ռ�Ĳ�ͬ���㹫ʽ
		@param
			d �ڵ��ƫ����
		@param
			relativeTo ָ������ĸ��ռ���ƽ�ƽڵ㣬
			Ĭ��ֵ TS_PARENT, ������Ը��ռ�ƽ�ƣ�
			��ֵΪ TS_LOCAL ʱ������Ա��ؿռ�ƽ�ƣ�
			��ֵΪ TS_WORLD ʱ�����������ռ�ƽ�ơ�
	*/
	void Translate(const D3DXVECTOR3& d, TransformSpace relativeTo = TS_PARENT);

	/** ʹ���ᣭ�ǶԵ���ʽ��ת�ڵ�
	@remarks
		�˴��ص��Ƶ������⣺��Բ�ͬ�ռ�Ĳ�ͬ���㹫ʽ
	@param
		axis ָ��������ĸ�������ת
	@param
		angle ��ת����
	@param
		relativeTo ָ������ĸ��ռ�����ת�ڵ㣬
		Ĭ��ֵ TS_LOCAL��������Ա��ؿռ���ת��
		��ֵΪ TS_PARENTʱ, ����Ը��ռ���ת��
		��ֵΪ TS_WORLD ʱ�����������ռ���ת��
	*/
	void Rotate(const D3DXVECTOR3& axis, float angle, TransformSpace relativeTo = TS_LOCAL);
	/// ��ŷ���ǵ���ʽ��ת�ڵ�
	void Yaw(float fYaw, TransformSpace relativeTo = TS_LOCAL);
	/// ��ŷ���ǵ���ʽ��ת�ڵ�
	void Pitch(float fPitch, TransformSpace relativeTo = TS_LOCAL);
	/// ��ŷ���ǵ���ʽ��ת�ڵ�
	void Roll(float fRoll, TransformSpace relativeTo = TS_LOCAL);

	/// ͨ�����ڵ�ķ���������˽ڵ��������ռ�ķ���
	void UpdateFromParent();
	/// �ݹ���ñ��ڵ�������ӽڵ��UpdateFromParent()�������������ռ�ķ���
	void Update(float fElapsedTime);

	/// �����ӽڵ�
	void addChild(CNode* child);
	/// ɾ���ӽڵ�
	void removeChild(CNode* child);

	/// ��ʶ�˽ڵ�Ϊ��Ҫ���¼���
	void NeedUpdate();

	/// �ҽ�һ������
	void AttachObject(CObject* pObject);
	/// �õ��˽ڵ�ҽӵ���������
	UINT GetNumAttachedObjects();
	/** ͨ�������õ�ĳ���ҽ�����
		@param
			uiIndex �����б��е�����
	*/
	CObject* GetAttachedObject(UINT uiIndex);
	/** ͨ�����ֵõ�ĳ���ҽ�����
		@param
			name ��������
	*/
	CObject* GetAttachedObject(TSTRING name);
	/** ȡ��ĳ������ͽڵ�֮��Ĺҽӹ�ϵ
		@param
			uiIndex �����б��е�����
	*/
	CObject* DetachAttachedObject(UINT uiIndex);
	/** ȡ��ĳ������ͽڵ�֮��Ĺҽӹ�ϵ
		@param
			pObject ����ָ��
	*/
	void DetachAttachedObject(CObject* pObject);
	/** ȡ��ĳ������ͽڵ�֮��Ĺҽӹ�ϵ
		@param
			name ��������
	*/
	CObject* DetachAttachedObject(TSTRING name);
	/** ȡ�����������뱾�ڵ��Ĺҽӹ�ϵ
	*/
	void DetachAllObjects();
	/** ������ת����
	*/
	void ResetRelativeRotation(void);
};

