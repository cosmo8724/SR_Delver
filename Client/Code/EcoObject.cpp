#include "stdafx.h"
#include "EcoObject.h"
#include "Export_Function.h"


CEcoObject::CEcoObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CEcoObject::~CEcoObject()
{
}

HRESULT CEcoObject::Ready_Object(void)
{

	return S_OK;
}

_int CEcoObject::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	m_pColliderCom->Calculate_WorldMatrix(*m_pTransCom->Get_WorldMatrixPointer());

	return 0;
}

void CEcoObject::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CEcoObject::Render_Obejct(void)
{

#ifdef _DEBUG
	// Collider
	m_pGraphicDev->SetTransform(D3DTS_WORLD,
		&(m_pColliderCom->Get_WorldMatrix()));
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pColliderCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif

}

void CEcoObject::Free(void)
{
	CGameObject::Free();
}

void CEcoObject::Billboard()
{
	// ºôº¸µå
	_matrix		matWorld, matView, matBill;
	D3DXMatrixIdentity(&matBill);

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	m_matWorld = matBill * matWorld;

	m_pTransCom->Set_WorldMatrix(&m_matWorld);


}
