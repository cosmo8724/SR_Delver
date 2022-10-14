#include "stdafx.h"
#include "UIMgr.h"
#include "Export_Function.h"

// UI
#include "HealthBar.h"
#include "HPGauge.h"
#include "QuickSlot.h"
#include "Inventory.h"
#include "EquipWindow.h"
#include "MapUI.h"
#include "MiniMap.h"
#include "CrossHair.h"
#include "SpiderBackGround.h"

// Font
#include "HPGauge.h"
#include "TalkWindow.h"

IMPLEMENT_SINGLETON(CUIMgr)

CUIMgr::CUIMgr()
{
}

CUIMgr::~CUIMgr()
{
	Free();
}

void CUIMgr::Ready_UI(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();
}
HRESULT CUIMgr::Ready_Proto()
{
	// NPC_TalkWindow
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TalkWindow_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/TalkWindow/TalkWindow.png", TEX_NORMAL, 1)), E_FAIL);

	/////////////UI
	// UI_HP
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UI_HealthBar_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/HealthBar/HealthBar0.png", TEX_NORMAL, 1)), E_FAIL);

	// UI_HPGauge
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UI_HPGauge_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/HealthBar/HealthBar1.png", TEX_NORMAL, 1)), E_FAIL);

	// UI_QuickSlot
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UI_QuickSlot_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/QuickSlot/QuickSlot.png", TEX_NORMAL, 1)), E_FAIL);

	// UI_Inventory
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UI_Inventory_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Inventory/Inventory.png", TEX_NORMAL, 1)), E_FAIL);

	// UI_EquipWindow
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UI_EquipWindow_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/EquipWindow/EquipWindow.png", TEX_NORMAL, 1)), E_FAIL);

	// UI_Map
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UI_Map_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Map/Map_BG.png", TEX_NORMAL, 1)), E_FAIL);

	// UI_MiniMap
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UI_MiniMap_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Minimap/Terrain.png", TEX_NORMAL, 1)), E_FAIL);

	// UI_MiniMap_Icon
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UI_MiniPlayer_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Minimap/Player.png", TEX_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UI_MiniMonster_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Minimap/Monster.png", TEX_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UI_MiniNPC_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Minimap/NPC.png", TEX_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UI_MiniBlock_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Minimap/Block.png", TEX_NORMAL, 1)), E_FAIL);

	// UI_CrossHair
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UI_CrossHair_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/CrossHair/CrossHair.png", TEX_NORMAL, 1)), E_FAIL);

	// UI_HitBackGround
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UI_HitBackGround_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/HitBackGround/HitBackGround.png", TEX_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UI_SpiderBackground_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Monster_Effect/SpiderBackground/SpiderBackground.png", TEX_NORMAL, 1)), E_FAIL);

	
	return S_OK;
}

HRESULT CUIMgr::Add_GameObject(CLayer * pLayer)
{
	CGameObject*		pGameObject = nullptr;

	// UI_HPGauge
	pGameObject = CHPGauge::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_HPGauge", pGameObject), E_FAIL);
	m_vecUI.push_back(pGameObject);

	// UI_HealthBar
	pGameObject = CHealthBar::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_HealthBar", pGameObject), E_FAIL);
	m_vecUI.push_back(pGameObject);

	// UI_QuickSlot
	pGameObject = CQuickSlot::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_QuickSlot", pGameObject), E_FAIL);
	m_vecUI.push_back(pGameObject);

	// UI_Inventory
	pGameObject = CInventory::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Inventory", pGameObject), E_FAIL);
	m_vecUI.push_back(pGameObject);

	// UI_EquipWindow
	pGameObject = CEquipWindow::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_EquipWindow", pGameObject), E_FAIL);
	m_vecUI.push_back(pGameObject);

	// NPC_TalkWindow
	pGameObject = CTalkWindow::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_TalkWindow", pGameObject), E_FAIL);
	m_vecUI.push_back(pGameObject);

	// UI_MAP
	pGameObject = CMapUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Map", pGameObject), E_FAIL);
	m_vecUI.push_back(pGameObject);

	// UI_MiniMap
	pGameObject = CMiniMap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_MiniMap", pGameObject), E_FAIL);
	m_vecUI.push_back(pGameObject);

	// UI_CrossHair
	pGameObject = CCrossHair::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_CrossHair", pGameObject), E_FAIL);
	m_vecUI.push_back(pGameObject);

	// UI_SpiderBackGround
	pGameObject = CSpiderBackGround::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_SpiderBackGround", pGameObject), E_FAIL);
	m_vecUI.push_back(pGameObject);

	return E_NOTIMPL;
}

inline void CUIMgr::Free(void)
{
	Safe_Release(m_pGraphicDev);
}