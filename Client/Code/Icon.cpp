#include "stdafx.h"
#include "..\Header\Icon.h"
#include "Export_Function.h"
#include "Player.h"
#include "Terrain.h"
#include "Block.h"

CIcon::CIcon(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pObj)
	: CUI(pGraphicDev)
	, m_pParentObj(pObj)
{
}


CIcon::~CIcon()
{
}

HRESULT CIcon::Ready_Object()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pParentObj);
	if (pPlayer)
	{
		m_eType = OBJ_PLAYER;
	}

	CMonster* pMonster = dynamic_cast<CMonster*>(m_pParentObj);
	if (pMonster)
	{
		m_eType = OBJ_MONSTER;
	}

	CNPC* pNPC = dynamic_cast<CNPC*>(m_pParentObj);
	if (pNPC)
	{
		m_eType = OBJ_NPC;
	}

	CTerrain* pTerrain = dynamic_cast<CTerrain*>(m_pParentObj);
	if (pTerrain)
	{
		m_eType = OBJ_TERRAIN;
	}

	CBlock* pBlock = dynamic_cast<CBlock*>(m_pParentObj);
	if (pBlock)
	{
		m_eType = OBJ_BLOCK;
	}

	if (m_eType == OBJ_END)
		return E_FAIL;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matWorld);

	if (m_eType == OBJ_PLAYER)
	{
		m_fScaleX = 8.f;
		m_fScaleY = 8.f;
	}
	else
	{
		m_fScaleX = 4.f;
		m_fScaleY = 4.f;
	}

	return S_OK;
}

_int CIcon::Update_Object(const _float & fTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);

	RotateByPlayer();

	POINT		pt = { LONG((WINCX >> 1) - 180.f), LONG((WINCY >> 1) - 180.f) };

	if (m_fPosX < pt.x + 130.f && m_fPosX > pt.x - 130.f
		&& m_fPosY < pt.y + 130.f && m_fPosY > pt.y - 130.f)
		m_bOnMinimap = true;
	else
		m_bOnMinimap = false;

	if (!m_bOnMinimap)
		return 0;

	Engine::CGameObject::Update_Object(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CIcon::LateUpdate_Object(void)
{
	if (!m_bOnMinimap)
		return;

	Engine::CGameObject::LateUpdate_Object();
}

void CIcon::Render_Obejct(void)
{
	if (!m_bOnMinimap)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->Set_Texture(0);	// 텍스처 정보 세팅을 우선적으로 한다.
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CIcon::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	switch (m_eType)
	{
	case OBJ_PLAYER:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UI_MiniPlayer_Texture"));
		NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_UI_MiniPlayer_Texture", pComponent });
		break;

	case OBJ_MONSTER:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UI_MiniMonster_Texture"));
		NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_UI_MiniMonster_Texture", pComponent });
		break;
			
	case OBJ_NPC:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UI_MiniNPC_Texture"));
		NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_UI_MiniNPC_Texture", pComponent });
		break;

	case OBJ_TERRAIN:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UI_MiniNPC_Texture"));
		NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_UI_MiniMap_Texture", pComponent });
		break;

	case OBJ_BLOCK:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UI_MiniBlock_Texture"));
		NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_UI_MiniBlock_Texture", pComponent });
		break;
	}

	

	return S_OK;
}

void CIcon::RotateByPlayer()
{
	CTransform* pTransCom = nullptr;
	if (m_eType == OBJ_BLOCK)
		pTransCom = dynamic_cast<CBlock*>(m_pParentObj)->m_pTransCom;
	else
		pTransCom = dynamic_cast<CTransform*>(m_pParentObj->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
	_vec3		vParentPos = pTransCom->Get_Pos();

	if (m_eType == OBJ_PLAYER)
	{
		m_fPosX = ((WINCX >> 1) - 180.f) + vParentPos.x * (130.f / WINCX);
		m_fPosY = ((WINCY >> 1) - 180.f) + vParentPos.z * (130.f / WINCY);
	}
	else
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"));
		CTransform* pPlayerTrans = dynamic_cast<CTransform*>(pPlayer->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
		_vec3		vPlayerPos = pPlayerTrans->Get_Pos();
		_float		fWidth = (vParentPos.x - vPlayerPos.x) * (8000.f / WINCY);
		_float		fHeight = (vParentPos.z - vPlayerPos.z) * (8000.f / WINCY);

		m_fPosX = ((WINCX >> 1) - 180.f) + fWidth;
		m_fPosY = ((WINCY >> 1) - 180.f) + fHeight;
	}	

	D3DXMatrixScaling(&m_matView, m_fScaleX, m_fScaleY, 1.f);

	if (m_eType == OBJ_PLAYER)
	{
		_vec3 vPlayerLook;
		pTransCom->Get_Info(INFO_LOOK, &vPlayerLook);
		vPlayerLook.y = 0.f;
		D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
		
		_vec3 vIconLook = { 0.f, 0.f, 1.f };
		//m_pTransCom->Get_Info(INFO_LOOK, &vIconLook);
		D3DXVec3Normalize(&vIconLook, &vIconLook);

		_matrix matRot;
		D3DXMatrixIdentity(&matRot);
		_float fDot = D3DXVec3Dot(&vPlayerLook, &vIconLook);
		_float fAngle = acosf(fDot);
		if (vPlayerLook.x > 0.f)
			fAngle *= -1.f;

		D3DXMatrixRotationZ(&matRot, fAngle);

		m_matWorld *= matRot;
	}

	m_matView._41 = m_fPosX;
	m_matView._42 = m_fPosY;
}

CIcon * CIcon::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject * pObj)
{
	CIcon*	pInstance = new CIcon(pGraphicDev, pObj);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CIcon::Free(void)
{
	CUI::Free();
}


