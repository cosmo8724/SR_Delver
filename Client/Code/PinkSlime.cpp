#include "stdafx.h"
#include "..\Header\PinkSlime.h"

#include "Export_Function.h"
#include "MiniMap.h"

// �浹
#include "Player.h"
#include "ParticleMgr.h"
#include "ItemMgr.h"

CPinkSlime::CPinkSlime(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_eSkill(SKILL_END)
	, m_eSeparation(SKILLSCALE_END)
	, m_fTimeAcc(0.f)
	, m_fJumpTimeAcc(0.f)
	, m_fScale(0.f)
	, m_fHeight(0.f)
	, m_bJump(false)
	, m_fJSpeed(0.f)
	, m_fJSpeed0(0.f)
	, m_fAccel(0.f)
{
	m_eType = MOB_PINKSLIME;
	//m_ObjTag = L"PinkSlime";
}

CPinkSlime::CPinkSlime(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	: CMonster(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_eSkill(SKILL_END)
	, m_eSeparation(SKILLSCALE_END)
	, m_fTimeAcc(0.f)
	, m_fJumpTimeAcc(0.f)
	, m_fScale(0.f)
	, m_fHeight(vPos.y)
	, m_bJump(false)
	, m_fJSpeed(0.f)
	, m_fJSpeed0(0.f)
	, m_fAccel(0.f)
{
	m_vPos = vPos;
	m_eType = MOB_PINKSLIME;
	//m_ObjTag = L"PinkSlime";
}

CPinkSlime::CPinkSlime(const CPinkSlime& rhs)
	: CMonster(rhs)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_fTimeAcc(0.f)
	, m_fJumpTimeAcc(0.f)
	, m_fScale(rhs.m_fScale)
	, m_fHeight(rhs.m_fHeight)
{
	m_eType = MOB_PINKSLIME;
	//m_ObjTag = L"PinkSlime";
}

CPinkSlime::CPinkSlime(const CMonster& rhs)
	: CMonster(rhs)
	, m_eCurState(IDLE)
{
	m_eType = MOB_PINKSLIME;
	m_ObjTag = L"PinkSlime";
}

CPinkSlime::~CPinkSlime()
{
}

HRESULT CPinkSlime::Ready_Object(SEPARATION dID)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_eCurState = IDLE;
	m_ePreState = MOTION_END;

	m_fIdle_Speed = 1.f;
	m_fAttack_Speed = 2.f;

	// jump variable
	m_fJSpeed = 0.2f;
	m_fJSpeed0 = 0.2f;
	m_fAccel = 0.01f;

	if (m_bClone)
		m_vPos = m_pTransCom->Get_Pos();

	m_eSeparation = dID;
	if (m_eSeparation == SEPARATION_ONE)
	{
		m_tInfo.iHp = 4;
		m_tInfo.iAttack = 1;
		m_tInfo.iExp = 1;
		m_fScale = 4.f;
		m_fHeight = m_vPos.y;
	}
	else if (m_eSeparation == SEPARATION_TWO || m_eSeparation == SEPARATION_THREE)
	{
		m_tInfo.iHp = 5;
		m_tInfo.iAttack = 2;
		m_tInfo.iExp = 1;
		m_fScale = 2.f;
		m_fHeight = m_vPos.y;
	}
	else if (m_eSeparation == SEPARATION_FOUR || m_eSeparation == SEPARATION_FIVE)
	{
		m_tInfo.iHp = 3;
		m_tInfo.iAttack = 3;
		m_tInfo.iExp = 5;
		m_fScale = 1.f;
		m_fHeight = m_vPos.y;
	}

	m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);
	m_pTransCom->Set_Scale(m_fScale, m_fScale, m_fScale);

	return S_OK;
}

_int CPinkSlime::Update_Object(const _float & fTimeDelta)
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


	if (m_bPinkDead)
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
		SKill_Update(fTimeDelta);

	return 0;
}

void CPinkSlime::LateUpdate_Object(void)
{
	CMonster::Billboard();
	Engine::CGameObject::LateUpdate_Object();
}

void CPinkSlime::Render_Obejct(void)
{
	if (!m_bRenderOFF)
		CMonster::Render_Obejct();
}

HRESULT CPinkSlime::Add_Component(void)
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

	m_pAnimtorCom->Add_Component(L"Proto_PinkSlimeIDLE_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_PinkSlimeATTACK_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_PinkSlimeHIT_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_PinkSlimeDIE_Texture");

	return S_OK;
}

void CPinkSlime::SKill_Update(const _float & fTimeDelta)
{
	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransformCom);

	_vec3		vPlayerPos, vPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	_float fDist = D3DXVec3Length(&(vPlayerPos - vPos));

	if (m_eSeparation == SEPARATION_ONE)
	{
		if (m_tInfo.iHp <= 0)
		{
			CLayer*   pLayer = Engine::Get_Layer(L"Layer_GameLogic");
			CGameObject* pGameObject = CPinkSlime::Create(m_pGraphicDev, _vec3(vPlayerPos.x - 3.f, -97.f, vPlayerPos.z - 3.f), SEPARATION_TWO);
			NULL_CHECK(pGameObject);
			pLayer->Add_GameObject(L"PinkSlime0", pGameObject);
			CMonsterMgr::GetInstance()->Add_Monster(pGameObject);

			pGameObject = CPinkSlime::Create(m_pGraphicDev, _vec3(vPlayerPos.x + 3.f, -97.f, vPlayerPos.z + 3.f), SEPARATION_THREE);
			NULL_CHECK(pGameObject);
			pLayer->Add_GameObject(L"PinkSlime1", pGameObject);
			CMonsterMgr::GetInstance()->Add_Monster(pGameObject);

			m_bPinkDead = true;
		}

		if (!m_bPinkJump && fDist < 15.f)
		{
			m_bJump = true;
			Jump(fTimeDelta);
		}

		if (m_bPinkJump)
		{
			m_eCurState = ATTACK;
			m_pTransCom->Set_Y(m_fHeight);
			m_pTransCom->ChangeHeight_Target(&vPlayerPos, m_fHeight, m_fAttack_Speed, fTimeDelta);
		}
	}
	else if (m_eSeparation == SEPARATION_TWO)
	{
		if (m_tInfo.iHp <= 0)
		{
			CLayer*   pLayer = Engine::Get_Layer(L"Layer_GameLogic");
			CGameObject* pGameObject = CPinkSlime::Create(m_pGraphicDev, _vec3(vPlayerPos.x - 4.f, -98.f, vPlayerPos.z - 3.f), SEPARATION_FOUR);
			NULL_CHECK(pGameObject);
			pLayer->Add_GameObject(L"PinkSlime2", pGameObject);
			CMonsterMgr::GetInstance()->Add_Monster(pGameObject);

			pGameObject = CPinkSlime::Create(m_pGraphicDev, _vec3(vPlayerPos.x + 4.f, -98.f, vPlayerPos.z + 3.f), SEPARATION_FOUR);
			NULL_CHECK(pGameObject);
			pLayer->Add_GameObject(L"PinkSlime3", pGameObject);
			CMonsterMgr::GetInstance()->Add_Monster(pGameObject);

			m_bPinkDead = true;
		}

		m_eCurState = IDLE;

		if (!m_bPinkJump && fDist < 17.f)
		{
			m_bJump = true;
			Jump(fTimeDelta);
		}

		if (m_bPinkJump)
		{
			m_eCurState = ATTACK;
			m_pTransCom->Set_Y(m_fHeight);
			m_pTransCom->ChangeHeight_Target(&vPlayerPos, m_fHeight, m_fAttack_Speed, fTimeDelta);
		}
	}
	else if (m_eSeparation == SEPARATION_THREE)
	{
		if (m_tInfo.iHp <= 0)
		{
			CLayer*   pLayer = Engine::Get_Layer(L"Layer_GameLogic");
			CGameObject* pGameObject = CPinkSlime::Create(m_pGraphicDev, _vec3(vPlayerPos.x - 3.f, -98.f, vPlayerPos.z + 2.f), SEPARATION_FIVE);
			NULL_CHECK(pGameObject);
			pLayer->Add_GameObject(L"PinkSlime4", pGameObject);
			CMonsterMgr::GetInstance()->Add_Monster(pGameObject);

			pGameObject = CPinkSlime::Create(m_pGraphicDev, _vec3(vPlayerPos.x + 3.f, -98.f, vPlayerPos.z - 2.f), SEPARATION_FIVE);
			NULL_CHECK(pGameObject);
			pLayer->Add_GameObject(L"PinkSlime5", pGameObject);
			CMonsterMgr::GetInstance()->Add_Monster(pGameObject);

			m_bPinkDead = true;
		}

		m_eCurState = IDLE;

		if (!m_bPinkJump && fDist < 17.f)
		{
			m_bJump = true;
			Jump(fTimeDelta);
		}

		if (m_bPinkJump)
		{
			m_eCurState = ATTACK;
			m_pTransCom->Set_Y(m_fHeight);
			m_pTransCom->ChangeHeight_Target(&vPlayerPos, m_fHeight, m_fAttack_Speed, fTimeDelta);
		}
	}
	else if (m_eSeparation == SEPARATION_FOUR || m_eSeparation == SEPARATION_FIVE)
	{
		if (m_tInfo.iHp <= 0)
			m_bPinkDead = true;

		if (!m_bPinkJump && fDist < 10.f)
		{
			m_bJump = true;
			Jump(fTimeDelta);
		}

		if (m_bPinkJump)
		{
			m_eCurState = ATTACK;
			m_pTransCom->Set_Y(m_fHeight);
			m_pTransCom->ChangeHeight_Target(&vPlayerPos, m_fHeight, m_fAttack_Speed, fTimeDelta);
		}
	}
}

void CPinkSlime::Jump(const _float & fTimeDelta)
{
	if (m_bJump)
	{
		_vec3 vPos;
		m_pTransCom->Get_Info(INFO_POS, &vPos);

		if (m_fJumpTimeAcc > 0.3f && m_fHeight >= vPos.y)
		{
			CParticleMgr::GetInstance()->Set_Info(this, 25, 1.f, { 1.f, 1.f, 1.f },
				1.f, { 1.f, 1.f, 1.f, 1.f }, 5.f, true);
			CParticleMgr::GetInstance()->Call_Particle(PTYPE_FOUNTAIN, TEXTURE_12);

			CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
			pPlayer->Set_KnockBack();

			m_bJump = false;
			m_bPinkJump = true;

			m_fJumpTimeAcc = 0.f;
			m_fJSpeed = m_fJSpeed0;
			m_pTransCom->Set_Y(m_fHeight);
		}
		else
		{
			m_fJSpeed -= m_fAccel;
			m_pTransCom->Plus_PosY(m_fJSpeed);
			m_fJumpTimeAcc += 0.01f;
		}
	}
}

void CPinkSlime::OnHit(const _float & fTimeDelta)
{
	if (!m_bHit)
		return;

	if (!m_bOneCheck)
	{
		m_eCurState = HIT;
		CMonster::KnockBack(fTimeDelta, m_fHeight);

		CParticleMgr::GetInstance()->Set_Info(this, 1, 0.7f, { 1.f, 1.f, 0.f },
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

void CPinkSlime::Dead()
{
	if (m_bDead)
		return;

	static _int			m_tKeyCount;
	m_tKeyCount++;
	m_eCurState = DIE;
	//cout << m_tKeyCount << endl;
	
	CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	pPlayer->Set_Level(m_tInfo.iHp, m_tInfo.iExp);

	CParticleMgr::GetInstance()->Set_Info(this,
		50,
		0.1f,
		{ 0.5f, 0.5f, 0.5f },
		1.f,
		{ 1.f, 0.2f, 0.8f, 1.f }); // pink
	CParticleMgr::GetInstance()->Call_Particle(PTYPE_FOUNTAIN, TEXTURE_5);

	if (m_tKeyCount == 7)
	{
		//cout << "key" << endl;
		CItemMgr::GetInstance()->Add_GameObject_Box(L"Key", ITEM_KEY, m_pTransCom->Get_Pos());
	}

	m_pColliderCom->Set_Free(true);
	m_bDead = true;
}

void CPinkSlime::CollisionEvent(CGameObject* pObj)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);
	if (pPlayer != pObj)
		m_bHit = true;
}

void CPinkSlime::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_pAnimtorCom->Change_Animation(L"Proto_PinkSlimeIDLE_Texture");
			break;

		case ATTACK:
			Engine::StopSound(SOUND_PINKSLIME);
			Engine::Play_Sound(L"M_PinkSlime_Jump.mp3", SOUND_PINKSLIME, 1.f);
			m_pAnimtorCom->Change_Animation(L"Proto_PinkSlimeATTACK_Texture");
			break;

		case HIT:
			Engine::StopSound(SOUND_PINKSLIME);
			Engine::Play_Sound(L"M_PinkSlime_Hit.mp3", SOUND_PINKSLIME, 1.f);
			m_pAnimtorCom->Change_Animation(L"Proto_PinkSlimeHIT_Texture");
			break;

		case DIE:
			Engine::StopSound(SOUND_PINKSLIME);
			Engine::Play_Sound(L"M_PinkSlime_Die.mp3", SOUND_PINKSLIME, 1.f);
			m_pAnimtorCom->Change_Animation(L"Proto_PinkSlimeDIE_Texture");
			break;
		}
		m_ePreState = m_eCurState;
	}
}

CPinkSlime * CPinkSlime::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, SEPARATION dID)
{
	CPinkSlime *	pInstance = new CPinkSlime(pGraphicDev, vPos);

	if (FAILED(pInstance->Ready_Object(dID)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CPinkSlime * CPinkSlime::Create(const CPinkSlime & rhs, SEPARATION dID)
{
	CPinkSlime *	pInstance = new CPinkSlime(rhs);

	if (FAILED(pInstance->Ready_Object(dID)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CPinkSlime * CPinkSlime::Create(CMonster * pMonster)
{
	CPinkSlime *	pInstance = new CPinkSlime(*pMonster);

	if (FAILED(pInstance->Ready_Object(SEPARATION_ONE)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPinkSlime::Free(void)
{
	CMonster::Free();
}