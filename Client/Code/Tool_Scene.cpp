#include "stdafx.h"
#include "..\Header\Tool_Scene.h"
#include "ImGuiMgr.h"
#include "DynamicCamera.h"


CTool_Scene::CTool_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CTool_Scene::~CTool_Scene()
{
}

HRESULT CTool_Scene::Ready_Scene()
{
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	Ready_Light();
	Ready_Layer_Environment(L"Layer_Tool_Environment");
	Ready_Layer_GameLogic(L"Layer_Tool_GameLogic");

	return S_OK;
}

_int CTool_Scene::Update_Scene(const _float & fTimeDelta)
{
	// Add Tool Windows Here.
	//CImGuiMgr::GetInstance()->ImGui_Map_Tool(m_pGraphicDev, fTimeDelta);
	_uint		iResult = Engine::CScene::Update_Scene(fTimeDelta);

	CImGuiMgr::GetInstance()->MapTool();
	CImGuiMgr::GetInstance()->ParticleTool();

	return iResult;
}

void CTool_Scene::LateUpdate_Scene()
{
	Engine::CScene::LateUpdate_Scene();
}

void CTool_Scene::Render_Scene()
{
}

HRESULT CTool_Scene::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	Engine::CLayer*	pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	
	CGameObject*		pGameObject = nullptr;

	pGameObject = CDynamicCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);

	pGameObject = CUserParticle::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Particle", pGameObject), E_FAIL);
	CImGuiMgr::GetInstance()->Set_Particle(static_cast<CUserParticle*>(pGameObject));

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CTool_Scene::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	Engine::CLayer*	pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CTool_Scene::Ready_Light()
{
	D3DLIGHT9		tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Direction = _vec3(0.f, -1.f, 1.f);

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

CTool_Scene * CTool_Scene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTool_Scene*	pInstance = new CTool_Scene(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTool_Scene::Free()
{
	CScene::Free();
}
