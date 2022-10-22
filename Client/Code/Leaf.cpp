#include "stdafx.h"
#include "..\Header\Leaf.h"

#include "Export_Function.h"
#include "BulletMgr.h"
#include "MiniMap.h"

// 충돌
#include "Player.h"
#include "ParticleMgr.h"
#include "ItemMgr.h"

CLeaf::CLeaf(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_OriginalPos(0.f, 0.f, 0.f)
	, m_fTimeAcc(0.f)
	, m_fTeleportingTimeAcc(0.f)
	, m_fBulletTimeAcc(0.f)
{
	m_eType = MOB_LEAF;
	m_ObjTag = L"Leaf";
}

CLeaf::CLeaf(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	:CMonster(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_OriginalPos(0.f, 0.f, 0.f)
	, m_fTimeAcc(0.f)
	, m_fTeleportingTimeAcc(0.f)
	, m_fBulletTimeAcc(0.f)
{
	m_eType = MOB_LEAF;
	m_vPos = vPos;
	m_ObjTag = L"Leaf";
}

CLeaf::CLeaf(const CMonster& rhs)
	:CMonster(rhs)
{
	m_ObjTag = L"Leaf";
}

CLeaf::~CLeaf()
{
}

HRESULT CLeaf::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.iHp = 5;
	m_tInfo.iAttack = 1;
	m_tInfo.iExp = 4;

	m_OriginalPos = { m_vPos.x, m_vPos.y, m_vPos.z };
	if (!m_bClone)
	{
		m_pTransCom->Set_Pos(m_OriginalPos.x, m_OriginalPos.y, m_OriginalPos.z);
	}
	m_eCurState = IDLE;

	m_fIdle_Speed = 1.f;
	m_fAttack_Speed = 2.f;

	return S_OK;
}

_int CLeaf::Update_Object(const _float & fTimeDelta)
{
	if (!m_bCreateIcon && !g_bIsTool)
	{
		CMiniMap* pMiniMap = dynamic_cast<CMiniMap*>(Engine::Get_GameObject(L"Layer_UI", L"UI_MiniMap"));
		pMiniMap->Add_Icon(m_pGraphicDev, this);
		m_bCreateIcon = true;
	}
	Engine::CMonster::Update_Object(fTimeDelta * 0.6f);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	m_pAnimtorCom->Play_Animation(fTimeDelta);
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
		SKillTeleporting(fTimeDelta);

	return 0;
}

void CLeaf::LateUpdate_Object(void)
{
	CMonster::Billboard();
	Engine::CGameObject::LateUpdate_Object();
}

void CLeaf::Render_Obejct(void)
{
	if (!m_bRenderOFF)
		CMonster::Render_Obejct();
}

HRESULT CLeaf::Add_Component(void)
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
	NULL_CHECK_RETURN(m_pColliderCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	m_pAnimtorCom->Add_Component(L"Proto_LeafIDLE_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_LeafATTACK_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_LeafHIT_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_LeafDIE_Texture");

	return S_OK;
}

void CLeaf::SKillTeleporting(const _float & fTimeDelta)
{
	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransformCom);

	_vec3		vPlayerPos, vPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	_float fDist = D3DXVec3Length(&(vPlayerPos - vPos));

 	if (fDist < 5.f) // 5.f Teleporting
	{
		m_eCurState = ATTACK;

		if (m_pAnimtorCom->Get_Currentframe() >= 12.f && m_pAnimtorCom->Get_Currentframe() < 13.f)
			Teleporting(vPlayerPos.x, vPlayerPos.z);
	}
	else
	{
		m_eCurState = IDLE;

		if (fDist < 10.f) // 10.f Bullet
		{
			m_fBulletTimeAcc += fTimeDelta;
			if (0.2f < m_fBulletTimeAcc)
			{
				CBulletMgr::GetInstance()->Fire(BULLET_M_LEAF);
				m_fBulletTimeAcc = 0.f;
			}
		}
	}
}

void CLeaf::Teleporting(const _float& fPlayerPosX, const _float& fPlayerPosZ)
{
	int iRandomNum = rand() % 7 + 2; 

	if (fPlayerPosX == (m_OriginalPos.x + iRandomNum) || fPlayerPosZ == (m_OriginalPos.z + iRandomNum) ||
		fPlayerPosX == (m_OriginalPos.x - iRandomNum) || fPlayerPosZ == (m_OriginalPos.z - iRandomNum))
		return; // PlayerPos == MonsterPos -> return

	if (iRandomNum % 2 == 0) // 짝수
	{
		if (iRandomNum < 5) // 5보다 크면으로 한 번 더 렌덤
			m_pTransCom->Set_Pos(m_OriginalPos.x - iRandomNum, m_vPos.y, m_OriginalPos.z + iRandomNum);
		else
			m_pTransCom->Set_Pos(m_OriginalPos.x + iRandomNum, m_vPos.y, m_OriginalPos.z + iRandomNum);
	}
	else // 홀수
	{
		if (iRandomNum < 5)
			m_pTransCom->Set_Pos(m_OriginalPos.x + iRandomNum, m_vPos.y, m_OriginalPos.z - iRandomNum);
		else 
			m_pTransCom->Set_Pos(m_OriginalPos.x - iRandomNum, m_vPos.y, m_OriginalPos.z + iRandomNum);
	}
}

void CLeaf::OnHit(const _float & fTimeDelta)
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

void CLeaf::Dead()
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

void CLeaf::CollisionEvent(CGameObject* pObj)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);
	if (pPlayer != pObj)
		m_bHit = true;
}

void CLeaf::Motion_Change()
{
		if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_pAnimtorCom->Change_Animation(L"Proto_LeafIDLE_Texture");
			break;

		case ATTACK:
			m_pAnimtorCom->Change_Animation(L"Proto_LeafATTACK_Texture");
			break;
		
		case HIT:
			m_pAnimtorCom->Change_Animation(L"Proto_LeafHIT_Texture");
			break;

		case DIE:
			m_pAnimtorCom->Change_Animation(L"Proto_LeafDIE_Texture");
			break;
		}
		m_ePreState = m_eCurState;
	}
}

CLeaf * CLeaf::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CLeaf *	pInstance = new CLeaf(pGraphicDev, vPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CLeaf * CLeaf::Create(CMonster * pMonster)
{
	CLeaf *	pInstance = new CLeaf(*pMonster);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLeaf::Free(void)
{
	CMonster::Free();
}
