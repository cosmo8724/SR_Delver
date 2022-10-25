#include "stdafx.h"
#include "..\Header\Mimic.h"

#include "Export_Function.h"
#include "MiniMap.h"

#include "Player.h"
#include "ParticleMgr.h"
#include "ItemMgr.h"
#include "CullingMgr.h"

CMimic::CMimic(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_fTimeAcc(0.f)
	, m_bInteract(0.f)
{
	m_eType = MOB_MIMIC;
	m_ObjTag = L"Mimic";
}

CMimic::CMimic(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	: CMonster(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_fTimeAcc(0.f)
	, m_bInteract(0.f)
{
	m_eType = MOB_MIMIC;
	m_vPos = vPos;
	m_ObjTag = L"Mimic";
}

CMimic::CMimic(const CMonster& rhs)
	: CMonster(rhs)
{
	m_ObjTag = L"Mimic";
}

CMimic::~CMimic()
{
}

HRESULT CMimic::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.iHp = 3;
	m_tInfo.iAttack = 2;
	m_tInfo.iExp = 5;

	if (m_bClone)
		m_vPos = m_pTransCom->Get_Pos();

	m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	m_eCurState = IDLE;
	m_ePreState = MOTION_END;

	m_fAttack_Speed = 2.f;

	return S_OK;
}

_int CMimic::Update_Object(const _float & fTimeDelta)
{
	if (!m_bCreateIcon && !g_bIsTool)
	{
		CMiniMap* pMiniMap = dynamic_cast<CMiniMap*>(Engine::Get_GameObject(L"Layer_UI", L"UI_MiniMap"));
		pMiniMap->Add_Icon(m_pGraphicDev, this);
		m_bCreateIcon = true;
	}
	Engine::CMonster::Update_Object(fTimeDelta);

	if(CCullingMgr::GetInstance()->Is_Inside(this))
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
		Target_Follow(fTimeDelta);
	return 0;
}

void CMimic::LateUpdate_Object(void)
{
	CMonster::Billboard();
	Engine::CGameObject::LateUpdate_Object();
}

void CMimic::Render_Obejct(void)
{


	if (!m_bRenderOFF)
	{

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
		//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		//// Set Material
		D3DMATERIAL9		tMtrl, tOldMtrl;
		ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));
		m_pGraphicDev->GetMaterial(&tMtrl);
		tOldMtrl = tMtrl;

		if (g_vPlayerPos.x < 0 || g_vPlayerPos.z < 0)
		{
			g_fAmbient -= 0.001f;
			if (0.2f >= g_fAmbient)
				g_fAmbient = 0.2f;

			_vec3 dist = g_vPlayerPos - m_pTransCom->Get_Pos();
			if (D3DXVec3Length(&dist) < 5.f)
			{
				g_fAmbient = min(g_fAmbient + 0.01f, 1.f);
			}
			tMtrl.Ambient = D3DXCOLOR(g_fAmbient, g_fAmbient, g_fAmbient, 1.f); // 환경반사
		}
		else
		{
			g_fAmbient += 0.01f;
			if (1.f <= g_fAmbient)
				g_fAmbient = 1.f;
			tMtrl.Ambient = D3DXCOLOR(g_fAmbient, g_fAmbient, g_fAmbient, 1.f); // 환경반사
		}

		tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
		tMtrl.Power = 0.f;

		m_pGraphicDev->SetMaterial(&tMtrl);
		//// *Set Material



		m_pAnimtorCom->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		m_pGraphicDev->SetMaterial(&tOldMtrl);
		//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);


	}
}

HRESULT CMimic::Add_Component(void)
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
	// Collider Component
	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pColliderCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	// m_pAnimtorCom
	pComponent = m_pAnimtorCom = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_AnimatorCom"));
	NULL_CHECK_RETURN(m_pAnimtorCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_AnimatorCom", pComponent });

	m_pAnimtorCom->Add_Component(L"Proto_MimicIDLE_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_MimicATTACK_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_MimicHIT_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_MimicDIE_Texture");

	return S_OK;
}

void CMimic::Target_Follow(const _float & fTimeDelta)
{
	if (!m_bInteract)
		return;

	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransformCom);

	_vec3		vPlayerPos, vPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	m_eCurState = ATTACK;
	m_pTransCom->Set_Y(m_vPos.y);
	m_pTransCom->Chase_Target(&vPlayerPos, m_fAttack_Speed, fTimeDelta);
}

void CMimic::OnHit(const _float & fTimeDelta)
{
	if (!m_bHit)
		return;

	if (!m_bOneCheck)
	{
		m_eCurState = HIT;
		CMonster::Set_KnockBack(m_vPos.y);

		CParticleMgr::GetInstance()->Set_Info(this, 1, 0.5f, { 1.f, 0.3f, 0.f },
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

void CMimic::Dead()
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
		{ 1.f, 1.f, 0.f, 1.f });
	CParticleMgr::GetInstance()->Call_Particle(PTYPE_FOUNTAIN, TEXTURE_5);

	_int iTex = rand() % 3;
	if (iTex == 0)
		CItemMgr::GetInstance()->Add_RandomObject(L"Layer_GameLogic", L"Potion", ITEM_POTION, m_pTransCom->Get_Pos());
	else if (iTex == 1)
		CItemMgr::GetInstance()->Add_RandomObject(L"Layer_GameLogic", L"Food", ITEM_FOOD, m_pTransCom->Get_Pos());
	else if (iTex == 2)
		CItemMgr::GetInstance()->Add_RandomObject(L"Layer_GameLogic", L"Gold", ITEM_GOLD, m_pTransCom->Get_Pos());

	m_pColliderCom->Set_Free(true);
	m_bDead = true;
}

void CMimic::CollisionEvent(CGameObject* pObj)
{
	if (m_eCurState == IDLE)
		return;

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);
	if(pPlayer != pObj)
		m_bHit = true;		
}

void CMimic::InteractEvent()
{
	m_bInteract = true;
}

void CMimic::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_tInfo.iAttack = 0;
			m_pAnimtorCom->Change_Animation(L"Proto_MimicIDLE_Texture");
			break;

		case ATTACK:
			Engine::StopSound(SOUND_MIMIC);
			Engine::Play_Sound(L"M_Mimic_Attack.mp3", SOUND_MIMIC, 1.f);
			m_tInfo.iAttack = 2;
			m_pAnimtorCom->Change_Animation(L"Proto_MimicATTACK_Texture");
			break;

		case HIT:
			Engine::StopSound(SOUND_MIMIC);
			Engine::Play_Sound(L"M_Mimic_Hit.mp3", SOUND_MIMIC, 1.f);
			m_pAnimtorCom->Change_Animation(L"Proto_MimicHIT_Texture");
			break;

		case DIE:
			Engine::StopSound(SOUND_MIMIC);
			Engine::Play_Sound(L"M_Mimic_Die.mp3", SOUND_MIMIC, 1.f);
			m_pAnimtorCom->Change_Animation(L"Proto_MimicDIE_Texture");
			break;
		}
		m_ePreState = m_eCurState;
	}
}

CMimic * CMimic::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CMimic *	pInstance = new CMimic(pGraphicDev, vPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CMimic * CMimic::Create(CMonster * pMonster)
{
	CMimic *	pInstance = new CMimic(*pMonster);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMimic::Free(void)
{
	CMonster::Free();
}