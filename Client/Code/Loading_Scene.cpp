#include "stdafx.h"
#include "Loading_Scene.h"
#include "Export_Function.h"
#include "Loading_BG.h"
#include "Intro.h"
#include "Stage.h"
#include "Tool_Scene.h"

CLoading_Scene::CLoading_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CLoading_Scene::~CLoading_Scene()
{
}

HRESULT CLoading_Scene::Ready_Scene(LOADINGID eID)
{
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	m_eID = eID;

	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer(L"Layer_Environment"), E_FAIL);

	return S_OK;
}

_int CLoading_Scene::Update_Scene(const _float & fTimeDelta)
{
	_int iResult = Engine::CScene::Update_Scene(fTimeDelta);

	if (!m_pLoading)
	{
		switch (m_eID)
		{
		case LOADING_TOOL:
			m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_TOOL);
			NULL_CHECK_RETURN(m_pLoading, E_FAIL);
			break;

		case LOADING_INTRO:
			m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_INTRO);
			NULL_CHECK_RETURN(m_pLoading, E_FAIL);
			break;

		case LOADING_STAGE:
			m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_STAGE);
			NULL_CHECK_RETURN(m_pLoading, E_FAIL);
			break;
		}
	}

	return iResult;
}

void CLoading_Scene::LateUpdate_Scene(void)
{
	if (m_pLoading->Get_Finish())
	{
		switch (m_pLoading->Get_LoadingID())
		{
		case LOADING_TOOL:
			m_pScene = CTool_Scene::Create(m_pGraphicDev);
			break;

		case LOADING_INTRO:
			m_pScene = CStage::Create(m_pGraphicDev);  //CIntro::Create(m_pGraphicDev);
			break;

		case LOADING_STAGE:
			m_pScene = CStage::Create(m_pGraphicDev);
			break;
		}

		NULL_CHECK(m_pScene);
		Engine::Set_Scene(m_pScene);

		return;
	}
}

void CLoading_Scene::Render_Scene(void)
{
	if (m_pLoading)
		Render_Font(L"Font_Hp", m_pLoading->Get_String(), &_vec2(135, 798), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

HRESULT CLoading_Scene::Ready_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	pGameObject = CLoading_BG::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackGround", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CLoading_Scene::Ready_Proto()
{
	Engine::Delete_Proto(L"Proto_LoadingTexture");
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LoadingTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Logo/Intro Loading.png", TEX_NORMAL)), E_FAIL);

	return S_OK;
}

CLoading_Scene * CLoading_Scene::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
	CLoading_Scene* pInstance = new CLoading_Scene(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene(eID)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLoading_Scene::Free()
{
	Safe_Release(m_pLoading);

	CScene::Free();
}
