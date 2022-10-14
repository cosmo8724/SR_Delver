#include "stdafx.h"
#include "..\Header\SongBoss.h"

#include "Export_Function.h"

#include "BulletMgr.h"
#include "MiniMap.h"
#include "Player.h"
#include "SongBossFloor.h"

CSongBoss::CSongBoss(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_eSkill(SKILL_END)
	, m_fTimeAcc(0.f)
	, m_fIdleTimeAcc(0.f)
	, m_fAttackTimeAcc(0.f)
{
	m_ObjTag = L"SongBoss";
}

CSongBoss::~CSongBoss()
{
}

HRESULT CSongBoss::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.iHp = 30;
	m_tInfo.iAttack = 5;

	m_pTransCom->Set_Scale(1.5f, 1.5f, 1.5f);
	//m_pTransCom->Set_Pos(10.f, 6.5f, 10.f);
	m_pTransCom->Set_Pos(10.f, 1.5f, 10.f);

	m_eCurState = IDLE;
	m_eSkill = SKILL_FLOOR;

	m_fIdle_Speed = 1.f;
	m_fAttack_Speed = 2.f;

	return S_OK;
}

_int CSongBoss::Update_Object(const _float & fTimeDelta)
{
	if (!m_bCreateIcon)
	{
		CMiniMap* pMiniMap = dynamic_cast<CMiniMap*>(Engine::Get_GameObject(L"Layer_UI", L"UI_MiniMap"));
		pMiniMap->Add_Icon(m_pGraphicDev, this);
		m_bCreateIcon = true;
	}
	Engine::CMonster::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	//m_pTransCom->Set_Y(1.5f);
	m_pAnimtorCom->Play_Animation(fTimeDelta * 0.5f); // TODO 보스의 HIT, DIE의 속도 조절해야함

	Motion_Change(fTimeDelta);
	SKill_Update(fTimeDelta);

	if (Engine::Get_DIKeyState(DIK_1) && 0x08)
	{
		m_eSkill = SKILL_BULLET;
	}
	if (Engine::Get_DIKeyState(DIK_2) && 0x08)
	{
		m_eSkill = SKILL_STUN;
	}
	if (Engine::Get_DIKeyState(DIK_3) && 0x08)
	{
		m_eSkill = SKILL_FLOOR;
	}

	return 0;
}

void CSongBoss::LateUpdate_Object(void)
{
	CMonster::Billboard();
	Engine::CGameObject::LateUpdate_Object();
}

void CSongBoss::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_BOTHINVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x05);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pAnimtorCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	CMonster::Render_Obejct();
}

void CSongBoss::CollisionEvent(CGameObject * pObj)
{
}

HRESULT CSongBoss::Add_Component(void)
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

	m_pAnimtorCom->Add_Component(L"Proto_SongBossMOVE_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_SongBossIDLE_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_SongBossATTACK_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_SongBossHIT_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_SongBossDIE_Texture");

	return S_OK;
}

void CSongBoss::SKill_Update(const _float & fTimeDelta)
{
	switch (m_eSkill)
	{
	case CSongBoss::SKILL_BULLET:
		SKillBullet_Update(fTimeDelta);
		break;
	case CSongBoss::SKILL_STUN:
		SKillStun_Update(fTimeDelta);
		break;
	case CSongBoss::SKILL_FLOOR:
		SKillFloor_Update(fTimeDelta);
		break;
	case CSongBoss::SKILL_END:
		break;
	}
}

void CSongBoss::SKillBullet_Update(const _float & fTimeDelta)
{
	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransformCom);

	_vec3		vPlayerPos, vPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	_float fDist = D3DXVec3Length(&(vPlayerPos - vPos));

	// 일정 거리 안 으로 들어 왔을 때 공격 시작
	if (fDist < 20.f)
	{
		m_fAttackTimeAcc += fTimeDelta;
		m_fIdleTimeAcc += m_fAttackTimeAcc;

		if (3.f < m_fAttackTimeAcc)
		{
			m_eCurState = ATTACK;
			CBulletMgr::GetInstance()->Fire(BULLET_SONGBOSS);
			m_fAttackTimeAcc = 0;
		}
		else if (5.5f < m_fIdleTimeAcc)
		{
			m_eCurState = IDLE;
			m_fIdleTimeAcc = 0.f;
		}
	}
	else
		m_eCurState = IDLE;
}

void CSongBoss::SKillStun_Update(const _float & fTimeDelta)
{
	// 일정 시간 내에 음표를 다 부셔야 하고, 다 부시지 못 하면 플레이어는 스턴 + 보스의 체력 증가
	m_eCurState = ATTACK;

	if (m_pAnimtorCom->Get_Currentframe() >= 7.f && m_pAnimtorCom->Get_Currentframe() < 8.f)
		CBulletMgr::GetInstance()->Fire(STUN_SONGBOSS);

	m_fStunTimeAcc += fTimeDelta;
	if (7.f < m_fStunTimeAcc) // 7.f >> 이내 음표를 못 부시면 스턴 (값 수정시 SongBossStun.cpp > LateUpdate도 수정해야함)
	{
		if (m_iStunCount != 4)
		{
			int a = m_iStunCount;
			CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
			pPlayer->Set_Stun();
		}
		m_fStunTimeAcc = 0.f;
	}
}

void CSongBoss::SKillFloor_Update(const _float & fTimeDelta)
{
	// 플레이어를 기준으로 5개의 음표가 생기고 피해야 한다
	m_eCurState = ATTACK;

	if (m_pAnimtorCom->Get_Currentframe() >= 8.f)
	{
		CBulletMgr::GetInstance()->Fire(FLOOR_SONGBOSS);

		//CAnimator* pFloorAnimator = dynamic_cast<CAnimator*>(Engine::Get_Component(L"Layer_GameLogic", L"SongBoss_FloorLightning", L"Proto_AnimatorCom", ID_DYNAMIC));
		//NULL_CHECK(pFloorAnimator);

		//pFloorAnimator->Get_Currentframe();
		m_fLightningTimeAcc += fTimeDelta;
		if (0.8f < m_fLightningTimeAcc)
		{
			CSongBossFloor* pSongBossFloor = static_cast<CSongBossFloor*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SongBoss_Floor0"));
			NULL_CHECK(pSongBossFloor);
			pSongBossFloor->Set_Attack();

			pSongBossFloor = static_cast<CSongBossFloor*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SongBoss_Floor1"));
			NULL_CHECK(pSongBossFloor);
			pSongBossFloor->Set_Attack();

			pSongBossFloor = static_cast<CSongBossFloor*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SongBoss_Floor2"));
			NULL_CHECK(pSongBossFloor);
			pSongBossFloor->Set_Attack();

			pSongBossFloor = static_cast<CSongBossFloor*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SongBoss_Floor3"));
			NULL_CHECK(pSongBossFloor);
			pSongBossFloor->Set_Attack();

			pSongBossFloor = static_cast<CSongBossFloor*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SongBoss_Floor4"));
			NULL_CHECK(pSongBossFloor);
			pSongBossFloor->Set_Attack();

			CBulletMgr::GetInstance()->Fire(LIGHTNING_SONGBOSS);
			m_fLightningTimeAcc = 0.f;
		}
	}
}

void CSongBoss::Motion_Change(const _float & fTimeDelta)
{
		if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_pAnimtorCom->Change_Animation(L"Proto_SongBossIDLE_Texture");
			break;

		case MOVE:
			m_pAnimtorCom->Change_Animation(L"Proto_SongBossMOVE_Texture");
			break;

		case ATTACK:
			m_pAnimtorCom->Change_Animation(L"Proto_SongBossATTACK_Texture");
			break;
		
		case HIT:
			m_pAnimtorCom->Change_Animation(L"Proto_SongBossHIT_Texture");
			break;

		case DIE:
			m_pAnimtorCom->Change_Animation(L"Proto_SongBossDIE_Texture");
			break;
		}
		m_ePreState = m_eCurState;
	}
}

CSongBoss * CSongBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSongBoss *	pInstance = new CSongBoss(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSongBoss::Free(void)
{
	CMonster::Free();
}
