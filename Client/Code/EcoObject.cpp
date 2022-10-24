#include "stdafx.h"
#include "EcoObject.h"
#include "Export_Function.h"


CEcoObject::CEcoObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CEcoObject::CEcoObject(const CEcoObject & rhs)
	: CGameObject(rhs)
	, m_eType(rhs.m_eType)
	, m_vPos(rhs.m_vPos)
	, m_matWorld(rhs.m_matWorld)
	, m_iTransparency(rhs.m_iTransparency)
	, m_iTexture(rhs.m_iTexture)
	, m_bClone(true)
{
	m_pTransCom = dynamic_cast<CTransform*>(rhs.m_pTransCom->Clone());
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", m_pTransCom });
}

CEcoObject::~CEcoObject()
{
}

HRESULT CEcoObject::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

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

CEcoObject * CEcoObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEcoObject *	pInstance = new CEcoObject(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
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
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	CCamera*	pCam = static_cast<CCamera*>(Get_GameObject(L"Layer_Environment", L"ObjectCamera"));
	_matrix matOldView;
	pCam->Get_ViewMatrix(matOldView);

	if (matView._21 > 0.f)
	{
		D3DXMatrixIdentity(&matBill);
		matBill._11 = matOldView._11;
		matBill._13 = matOldView._13;
		matBill._31 = matOldView._31;
		matBill._33 = matOldView._33;
	}
	else
	{
		D3DXMatrixIdentity(&matBill);
		matBill._11 = matView._11;
		matBill._13 = matView._13;
		matBill._31 = matView._31;
		matBill._33 = matView._33;

		//m_matOldBill = matBill;
	}

	D3DXMatrixInverse(&matBill, 0, &matBill);

	_vec3 vScale = m_pTransCom->Get_Scale();
	_matrix matScale, matScaleInv;
	D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
	D3DXMatrixInverse(&matScaleInv, 0, &matScale);

	m_matWorld = matBill *matWorld;
	m_pTransCom->Set_WorldMatrix(&m_matWorld);
}

HRESULT CEcoObject::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	if (!m_bClone)
	{
		pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
		NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });
	}
	return S_OK;
}
