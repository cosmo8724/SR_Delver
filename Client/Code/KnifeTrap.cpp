#include "stdafx.h"
#include "..\Header\KnifeTrap.h"
#include "Export_Function.h"
#include "CullingMgr.h"
#include "Player.h"

CKnifeTrap::CKnifeTrap(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	:CEcoObject(pGraphicDev)
{

	m_vPos = vPos;
	m_eType = ECO_KNIFETRAP;
	//m_fTopPosY = m_vPos.y;
	//m_fGroundY = -m_vPos.y;
	m_fTopPosY = m_vPos.y + 0.5f;
	m_fGroundY = -m_vPos.y;

	m_fSpeed = 0.5f;
	m_fCurSpeed = m_fSpeed;
	m_fDist = 5.f;

	m_bChargeTime = 0.8f;
}

CKnifeTrap::CKnifeTrap(const CEcoObject & rhs)
	: CEcoObject(rhs)
{
	//m_fTopPosY = m_vPos.y + 0.5f;
	//m_fGroundY = -m_vPos.y;
	m_fSpeed = 0.5f;
	m_fCurSpeed = m_fSpeed;
	m_fDist = 5.f;

	m_bChargeTime = 0.8f;
}

CKnifeTrap::~CKnifeTrap()
{
}

HRESULT CKnifeTrap::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fTopPosY = m_pTransCom->Get_Pos().y + 0.5f;
	m_fGroundY = m_pTransCom->Get_Pos().y - 32.f;

	if (!m_bClone)
		m_pTransCom->Set_Pos(m_vPos.x, m_fGroundY, m_vPos.z);

	m_pTransCom->Set_Scale(0.4f, 1.f, 1.f);



	return S_OK;
}

_int CKnifeTrap::Update_Object(const _float & fTimeDelta)
{
	CEcoObject::Update_Object(fTimeDelta);

	if (!m_bReady)
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
		if (nullptr != pPlayer)
		{
			m_pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
			pPlayer->Add_CollisionGroup(this);
			m_bReady = true;
		}
		else
			return 0;
	}

	if (Is_Player_Around())
	{
		m_bActivate = true;
	}

	Activate(fTimeDelta);

	return 0;
}

void CKnifeTrap::LateUpdate_Object(void)
{
	if (CCullingMgr::GetInstance()->Is_Inside(this))
		Add_RenderGroup(RENDER_ALPHA, this);

	CEcoObject::LateUpdate_Object();
}

void CKnifeTrap::Render_Obejct(void)
{
	if (g_bIsTool)
		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	m_pTextureCom->Set_Texture(m_iTexture);
	this->m_pBufferCom->Render_Buffer();

	// 알파값 변화 해제
	//m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


	CEcoObject::Render_Obejct();	// collider 출력
}

HRESULT CKnifeTrap::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	if (!m_bClone)
	{
		pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
		NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });
	}

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_KnifeTrap_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_KnifeTrap_Texture", pComponent });

	return S_OK;
}

_bool CKnifeTrap::Is_Player_Around()
{
	_float fDist = D3DXVec3Length(&_vec3(m_pPlayerTransCom->Get_Pos() - m_pTransCom->Get_Pos()));

	if (m_fDist > fDist)
		return true;

	return false;
}

void CKnifeTrap::Activate(const _float& fTimeDelta)
{
	if (!m_bActivate)
		return;

	_float fY = m_pTransCom->Get_Pos().y;

	if (m_bCharge)
	{
		m_bChargeTime += fTimeDelta;
		if (0.8f < m_bChargeTime)
		{
			m_bCharge = false;
			m_bChargeTime = 0.f;
		}
		else
			return;
	}

	if (m_fGroundY >= fY)
	{
		m_fCurSpeed = m_fSpeed;
		m_bFall = true;
		if (!m_bCharge)
			m_bCharge = true;
	}
	else if (true == m_bFall && m_fTopPosY <= fY)
	{
		m_fCurSpeed = -m_fSpeed;
		m_bFall = false;
		if (!m_bCharge)
			m_bCharge = true;
	}



	m_pTransCom->Plus_PosY(m_fCurSpeed);

}

CKnifeTrap * CKnifeTrap::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CKnifeTrap *	pInstance = new CKnifeTrap(pGraphicDev, vPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CKnifeTrap * CKnifeTrap::Create(CEcoObject * pEcoObject)
{
	CKnifeTrap *	pInstance = new CKnifeTrap(*pEcoObject);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CKnifeTrap::Free(void)
{
	CEcoObject::Free();
}

void CKnifeTrap::CollisionEvent(CGameObject * pObj)
{
}
