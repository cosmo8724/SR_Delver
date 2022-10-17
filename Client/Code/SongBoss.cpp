#include "stdafx.h"
#include "..\Header\SongBoss.h"

#include "Export_Function.h"

#include "BulletMgr.h"
#include "MiniMap.h"
#include "SongBossFloor.h"

// �浹
#include "Player.h"
#include "ParticleMgr.h"
#include "ItemMgr.h"

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

CSongBoss::CSongBoss(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	:CMonster(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_eSkill(SKILL_END)
	, m_fTimeAcc(0.f)
	, m_fIdleTimeAcc(0.f)
	, m_fAttackTimeAcc(0.f)
{
	m_vPos = vPos;
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

	m_fHeight = m_vPos.y;
	m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	m_eCurState = IDLE;
	m_eSkill = SKILL_BULLET;

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

	//m_pTransCom->Set_Y(m_fHeight);
	m_pAnimtorCom->Play_Animation(fTimeDelta * 0.7f); // TODO ������ HIT, DIE�� �ӵ� ���� �ؾ� �� ����
	Motion_Change(fTimeDelta);

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
		SKill_Update(fTimeDelta);	

	return 0;
}

void CSongBoss::LateUpdate_Object(void)
{
	CMonster::Billboard();
	Engine::CGameObject::LateUpdate_Object();
}

void CSongBoss::Render_Obejct(void)
{
	if (!m_bRenderOFF)
		CMonster::Render_Obejct();
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
	if (m_bSKill)
		return;

	//// �ڵ� - �̿ϼ�
	//if (2 < m_bSkillBullet && !m_bSkillStun && !m_bSkillFloor)
	//{
	//	m_fSkillTimeAcc += fTimeDelta;
	//	if (2.f < m_fSkillTimeAcc)
	//	{
	//		int m_eSkill = rand() % 3;
	//		m_fSkillTimeAcc = 0.f;

	//		cout << m_eSkill << endl;

	//		switch (m_eSkill)
	//		{
	//		case CSongBoss::SKILL_BULLET:
	//		{
	//			m_bSkillBullet = 0;
	//			m_bBullet = true;
	//			SKillBullet_Update(fTimeDelta);
	//		}
	//		break;
	//		case CSongBoss::SKILL_STUN:
	//		{
	//			m_bSkillStun = true;
	//			m_bStun = true;
	//			m_iStunCount = 0;
	//			m_iStunCreate = 0;
	//			SKillStun_Update(fTimeDelta);
	//		}
	//		break;
	//		case CSongBoss::SKILL_FLOOR:
	//		{
	//			m_bSkillFloor = true;
	//			m_bFloor = true;
	//			m_iFloorCreate = 0;
	//			m_iLightningCreate = 0;
	//			m_bFloorOneCheck = true;
	//			SKillFloor_Update(fTimeDelta);
	//		}
	//		break;
	//		}

	//		m_bSKill = true;
	//	}
	//}

	// ����
	if (Key_Down(DIK_7))
	{
		m_bSkillBullet = 0;
		m_bBullet = true;
		m_eSkill = SKILL_BULLET;
	}
	if (Key_Down(DIK_8))
	{
		m_bSkillStun = true;
		m_bStun = true;
		m_iStunCount = 0;
		m_iStunCreate = 0;
		m_eSkill = SKILL_STUN;
	}
	if (Key_Down(DIK_9))
	{
		m_bSkillFloor = true;
		m_bFloor = true;
		m_iFloorCreate = 0;
		m_iLightningCreate = 0;
		m_bFloorOneCheck = true;
		m_eSkill = SKILL_FLOOR;
	}

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
	if (2 < m_bSkillBullet)
	{
		m_eCurState = IDLE;
		m_bSKill = false;
		return;
	}

	if (!m_bBullet)
		return;

	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransformCom);

	_vec3		vPlayerPos, vPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	// MonsterLook -> Player
	_vec3 vRight, vUp, vLook;
	vLook = vPlayerPos - vPos;
	m_pTransCom->Set_Look(&vLook);

	_float fDist = D3DXVec3Length(&(vPlayerPos - vPos));

	// ���� �Ÿ� �� ���� ��� ���� �� ���� ����
	if (fDist < 10.f)
	{
		m_fAttackTimeAcc += fTimeDelta;
		m_fIdleTimeAcc += m_fAttackTimeAcc;

		if (3.f < m_fAttackTimeAcc)
		{
			m_eCurState = ATTACK;
			CBulletMgr::GetInstance()->Fire(BULLET_SONGBOSS);
			m_bSkillBullet++;
			m_fAttackTimeAcc = 0;
		}
		else if (5.5f < m_fIdleTimeAcc)
		{
			m_eCurState = IDLE;
			m_fIdleTimeAcc = 0.f;
		}
	}
}

void CSongBoss::SKillStun_Update(const _float & fTimeDelta)
{
	if (!m_bSkillStun)
		return;

	if (!m_bStun)
		return;

	// ���� �ð� ���� ��ǥ�� �� �μž� �ϰ�, �� �ν��� �� �ϸ� �÷��̾�� ���� + ������ ü�� ����
	m_eCurState = ATTACK;

	if (m_iStunCreate != 4) // MusicNote Create > 4
	{
		if (m_pAnimtorCom->Get_Currentframe() >= 9.f)
		{
			CBulletMgr::GetInstance()->Fire(STUN_SONGBOSS);
			++m_iStunCreate;
		}
	}
	else
	{
		m_eCurState = IDLE;

		m_fStunTimeAcc += fTimeDelta;
		if (4.f < m_fStunTimeAcc)  // 5.f >> �̳� ��ǥ�� �� �νø� ���� (�� ������ SongBossStun.cpp > LateUpdate�� �����ؾ���)
		{						   // �÷��̾��� ���Ͻð��� ����ϸ鼭 ���� �����ؾ� �Ѵ�
			if (m_iStunCount != 4) // Player Stun
			{
				CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
				pPlayer->Set_Stun();
			}
			m_fStunTimeAcc = 0.f;
			m_bStun = false;
			m_bSkillStun = false;
			m_bSKill = false;
		}
	}
}

void CSongBoss::SKillFloor_Update(const _float & fTimeDelta)
{
	if (!m_bSkillFloor)
		return;

	if (!m_bFloor)
		return;

	// �÷��̾ �������� 5���� ��ǥ�� ����� ���ؾ� �Ѵ�
	m_eCurState = ATTACK;

	if (m_iFloorCreate != 5) // MusicNote Create > 5
	{
		if (m_pAnimtorCom->Get_Currentframe() >= 9.f)
		{
			CBulletMgr::GetInstance()->Fire(FLOOR_SONGBOSS);
			++m_iFloorCreate;
		}
	}
	else
	{
		m_eCurState = IDLE;

		wstring objTags[5];
		for (_int i = 0; i < 5; i++)
		{
			objTags[i] = L"SongBoss_Floor";
			wchar_t index[10];
			_itow_s(i, index, 10);
			objTags[i] += index;

			CSongBossFloor* pSongBossFloor = static_cast<CSongBossFloor*>(Engine::Get_GameObject(L"Layer_GameLogic", objTags[i].c_str()));
			NULL_CHECK(pSongBossFloor);
			if (pSongBossFloor->Get_StartLightning())
			{
				CBulletMgr::GetInstance()->Fire(LIGHTNING_SONGBOSS);
				++m_iLightningCreate;
			}
		}

		if (m_iLightningCreate >= 5)
		{
			m_bFloor = false;
			m_bSkillFloor = false;
		}
	}
}

void CSongBoss::OnHit(const _float & fTimeDelta)
{
	if (!m_bSkillFloor)
		return;

	if (!m_bHit)
		return;

	if (!m_bOneCheck)
	{
		m_eCurState = HIT;
		//CMonster::Set_KnockBack();
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

void CSongBoss::Dead()
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

void CSongBoss::CollisionEvent(CGameObject * pObj)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);
	if (pPlayer != pObj)
		m_bHit = true;
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

CSongBoss * CSongBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CSongBoss *	pInstance = new CSongBoss(pGraphicDev, vPos);

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
