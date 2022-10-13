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
	, m_eSkill(SKILL_END)
	, m_fMoveTimeAcc(0.f)

{
	m_ObjTag = L"Stick";
}

CStick::~CStick()
{
}

HRESULT CStick::Ready_Object(_int iAngerCount)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.iHp = 5;
	m_tInfo.iAttack = 2;

	m_OriginalPos = { 10.f, 1.f, 10.f };
	m_pTransCom->Set_Pos(m_OriginalPos.x, m_OriginalPos.y, m_OriginalPos.z);
	m_pTransCom->Set_Scale(0.7f, 0.7f, 0.7f);

	m_eCurState = IDLE;
	m_eSkill = SKILL_ANGER;

	m_fIdle_Speed = 1.f;
	m_fAttack_Speed = 3.f;

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

	m_pAnimtorCom->Play_Animation(fTimeDelta);
	Motion_Change();

	if (!m_bParticle)
	{
		CParticleMgr::GetInstance()->Set_Info(this, 5, 1.f, { 1.f, 1.f, 1.f },
		1.f, { 1.f,1.f,1.f, 0.1f });
		CParticleMgr::GetInstance()->Call_Particle(PTYPE_TRACER, TEXTURE_7);
		m_bParticle = true;
	}






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
		Skill_Update(fTimeDelta);

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
	// 플레이어 따라가기
	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransformCom);

	_vec3		vPlayerPos, vPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	_float fDist = D3DXVec3Length(&(vPlayerPos - vPos));
}

void CStick::Skill_Update(const _float & fTimeDelta)
{
	switch (m_eSkill)
	{
	case Engine::CStick::SKILL_MOVE:
		SkillMove_Update(fTimeDelta);
		break;
	case Engine::CStick::SKILL_ATTACK:
		SkillAttack_Update(fTimeDelta);
		break;
	case Engine::CStick::SKILL_ANGER:
		SkillAnger_Update(fTimeDelta);
		break;
	case Engine::CStick::SKILL_END:
		break;
	}
}

void CStick::SkillMove_Update(const _float & fTimeDelta)
{
	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransformCom);

	_vec3		vPlayerPos, vPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	_vec3 vRight, vUp, vLook;
	vLook = vPlayerPos - vPos;
	D3DXVec3Normalize(&vLook, &vLook);
	vUp = _vec3(0.f, 1.f, 0.f);

	D3DXVec3Cross(&vRight, &vUp, &vLook);

	m_fMoveTimeAcc += fTimeDelta;
	if (1.f < m_fMoveTimeAcc)
	{
		m_fIdle_Speed *= -1;
		m_fMoveTimeAcc = 0.f;
	}

	D3DXVec3Normalize(&vRight, &vRight);
	m_pTransCom->Move_Pos(&(vRight * m_fIdle_Speed * fTimeDelta));
}

void CStick::SkillAttack_Update(const _float & fTimeDelta)
{
}

void CStick::SkillAnger_Update(const _float & fTimeDelta)
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
		if (2.f < m_fMoveTimeAcc)
		{
			m_pTransCom->Chase_Target(&vPlayerPos, m_fAttack_Speed, fTimeDelta);



			m_fMoveTimeAcc = 0.f;
		}




	}
	else
		m_eCurState = IDLE;
}

void CStick::Teleporting(const _float & fPlayerPosX, const _float & fPlayerPosZ)
{
	int iRandomNum = rand() % 7 + 2;

	if (fPlayerPosX == (m_OriginalPos.x + iRandomNum) || fPlayerPosZ == (m_OriginalPos.z + iRandomNum) ||
		fPlayerPosX == (m_OriginalPos.x - iRandomNum) || fPlayerPosZ == (m_OriginalPos.z - iRandomNum))
		return; // PlayerPos == MonsterPos -> return

	if (iRandomNum % 2 == 0) // 짝수
	{
		if (iRandomNum < 5) // 5보다 크면으로 한 번 더 렌덤
			m_pTransCom->Set_Pos(m_OriginalPos.x - iRandomNum, 1.f, m_OriginalPos.z + iRandomNum);
		else
			m_pTransCom->Set_Pos(m_OriginalPos.x + iRandomNum, 1.f, m_OriginalPos.z + iRandomNum);
	}
	else // 홀수
	{
		if (iRandomNum < 5)
			m_pTransCom->Set_Pos(m_OriginalPos.x + iRandomNum, 1.f, m_OriginalPos.z - iRandomNum);
		else
			m_pTransCom->Set_Pos(m_OriginalPos.x - iRandomNum, 1.f, m_OriginalPos.z + iRandomNum);
	}
}

void CStick::OnHit(const _float & fTimeDelta)
{
	if (!m_bHit)
		return;

	if (!m_bOneCheck)
	{
		m_eCurState = HIT;
		CMonster::Set_KnockBack();
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

CStick * CStick::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iAngerCount)
{
	CStick *	pInstance = new CStick(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iAngerCount)))
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