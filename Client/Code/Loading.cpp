#include "stdafx.h"
#include "..\Header\Loading.h"

#include "Export_Function.h"
#include "ImGuiMgr.h"
#include "BulletMgr.h"
#include "ItemMgr.h"
#include "ParticleMgr.h"
#include "MonsterMgr.h"
#include "NPCMgr.h"
#include "UIMgr.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
	, m_eID(LOADING_END)	
{
	ZeroMemory(m_szLoading, sizeof(m_szLoading));
	m_pGraphicDev->AddRef();
}


CLoading::~CLoading()
{
}

HRESULT CLoading::Ready_Loading(LOADINGID eID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);

	m_eID = eID;

	return S_OK;
}

_uint CLoading::Loading_ForMapTool()
{
	lstrcpy(m_szLoading, L"Loading Components...");
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/Waterfall Skybox.dds", TEX_CUBE)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Tile/textures_%d.png", TEX_NORMAL, 21)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Cave_BlockTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Cave/textures_%d.dds", TEX_CUBE, 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Cave_CubeExampleImage", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Cave/textures_%d.png", TEX_NORMAL, 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Cold_BlockTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Cold/textures_%d.dds", TEX_CUBE, 16)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Cold_CubeExampleImage", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Cold/textures_%d.png", TEX_NORMAL, 16)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Dungeon_BlockTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Dungeon/textures_%d.dds", TEX_CUBE, 25)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Dungeon_CubeExampleImage", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Dungeon/textures_%d.png", TEX_NORMAL, 25)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Room_BlockTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Room/textures_%d.dds", TEX_CUBE, 16)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Room_CubeExampleImage", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Room/textures_%d.png", TEX_NORMAL, 16)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sewer_BlockTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Sewer/textures_%d.dds", TEX_CUBE, 24)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sewer_CubeExampleImage", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Sewer/textures_%d.png", TEX_NORMAL, 24)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Temple_BlockTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Temple/textures_%d.dds", TEX_CUBE, 21)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Temple_CubeExampleImage", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Temple/textures_%d.png", TEX_NORMAL, 21)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlockTexCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);

	_int	iWidth, iDepth, iInterval;
	//CImGuiMgr::GetInstance()->Get_MapWidth(&iWidth, &iDepth, &iInterval);
	iWidth = 10; iDepth = 10; iInterval = 1;
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexCom", CTerrainTex::Create(m_pGraphicDev, iWidth, iDepth, iInterval)), E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TransformCom", CTransform::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CalculatorCom", CCalculator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ColliderCom", CCollider::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_AnimatorCom", CAnimator::Create(m_pGraphicDev)), E_FAIL);

	//// Particle ���� �ؽ���
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Particle0_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Particle/particle0/particle0_%d.png", TEX_NORMAL, 4)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Particle1_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Particle/particle1/particle1_%d.png", TEX_NORMAL, 3)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Particle2_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Particle/particle2/particle2_%d.png", TEX_NORMAL, 8)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Particle3_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Particle/particle3/particle3_%d.png", TEX_NORMAL, 9)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Particle4_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Particle/particle4/particle4_%d.png", TEX_NORMAL, 12)), E_FAIL);

	//// Particle ����������Ʈ
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PtBufferCom", CPtBuffer::Create(m_pGraphicDev)), E_FAIL);

	// ��ƼŬ �Ŵ���
	FAILED_CHECK_RETURN(CParticleMgr::GetInstance()->Ready_Proto(), -1);

	// Eco Object Texture
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoStone_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject0_%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoGrass_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject1_%d.png", TEX_NORMAL, 12)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoTree_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject2_%d.png", TEX_NORMAL, 11)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoJar_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject3_%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoBonfire_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject6_%d.png", TEX_NORMAL, 18)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoJam_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject8_%d.png", TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoWeb_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject9_%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoMushroom_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject10_%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoDoor_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject11_%d.png", TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoStatue_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject12_%d.png", TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoTorch1_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject13_%d.png", TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoTorch2_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject14_%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RockFall_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject15_%d.dds", TEX_CUBE, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Door_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject17_%d.dds", TEX_CUBE, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_KnifeTrap_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject18.png", TEX_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TreasureBox_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject16_%d.png", TEX_NORMAL, 2)), E_FAIL);


	CMonsterMgr::GetInstance()->Ready_Proto();

	m_bFinish = true;

	//lstrcpy(m_szLoading, L"Loading Complete!!!!!!!!!!!");

	return 0;
}

_uint CLoading::Loading_ForStage(void)
{
	lstrcpy(m_szLoading, L"Loading...");
	m_bFinish = true;

	return 0;
}

_uint CLoading::Loading_ForIntro()
{
	lstrcpy(m_szLoading, L"Loading Components...");
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TriColCom", CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcColCom", CRcCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlockTexCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SphereTexCom", CSphereTex::Create(m_pGraphicDev)), E_FAIL);

	// Logics
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CalculatorCom", CCalculator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_AnimatorCom", CAnimator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ColliderCom", CCollider::Create(m_pGraphicDev)), E_FAIL);

	// ItemMgr
	lstrcpy(m_szLoading, L"Loading Item,,,");
	CItemMgr::GetInstance()->Ready_Proto();

	// bulletmgr
	CBulletMgr::GetInstance()->Ready_Proto(m_pGraphicDev);

	// particleMgr
	lstrcpy(m_szLoading, L"Loading Particle...");
	FAILED_CHECK_RETURN(CParticleMgr::GetInstance()->Ready_Proto(), -1);

	// MonsterMgr
	lstrcpy(m_szLoading, L"Loading Monster...");
	CMonsterMgr::GetInstance()->Ready_Proto();

	//NPCMgr
	lstrcpy(m_szLoading, L"Loading NPC...");
	CNPCMgr::GetInstance()->Ready_Proto();

	// UIMgr
	lstrcpy(m_szLoading, L"Loading UI...");
	CUIMgr::GetInstance()->Ready_Proto();

	// *Textures*
	lstrcpy(m_szLoading, L"Loading Textures...");
	// Player
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlayerTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/Player/Player%d.png", TEX_NORMAL, 4)), E_FAIL);
	
	// SkyBox
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/RedSky.dds", TEX_CUBE)), E_FAIL);
	
	// Effect
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EffectTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Explosion/Explosion%d.png", TEX_NORMAL, 90)), E_FAIL);

	// Eco Object
	lstrcpy(m_szLoading, L"Loading Objects...");
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoStone_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject0_%d.png", TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoGrass_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject1_%d.png", TEX_NORMAL, 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoTree_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject2_%d.png", TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoJar_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject3_%d.png", TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoBonfire_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject6_%d.png", TEX_NORMAL, 18)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoJam_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject8_%d.png", TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoWeb_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject9_%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoMushroom_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject10_%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoDoor_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject11_%d.png", TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoStatue_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject12_%d.png", TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoTorch1_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject13_%d.png", TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoTorch2_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject14_%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RockFall_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject15_%d.dds", TEX_CUBE, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TreasureBox_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject16_%d.png", TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Door_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject17_%d.dds", TEX_CUBE, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_KnifeTrap_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject18.png", TEX_NORMAL, 1)), E_FAIL);


	// Cat
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CatIDLE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/Cat/Idle/Cat_%d.png", TEX_NORMAL, 16)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CatWALKING_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/Cat/Walking/Cat_%d.png", TEX_NORMAL, 16)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CatHILL_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/Cat/Hill/Cat_%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CatATTACK_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/Cat/Attack/Cat_%d.png", TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CatHIT_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/Cat/Hit/Cat_%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CatDIE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/Cat/Die/Cat_%d.png", TEX_NORMAL, 4)), E_FAIL);

	// Blocks
	lstrcpy(m_szLoading, L"Loading Terrain...");
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Tile/textures_%d.png", TEX_NORMAL, 21)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Cave_BlockTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Cave/textures_%d.dds", TEX_CUBE, 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Cave_CubeExampleImage", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Cave/textures_%d.png", TEX_NORMAL, 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Cold_BlockTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Cold/textures_%d.dds", TEX_CUBE, 16)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Cold_CubeExampleImage", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Cold/textures_%d.png", TEX_NORMAL, 16)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Dungeon_BlockTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Dungeon/textures_%d.dds", TEX_CUBE, 25)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Dungeon_CubeExampleImage", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Dungeon/textures_%d.png", TEX_NORMAL, 25)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Room_BlockTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Room/textures_%d.dds", TEX_CUBE, 16)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Room_CubeExampleImage", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Room/textures_%d.png", TEX_NORMAL, 16)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sewer_BlockTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Sewer/textures_%d.dds", TEX_CUBE, 24)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sewer_CubeExampleImage", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Sewer/textures_%d.png", TEX_NORMAL, 24)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Temple_BlockTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Temple/textures_%d.dds", TEX_CUBE, 21)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Temple_CubeExampleImage", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Temple/textures_%d.png", TEX_NORMAL, 21)), E_FAIL);

	m_bFinish = true;
	lstrcpy(m_szLoading, L"Loading Done...");

	return 0;
}

_uint CLoading::Loading_ForBoss(void)
{
	return _uint();
}

_uint CLoading::Loading_ForTool_Scene()
{
	lstrcpy(m_szLoading, L"Loading...");
	m_bFinish = true;

	return 0;
}

unsigned int CLoading::Thread_Main(void * pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	_uint iFlag = 0;

	EnterCriticalSection(&(pLoading->Get_Crt()));

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_TOOL:
		iFlag = pLoading->Loading_ForMapTool();
		break;

	case LOADING_INTRO:
		iFlag = pLoading->Loading_ForIntro();
		break;

	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;

	case LOADING_BOSS:
		iFlag = pLoading->Loading_ForBoss();
		break;
	}
	
	LeaveCriticalSection(&(pLoading->Get_Crt()));

	return iFlag;
}

CLoading * CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
	CLoading*		pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eID)))
	{
		MSG_BOX("Loading Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);
}
