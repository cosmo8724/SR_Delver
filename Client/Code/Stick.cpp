#include "stdafx.h"
#include "..\Header\Stick.h"

#include "Export_Function.h"

#include "BulletMgr.h"
#include "MiniMap.h"

CStick::CStick(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_eSkill(SKILL_END)
	, m_iTransparency(0)
	, m_fTimeAcc(0.f)
	, m_fTransparencyTimeAcc(0.f)
	, m_AngerTimeAcc(0.f)
{
}

CStick::~CStick()
{
}

HRESULT CStick::Ready_Object(_int iAngerCount)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.iHp = 5;
	m_tInfo.iAttack = 2;

	m_pTransCom->Set_Pos(11.f, 1.f, 12.f);

	m_eCurState = IDLE;
	m_eSkill = SKILL_ANGER;

	m_fIdle_Speed = 1.f;
	m_fAttack_Speed = 2.f;

	m_iAngerCount = iAngerCount;
	m_iTransparency = 255;

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

	m_pAnimtorCom->Play_Animation(fTimeDelta);

	Motion_Change(fTimeDelta);
	Target_Follow(fTimeDelta);
	//Skill_Update(fTimeDelta);

	_vec3 vTest;
	m_pTransCom->Get_Info(INFO_POS, &vTest);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	m_fTransparencyTimeAcc += fTimeDelta;

	return 0;
}

void CStick::LateUpdate_Object(void)
{
	if (!m_bFire)
		return;


	// 투명도 조절
	//if (0.1f < m_fTransparencyTimeAcc)
	//{
	//	m_iTransparency += 10;
	//	m_fTransparencyTimeAcc = 0.f;
	//}

	CMonster::Billboard();
	Engine::CGameObject::LateUpdate_Object();
}

void CStick::Render_Obejct(void)
{
	if (!m_bFire)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iTransparency, 0, 0, 0));
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x05);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	m_pAnimtorCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	CMonster::Render_Obejct();
}

void CStick::CollisionEvent(CGameObject * pObj)
{
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
	// 몬스터 좌우로 이동하기
	_vec3		vRight;
	m_pTransCom->Get_Info(INFO_RIGHT, &vRight);

	m_fTimeAcc += fTimeDelta;
	if (2.f < m_fTimeAcc)
	{
		m_fIdle_Speed *= -1;
		m_fTimeAcc = 0.f;
	}

	D3DXVec3Normalize(&vRight, &vRight);
	m_pTransCom->Move_Pos(&(vRight * m_fIdle_Speed * fTimeDelta));
}

void CStick::SkillAttack_Update(const _float & fTimeDelta)
{
}

void CStick::SkillAnger_Update(const _float & fTimeDelta)
{
	//if (Engine::Key_Down(DIK_9))
	//	CBulletMgr::GetInstance()->Fire(STICK_ANGER);

	// TODO 시간 초를 주어 객체가 스르르 순서대로 생성되도록 해야함
	m_AngerTimeAcc += fTimeDelta;
	if (3.f < m_AngerTimeAcc)
	{
		//CBulletMgr::GetInstance()->Fire(STICK_ANGER);
		m_AngerTimeAcc = 0;
	}

	if (!m_bFire)
		return;

	if (!m_bReady)
	{

		_vec3 vPos;
		m_pTransCom->Get_Info(INFO_POS, &vPos);

		if (m_iAngerCount == 0)
		{
			m_iTransparency = 255;
			m_pTransCom->Set_Pos(vPos.x + 2.f, vPos.y, vPos.z);

		}
		else if (m_iAngerCount == 2)
		{
			m_iTransparency = 213;
			m_pTransCom->Set_Pos(vPos.x + 1.5f, vPos.y, vPos.z + 0.1f);
			m_fLifeTime += fTimeDelta;

			if (4.f < m_fLifeTime)
			{
				Reset();
				m_fLifeTime = 0;
			}
		}
		else if (m_iAngerCount == 4)
		{
			m_iTransparency = 172;
			m_pTransCom->Set_Pos(vPos.x + 1.f, vPos.y, vPos.z + 0.2f);
			m_fLifeTime += fTimeDelta;

			if (3.f < m_fLifeTime)
			{
				Reset();
				m_fLifeTime = 0;
			}
		}
		else if (m_iAngerCount == 1)
		{
			m_iTransparency = 131;
			m_pTransCom->Set_Pos(vPos.x + 0.5f, vPos.y, vPos.z + 0.3f);
			m_fLifeTime += fTimeDelta;

			if (2.f < m_fLifeTime)
			{
				Reset();
				m_fLifeTime = 0;
			}
		}
		else if (m_iAngerCount == 3)
		{
			m_iTransparency = 90;
			m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z + 0.4f);

			m_fLifeTime += fTimeDelta;

			if (1.f < m_fLifeTime)
			{
				Reset();
				m_fLifeTime = 0;
			}
		}
		m_bReady = true;
	}
}

void CStick::Motion_Change(const _float& fTimeDelta)
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

void CStick::Reset()
{
	m_bFire = false;
	m_bDead = false;
	m_fLifeTime = 0.f;
	m_fFrame = 0.f;
	m_bReady = false;
}