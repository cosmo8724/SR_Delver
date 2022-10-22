#include "stdafx.h"
#include "..\Header\Stick.h"

#include "Export_Function.h"
#include "MiniMap.h"

// 충돌
#include "Player.h"
#include "ParticleMgr.h"
#include "ItemMgr.h"

CStick::CStick(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_bParticle(false)
	, m_fMoveTimeAcc(0.f)

{
	m_ObjTag = L"Stick";
}

CStick::CStick(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	: CMonster(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_bParticle(false)
	, m_fMoveTimeAcc(0.f)

{
	m_vPos = vPos;
	m_ObjTag = L"Stick";
}

CStick::~CStick()
{
}

HRESULT CStick::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.iHp = 5;
	m_tInfo.iAttack = 2;

	m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);
	//m_pTransCom->Set_Scale(0.7f, 0.7f, 0.7f);

	m_eCurState = IDLE;

	m_fIdle_Speed = 1.f;
	m_fAttack_Speed = 5.f;

	return S_OK;
}

_int CStick::Update_Object(const _float & fTimeDelta)
{
	if (!m_bCreateIcon)
	{
		CMiniMap* pMiniMap = dynamic_cast<CMiniMap*>(Engine::Get_GameObject(L"Layer_UI", L"UI_MiniMap"));
		pMiniMap->Add_Icon(m_pGraphicDev, this);
		m_bCreateIcon = true;
	}
	Engine::CMonster::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	m_pAnimtorCom->Play_Animation(fTimeDelta * 1.5f);
	Motion_Change();

	if (0 >= m_tInfo.iHp)
	{
		Dead();
		m_fRenderOFFTimeAcc += fTimeDelta;
		if (1.f < m_fRenderOFFTimeAcc)
		{
			m_bRenderOFF = true;
			m_fRenderOFFTimeAcc = 0.f;
		}
		return OBJ_DEAD;
	}
	OnHit(fTimeDelta);

	if (!m_bHit)
		Target_Follow(fTimeDelta);

	return 0;
}

void CStick::LateUpdate_Object(void)
{
	CMonster::Billboard();
	Engine::CGameObject::LateUpdate_Object();
}

void CStick::Render_Obejct(void)
{
	if (!m_bRenderOFF)
		CMonster::Render_Obejct();
}

HRESULT CStick::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// m_pAnimtorCom
	pComponent = m_pAnimtorCom = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_AnimatorCom"));
	NULL_CHECK_RETURN(m_pAnimtorCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_AnimatorCom", pComponent });

	// Collider Component
	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	m_pAnimtorCom->Add_Component(L"Proto_StickIDLE_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_StickANGER_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_StickATTACK_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_StickHIT_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_StickDIE_Texture");

	return S_OK;
}

void CStick::Target_Follow(const _float & fTimeDelta)
{
	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransform);

	_vec3 vPlayerPos, vPos;
	pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	_float fDist = D3DXVec3Length(&(vPlayerPos - vPos));

	if (fDist < 10.f)
	{
		m_eCurState = ANGER; // 거리 안 으로 들어오면 분노 상태
		// 일정 시간 후에 플레이어 근처로 빠르게 이동
		// 다시 랜덤한 위치로 이동
		// 다시 분노 후 일정시간 후 플레이어 근처로 빠르게 이동 랜덤 위치로 이동을 반복

		m_fMoveTimeAcc += fTimeDelta;
		if (1.5f < m_fMoveTimeAcc)
		{
			m_eCurState = IDLE;
			m_pTransCom->Set_Y(m_vPos.y);
			m_pTransCom->Chase_Target(&vPlayerPos, m_fAttack_Speed, fTimeDelta);

			if (fDist < 5.f)
			{
				m_eCurState = ATTACK;
				if (!m_bParticle)
				{
					CParticleMgr::GetInstance()->Set_Info(this, 10, 1.f, { 1.f, 1.f, 1.f },
						1.f, { 1.f,1.f,1.f, 0.1f });
					CParticleMgr::GetInstance()->Call_Particle(PTYPE_TRACER, TEXTURE_7);
					m_bParticle = true;
				}
				if (m_pAnimtorCom->Get_Currentframe() >= 5.f)
				{
					m_bParticle = false;
					m_fMoveTimeAcc = 0.f;
				}
			}
		}
	}
	else
		m_eCurState = IDLE;
}

void CStick::OnHit(const _float & fTimeDelta)
{
	if (!m_bHit)
		return;

	if (!m_bOneCheck)
	{
		m_eCurState = HIT;
		CMonster::Set_KnockBack(m_vPos.y);

		CParticleMgr::GetInstance()->Set_Info(this, 1, 0.5f, { 1.f, 1.f, 0.f },
			1.f, { 1.f, 1.f, 1.f, 1.f }, 5.f, true);
		CParticleMgr::GetInstance()->Add_Info_Spot(false, true);
		CParticleMgr::GetInstance()->Call_Particle(PTYPE_SPOT, TEXTURE_14);

		m_bOneCheck = true;
	}

	m_fHitTimeAcc += fTimeDelta;
	if (0.7f < m_fHitTimeAcc) // 0.7 > Monster Hit Time
	{
		// MinusHp
		CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
		m_tInfo.iHp -= pPlayer->Get_PlayerAttack();

		// Initialization
		m_bHit = false;
		m_bOneCheck = false;
		m_fHitTimeAcc = 0.f;
	}
}

void CStick::Dead()
{
	if (m_bDead)
		return;

	m_eCurState = DIE;

	CParticleMgr::GetInstance()->Set_Info(this,
		50,
		0.1f,
		{ 0.5f, 0.5f, 0.5f },
		1.f,
		{ 1.f, 0.f, 0.f, 1.f });
	CParticleMgr::GetInstance()->Call_Particle(PTYPE_FOUNTAIN, TEXTURE_5);

	CItemMgr::GetInstance()->Add_RandomObject(L"Layer_GameLogic", L"Potion", ITEM_POTION, m_pTransCom->Get_Pos());

	m_pColliderCom->Set_Free(true);
	m_bDead = true;
}

void CStick::CollisionEvent(CGameObject* pObj)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);
	if (pPlayer != pObj)
		m_bHit = true;
}

void CStick::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_pAnimtorCom->Change_Animation(L"Proto_StickIDLE_Texture");
			break;

		case ANGER:
			m_pAnimtorCom->Change_Animation(L"Proto_StickANGER_Texture");
			break;

		case ATTACK:
			m_pAnimtorCom->Change_Animation(L"Proto_StickATTACK_Texture");
			break;

		case HIT:
			m_pAnimtorCom->Change_Animation(L"Proto_StickHIT_Texture");
			break;

		case DIE:
			m_pAnimtorCom->Change_Animation(L"Proto_StickDIE_Texture");
			break;
		}
		m_ePreState = m_eCurState;
	}
}

CStick * CStick::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CStick *	pInstance = new CStick(pGraphicDev, vPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStick::Free(void)
{
	CMonster::Free();
}