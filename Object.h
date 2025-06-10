#pragma once

#include "myHead.h"

#include "Node.h"

//һ���߼����󶼼̳и���
class CObject
{

public:
	CObject(void);
	virtual ~CObject(void);
	//���ؽű��ļ�
	HRESULT InitObject(TSTRING name);
	//���ؾ��������
	virtual HRESULT InitData();
	/// �õ���������
	TSTRING  GetName();
	/// �жϴ������Ƿ�ҽ��ڽڵ���
	bool IsAttached();
	/// ���ô�����ҽӵĽڵ�
	void SetNode(CNode* pNode);
	/// �õ�������ҽӵĽڵ�
	CNode* GetNode();

	virtual void Update(float fElapsedTime) {};
	/** ��Ⱦ��͸������
		@remarks
			Ϊ��ʵ����ȷ����Ⱦ�����Ӧ������Ⱦ��͸����������Ⱦ͸�����塣
			�������˼򵥵Ĵ�������͸�������͸���������Ⱦ�ֿ���
	*/
	virtual void Render() {};
	/** ��Ⱦ͸������
	*/
	virtual void RenderWithAlpha() {};
protected:
	/// ������ҽӵ��Ľڵ�

	TiXmlElement* m_pRootElem = NULL;

	CNode* m_pNode;//�ڵ��ౣ������߼�����
	/// ����������֣�Ҫ����������������Ωһ��
	TSTRING m_strObjectName;
};

//�����Ķ���
	//��������Ķ���*����Ķ���
					//�����������*̫���Ķ���
