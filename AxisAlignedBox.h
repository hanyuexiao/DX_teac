/********************************************************************


purpose:	实现AABB(Axis Aligned Bounding Box,轴对齐绑定盒)，用来
			处理碰撞检测
*********************************************************************/

#pragma once
#include "myHead.h"



struct BOXVERTEX
{
	D3DXVECTOR3 pos;
	DWORD color;//半透明颜色
};

#define BOXVERTEXFVF (D3DFVF_XYZ|D3DFVF_DIFFUSE)
/** 自身包围盒，AABB类。
*/
class CAxisAlignedBox//轴对齐的包围盒
{
protected:
	D3DXVECTOR3 mMinimum;//本地坐标的盒子最小点
	D3DXVECTOR3 mMaximum;//本地坐标的盒子最大点

	D3DXVECTOR3 mMinimumWorld;//世界坐标的盒子最小点
	D3DXVECTOR3 mMaximumWorld;//世界坐标的盒子最大点
	bool mNull;

	D3DXVECTOR3 mCorners[8];//本地坐标的8个点
	D3DXVECTOR3 mCornersWorld[8];
	void _UpdateCorners(void);
	void _UpdateCornersWorld(void);

public:
	CAxisAlignedBox();
	CAxisAlignedBox( const D3DXVECTOR3& min, const D3DXVECTOR3& max );
	CAxisAlignedBox(
		float mx, float my, float mz,
		float Mx, float My, float Mz );

	/** 得到盒子上值最小的顶点。
	*/
	D3DXVECTOR3 GetMinimum(void) const;
	/** 得到盒子上值最大的顶点。
	*/
	D3DXVECTOR3 GetMaximum(void) const;
	/** 设置盒子上值最小的顶点。
	*/
	  void SetMinimum( const D3DXVECTOR3& vec );
	void SetMinimum( float x, float y, float z );
	/** 设置盒子上值最大的顶点。
	*/
	void SetMaximum( const D3DXVECTOR3& vec );
	void SetMaximum( float x, float y, float z );
	/** 设置盒子上值最小和最大的顶点。
	*/
	void SetExtents( const D3DXVECTOR3& min, const D3DXVECTOR3& max );
	void SetExtentsWorld( const D3DXVECTOR3& min, const D3DXVECTOR3& max );
	void SetExtents(
		float mx, float my, float mz,
		float Mx, float My, float Mz );
	const D3DXVECTOR3* GetAllCornersBox(void) const;
	
	

	/** 返回盒子的8个顶点，用来计算和非轴对齐盒子的碰撞。
	@remarks
		下图显示了8个顶点的位置，2和5分别是mMinimum、mMaximum。
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
	/** 将传入的AABB组合进当前的AABB内。
	*/
	void Merge( const CAxisAlignedBox& rhs );

	/** 根据提供的矩阵变换AABB
		@remarks
			当物体旋转的时候，其AABB需要重新计算。
	*/
	void Transform( const D3DXMATRIX &matrix );


	void TransformBox(const D3DXMATRIX& matrix);
	/** 空盒子，没有体积
	*/
	void SetNull();
	/** 判断是否为空盒子
	*/
	bool IsNull(void) const;
	/** 判断是否和另一个AABB相交
	*/
	bool Intersects(const CAxisAlignedBox& b2) const;

	//游戏中无需绘制，仅仅测试使用
	bool m_bRenderAABB = false; // 是否渲染AABB
	bool m_bRenderBox = false; // 是否渲染包围盒
	void RenderAABB();
	void RenderBox();

	LPD3DXMESH m_pAABBMesh = nullptr; // AABB网格
	LPD3DXMESH m_pBoxMesh = nullptr; // 包围盒网格
};
