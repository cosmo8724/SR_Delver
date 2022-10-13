#include "stdafx.h"
#include "CurlingMgr.h"
#include "Export_Function.h"

IMPLEMENT_SINGLETON(CCurlingMgr)


CCurlingMgr::CCurlingMgr()
{
}


CCurlingMgr::~CCurlingMgr()
{
	Free();
}

void CCurlingMgr::Ready_CurlingMgr(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();
}

_int CCurlingMgr::Update(const _float & fTimeDelta)
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

	// plane1
	// plane2
	// plane3
	// plane4
	// plane5
	// plane6



	return 0;

}

void CCurlingMgr::LateUpdate(void)
{
}

_bool CCurlingMgr::Is_Inside(CGameObject * pObj)
{
	// 모두 음수여야만 true 리턴(절두체 내)




	return true;
}

inline void CCurlingMgr::Free(void)
{
	Safe_Release(m_pGraphicDev);
}
