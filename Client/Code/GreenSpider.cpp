#include "stdafx.h"
#include "..\Header\GreenSpider.h"

#include "Export_Function.h"
#include "MiniMap.h"
#include "BulletMgr.h"

// 충돌
#include "Player.h"
#include "ParticleMgr.h"
#include "ItemMgr.h"

CGreenSpider::CGreenSpider(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_bParticle(false)
	, m_fIdleTimeAcc(0.f)
	, m_fAttackTimeAcc(0.f)

{
	m_eType = MOB_GREENSPIDER;
	m_ObjTag = L"GreenSpider";
}

CGreenSpider::CGreenSpider(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	: CMonster(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_bParticle(false)
	, m_fIdleTimeAcc(0.f)
	, m_fAttackTimeAcc(0.f)

{
	m_eType = MOB_GREENSPIDER;
	m_vPos = vPos;
	m_ObjTag = L"GreenSpider";
}

CGreenSpider::CGreenSpider(const CMonster& rhs)
	: CMonster(rhs)
{
	m_ObjTag = L"GreenSpider";
}

CGreenSpider::~CGreenSpider()
{
}

HRESULT CGreenSpider::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.iHp = 3;
	m_tInfo.iAttack = 1;
	m_tInfo.iExp = 4;

	if (m_bClone)
		m_vPos = m_pTransCom->Get_Pos();

	m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	m_eCurState = IDLE;
	m_ePreState = MOTION_END;

	m_fIdle_Speed = 1.f;
	m_fAttack_Speed = 2.f;

	return S_OK;
}

_int CGreenSpider::Update_Object(const _float & fTimeDelta)
{
	if (!m_bCreateIcon && !g_bIsTool)
	{
		CMiniMap* pMiniMap = dynamic_cast<CMiniMap*>(Engine::Get_GameObject(L"Layer_UI", L"UI_MiniMap"));
		pMiniMap->Add_Icon(m_pGraphicDev, this);
		m_bCreateIcon = true;
	}
	Engine::CMonster::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	m_pAnimtorCom->Play_Animation(fTimeDelta * 1.5f);
	Motion_Change();

	if (g_bIsTool)
		return 0;

	if (0 >= m_tInfo.iHp)
	{
		Dead();
		m_fRenderOFFTimeAcc += fTimeDelta;
		if (1.5f < m_fRenderOFFTimeAcc)
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

void CGreenSpider::LateUpdate_Object(void)
{
	CMonster::Billboard();
	Engine::CGameObject::LateUpdate_Object();
}

void CGreenSpider::Render_Obejct(void)
{
	if (!m_bRenderOFF)
		CMonster::Render_Obejct();
}

HRESULT CGreenSpider::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	if (!m_bClone)
	{
		pComponent = m_pTransCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_TransformCom"));
		NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });
	}
	// m_pAnimtorCom
	pComponent = m_pAnimtorCom = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_AnimatorCom"));
	NULL_CHECK_RETURN(m_pAnimtorCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_AnimatorCom", pComponent });

	// Collider Component
	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pColliderCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	m_pAnimtorCom->Add_Component(L"Proto_GreenSpiderIDLE_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_GreenSpiderATTACK_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_GreenSpiderHIT_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_GreenSpiderDIE_Texture");

	return S_OK;
}

void CGreenSpider::Target_Follow(const _float & fTimeDelta)
{
	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransform);

	_vec3 vPlayerPos, vPos;
	pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	_float fDist = D3DXVec3Length(&(vPlayerPos - vPos));

	// 플레이어가 일정 거리 안 으로 들어오면 플레이어에게 거미줄 발사
	// 플레이는 거미줄을 맞으면, 화면에 거미줄이 쳐지며, 이동속도가 00초 동안 느려진다

	// 플레이어가 일정 거리 안 으로 들어왔을 경우 뒤로 물러남
	if (fDist < 5.f)
	{
		//Engine::StopSound(SOUND_GREENSPIDER);
		//Engine::Play_Sound(L"M_GreenSpider_Walk.mp3", SOUND_GREENSPIDER, 1.f);

		m_eCurState = IDLE;
		m_pTransCom->Set_Y(m_vPos.y);
		m_pTransCom->Chase_Target(&vPlayerPos, -m_fAttack_Speed, fTimeDelta);
		return;
	}

	// 일정 거리 안 으로 들어 왔을 때 공격 시작
	if (fDist < 25.f)
	{
		m_fAttackTimeAcc += fTimeDelta;
		m_fIdleTimeAcc += m_fAttackTimeAcc;

		if (5.f < m_fAttackTimeAcc)
		{
			m_eCurState = ATTACK;
			CBulletMgr::GetInstance()->Set_Obj(BULLET_M_SPIDER, this);
			CBulletMgr::GetInstance()->Fire(BULLET_M_SPIDER);
			m_fAttackTimeAcc = 0;
		}
		else if (7.5f < m_fIdleTimeAcc)
		{
			m_eCurState = IDLE;
			m_fIdleTimeAcc = 0.f;
		}
	}
	else
		m_eCurState = IDLE;
}

void CGreenSpider::OnHit(const _float & fTimeDelta)
{
	if (!m_bHit)
		return;

	if (!m_bOneCheck)
	{
		m_eCurState = HIT;
		CMonster::Set_KnockBack(m_vPos.y);

		CParticleMgr::GetInstance()->Set_Info(this, 1, 0.5f, { 1.f, 0.3f, 0.f },
			1.f, { 1.f, 1.f, 1.f, 1.f }, 5.f, true);
		CParticleMgr::GetInstance()->Add_Info_Spot(false, true);
		CParticleMgr::GetInstance()->Call_Particle(PTYPE_CIRCLING, TEXTURE_14);

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

void CGreenSpider::Dead()
{
	if (m_bDead)
		return;

	m_eCurState = DIE;

	CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	pPlayer->Set_Level(m_tInfo.iHp, m_tInfo.iExp);

	CParticleMgr::GetInstance()->Set_Info(this,
		50,
		0.1f,
		{ 0.5f, 0.5f, 0.5f },
		1.f,
		{ 1.f, 0.f, 0.f, 1.f });
	CParticleMgr::GetInstance()->Call_Particle(PTYPE_FOUNTAIN, TEXTURE_5);

	_int iTex = rand() % 3;
	if (iTex == 0)
		CItemMgr::GetInstance()->Add_RandomObject(L"Layer_GameLogic", L"Potion", ITEM_POTION, m_pTransCom->Get_Pos());
	else if (iTex == 1)
		CItemMgr::GetInstance()->Add_RandomObject(L"Layer_GameLogic", L"Food", ITEM_FOOD, m_pTransCom->Get_Pos());
	else if (iTex == 2)
		CItemMgr::GetInstance()->Add_RandomObject(L"Layer_GameLogic", L"Gold", ITEM_GOLD, m_pTransCom->Get_Pos());

	m_pColliderCom->Set_Free(true);
	m_bDead = true;
}

void CGreenSpider::CollisionEvent(CGameObject* pObj)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);
	if (pPlayer != pObj)
		m_bHit = true;
}

void CGreenSpider::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_pAnimtorCom->Change_Animation(L"Proto_GreenSpiderIDLE_Texture");
			break;

		case ATTACK:
			Engine::StopSound(SOUND_GREENSPIDER);
			Engine::Play_Sound(L"M_GreenSpider_Attack.mp3", SOUND_GREENSPIDER, 1.f);
			m_pAnimtorCom->Change_Animation(L"Proto_GreenSpiderATTACK_Texture");
			break;

		case HIT:
			Engine::StopSound(SOUND_GREENSPIDER);
			Engine::Play_Sound(L"M_GreenSpider_Hit.mp3", SOUND_GREENSPIDER, 1.f);
			m_pAnimtorCom->Change_Animation(L"Proto_GreenSpiderHIT_Texture");
			break;

		case DIE:
			Engine::StopSound(SOUND_GREENSPIDER);
			Engine::Play_Sound(L"M_GreenSpider_Die.mp3", SOUND_GREENSPIDER, 1.f);
			m_pAnimtorCom->Change_Animation(L"Proto_GreenSpiderDIE_Texture");
			break;
		}
		m_ePreState = m_eCurState;
	}
}

CGreenSpider * CGreenSpider::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CGreenSpider *	pInstance = new CGreenSpider(pGraphicDev, vPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGreenSpider * CGreenSpider::Create(CMonster * pMonster)
{
	CGreenSpider *	pInstance = new CGreenSpider(*pMonster);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGreenSpider::Free(void)
{
	CMonster::Free();
}