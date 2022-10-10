#include "stdafx.h"
#include "..\Header\PinkSlime.h"

#include "Export_Function.h"

CPinkSlime::CPinkSlime(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_eSkill(SKILL_END)
	, m_eSkill_Scale(SKILLSCALE_END)
	, m_fTimeAcc(0.f)
	, m_fJumpTimeAcc(0.f)
	, m_fScale(0.f)
	, m_fHeight(0.f)
	, m_bJump(false)
	, m_fJSpeed(0.f)
	, m_fJSpeed0(0.f)
	, m_fAccel(0.f)
{
}

CPinkSlime::CPinkSlime(const CPinkSlime& rhs)
	: CMonster(rhs)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_fTimeAcc(0.f)
	, m_fJumpTimeAcc(0.f)
	, m_fScale(rhs.m_fScale)
	, m_fHeight(rhs.m_fHeight)
	, m_bClone(true)
{
}

CPinkSlime::~CPinkSlime()
{
}

HRESULT CPinkSlime::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.iHp = 20;
	m_tInfo.iAttack = 1;

	m_eCurState = IDLE;

	m_fIdle_Speed = 1.f;
	m_fAttack_Speed = 2.f;

	m_fScale = 2.f;
	m_fHeight = 2.f;

	// 점프 관련 변수 초기화
	m_fJSpeed = 5.f;
	m_fJSpeed0 = 5.f;
	m_fAccel = 0.1f;

	m_pTransCom->Set_Pos(10.f, m_fHeight, 25.f);
	m_pTransCom->Set_Scale(m_fScale, m_fScale, m_fScale);

	return S_OK;
}

_int CPinkSlime::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	m_pTransCom->Set_Y(m_fHeight);

	m_pAnimtorCom->Play_Animation(fTimeDelta);

	SKill_Update(fTimeDelta);


	Motion_Change();

	return 0;
}

void CPinkSlime::LateUpdate_Object(void)
{
	CMonster::Billboard();
	Engine::CGameObject::LateUpdate_Object();
}

void CPinkSlime::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pAnimtorCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CPinkSlime::Add_Component(void)
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

	m_pAnimtorCom->Add_Component(L"Proto_PinkSlimeATTACK_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_PinkSlimeHIT_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_PinkSlimeDIE_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_PinkSlimeIDLE_Texture");

	return S_OK;
}

void CPinkSlime::SKill_Update(const _float & fTimeDelta)
{
	/*
	1) IDLE 상태로 가만히 있는다
	2) 플레이어가 다가가면 점프를 한 번 하며 플레이어를 넉백 시킨다
	3) 그리고 계속 플레이어를 따라간다
	
	플레이어에게 공격을 당할시 일정시간 가만히 있다가 플레이어를 따라간다	

	플레이어에게 공격 2번을 당하면 스케일이 줄어들면서 분리되며
	최대 3번까지 분리되며, 몬스터는 총 4개까지 존재한다
	*/

	// 플레이어에게 공격 당하면 모두 멈추고 HIT상태
	if (Engine::Key_Down(DIK_U))
	{
		m_eCurState = HIT;
		return;
	}

	// 플레이어 따라가기
	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransformCom);

	_vec3		vPlayerPos, vPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	_float fDist = D3DXVec3Length(&(vPlayerPos - vPos));

	if (!m_bSkillJumpStart && fDist < 5.f) // 처음 플레이어가 다가가면 1초 있다가 점프 한 번
	{
		m_SkillJumpTimeAcc += fTimeDelta;
		if (1.5f < m_SkillJumpTimeAcc)
		{
			m_bJump = true;
			m_eSkill = SKILL_JUMP;

			// TODO 플레이어 넉백 넣어야함
		}
		if (2.f < m_SkillJumpTimeAcc)
		{
			m_bSkillJumpStart = true;
			m_SkillJumpTimeAcc = 0.f;
		}
	}

	if (m_bSkillJumpStart)
	{
		if (!m_bSkillFollowStart)
		{
			// 점프를 한 번 하고 나면 플레이어를 계속 따라간다
			m_eSkill = SKILL_FOLLOW;
		}

		// 그러다가 공격 플레이어에게 공격을 2번 당하면 크기가 줄어들면서 분해된다
		if (Engine::Key_Down(DIK_P))
		{
			m_eSkill_Scale = SKILLSCALE_BIG;
		}
		if (Engine::Key_Down(DIK_P))
		{
			m_eSkill_Scale = SKILLSCALE_MEDIUM;
		}
		if (Engine::Key_Down(DIK_P))
		{
			m_eSkill_Scale = SKILLSCALE_SMALL;
		}
	}


	switch (m_eSkill)
	{
	case CPinkSlime::SKILL_JUMP:
		SKillJump_Update(fTimeDelta);
		break;
	case CPinkSlime::SKILL_FOLLOW:
		SKillFollow_Update(fTimeDelta, fDist, &vPlayerPos);
		break;
	case CPinkSlime::SKILL_SCALE:
		SKillScale_Update(fTimeDelta);
		break;
	}
}

void CPinkSlime::SKillJump_Update(const _float & fTimeDelta)
{
	//if (Key_Down(DIK_0))
	//	m_bJump = true;

	if (m_bJump)
	{
		_vec3 vPos;
		m_pTransCom->Get_Info(INFO_POS, &vPos);

		if (m_fJumpTimeAcc > 0.3f && m_fHeight >= vPos.y)
		{
			m_bJump = false;
			m_fJumpTimeAcc = 0.f;

			m_pTransCom->Set_Y(m_fHeight);
			m_fJSpeed = m_fJSpeed0;
		}
		else
		{
			m_fJSpeed -= m_fAccel;
			m_pTransCom->Plus_PosY(m_fJSpeed);
			m_fJumpTimeAcc += 0.01f;
		}
	}
}

void CPinkSlime::SKillFollow_Update(const _float & fTimeDelta, _float fDist, _vec3* vPlayerPos)
{
	m_eCurState = ATTACK;
	m_pTransCom->ChangeHeight_Target(vPlayerPos, m_fHeight, m_fAttack_Speed, fTimeDelta);
}

void CPinkSlime::SKillScale_Update(const _float & fTimeDelta)
{
	_float fSize = 0.f;

	CLayer*   pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	//if (Engine::Get_DIKeyState(DIK_0))
	//{
	//	pLayer->Delete_GameObject(L"PinkSlime0");
	//	pLayer->Delete_GameObject(L"PinkSlime1");
	//	pLayer->Delete_GameObject(L"PinkSlime2");
	//}

	switch (m_eSkill_Scale)
	{
	case CPinkSlime::SKILLSCALE_BIG:

		pGameObject = CPinkSlime::Create(m_pGraphicDev);
		if (pGameObject == nullptr)
		{
			MSG_BOX("PinkSlime Create Failure");
			return;
		}
		pLayer->Add_GameObject(L"PinkSlime0", pGameObject);

		fSize = 0.9f;

		m_pTransCom->Set_Scale(m_fScale * fSize, m_fScale * fSize, m_fScale * fSize);
		m_fHeight = (m_fScale * fSize);

		break;

	case CPinkSlime::SKILLSCALE_MEDIUM:

		pGameObject = CPinkSlime::Create(m_pGraphicDev);
		if (pGameObject == nullptr)
		{
			MSG_BOX("PinkSlime Create Failure");
			return;
		}
		pLayer->Add_GameObject(L"PinkSlime1", pGameObject);

		fSize = 0.7f;

		m_pTransCom->Set_Scale(m_fScale * fSize, m_fScale * fSize, m_fScale * fSize);
		m_fHeight = (m_fScale * fSize);

		break;

	case CPinkSlime::SKILLSCALE_SMALL:

		pGameObject = CPinkSlime::Create(m_pGraphicDev);
		if (pGameObject == nullptr)
		{
			MSG_BOX("PinkSlime Create Failure");
			return;
		}
		pLayer->Add_GameObject(L"PinkSlime2", pGameObject);

		fSize = 0.5f;

		m_pTransCom->Set_Scale(m_fScale * fSize, m_fScale * fSize, m_fScale * fSize);
		m_fHeight = (m_fScale * fSize);

		break;
	}
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
			m_pAnimtorCom->Change_Animation(L"Proto_PinkSlimeATTACK_Texture");
			break;

		case HIT:
			m_pAnimtorCom->Change_Animation(L"Proto_PinkSlimeHIT_Texture");
			break;

		case DIE:
			m_pAnimtorCom->Change_Animation(L"Proto_PinkSlimeDIE_Texture");
			break;
		}
		m_ePreState = m_eCurState;
	}
}

CPinkSlime * CPinkSlime::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPinkSlime *	pInstance = new CPinkSlime(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CPinkSlime * CPinkSlime::Create(const CPinkSlime & rhs)
{
	CPinkSlime *	pInstance = new CPinkSlime(rhs);

	if (FAILED(pInstance->Ready_Object()))
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