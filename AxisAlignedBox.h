/********************************************************************


purpose:	ʵ��AABB(Axis Aligned Bounding Box,�����󶨺�)������
			������ײ���
*********************************************************************/

#pragma once
#include "myHead.h"



struct BOXVERTEX
{
	D3DXVECTOR3 pos;
	DWORD color;//��͸����ɫ
};

#define BOXVERTEXFVF (D3DFVF_XYZ|D3DFVF_DIFFUSE)
/** �����Χ�У�AABB�ࡣ
*/
class CAxisAlignedBox//�����İ�Χ��
{
protected:
	D3DXVECTOR3 mMinimum;//��������ĺ�����С��
	D3DXVECTOR3 mMaximum;//��������ĺ�������

	D3DXVECTOR3 mMinimumWorld;//��������ĺ�����С��
	D3DXVECTOR3 mMaximumWorld;//��������ĺ�������
	bool mNull;

	D3DXVECTOR3 mCorners[8];//���������8����
	D3DXVECTOR3 mCornersWorld[8];
	void _UpdateCorners(void);
	void _UpdateCornersWorld(void);

public:
	CAxisAlignedBox();
	CAxisAlignedBox( const D3DXVECTOR3& min, const D3DXVECTOR3& max );
	CAxisAlignedBox(
		float mx, float my, float mz,
		float Mx, float My, float Mz );

	/** �õ�������ֵ��С�Ķ��㡣
	*/
	D3DXVECTOR3 GetMinimum(void) const;
	/** �õ�������ֵ���Ķ��㡣
	*/
	D3DXVECTOR3 GetMaximum(void) const;
	/** ���ú�����ֵ��С�Ķ��㡣
	*/
	  void SetMinimum( const D3DXVECTOR3& vec );
	void SetMinimum( float x, float y, float z );
	/** ���ú�����ֵ���Ķ��㡣
	*/
	void SetMaximum( const D3DXVECTOR3& vec );
	void SetMaximum( float x, float y, float z );
	/** ���ú�����ֵ��С�����Ķ��㡣
	*/
	void SetExtents( const D3DXVECTOR3& min, const D3DXVECTOR3& max );
	void SetExtentsWorld( const D3DXVECTOR3& min, const D3DXVECTOR3& max );
	void SetExtents(
		float mx, float my, float mz,
		float Mx, float My, float Mz );
	const D3DXVECTOR3* GetAllCornersBox(void) const;
	
	

	/** ���غ��ӵ�8�����㣬��������ͷ��������ӵ���ײ��
	@remarks
		��ͼ��ʾ��8�������λ�ã�2��5�ֱ���mMinimum��mMaximum��
	<pre>
				   4-----5
				  /|    /|
				 / |   / |
				0-----1  |
				|  6--|--7
				| /   | /
				|/    |/
				2-----3
	</pre>
	*/
	const D3DXVECTOR3* GetAllCorners(void) const;
	/** �������AABB��Ͻ���ǰ��AABB�ڡ�
	*/
	void Merge( const CAxisAlignedBox& rhs );

	/** �����ṩ�ľ���任AABB
		@remarks
			��������ת��ʱ����AABB��Ҫ���¼��㡣
	*/
	void Transform( const D3DXMATRIX &matrix );


	void TransformBox(const D3DXMATRIX& matrix);
	/** �պ��ӣ�û�����
	*/
	void SetNull();
	/** �ж��Ƿ�Ϊ�պ���
	*/
	bool IsNull(void) const;
	/** �ж��Ƿ����һ��AABB�ཻ
	*/
	bool Intersects(const CAxisAlignedBox& b2) const;

	//��Ϸ��������ƣ���������ʹ��
	bool m_bRenderAABB = false; // �Ƿ���ȾAABB
	bool m_bRenderBox = false; // �Ƿ���Ⱦ��Χ��
	void RenderAABB();
	void RenderBox();

	LPD3DXMESH m_pAABBMesh = nullptr; // AABB����
	LPD3DXMESH m_pBoxMesh = nullptr; // ��Χ������
};
