#include "stdafx.h"
#include "..\Header\TreasureBox.h"
#include "Export_Function.h"
#include "ItemMgr.h"
#include "Player.h"
#include "ParticleMgr.h"

_bool	g_bOpenAtOnce = false;

CTreasureBox::CTreasureBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEcoObject(pGraphicDev)
{
}

CTreasureBox::CTreasureBox(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	: CEcoObject(pGraphicDev)
{
	m_eType = ECO_TREASUREBOX;
	m_vPos = vPos;
}

CTreasureBox::CTreasureBox(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, LOADINGID eID)
	: CEcoObject(pGraphicDev)
{
	m_eLoadingType = eID;
	m_eType = ECO_TREASUREBOX;
	m_vPos = vPos;
}

CTreasureBox::CTreasureBox(const CEcoObject& rhs)
	: CEcoObject(rhs)
{
}

CTreasureBox::~CTreasureBox()
{
}

HRESULT CTreasureBox::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (!m_bClone)
		m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	m_iTexture = 0;

	return S_OK;
}

_int CTreasureBox::Update_Object(const _float & fTimeDelta)
{
	if (!g_bIsTool && !m_bReady)
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
		NULL_CHECK(pPlayer);

		pPlayer->Add_CollisionGroup(this);
		m_bReady = true;
	}

	CEcoObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_ALPHA, this);

	m_fParticle += fTimeDelta;
	return OBJ_NOEVENT;
}

void CTreasureBox::LateUpdate_Object(void)
{
	if (m_bDead)
		return;

	if (m_eLoadingType != LOADING_BOSS)
		Billboard();

	if (g_bOpenAtOnce && !m_iTexture)
	{
		m_iTexture = 1;

		_int iRand = rand() % 10 + 100;
		for (int i = 0; i < iRand; ++i)
		{
			CItemMgr::GetInstance()->Add_GameObject_Box(L"Gold", ITEM_GOLD, m_pTransCom->Get_Pos());
		}
	}
	CEcoObject::LateUpdate_Object();
}

void CTreasureBox::Render_Obejct(void)
{
	if (g_bIsTool)
		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

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
		tMtrl.Ambient = D3DXCOLOR(g_fAmbient, g_fAmbient, g_fAmbient, 1.f); // ȯ��ݻ�
	}
	else
	{
		g_fAmbient += 0.01f;
		if (1.f <= g_fAmbient)
			g_fAmbient = 1.f;
		tMtrl.Ambient = D3DXCOLOR(g_fAmbient, g_fAmbient, g_fAmbient, 1.f); // ȯ��ݻ�
	}

	tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrl.Power = 0.f;

	m_pGraphicDev->SetMaterial(&tMtrl);
	//// *Set Material



	m_pTextureCom->Set_Texture(m_iTexture);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDev->SetMaterial(&tOldMtrl);
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);


	CEcoObject::Render_Obejct();	// collider ���
}

HRESULT CTreasureBox::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	if (!m_bClone)
	{
		pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
		NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });
	}

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_TreasureBox_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TreasureBox_Texture", pComponent });


	return S_OK;
}

CTreasureBox * CTreasureBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, LOADINGID eID)
{
	CTreasureBox *	pInstance = new CTreasureBox(pGraphicDev, vPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CTreasureBox * CTreasureBox::Create(CEcoObject * pEcoObject)
{
	CTreasureBox *	pInstance = new CTreasureBox(*pEcoObject);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTreasureBox::Free(void)
{
	CEcoObject::Free();
}

void CTreasureBox::CollisionEvent(CGameObject * pObj)
{
	if (m_iTexture == 1)
		return;
}

void CTreasureBox::InteractEvent()
{
	if (m_iTexture == 0)
	{
		if (m_eLoadingType == LOADING_STAGE)
		{
			Engine::StopSound(SOUND_TREASUREBOX);
			Engine::Play_Sound(L"E_Box.mp3", SOUND_TREASUREBOX, 1.f);

			m_iTexture = 1;

			//CItemMgr::GetInstance()->Add_RandomObject(
			//	L"Layer_GameLogic", L"Key", ITEM_KEY, m_pTransCom->Get_Pos());

			m_pColliderCom->Set_Free(true);


			CParticleMgr::GetInstance()->Set_Info(this,
				50,
				0.7f,
				{ 0.5f, 0.5f, 0.5f },
				1.f,
				{ 1.f, 1.f, 1.f, 1.f }, 1.f, false, true);
			CParticleMgr::GetInstance()->Call_Particle(PTYPE_FOUNTAIN, TEXTURE_17);


			//CItemMgr::GetInstance()->Add_GameObject_Box(L"Key", ITEM_KEY, m_pTransCom->Get_Pos());


			_int iRand = rand() % 10 + 5;
			for (int i = 0; i < iRand; ++i)
			{
				CItemMgr::GetInstance()->Add_GameObject_Box(L"Gold", ITEM_GOLD, m_pTransCom->Get_Pos());
			}
			CItemMgr::GetInstance()->Add_GameObject_Box(L"GreenWand", ITEM_WEAPON, _vec3(m_pTransCom->Get_Pos().x, m_pTransCom->Get_Pos().y + 1.f, m_pTransCom->Get_Pos().z));
		}
		else if (m_eLoadingType == LOADING_BOSS)
		{
			g_bOpenAtOnce = true;
			m_iTexture = 1;

			Engine::StopSound(SOUND_TREASUREBOX);
			Engine::Play_Sound(L"E_Box.mp3", SOUND_TREASUREBOX, 1.f);

			m_iTexture = 1;

			if (0.1f < m_fParticle)
			{
				CParticleMgr::GetInstance()->Set_Info(this,
					50,
					1.f,
					{ 1.f, 1.f, 1.f },
					1.f,
					{ 1.f, 1.f, 1.f, 1.f }, 1.f, false, true);
				CParticleMgr::GetInstance()->Call_Particle(PTYPE_CIRCLING, TEXTURE_4);
				
				//CParticleMgr::GetInstance()->Set_Info(this,
				//	500,
				//	10.f,
				//	{ 1.f, 1.f, 1.f },
				//	1.f,
				//	{ 1.f, 1.f, 1.f, 1.f }, 1.f, false, true);
				//CParticleMgr::GetInstance()->Call_Particle(PTYPE_FIREWORK, TEXTURE_4);

				m_fParticle = 0.f;
			}

			m_pColliderCom->Set_Free(true);

			_int iRand = rand() % 10 + 100;
			for (int i = 0; i < iRand; ++i)
			{
				CItemMgr::GetInstance()->Add_GameObject_Box(L"Gold", ITEM_GOLD, m_pTransCom->Get_Pos());
			}
			CItemMgr::GetInstance()->Add_GameObject_Box(L"Ring", ITEM_RING, m_pTransCom->Get_Pos());
			CItemMgr::GetInstance()->Add_GameObject_Box(L"Necklace", ITEM_NECKLACE, m_pTransCom->Get_Pos());
		}
	}
}
