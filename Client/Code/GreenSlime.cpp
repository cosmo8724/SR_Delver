#include "stdafx.h"
#include "..\Header\GreenSlime.h"

#include "Export_Function.h"
#include "MiniMap.h"

#include "Player.h"
#include "ParticleMgr.h"
#include "ItemMgr.h"
#include "RedWandBullet.h"
#include "Dagger.h"

CGreenSlime::CGreenSlime(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_fTimeAcc(0.f)
{
	m_eType = MOB_GREENSLIME;
	m_ObjTag = L"GreenSlime";
}

CGreenSlime::CGreenSlime(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	: CMonster(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_fTimeAcc(0.f)
{
	m_eType = MOB_GREENSLIME;
	m_vPos = vPos;
	m_ObjTag = L"GreenSlime";
}

CGreenSlime::CGreenSlime(const CMonster & rhs)
	: CMonster(rhs)
{
	m_ObjTag = L"GreenSlime";
}

CGreenSlime::~CGreenSlime()
{
}

HRESULT CGreenSlime::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.iHp = 2;
	m_tInfo.iAttack = 1;

	m_fHeight = m_vPos.y;
	if (!m_bClone)
		m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);
	//m_pTransCom->Set_Pos(15.f, 1.f, 15.f);

	m_eCurState = IDLE;

	m_fIdle_Speed = 1.f;
	m_fAttack_Speed = 2.f;

	return S_OK;
}

_int CGreenSlime::Update_Object(const _float & fTimeDelta)
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
	Motion_Change();

	if (g_bIsTool)
		return 0;

	//// 기존 이미지 돌리는 코드
	//m_fFrame += m_pTextureCom->Get_FrameEnd()  * fTimeDelta;

	//if (m_fFrame >= m_pTextureCom->Get_FrameEnd())
	//	m_fFrame = 0;


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

	Lock(fTimeDelta);
	OnHit(fTimeDelta);

	if (!m_bHit)
		Target_Follow(fTimeDelta);
	return 0;
}

void CGreenSlime::LateUpdate_Object(void)
{
	CMonster::Billboard();
	Engine::CGameObject::LateUpdate_Object();
}

void CGreenSlime::Render_Obejct(void)
{
	if (!m_bRenderOFF)
		CMonster::Render_Obejct();
}

HRESULT CGreenSlime::Add_Component(void)
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

	m_pAnimtorCom->Add_Component(L"Proto_GreenSlimeIDLE_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_GreenSlimeATTACK_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_GreenSlimeHIT_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_GreenSlimeDIE_Texture");

	//// m_pTextureCom	
	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_GreenSlimeIDLE_Texture"));
	//NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Proto_GreenSlimeIDLE_Texture", pComponent });

	//pComponent = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_GreenSlimeATTACK_Texture"));
	//NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Proto_GreenSlimeATTACK_Texture", pComponent });

	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_GreenSlimeHIT_Texture"));
	//NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Proto_GreenSlimeHIT_Texture", pComponent });

	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_GreenSlimeDIE_Texture"));
	//NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Proto_GreenSlimeDIE_Texture", pComponent });
	/////////

	return S_OK;
}

void CGreenSlime::Target_Follow(const _float & fTimeDelta)
{
	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransformCom);

	_vec3		vPlayerPos, vPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	_float fDist = D3DXVec3Length(&(vPlayerPos - vPos));

	if (fDist < 8.f)
	{
		m_eCurState = ATTACK;
		m_pTransCom->Set_Y(m_fHeight);
		m_pTransCom->Chase_Target(&vPlayerPos, m_fAttack_Speed, fTimeDelta);
	}
	else
	{
		m_eCurState = IDLE;

		_vec3 vRight, vUp, vLook;
		vLook = vPlayerPos - vPos;
		D3DXVec3Normalize(&vLook, &vLook);
		vUp = _vec3(0.f, 1.f, 0.f);

		D3DXVec3Cross(&vRight, &vUp, &vLook);

		m_fTimeAcc += fTimeDelta;
		if (2.f < m_fTimeAcc)
		{
			m_fIdle_Speed *= -1;
			m_fTimeAcc = 0.f;
		}

		D3DXVec3Normalize(&vRight, &vRight);
		m_pTransCom->Move_Pos(&(vRight * m_fIdle_Speed * fTimeDelta));

		//// 몬스터 좌우로 움직이는
		//_vec3		vRight;
		//m_pTransCom->Get_Info(INFO_RIGHT, &vRight);

		//m_fTimeAcc += fTimeDelta;
		//if (2.f < m_fTimeAcc)
		//{
		//	m_fIdle_Speed *= -1;
		//	m_fTimeAcc = 0.f;
		//}

		//D3DXVec3Normalize(&vRight, &vRight);
		//m_pTransCom->Move_Pos(&(vRight * m_fIdle_Speed * fTimeDelta));
	}
}

void CGreenSlime::OnHit(const _float & fTimeDelta)
{
	if (!m_bHit)
		return;

	if (!m_bOneCheck)
	{
		m_eCurState = HIT;
		CMonster::KnockBack(fTimeDelta, m_fHeight);

		CParticleMgr::GetInstance()->Set_Info(this, 1, 0.5f, { 1.f, 0.3f, 0.f },
			1.f, { 1.f, 1.f, 1.f, 1.f }, 5.f, true);
		CParticleMgr::GetInstance()->Add_Info_Spot(false, true);
		CParticleMgr::GetInstance()->Call_Particle(PTYPE_SPOT, TEXTURE_14);

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

void CGreenSlime::Dead()
{
	if (m_bDead) // �� ���� �����
		return;

	m_eCurState = DIE;

	CParticleMgr::GetInstance()->Set_Info(this,
		50,
		0.1f,
		{ 0.5f, 0.5f, 0.5f },
		1.f,
		{ 0.f, 1.f, 0.f, 1.f });
	CParticleMgr::GetInstance()->Call_Particle(PTYPE_FOUNTAIN, TEXTURE_5);

	_int iTex = rand() % 3;
	iTex = 2;
	if (iTex == 0)
	{
		CItemMgr::GetInstance()->Add_RandomObject(L"Layer_GameLogic", L"Potion", ITEM_POTION, m_pTransCom->Get_Pos());
	}
	else if (iTex == 1)
	{
		CItemMgr::GetInstance()->Add_RandomObject(L"Layer_GameLogic", L"Food", ITEM_FOOD, m_pTransCom->Get_Pos());
	}
	else if (iTex == 2)
	{
		CItemMgr::GetInstance()->Add_RandomObject(L"Layer_GameLogic", L"Gold", ITEM_GOLD, m_pTransCom->Get_Pos());
	}

	m_pColliderCom->Set_Free(true);
	m_bDead = true;
}

void CGreenSlime::Lock(const _float& fTimeDelta)
{
	if (!m_bLock || m_tInfo.iHp == 0)
		return;

	m_eCurState = HIT;
	m_fAttack_Speed = 0.f;

	m_fLockTimeAcc += fTimeDelta;
	if (2.f < m_fLockTimeAcc)
	{
		m_eCurState = ATTACK;
		m_fAttack_Speed = 2.f;
		m_fLockTimeAcc = 0.f;
		m_bLock = false;
	}
}

void CGreenSlime::CollisionEvent(CGameObject* pObj)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);
	if (pPlayer)
		return;

	CItem* pItem = dynamic_cast<CItem*>(pObj);
	if (pItem)
	{
		if (pItem->Get_State() == STATE_GROUND)
			return;
	}

	m_bHit = true;

 //	CRedWandBullet* pRedWandBullet = dynamic_cast<CRedWandBullet*>(pObj);
	//if (pRedWandBullet == pObj)
	//	m_bLock = true;
}

void CGreenSlime::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_pAnimtorCom->Change_Animation(L"Proto_GreenSlimeIDLE_Texture");
			//m_pTextureCom = static_cast<CTexture*>(Find_Component(L"Proto_GreenSlimeIDLE_Texture", ID_STATIC));
			break;

		case ATTACK:
			m_pAnimtorCom->Change_Animation(L"Proto_GreenSlimeATTACK_Texture");
			//m_pTextureCom = static_cast<CTexture*>(Find_Component(L"Proto_GreenSlimeATTACK_Texture", ID_STATIC));
			break;

		case HIT:
			m_pAnimtorCom->Change_Animation(L"Proto_GreenSlimeHIT_Texture");
			//m_pTextureCom = static_cast<CTexture*>(Find_Component(L"Proto_GreenSlimeHIT_Texture", ID_STATIC));
			break;

		case DIE:
			m_pAnimtorCom->Change_Animation(L"Proto_GreenSlimeDIE_Texture");
			//m_pTextureCom = static_cast<CTexture*>(Find_Component(L"Proto_GreenSlimeDIE_Texture", ID_STATIC));
			break;
		}
		m_ePreState = m_eCurState;
	}
}

//CGreenSlime * CGreenSlime::Create(LPDIRECT3DDEVICE9 pGraphicDev)
//{
//	CGreenSlime *	pInstance = new CGreenSlime(pGraphicDev);
//
//	if (FAILED(pInstance->Ready_Object()))
//	{
//		Safe_Release(pInstance);
//		return nullptr;
//	}
//
//	return pInstance;
//}

CGreenSlime * CGreenSlime::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CGreenSlime *	pInstance = new CGreenSlime(pGraphicDev, vPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGreenSlime * CGreenSlime::Create(CMonster * pMonster)
{
	CGreenSlime *	pInstance = new CGreenSlime(*pMonster);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGreenSlime::Free(void)
{
	CMonster::Free();
}