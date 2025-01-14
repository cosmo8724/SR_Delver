#include "stdafx.h"
#include "..\Header\Logo.h"

#include "Export_Function.h"
#include "TestPlayer.h"
#include "TestMonster.h"
#include "Stage.h"
#include "Tool_Scene.h"
#include "FontLogo.h"
#include "FontStart.h"
#include "ImGuiMgr.h"
#include "Intro.h"
#include "Loading_Scene.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Ready_Layer_Environment"), E_FAIL);
		
	return S_OK;
}

Engine::_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	static _bool bBGM = false;
	if (!bBGM)
	{
		Engine::PlayBGM(L"BGM_Logo.mp3", 1.f);
		bBGM = true;
	}

	_int iResult = Engine::CScene::Update_Scene(fTimeDelta);

	// �ε� Ŭ���� ����
	//if (!m_pLoading)
	//{
	//	if (Key_Down(DIK_SPACE))
	//	{
	//		m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_INTRO);
	//		NULL_CHECK_RETURN(m_pLoading, E_FAIL);
	//	}
	//	if (Key_Down(DIK_F1))
	//	{
	//		m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_TOOL);
	//		NULL_CHECK_RETURN(m_pLoading, E_FAIL);
	//	}
	//}
	//else
	//{
	//	if (m_pLoading->Get_Finish())
	//	{
	//		//if (Key_Down(DIK_SPACE))
	//		//{
	//		//	//CScene*		pScene = CStage::Create(m_pGraphicDev);
	//		//	//NULL_CHECK_RETURN(pScene, E_FAIL);

	//		//	//FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);

	//		//	return 0;
	//		//}
	//		if (Key_Down(DIK_F1))
	//		{
	//			CScene* pScene = CTool_Scene::Create(m_pGraphicDev);
	//			NULL_CHECK_RETURN(pScene, E_FAIL);
	//			FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);

	//			return 0;
	//		}
	//	}
	//}

	return iResult;
}

void CLogo::LateUpdate_Scene(void)
{
	Engine::CScene::LateUpdate_Scene();

	if (Engine::Key_Down(DIK_F1))
	{
		g_bIsTool = true;

		CScene*	pLoadingScene = CLoading_Scene::Create(m_pGraphicDev, LOADING_TOOL);
		NULL_CHECK(pLoadingScene);
		Engine::Set_Scene(pLoadingScene);
		return;
	}

	if (Engine::Key_Down(DIK_SPACE))
	{
		CScene*	pLoadingScene = CLoading_Scene::Create(m_pGraphicDev, LOADING_INTRO);
		NULL_CHECK(pLoadingScene);
		Engine::Set_Scene(pLoadingScene);
		return;
	}

	/*if (!m_pLoading)
		return;

	if (m_pLoading->Get_Finish())
	{
		if (Key_Down(DIK_SPACE))
		{
			CScene*		pScene = CIntro::Create(m_pGraphicDev);
			NULL_CHECK(pScene);

			Engine::Set_Scene(pScene);
		}
	}*/
}

void CLogo::Render_Scene(void)
{
	// ������ ��� ��� �Լ�
	if (m_pLoading)
		Render_Font(L"Font_Jinji", m_pLoading->Get_String(), &_vec2(230.f, 500.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

}

HRESULT CLogo::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);


	CGameObject*		pGameObject = nullptr;

	// backGround

	pGameObject = CBackGround::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackGround", pGameObject), E_FAIL);

	pGameObject = CFontLogo::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FontLogo", pGameObject), E_FAIL);

	pGameObject = CFontStart::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FontStart", pGameObject), E_FAIL);

		
	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

CLogo * CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo*	pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLogo::Free(void)
{
	Safe_Release(m_pLoading);

	Engine::CScene::Free();
}

HRESULT CLogo::Ready_Proto(void)
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcTexCom", CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TransformCom", CTransform::Create()), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LogoTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Logo/DelverCreationBG.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DelverTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Logo/DelverLogo.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LogoTextTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Logo/PressSpacebarStart.png", TEX_NORMAL)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShaderRect", CShader::Create(m_pGraphicDev, TEXT("../Bin/ShaderFiles/Shader_Rect.hlsl"))), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShaderWater", CShader::Create(m_pGraphicDev, TEXT("../Bin/ShaderFiles/Shader_Water.hlsl"))), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShaderWave", CShader::Create(m_pGraphicDev, TEXT("../Bin/ShaderFiles/Shader_Wave.hlsl"))), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShaderLight", CShader::Create(m_pGraphicDev, TEXT("../Bin/ShaderFiles/Shader_Light.hlsl"))), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShaderSpecular", CShader::Create(m_pGraphicDev, TEXT("../Bin/ShaderFiles/Shader_Specular.hlsl"))), E_FAIL);
		
	
	return S_OK;
}
