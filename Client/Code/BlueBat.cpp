#include "stdafx.h"
#include "..\Header\BlueBat.h"

#include "Export_Function.h"

#include "StaticCamera.h"
#include "MiniMap.h"

// 충돌
#include "Player.h"
#include "ParticleMgr.h"
#include "ItemMgr.h"

CBlueBat::CBlueBat(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_bIdle(false)
	, m_fJSpeed(0.2f)
	, m_fJSpeed0(0.2f)
	, m_fAccel(0.015f)
	, m_fTimeAcc(0.f)
	, m_fJumpTimeAcc(0.f)
	, m_fIdleTimeAcc(0.f)
	, m_fSkillTimeAcc(0.f)
{
	m_eType = MOB_BLUEBAT;
	m_ObjTag = L"BlueBat";
}

CBlueBat::CBlueBat(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	: CMonster(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_bIdle(false)
	, m_fJSpeed(0.2f)
	, m_fJSpeed0(0.2f)
	, m_fAccel(0.015f)
	, m_fTimeAcc(0.f)
	, m_fJumpTimeAcc(0.f)
	, m_fIdleTimeAcc(0.f)
	, m_fSkillTimeAcc(0.f)
{
	m_eType = MOB_BLUEBAT;
	m_vPos = vPos;
	m_ObjTag = L"BlueBat";
}

CBlueBat::CBlueBat(const CMonster& rhs)
	: CMonster(rhs)
{
	m_ObjTag = L"BlueBat";
}

CBlueBat::~CBlueBat()
{
}

HRESULT CBlueBat::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.iHp = 3;
	m_tInfo.iAttack = 2;
	m_tInfo.iExp = 3;

	if (m_bClone)
		m_vPos = m_pTransCom->Get_Pos();

	m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	m_eCurState = IDLE;

	m_fIdle_Speed = 1.f;
	m_fAttack_Speed = 2.f;

	return S_OK;
}

_int CBlueBat::Update_Object(const _float & fTimeDelta)
{
	if (!m_bCreateIcon && !g_bIsTool)
	{
		CMiniMap* pMiniMap = dynamic_cast<CMiniMap*>(Engine::Get_GameObject(L"Layer_UI", L"UI_MiniMap"));
		pMiniMap->Add_Icon(m_pGraphicDev, this);
		m_bCreateIcon = true;
	}
	Engine::CMonster::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	m_pAnimtorCom->Play_Animation(fTimeDelta);
	Motion_Change(fTimeDelta);

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

void CBlueBat::LateUpdate_Object(void)
{
	CMonster::Billboard();
	Engine::CGameObject::LateUpdate_Object();
}

void CBlueBat::Render_Obejct(void)
{
	if (!m_bRenderOFF)
		CMonster::Render_Obejct();
}

HRESULT CBlueBat::Add_Component(void)
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

	m_pAnimtorCom->Add_Component(L"Proto_BlueBatIDLE_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_BlueBatATTACK_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_BlueBatHIT_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_BlueBatDIE_Texture");

	return S_OK;
}

void CBlueBat::Target_Follow(const _float & fTimeDelta)
{
	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransformCom);

	_vec3		vPlayerPos, vPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	_float fDist = D3DXVec3Length(&(vPlayerPos - vPos));

	if (fDist < 7.f && fDist > 4.f) // Follow
	{
		Engine::StopSound(SOUND_BLUEBAT);
		Engine::Play_Sound(L"M_BlueBat_Alert.mp3", SOUND_BLUEBAT, 1.f);

		m_eCurState = IDLE;

		m_pTransCom->Chase_Target(&vPlayerPos, m_fAttack_Speed, fTimeDelta);
		m_pTransCom->Set_Y(m_vPos.y);
	}
	else if (fDist < 4.f && fDist > 1.f) // Jump
	{
		m_fSkillTimeAcc += fTimeDelta;
		if (2.f < m_fSkillTimeAcc)
		{
			m_bIdle = true;
		}
		if (2.1f < m_fSkillTimeAcc)
		{
			m_bIdle = false;
			m_fSkillTimeAcc = 0;
		}

		if (m_bIdle == true)
			m_bJump = true;

		Jump(fTimeDelta);
	}
}

void CBlueBat::Jump(const _float & fTimeDelta)
{
	//if (Engine::Key_Down(DIK_V))
	//	m_bJump = true;

	if (m_bJump)
	{
		_vec3 vPos;
		m_pTransCom->Get_Info(INFO_POS, &vPos);

		if (m_fJumpTimeAcc > 0.2f && m_vPos.y >= vPos.y)
		{
			m_bJump = false;
			m_fJumpTimeAcc = 0.f;
			m_pTransCom->Set_Y(m_vPos.y);
			m_fJSpeed = m_fJSpeed0;
		}
		else
		{
			m_eCurState = ATTACK;

			m_fJSpeed -= m_fAccel;
			m_pTransCom->Plus_PosY(m_fJSpeed);
			m_fJumpTimeAcc += 0.01f;

			if (m_pAnimtorCom->Get_Currentframe() >= 5.f && m_pAnimtorCom->Get_Currentframe() < 8.f) // CameraShake
			{
				CStaticCamera* pStaticCamera = dynamic_cast<CStaticCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"StaticCamera"));
				NULL_CHECK(pStaticCamera);
				pStaticCamera->Shake_Camera(1.f, 2.f);
				//pStaticCamera->Set_ShakeY();
			}
		}
	}
	else
	{
		m_fIdleTimeAcc += fTimeDelta;
		if (m_fIdleTimeAcc > 0.55f)
		{
			m_eCurState = IDLE;
			m_fIdleTimeAcc = 0.f;
		}
	}
}

void CBlueBat::OnHit(const _float & fTimeDelta)
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

void CBlueBat::Dead()
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

	CItemMgr::GetInstance()->Add_RandomObject(L"Layer_GameLogic", L"Potion", ITEM_POTION, m_pTransCom->Get_Pos());

	m_pColliderCom->Set_Free(true);
	m_bDead = true;
}

void CBlueBat::CollisionEvent(CGameObject* pObj)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);
	if (pPlayer != pObj)
		m_bHit = true;
}

void CBlueBat::Motion_Change(const _float& fTimeDelta)
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_pAnimtorCom->Change_Animation(L"Proto_BlueBatIDLE_Texture");
			break;

		case ATTACK:
			Engine::StopSound(SOUND_BLUEBAT);
			Engine::Play_Sound(L"M_BlueBat_Attack.mp3", SOUND_BLUEBAT, 1.f);
			m_pAnimtorCom->Change_Animation(L"Proto_BlueBatATTACK_Texture");
			break;

		case HIT:
			Engine::StopSound(SOUND_BLUEBAT);
			Engine::Play_Sound(L"M_BlueBat_Hit.mp3", SOUND_BLUEBAT, 1.f);
			m_pAnimtorCom->Change_Animation(L"Proto_BlueBatHIT_Texture");
			break;

		case DIE:
			Engine::StopSound(SOUND_BLUEBAT);
			Engine::Play_Sound(L"M_BlueBat_Die.mp3", SOUND_BLUEBAT, 1.f);
			m_pAnimtorCom->Change_Animation(L"Proto_BlueBatDIE_Texture");
			break;
		}
		m_ePreState = m_eCurState;
	}
}

CBlueBat * CBlueBat::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CBlueBat *	pInstance = new CBlueBat(pGraphicDev, vPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CBlueBat * CBlueBat::Create(CMonster * pMonster)
{
	CBlueBat *	pInstance = new CBlueBat(*pMonster);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBlueBat::Free(void)
{
	CMonster::Free();
}
