#include "stdafx.h"
#include "..\Header\BrownBat.h"

#include "Export_Function.h"
#include "MiniMap.h"

// 충돌
#include "Player.h"
#include "ParticleMgr.h"
#include "ItemMgr.h"

CBrownBat::CBrownBat(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_fHeight(0.f)
	, m_fTimeAcc(0.f)
	, m_fAttackTimeAcc(0.f)
{
	m_eType = MOB_BROWNBAT;
	m_ObjTag = L"BrownBat";
}

CBrownBat::CBrownBat(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	: CMonster(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_fHeight(0.f)
	, m_fTimeAcc(0.f)
	, m_fAttackTimeAcc(0.f)
{
	m_eType = MOB_BROWNBAT;
	m_vPos = vPos;
	m_ObjTag = L"BrownBat";
}

CBrownBat::CBrownBat(const CMonster& rhs)
	: CMonster(rhs)
{
	m_ObjTag = L"BrownBat";
}

CBrownBat::~CBrownBat()
{
}

HRESULT CBrownBat::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.iHp = 2;
	m_tInfo.iAttack = 1;
	m_tInfo.iExp = 2;

	if (m_bClone)
		m_vPos = m_pTransCom->Get_Pos();

	m_fHeight = m_vPos.y; // 3.f
	m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);
	//m_pTransCom->Set_Pos(15.f, m_fHeight, 15.f);
	m_eCurState = IDLE;

	m_fIdle_Speed = 5.f;
	m_fAttack_Speed = 7.f;

	return S_OK;
}

_int CBrownBat::Update_Object(const _float & fTimeDelta)
{
	if (!m_bCreateIcon && !g_bIsTool)
	{
		CMiniMap* pMiniMap = dynamic_cast<CMiniMap*>(Engine::Get_GameObject(L"Layer_UI", L"UI_MiniMap"));
		pMiniMap->Add_Icon(m_pGraphicDev, this);
		m_bCreateIcon = true;
	}
	Engine::CMonster::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	m_pAnimtorCom->Play_Animation(fTimeDelta * 3.f);
	Motion_Change();

	if (g_bIsTool)
		return 0;

	if (0 >= m_tInfo.iHp)
	{
		Dead();
		m_fRenderOFFTimeAcc += fTimeDelta;
		if (2.f < m_fRenderOFFTimeAcc)
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

void CBrownBat::LateUpdate_Object(void)
{
	CMonster::Billboard();
	Engine::CGameObject::LateUpdate_Object();
}

void CBrownBat::Render_Obejct(void)
{
	if (!m_bRenderOFF)
		CMonster::Render_Obejct();
}

HRESULT CBrownBat::Add_Component(void)
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

	m_pAnimtorCom->Add_Component(L"Proto_BrownBatIDLE_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_BrownBatATTACK_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_BrownBatHIT_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_BrownBatDIE_Texture");

	return S_OK;
}

void CBrownBat::Target_Follow(const _float & fTimeDelta)
{
	// 플레이어 따라가기
	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransformCom);

	_vec3		vPlayerPos, vPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	_float fDist = D3DXVec3Length(&(vPlayerPos - vPos));

	if (fDist < 7.f)
	{
		m_fAttackTimeAcc += fTimeDelta;
		if (0.7f < m_fAttackTimeAcc)
		{
			if (0 < m_fAttack_Speed)
				m_eCurState = IDLE;
			else if (0 > m_fAttack_Speed)
				m_eCurState = ATTACK;

			m_fAttack_Speed *= -1;
			m_fAttackTimeAcc = 0.f;
		}

		if (-60.f <= vPlayerPos.y) // Player.y가 1.f 이상이면 Height 조정
			m_pTransCom->Set_Y(m_vPos.y);

		m_pTransCom->Chase_Target(&_vec3(vPlayerPos.x, vPlayerPos.y + 0.5f, vPlayerPos.z), m_fAttack_Speed, fTimeDelta);
	}
	else
	{
		m_eCurState = IDLE;

		_vec3		vRight;
		m_pTransCom->Get_Info(INFO_RIGHT, &vRight);

		m_fTimeAcc += fTimeDelta;
		if (1.f < m_fTimeAcc)
		{
			Engine::StopSound(SOUND_BROWNDBAT);
			Engine::Play_Sound(L"M_BrownBat_Idle.mp3", SOUND_BROWNDBAT, 1.f);

			m_fIdle_Speed *= -1;
			m_fTimeAcc = 0.f;
		}

		D3DXVec3Normalize(&vRight, &vRight);
		m_pTransCom->Move_Pos(&(vRight * m_fIdle_Speed * fTimeDelta));

		//// 플레이어를 따라다니지 않을 때 좌우 이동
		//_vec3 vRight, vUp, vLook;
		//vLook = vPlayerPos - vPos;
		//D3DXVec3Normalize(&vLook, &vLook);
		//vUp = _vec3(0.f, 1.f, 0.f);

		//D3DXVec3Cross(&vRight, &vUp, &vLook);

		//m_fTimeAcc += fTimeDelta;
		//if (1.f < m_fTimeAcc)
		//{
		//	m_fIdle_Speed *= -1;
		//	m_fTimeAcc = 0.f;
		//}

		//D3DXVec3Normalize(&vRight, &vRight);
		//m_pTransCom->Move_Pos(&(vRight * m_fIdle_Speed * fTimeDelta));
	}
}

void CBrownBat::OnHit(const _float & fTimeDelta)
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

void CBrownBat::Dead()
{
	if (m_bDead)
		return;
	
	m_eCurState = DIE;
	m_pTransCom->Set_Y(m_vPos.y - 3.f);

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

void CBrownBat::CollisionEvent(CGameObject* pObj)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);
	if (pPlayer != pObj)
		m_bHit = true;
}

void CBrownBat::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_pAnimtorCom->Change_Animation(L"Proto_BrownBatIDLE_Texture");
			break;

		case ATTACK:
			Engine::StopSound(SOUND_BROWNDBAT);
			Engine::Play_Sound(L"M_BrownBat_Attack.mp3", SOUND_BROWNDBAT, 1.f);
			m_pAnimtorCom->Change_Animation(L"Proto_BrownBatATTACK_Texture");
			break;

		case HIT:
			Engine::StopSound(SOUND_BROWNDBAT);
			Engine::Play_Sound(L"M_BrownBat_Hit.mp3", SOUND_BROWNDBAT, 1.f);
			m_pAnimtorCom->Change_Animation(L"Proto_BrownBatHIT_Texture");
			break;

		case DIE:
			Engine::StopSound(SOUND_BROWNDBAT);
			Engine::Play_Sound(L"M_BrownBat_Die.mp3", SOUND_BROWNDBAT, 1.f);
			m_pAnimtorCom->Change_Animation(L"Proto_BrownBatDIE_Texture");
			break;
		}
		m_ePreState = m_eCurState;
	}
}

CBrownBat * CBrownBat::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CBrownBat *	pInstance = new CBrownBat(pGraphicDev, vPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CBrownBat * CBrownBat::Create(CMonster * pMonster)
{
	CBrownBat *	pInstance = new CBrownBat(*pMonster);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBrownBat::Free(void)
{
	CMonster::Free();
}
