
#include "axisalignedbox.h"

void CAxisAlignedBox::_UpdateCorners(void)
{
	mCorners[0].x =	mMinimum.x; mCorners[0].y = mMaximum.y; mCorners[0].z = mMinimum.z;
	mCorners[1].x = mMaximum.x; mCorners[1].y = mMaximum.y; mCorners[1].z = mMinimum.z; 
	mCorners[2] = mMinimum;
	mCorners[3].x = mMaximum.x; mCorners[3].y = mMinimum.y; mCorners[3].z = mMinimum.z;            

	
	mCorners[4].x = mMinimum.x; mCorners[4].y = mMaximum.y; mCorners[4].z = mMaximum.z;
	mCorners[5] = mMaximum;
	mCorners[6].x = mMinimum.x; mCorners[6].y = mMinimum.y; mCorners[6].z = mMaximum.z;
	mCorners[7].x = mMaximum.x; mCorners[7].y = mMinimum.y; mCorners[7].z = mMaximum.z;            
}    

void CAxisAlignedBox::_UpdateCornersWorld(void)
{
	mCornersWorld[0].x = mMinimumWorld.x; mCornersWorld[0].y = mMaximumWorld.y; mCornersWorld[0].z = mMinimumWorld.z;
	mCornersWorld[1].x = mMaximumWorld.x; mCornersWorld[1].y = mMaximumWorld.y; mCornersWorld[1].z = mMinimumWorld.z;
	mCornersWorld[2] = mMinimumWorld;
	mCornersWorld[3].x = mMaximumWorld.x; mCornersWorld[3].y = mMinimumWorld.y; mCornersWorld[3].z = mMinimumWorld.z;


	mCornersWorld[5].x = mMinimumWorld.x; mCornersWorld[5].y = mMaximumWorld.y; mCornersWorld[5].z = mMaximumWorld.z;
	mCornersWorld[5] = mMaximumWorld;
	mCornersWorld[6].x = mMinimumWorld.x; mCornersWorld[6].y = mMinimumWorld.y; mCornersWorld[6].z = mMaximumWorld.z;
	mCornersWorld[7].x = mMaximumWorld.x; mCornersWorld[7].y = mMinimumWorld.y; mCornersWorld[7].z = mMaximumWorld.z;
}

CAxisAlignedBox::CAxisAlignedBox()
{
	SetMinimum( -0.5, -0.5, -0.5 );
	SetMaximum( 0.5, 0.5, 0.5 );
	mNull = true;
}

CAxisAlignedBox::CAxisAlignedBox( const D3DXVECTOR3& min, const D3DXVECTOR3& max )
{
	SetExtents( min, max );
}

CAxisAlignedBox::CAxisAlignedBox(
				float mx, float my, float mz,
				float Mx, float My, float Mz )
{
	SetExtents( mx, my, mz, Mx, My, Mz );
}


D3DXVECTOR3 CAxisAlignedBox::GetMinimum(void) const
{ 
	return mMinimum; 
}

D3DXVECTOR3 CAxisAlignedBox::GetMaximum(void) const
{ 
	return mMaximum;
}

void CAxisAlignedBox::SetMinimum( const D3DXVECTOR3& vec )
{
	mNull = false;
	mMinimum = vec;
	_UpdateCorners();
}

void CAxisAlignedBox::SetMinimum( float x, float y, float z )
{
	mNull = false;
	mMinimum.x = x;
	mMinimum.y = y;
	mMinimum.z = z;
	_UpdateCorners();
}

void CAxisAlignedBox::SetMaximum( const D3DXVECTOR3& vec )
{
	mNull = false;
	mMaximum = vec;
	_UpdateCorners();
}

void CAxisAlignedBox::SetMaximum( float x, float y, float z )
{
	mNull = false;
	mMaximum.x = x;
	mMaximum.y = y;
	mMaximum.z = z;
	_UpdateCorners();
}

void CAxisAlignedBox::SetExtentsWorld( const D3DXVECTOR3& min, const D3DXVECTOR3& max )
{
	mNull = false;
	mMinimumWorld = min;
	mMaximumWorld = max;
	_UpdateCornersWorld();
}

void CAxisAlignedBox::SetExtents(const D3DXVECTOR3& min, const D3DXVECTOR3& max)
{
	mNull = false;
	mMinimum = min;
	mMaximum = max;
	_UpdateCorners();
}

void CAxisAlignedBox::SetExtents( float mx, float my, float mz,
				 float Mx, float My, float Mz )
{
	mNull = false;

	mMinimum.x = mx;
	mMinimum.y = my;
	mMinimum.z = mz;

	mMaximum.x = Mx;
	mMaximum.y = My;
	mMaximum.z = Mz;

	_UpdateCorners();
}


const D3DXVECTOR3* CAxisAlignedBox::GetAllCorners(void) const
{
	return (const D3DXVECTOR3*)mCorners;
}

const D3DXVECTOR3* CAxisAlignedBox::GetAllCornersBox(void) const
{
	return (const D3DXVECTOR3*)mCornersWorld;
}

void makeFloor( D3DXVECTOR3& src, const D3DXVECTOR3& cmp )
{
	if( cmp.x < src.x ) src.x = cmp.x;
	if( cmp.y < src.y ) src.y = cmp.y;
	if( cmp.z < src.z ) src.z = cmp.z;
}

void makeCeil( D3DXVECTOR3& src, const D3DXVECTOR3& cmp )
{
	if( cmp.x > src.x ) src.x = cmp.x;
	if( cmp.y > src.y ) src.y = cmp.y;
	if( cmp.z > src.z ) src.z = cmp.z;
}

void CAxisAlignedBox::Merge( const CAxisAlignedBox& rhs )
{
	// Do nothing if rhs null
	if (rhs.mNull)
	{
		return;
	}
	// Otherwise if current null, just take rhs
	else if (mNull)
	{
		SetExtents(rhs.mMinimum, rhs.mMaximum);
	}
	// Otherwise merge
	else
	{
		D3DXVECTOR3 min = mMinimum;
		D3DXVECTOR3 max = mMaximum;
		makeCeil(max, rhs.mMaximum);
		makeFloor(min, rhs.mMinimum);

		SetExtents(min, max);
	}

}

/** Transforms the box according to the matrix supplied.
@remarks
By calling this method you get the axis-aligned box which
surrounds the transformed version of this box. Therefore each
corner of the box is transformed by the matrix, then the
extents are mapped back onto the axes to produce another
AABB. Useful when you have a local AABB for an object which
is then transformed.
*/
//计算轴对齐的包围盒的8个点，在世界空间下的坐标
void CAxisAlignedBox::Transform( const D3DXMATRIX &matrix )
{
if( mNull )
	return;

	D3DXVECTOR3 min, max, temp;
	bool first = true;
	int i;

	for( i = 0; i < 8; ++i )
	{
		D3DXVec3TransformCoord( &temp, &mCorners[i], &matrix );
		if( first || temp.x > max.x )
			max.x = temp.x;
		if( first || temp.y > max.y )
			max.y = temp.y;
		if( first || temp.z > max.z )
			max.z = temp.z;
		if( first || temp.x < min.x )
			min.x = temp.x;
		if( first || temp.y < min.y )
			min.y = temp.y;
		if( first || temp.z < min.z )
			min.z = temp.z;

		first = false;
	}

	SetExtentsWorld( min,max );
	

}

//计算自身包围盒的8个点在世界空间下的坐标
void CAxisAlignedBox::TransformBox(const D3DXMATRIX& matrix)
{
	if (mNull)
		return;
	int i;

	for (i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&mCornersWorld[i], &mCorners[i], &matrix);
	}
}

void CAxisAlignedBox::SetNull()
{
	mNull = true;
}

bool CAxisAlignedBox::IsNull(void) const
{
	return mNull;
}

bool CAxisAlignedBox::Intersects(const CAxisAlignedBox& b2) const
{
	// Use up to 6 separating planes
	if (mMaximumWorld.x < b2.mMinimumWorld.x)
		return false;
	if (mMaximumWorld.y < b2.mMinimumWorld.y)
		return false;
	if (mMaximumWorld.z < b2.mMinimumWorld.z)
		return false;

	if (mMinimumWorld.x > b2.mMaximumWorld.x)
		return false;
	if (mMinimumWorld.y > b2.mMaximumWorld.y)
		return false;
	if (mMinimumWorld.z > b2.mMaximumWorld.z)
		return false;

	// otherwise, must be intersecting
	return true;

}

void CAxisAlignedBox::RenderAABB()
{
	if (m_bRenderAABB)
	{
		if (!m_pAABBMesh)
		{
			D3DXCreateMeshFVF(12, 24, D3DXMESH_MANAGED, BOXVERTEXFVF, CGraphic::GetSingleObjPtr()->m_pDevice, &m_pAABBMesh);

			BOXVERTEX* pVertexs;
			m_pAABBMesh->LockVertexBuffer(0, (void**)&pVertexs);
			//24个点赋值
			//前平面4个点
			pVertexs[0] = { mCornersWorld[0],0x88ff0000 };
			pVertexs[1] = { mCornersWorld[1],0x88ff0000 };
			pVertexs[2] = { mCornersWorld[2],0x88ff0000 };
			pVertexs[3] = { mCornersWorld[3],0x88ff0000 };

			//左平面4个点
			pVertexs[4] = { mCornersWorld[4],0x880000ff };
			pVertexs[5] = { mCornersWorld[0],0x880000ff };
			pVertexs[6] = { mCornersWorld[6],0x880000ff };
			pVertexs[7] = { mCornersWorld[2],0x880000ff };

			//右平面4个点
			pVertexs[8] = { mCornersWorld[1],0x880000ff };
			pVertexs[9] = { mCornersWorld[5],0x880000ff };
			pVertexs[10] = { mCornersWorld[3],0x880000ff };
			pVertexs[11] = { mCornersWorld[7],0x880000ff };

			//上平面4个点
			pVertexs[12] = { mCornersWorld[4],0x8800ff00 };
			pVertexs[13] = { mCornersWorld[5],0x8800ff00 };
			pVertexs[14] = { mCornersWorld[0],0x8800ff00 };
			pVertexs[15] = { mCornersWorld[1],0x8800ff00 };

			//下平面4个点
			pVertexs[16] = { mCornersWorld[7],0x8800ff00 };
			pVertexs[17] = { mCornersWorld[6],0x8800ff00 };
			pVertexs[18] = { mCornersWorld[3],0x8800ff00 };
			pVertexs[19] = { mCornersWorld[2],0x8800ff00 };

			//后平面4个点
			pVertexs[20] = { mCornersWorld[5],0x880000ff };
			pVertexs[21] = { mCornersWorld[4],0x880000ff };
			pVertexs[22] = { mCornersWorld[7],0x880000ff };
			pVertexs[23] = { mCornersWorld[6],0x880000ff };

			m_pAABBMesh->UnlockVertexBuffer();

			//36个索引值
			WORD arrayIndex[36] = {
							0,1,3,0,3,2,
							4,5,7,4,7,6,
							8,9,11,8,11,10,
							12,13,15,12,15,14,
							16,17,19,16,19,18,
							20,21,23,20,23,21,
			};

			WORD* pIndex;
			m_pAABBMesh->LockIndexBuffer(0, (void**)&pIndex);
			memcpy(pIndex, arrayIndex, 36 * sizeof(WORD));
			m_pAABBMesh->UnlockIndexBuffer();
		}
		CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);
		CGraphic::GetSingleObjPtr()->m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);

		m_pAABBMesh->DrawSubset(0);

		CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
	else
	{
		m_pAABBMesh = nullptr;
		return;
	}
}

void CAxisAlignedBox::RenderBox()
{
	if (m_bRenderBox)
	{
		if (!m_pBoxMesh)
		{
			D3DXCreateMeshFVF(12, 24, D3DXMESH_MANAGED, BOXVERTEXFVF, CGraphic::GetSingleObjPtr()->m_pDevice, &m_pBoxMesh);

			BOXVERTEX *pVertexs;
			m_pBoxMesh->LockVertexBuffer(0, (void**)&pVertexs);
			//24个点赋值
			//前平面4个点
			pVertexs[0] = { mCornersWorld[0],0x88ff0000 };
			pVertexs[1] = { mCornersWorld[1],0x88ff0000 };
			pVertexs[2] = { mCornersWorld[2],0x88ff0000 };
			pVertexs[3] = { mCornersWorld[3],0x88ff0000 };

			//左平面4个点
			pVertexs[4] = { mCornersWorld[4],0x880000ff };
			pVertexs[5] = { mCornersWorld[0],0x880000ff };
			pVertexs[6] = { mCornersWorld[6],0x880000ff };
			pVertexs[7] = { mCornersWorld[2],0x880000ff };

			//右平面4个点
			pVertexs[8] = { mCornersWorld[1],0x880000ff };
			pVertexs[9] = { mCornersWorld[5],0x880000ff };
			pVertexs[10] = { mCornersWorld[3],0x880000ff };
			pVertexs[11] = { mCornersWorld[7],0x880000ff };

			//上平面4个点
			pVertexs[12] = { mCornersWorld[4],0x8800ff00 };
			pVertexs[13] = { mCornersWorld[5],0x8800ff00 };
			pVertexs[14] = { mCornersWorld[0],0x8800ff00 };
			pVertexs[15] = { mCornersWorld[1],0x8800ff00 };

			//下平面4个点
			pVertexs[16] = { mCornersWorld[7],0x8800ff00 };
			pVertexs[17] = { mCornersWorld[6],0x8800ff00 };
			pVertexs[18] = { mCornersWorld[3],0x8800ff00 };
			pVertexs[19] = { mCornersWorld[2],0x8800ff00 };

			//后平面4个点
			pVertexs[20] = { mCornersWorld[5],0x880000ff };
			pVertexs[21] = { mCornersWorld[4],0x880000ff };
			pVertexs[22] = { mCornersWorld[7],0x880000ff };
			pVertexs[23] = { mCornersWorld[6],0x880000ff };

			m_pBoxMesh->UnlockVertexBuffer();

			//36个索引值
			WORD arrayIndex[36] = {
							0,1,3,0,3,2,
							4,5,7,4,7,6,
							8,9,11,8,11,10,
							12,13,15,12,15,14,
							16,17,19,16,19,18,
							20,21,23,20,23,21,
			};

			WORD *pIndex;
			m_pBoxMesh->LockIndexBuffer(0, (void**)&pIndex);
			memcpy(pIndex, arrayIndex, 36 * sizeof(WORD));
			m_pBoxMesh->UnlockIndexBuffer();
		}

		CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);
		CGraphic::GetSingleObjPtr()->m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);

		m_pBoxMesh->DrawSubset(0);

		CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
	else
	{	
		m_pBoxMesh=nullptr;
		return;
	}
}
