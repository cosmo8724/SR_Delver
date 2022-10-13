#include "stdafx.h"
#include "NPCMgr.h"
#include "Export_Function.h"

// NPC
#include "Dog.h"
#include "Grandfather.h"
#include "GuitarMan.h"
#include "HoodMan.h"
#include "SmileMan.h"
#include "WinkMan.h"

IMPLEMENT_SINGLETON(CNPCMgr)

CNPCMgr::CNPCMgr()
{
}

CNPCMgr::~CNPCMgr()
{
	Free();
}

void CNPCMgr::Ready_NPC(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();
}
HRESULT CNPCMgr::Ready_Proto()
{
	// NPC_Dog, Grandfather, GuitarMan, HoodMan, SmileMan, WinkMan
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_NPC_Dog_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/Dog/Dog%d.png", TEX_NORMAL, 33)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_NPC_Grandfather_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/Grandfather/Grandfather%d.png", TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_NPC_GuitarMan_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/GuitarMan/GuitarMan%d.png", TEX_NORMAL, 22)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_NPC_HoodMan_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/HoodMan/HoodMan%d.png", TEX_NORMAL, 21)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_NPC_SmileMan_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/SmileMan/SmileMan%d.png", TEX_NORMAL, 13)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_NPC_WinkMan_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/WinkMan/WinkMan%d.png", TEX_NORMAL, 17)), E_FAIL);

	return S_OK;
}

HRESULT CNPCMgr::Add_GameObject(CLayer * pLayer)
{
	CGameObject*		pGameObject = nullptr;

	//////////////////NPC
	// Dog
	pGameObject = CDog::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Dog", pGameObject), E_FAIL);
	m_vecNPC.push_back(pGameObject);

	// Grandfather
	pGameObject = CGrandfather::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Grandfather", pGameObject), E_FAIL);
	m_vecNPC.push_back(pGameObject);

	// GuitarMan
	pGameObject = CGuitarMan::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GuitarMan", pGameObject), E_FAIL);
	m_vecNPC.push_back(pGameObject);

	// HoodMan
	pGameObject = CHoodMan::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"HoodMan", pGameObject), E_FAIL);
	m_vecNPC.push_back(pGameObject);

	// SmileMan
	pGameObject = CSmileMan::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SmileMan", pGameObject), E_FAIL);
	m_vecNPC.push_back(pGameObject);

	// WinkMan
	pGameObject = CWinkMan::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WinkMan", pGameObject), E_FAIL);
	m_vecNPC.push_back(pGameObject);

	return E_NOTIMPL;
}

inline void CNPCMgr::Free(void)
{
	Safe_Release(m_pGraphicDev);
}