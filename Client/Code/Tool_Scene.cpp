#include "stdafx.h"
#include "..\Header\Tool_Scene.h"
#include "ImGuiMgr.h"
#include "DynamicCamera.h"
#include "CameraMgr.h"
#include "BlockVIBuffer.h"
#include "SkyBox.h"


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
	CImGuiMgr::GetInstance()->ObjectTool();
	//CImGuiMgr::GetInstance()->ParticleTool();
	CImGuiMgr::GetInstance()->InfoTool();

	return iResult;
}

void CTool_Scene::LateUpdate_Scene()
{
	Engine::CScene::LateUpdate_Scene();

	for (_int i = 0; i < BLOCKTYPE_END; ++i)
	{
		if (i == BLOCK_CAVE)
		{
			for (_int j = 0; j < CAVETEX_CNT; ++j)
				CBlockVIBuffer::GetInstance()->Ready_Buffer(m_pGraphicDev, (BLOCKTYPE)i, j);
		}
		else if (i == BLOCK_COLD)
		{
			for (_int j = 0; j < COLDTEX_CNT; ++j)
				CBlockVIBuffer::GetInstance()->Ready_Buffer(m_pGraphicDev, (BLOCKTYPE)i, j);
		}
		else if (i == BLOCK_DUNGEON)
		{
			for (_int j = 0; j < DUNGEONTEX_CNT; ++j)
				CBlockVIBuffer::GetInstance()->Ready_Buffer(m_pGraphicDev, (BLOCKTYPE)i, j);
		}
		else if (i == BLOCK_ROOM)
		{
			for (_int j = 0; j < ROOMTEX_CNT; ++j)
				CBlockVIBuffer::GetInstance()->Ready_Buffer(m_pGraphicDev, (BLOCKTYPE)i, j);
		}
		else if (i == BLOCK_SEWER)
		{
			for (_int j = 0; j < SEWERTEX_CNT; ++j)
				CBlockVIBuffer::GetInstance()->Ready_Buffer(m_pGraphicDev, (BLOCKTYPE)i, j);
		}
		else if (i == BLOCK_TEMPLE)
		{
			for (_int j = 0; j < TEMPLETEX_CNT; ++j)
				CBlockVIBuffer::GetInstance()->Ready_Buffer(m_pGraphicDev, (BLOCKTYPE)i, j);
		}
	}
}

void CTool_Scene::Render_Scene()
{
	for (_int i = 0; i < BLOCKTYPE_END; ++i)
	{
		if (i == BLOCK_CAVE)
		{
			for (_int j = 0; j < CAVETEX_CNT; ++j)
				CBlockVIBuffer::GetInstance()->Render_Buffer(m_pGraphicDev, (BLOCKTYPE)i, j);
		}
		else if (i == BLOCK_COLD)
		{
			for (_int j = 0; j < COLDTEX_CNT; ++j)
				CBlockVIBuffer::GetInstance()->Render_Buffer(m_pGraphicDev, (BLOCKTYPE)i, j);
		}
		else if (i == BLOCK_DUNGEON)
		{
			for (_int j = 0; j < DUNGEONTEX_CNT; ++j)
				CBlockVIBuffer::GetInstance()->Render_Buffer(m_pGraphicDev, (BLOCKTYPE)i, j);
		}
		else if (i == BLOCK_ROOM)
		{
			for (_int j = 0; j < ROOMTEX_CNT; ++j)
				CBlockVIBuffer::GetInstance()->Render_Buffer(m_pGraphicDev, (BLOCKTYPE)i, j);
		}
		else if (i == BLOCK_SEWER)
		{
			for (_int j = 0; j < SEWERTEX_CNT; ++j)
				CBlockVIBuffer::GetInstance()->Render_Buffer(m_pGraphicDev, (BLOCKTYPE)i, j);
		}
		else if (i == BLOCK_TEMPLE)
		{
			for (_int j = 0; j < TEMPLETEX_CNT; ++j)
				CBlockVIBuffer::GetInstance()->Render_Buffer(m_pGraphicDev, (BLOCKTYPE)i, j);
		}
	}
}

HRESULT CTool_Scene::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	Engine::CLayer*	pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	
	CGameObject*		pGameObject = nullptr;

	/*pGameObject = CDynamicCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);*/

	// skybox
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);

	CCameraMgr::GetInstance()->Add_GameObject(pLayer);
	CCameraMgr::GetInstance()->Change_Camera(CAM_DYNAMIC);

	/*pGameObject = CUserParticle::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Particle", pGameObject), E_FAIL);
	CImGuiMgr::GetInstance()->Set_Particle(static_cast<CUserParticle*>(pGameObject));*/


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
	// Default
	//D3DLIGHT9        tLightInfo0;
	//ZeroMemory(&tLightInfo0, sizeof(D3DLIGHT9));

	//tLightInfo0.Type = D3DLIGHT_POINT;
	//tLightInfo0.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo0.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo0.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo0.Position = _vec3(50.f, 50.f, 50.f);
	//tLightInfo0.Range = 3000.f;

	//FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo0, LIGHT_STAGE), E_FAIL);

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
