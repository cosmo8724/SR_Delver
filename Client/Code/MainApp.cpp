#include "stdafx.h"
#include "..\Header\MainApp.h"

#include "Logo.h"
#include "ImGuiMgr.h"
#include "ItemMgr.h"
#include "ParticleMgr.h"

USING(Engine)
CMainApp::CMainApp()	
	: m_pGraphicDev(nullptr)
{
}


CMainApp::~CMainApp()
{
	
}

HRESULT CMainApp::Ready_MainApp(void)
{
	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);	

	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagementClass), E_FAIL);
	
	return S_OK;
}

_int CMainApp::Update_MainApp(const _float & fTimeDelta)
{
	Engine::SetUp_InputDev();

	// ImGui Frame
	CImGuiMgr::GetInstance()->Start_ImGui(fTimeDelta);

	NULL_CHECK_RETURN(m_pManagementClass, -1);
		
	m_pManagementClass->Update_Scene(fTimeDelta);

	return 0;
}

void CMainApp::LateUpdate_MainApp(void)
{
	// *ImGui Frame
	CImGuiMgr::GetInstance()->End_ImGui();

	NULL_CHECK(m_pManagementClass);

	m_pManagementClass->LateUpdate_Scene();

	Engine::LateUpdate_InputDev();
}

void CMainApp::Render_MainApp(void)
{
	
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	Engine::Render_Begin(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f));

	m_pManagementClass->Render_Scene(m_pGraphicDev);

	// ImGui Render
	CImGuiMgr::GetInstance()->Render_ImGui();
	// *ImGui Render

	Engine::Render_End();
}

HRESULT CMainApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	// 디바이스 초기화
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass), E_FAIL);
	m_pDeviceClass->AddRef();

	(*ppGraphicDev) = m_pDeviceClass->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();

	// InputDev
	FAILED_CHECK_RETURN(Engine::Ready_InputDev(g_hInst, g_hWnd), E_FAIL);

	// Font
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Jinji", L"바탕", 7, 18, FW_HEAVY), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Jinji", L"돋움", 7, 18, FW_NORMAL), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Jinji", L"궁서", 7, 18, FW_NORMAL), E_FAIL);

	// Init ImGui
	CImGuiMgr::GetInstance()->DefaultSetting_ImGui(m_pGraphicDev);
	// *Init ImGui

	// 이미지 필터링
	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_GAUSSIANQUAD);
	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_GAUSSIANQUAD);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	return S_OK;
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement** ppManagement)
{
	Engine::CScene*			pScene = nullptr;

	pScene = CLogo::Create(pGraphicDev);
	NULL_CHECK_RETURN(pScene, E_FAIL);

	FAILED_CHECK_RETURN(Engine::Create_Management(pGraphicDev, ppManagement), E_FAIL);
	(*ppManagement)->AddRef();

	FAILED_CHECK_RETURN((*ppManagement)->Set_Scene(pScene), E_FAIL);

	// 매니저 클래스들 생성
	CItemMgr::GetInstance()->Ready_ItemMgr(m_pGraphicDev);
	CParticleMgr::GetInstance()->Ready_ParticleMgr(m_pGraphicDev);

	return S_OK;
}

CMainApp * CMainApp::Create(void)
{
	CMainApp*	pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
	{
		MSG_BOX("MainApp Create Failed");
		Safe_Release<CMainApp*>(pInstance);		
	}
	
	return pInstance;
}

void CMainApp::Free(void)
{
	// ImGui Release
	CImGuiMgr::GetInstance()->DestroyInstance();
	// *ImGui Release

	CItemMgr::DestroyInstance();
	CParticleMgr::DestroyInstance();

	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pDeviceClass);
	Safe_Release(m_pManagementClass);

	Engine::Release_Utility();
	Engine::Release_System();
}
