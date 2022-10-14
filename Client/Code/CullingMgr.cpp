#include "stdafx.h"
#include "CullingMgr.h"
#include "Export_Function.h"

IMPLEMENT_SINGLETON(CCullingMgr)


CCullingMgr::CCullingMgr()
{
}


CCullingMgr::~CCullingMgr()
{
	Free();
}

void CCullingMgr::Ready_CullingMgr(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();
}

_int CCullingMgr::Update(const _float & fTimeDelta)
{
	_matrix matProj, matView;
	
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXMatrixInverse(&matView, 0, &matView);


	// projection space vertex coordinate
	// clockwise

	// near
	vertexArr[0] = { -1.f, 1.f, 0.f };
	vertexArr[1] = { 1.f, 1.f, 0.f };
	vertexArr[2] = { 1.f, -1.f, 0.f };
	vertexArr[3] = { -1.f, -1.f, 0.f };

	// far
	vertexArr[4] = { -1.f, 1.f, 1.f };
	vertexArr[5] = { 1.f, 1.f, 1.f };
	vertexArr[6] = { 1.f, -1.f, 1.f };
	vertexArr[7] = { -1.f, -1.f, 1.f };

	for (int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&vertexArr[i], &vertexArr[i], &matProj);
		D3DXVec3TransformCoord(&vertexArr[i], &vertexArr[i], &matView);
	}

	//plane
	D3DXPlaneFromPoints(&planeArr[0], &vertexArr[0], &vertexArr[1], &vertexArr[2]);
	D3DXPlaneFromPoints(&planeArr[1], &vertexArr[2], &vertexArr[1], &vertexArr[5]);
	D3DXPlaneFromPoints(&planeArr[2], &vertexArr[3], &vertexArr[2], &vertexArr[6]);
	D3DXPlaneFromPoints(&planeArr[3], &vertexArr[0], &vertexArr[3], &vertexArr[7]);
	D3DXPlaneFromPoints(&planeArr[4], &vertexArr[1], &vertexArr[0], &vertexArr[4]);
	D3DXPlaneFromPoints(&planeArr[5], &vertexArr[5], &vertexArr[4], &vertexArr[7]);

	return 0;

}

//void CCullingMgr::LateUpdate(void)
//{
//
//}

_bool CCullingMgr::Is_Inside(CGameObject * pObj)
{
	// 모두 음수여야만 true 리턴(절두체 내)
	CTransform* pTransform = dynamic_cast<CTransform*>(pObj->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
	if (nullptr == pTransform)
	{
		pTransform = dynamic_cast<CTransform*>(pObj->Get_Component(L"Proto_TransformCom", ID_STATIC));
		if (nullptr == pTransform)
			return false;
	}
	_vec3 vPos = pTransform->Get_Pos();

	CCollider*	pCollider = dynamic_cast<CCollider*>(pObj->Get_Component(L"Proto_ColliderCom", ID_STATIC));
	if (nullptr == pCollider)
	{
		MSG_BOX("Collider 없음");
	}
	_float fRange = pCollider->Get_Radius();

	for (int i = 0; i < 6; ++i)
	{
		// ax + by + cd + z
		_float fResult = planeArr[i].a * vPos.x + planeArr[i].b * vPos.y + planeArr[i].c * vPos.z + planeArr[i].d;
		if (fResult > fRange)
			return false;
	}

	return true;
}

inline void CCullingMgr::Free(void)
{
	Safe_Release(m_pGraphicDev);
}
