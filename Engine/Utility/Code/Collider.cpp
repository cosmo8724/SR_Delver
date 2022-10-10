#include "..\..\Header\Collider.h"
#include "Export_Function.h"

USING(Engine)

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
	, m_bClone(false)
	, m_pPos(nullptr)
	, m_vMax({ 0.f, 0.f, 0.f })
	, m_vMin({ 0.f, 0.f, 0.f })
	, m_vCenter({ 0.f, 0.f, 0.f })
	, m_fRadius(0.f)
	, m_dwColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f))
	, m_bFree(false)
{
}

CCollider::CCollider(const CCollider & rhs)
	: CVIBuffer(rhs)
	, m_bClone(true)
	, m_pPos(rhs.m_pPos)
	, m_vMax(rhs.m_vMax)
	, m_vMin(rhs.m_vMin)
	, m_vCenter(rhs.m_vCenter)
	, m_fRadius(rhs.m_fRadius)
	, m_dwColor(rhs.m_dwColor)
	, m_bFree(rhs.m_bFree)
{
	m_pSphereBuffer = CSphereTex::Create(m_pGraphicDev, m_vCenter, m_fRadius, m_dwColor);
}

CCollider::~CCollider()
{
}


HRESULT CCollider::Ready_Buffer(void)
{
	m_dwVtxCnt = 8;
	m_pPos = new _vec3[m_dwVtxCnt];
	m_dwTriCnt = 12;
	m_dwVtxSize = sizeof(VTXCUBE);
	m_dwFVF = FVF_CUBE;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCUBE*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	// 3인자 : 배열에 저장된 첫 번째 버텍스의 주소를 얻어옴

	// 앞면
	pVertex[0].vPos = { -1.f, 1.f, -1.f };
	pVertex[0].vTexUV = pVertex[0].vPos;
	m_pPos[0] = pVertex[0].vPos;

	pVertex[1].vPos = { 1.f, 1.f, -1.f };
	pVertex[1].vTexUV = pVertex[1].vPos;
	m_pPos[1] = pVertex[1].vPos;

	pVertex[2].vPos = { 1.f, -1.f, -1.f };
	pVertex[2].vTexUV = pVertex[2].vPos;
	m_pPos[2] = pVertex[2].vPos;

	pVertex[3].vPos = { -1.f, -1.f, -1.f };
	pVertex[3].vTexUV = pVertex[3].vPos;
	m_pPos[3] = pVertex[3].vPos;

	// 뒷면
	pVertex[4].vPos = { -1.f, 1.f, 1.f };
	pVertex[4].vTexUV = pVertex[4].vPos;
	m_pPos[4] = pVertex[4].vPos;

	pVertex[5].vPos = { 1.f, 1.f, 1.f };
	pVertex[5].vTexUV = pVertex[5].vPos;
	m_pPos[5] = pVertex[5].vPos;

	pVertex[6].vPos = { 1.f, -1.f, 1.f };
	pVertex[6].vTexUV = pVertex[6].vPos;
	m_pPos[6] = pVertex[6].vPos;

	pVertex[7].vPos = { -1.f, -1.f, 1.f };
	pVertex[7].vTexUV = pVertex[7].vPos;
	m_pPos[7] = pVertex[7].vPos;

	m_pVB->Unlock();


	INDEX32*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// X+

	pIndex[0]._0 = 1;
	pIndex[0]._1 = 5;
	pIndex[0]._2 = 6;

	pIndex[1]._0 = 1;
	pIndex[1]._1 = 6;
	pIndex[1]._2 = 2;

	// X-
	pIndex[2]._0 = 4;
	pIndex[2]._1 = 0;
	pIndex[2]._2 = 3;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 3;
	pIndex[3]._2 = 7;

	// Y+
	pIndex[4]._0 = 4;
	pIndex[4]._1 = 5;
	pIndex[4]._2 = 1;

	pIndex[5]._0 = 4;
	pIndex[5]._1 = 1;
	pIndex[5]._2 = 0;

	// Y-
	pIndex[6]._0 = 3;
	pIndex[6]._1 = 2;
	pIndex[6]._2 = 6;

	pIndex[7]._0 = 3;
	pIndex[7]._1 = 6;
	pIndex[7]._2 = 7;

	// Z+
	pIndex[8]._0 = 7;
	pIndex[8]._1 = 6;
	pIndex[8]._2 = 5;

	pIndex[9]._0 = 7;
	pIndex[9]._1 = 5;
	pIndex[9]._2 = 4;

	// Z-
	pIndex[10]._0 = 0;
	pIndex[10]._1 = 1;
	pIndex[10]._2 = 2;

	pIndex[11]._0 = 0;
	pIndex[11]._1 = 2;
	pIndex[11]._2 = 3;

	m_pIB->Unlock();


	m_vMin = pVertex[3].vPos;
	m_vMax = pVertex[5].vPos;
	m_vCenter = { m_vMin.x + 1.f, m_vMin.y + 1.f, m_vMin.z + 1.f };
	m_fRadius = 2.f;

	m_pSphereBuffer = CSphereTex::Create(m_pGraphicDev, m_vCenter, m_fRadius, m_dwColor);

	return S_OK;

}

void CCollider::Render_Buffer(void)
{
	static	_bool	bDrawCube = true;
	static	_bool	bDrawSphere = true;

	if (Key_Down(DIK_F2))
		bDrawCube = !bDrawCube;
	if (Key_Down(DIK_F3))
		bDrawSphere = !bDrawSphere;

	if (bDrawCube)
		CVIBuffer::Render_Buffer();
	if (bDrawSphere)
		m_pSphereBuffer->Render_Buffer();
}

CComponent * CCollider::Clone(void)
{
	return new CCollider(*this);
}

void CCollider::Free(void)
{
	if (false == m_bClone)
	{
		Safe_Delete_Array(m_pPos);
	}
	Safe_Release(m_pSphereBuffer);
	CVIBuffer::Free();
}

CCollider * CCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCollider*	pInstance = new CCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCollider::Calculate_WorldMatrix(_matrix matWorld)
{
	// Only need Scale and Translation

	_matrix matScale, matTrans;

	D3DXMatrixScaling(&matScale, 
			D3DXVec3Length(&_vec3({ matWorld._11, matWorld._12, matWorld._13 })),
			D3DXVec3Length(&_vec3({ matWorld._21, matWorld._22, matWorld._23 })),
			D3DXVec3Length(&_vec3({ matWorld._31, matWorld._32, matWorld._33 })));

	D3DXMatrixTranslation(&matTrans,
		matWorld._41, matWorld._42, matWorld._43);


	m_matWorld = matScale * matTrans;

	_vec3 vCenter = { m_matWorld._41, m_matWorld._42, m_matWorld._43 };

	m_vMin = { vCenter.x - matScale._11, vCenter.y - matScale._22, vCenter.z - matScale._33 };
	m_vMax = { vCenter.x + matScale._11, vCenter.y + matScale._22, vCenter.z + matScale._33 };


	//m_vMin = { m_matWorld._41 }
	//D3DXVec3TransformCoord(&m_vMin, &m_vMin, &m_matWorld);
	//D3DXVec3TransformCoord(&m_vMax, &m_vMax, &m_matWorld);


}

_bool	CCollider::CollideAABB(CCollider* _collider)
{
	if (_collider->Is_Free() || this->Is_Free())
		return false;

	if ((this->m_vMax.x < _collider->m_vMin.x) || (this->m_vMin.x > _collider->m_vMax.x))
		return false;
	if ((this->m_vMax.y < _collider->m_vMin.y) || (this->m_vMin.y > _collider->m_vMax.y))
		return false;
	if ((this->m_vMax.z < _collider->m_vMin.z) || (this->m_vMin.z > _collider->m_vMax.z))
		return false;


	return true;
}

_bool	CCollider::CollideSphere(CCollider* _collider)
{
	if (_collider->Is_Free())
		return false;

	if (pow(this->m_vCenter.x - _collider->m_vCenter.x, 2)
		+ pow(this->m_vCenter.y - _collider->m_vCenter.y, 2)
		+ pow(this->m_vCenter.z - _collider->m_vCenter.z, 2)
		< pow(this->m_fRadius + _collider->m_fRadius, 2))
	{
		return true;
	}

	return false;
}