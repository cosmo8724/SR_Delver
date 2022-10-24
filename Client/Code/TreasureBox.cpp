#include "stdafx.h"
#include "..\Header\TreasureBox.h"
#include "Export_Function.h"
#include "ItemMgr.h"
#include "Player.h"

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

	return OBJ_NOEVENT;
}

void CTreasureBox::LateUpdate_Object(void)
{
	if (m_bDead)
		return;

	Billboard();
	CEcoObject::LateUpdate_Object();
}

void CTreasureBox::Render_Obejct(void)
{
	if (m_bDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	m_pTextureCom->Set_Texture(m_iTexture);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);


	CEcoObject::Render_Obejct();	// collider Ãâ·Â
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

CTreasureBox * CTreasureBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
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
		Engine::StopSound(SOUND_TREASUREBOX);
		Engine::Play_Sound(L"E_Box.mp3", SOUND_TREASUREBOX, 1.f);

		m_iTexture = 1;

		CItemMgr::GetInstance()->Add_RandomObject(
			L"Layer_GameLogic", L"Key", ITEM_KEY, m_pTransCom->Get_Pos());
	}
}
