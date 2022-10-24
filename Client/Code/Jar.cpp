#include "stdafx.h"
#include "..\Header\Jar.h"

#include "Export_Function.h"
#include "ParticleMgr.h"
#include "ItemMgr.h"
#include "Player.h"

CJar::CJar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEcoObject(pGraphicDev)
{
}

CJar::CJar(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	: CEcoObject(pGraphicDev)
{
	m_eType = ECO_JAR;
	m_vPos = vPos;
}

CJar::CJar(const CEcoObject & rhs)
	: CEcoObject(rhs)
{
}

CJar::~CJar()
{
}

HRESULT CJar::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (!m_bClone)
		m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	return S_OK;
}

_int CJar::Update_Object(const _float & fTimeDelta)
{
	if (!m_bReady)
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
		if (nullptr != pPlayer)
		{
			pPlayer->Add_CollisionGroup(this);
			m_bReady = true;
		}
		else
			return 0;
		
	}

	if (m_bDead)
		return OBJ_DEAD;

	CEcoObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_ALPHA, this);

	return OBJ_NOEVENT;
}

void CJar::LateUpdate_Object(void)
{
	if (m_bDead)
		return;

	Billboard();
	CEcoObject::LateUpdate_Object();
}

void CJar::Render_Obejct(void)
{
	if (m_bDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 알파값 변화
	//m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iTransparency, 0, 0, 0));
	//m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	m_pTextureCom->Set_Texture(m_iTexture);
	m_pBufferCom->Render_Buffer();

	// 알파값 변화 해제
	//m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	CEcoObject::Render_Obejct();	// collider 출력
}

HRESULT CJar::Add_Component(void)
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_EcoJar_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_EcoJar_Texture", pComponent });


	return S_OK;
}

CJar * CJar::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CJar *	pInstance = new CJar(pGraphicDev, vPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CJar * CJar::Create(CEcoObject * pEcoObject)
{
	CJar *	pInstance = new CJar(*pEcoObject);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CJar::Free(void)
{
	CEcoObject::Free();
}

void CJar::CollisionEvent(CGameObject * pObj)
{
	//MSG_BOX("충돌");
	//m_pTransCom->Set_Pos(-1000.f, -1000.f, -1000.f);

	m_bDead = true;
	m_pColliderCom->Set_Free(true);
	CParticleMgr::GetInstance()->Set_Info(this, 20, 1.f, { 0.1f, 0.1f, 0.1f },
		1.f, { 1.f,1.f,1.f,1.f });
	CParticleMgr::GetInstance()->Call_Particle(PTYPE_FOUNTAIN, TEXTURE_6);

	//CItemMgr::GetInstance()->Add_RandomObject(
	//	L"Layer_GameLogic", L"Arrow", ITEM_WEAPON, m_pTransCom->Get_Pos());

	CItemMgr::GetInstance()->Add_RandomObject(
		L"Layer_GameLogic", L"Potion", ITEM_POTION, m_pTransCom->Get_Pos());

}
