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
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/SkyBox.dds", TEX_CUBE)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Tile/textures_%d.png", TEX_NORMAL, 21)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Cave_BlockTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Cave/textures_%d.dds", TEX_CUBE, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Cave_CubeExampleImage", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Cave/textures_%d.png", TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Cold_BlockTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Cold/textures_%d.dds", TEX_CUBE, 16)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Cold_CubeExampleImage", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Cold/textures_%d.png", TEX_NORMAL, 16)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Dungeon_BlockTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Dungeon/textures_%d.dds", TEX_CUBE, 25)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Dungeon_CubeExampleImage", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Dungeon/textures_%d.png", TEX_NORMAL, 25)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Room_BlockTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Room/textures_%d.dds", TEX_CUBE, 16)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Room_CubeExampleImage", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Room/textures_%d.png", TEX_NORMAL, 16)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sewer_BlockTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Sewer/textures_%d.dds", TEX_CUBE, 17)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sewer_CubeExampleImage", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Sewer/textures_%d.png", TEX_NORMAL, 17)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Temple_BlockTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Temple/textures_%d.dds", TEX_CUBE, 21)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Temple_CubeExampleImage", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Cube Texture/Temple/textures_%d.png", TEX_NORMAL, 21)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlockTexCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);

	_int	iWidth, iDepth, iInterval;
	CImGuiMgr::GetInstance()->Get_MapWidth(&iWidth, &iDepth, &iInterval);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexCom", CTerrainTex::Create(m_pGraphicDev, iWidth, iDepth, iInterval)), E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TransformCom", CTransform::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CalculatorCom", CCalculator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ColliderCom", CCollider::Create(m_pGraphicDev)), E_FAIL);

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
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoStone_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject0_0.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoGrass_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject1_%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoTree_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject2_%d.png", TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoJar_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject3_0.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoBonfire_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject6_0.png", TEX_NORMAL)), E_FAIL);

	m_bFinish = true;

	//lstrcpy(m_szLoading, L"Loading Complete!!!!!!!!!!!");

	return 0;
}

_uint CLoading::Loading_ForStage(void)
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TriColCom", CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcColCom", CRcCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SphereTexCom", CSphereTex::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlayerTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/Player/Player%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EffectTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Explosion/Explosion%d.png", TEX_NORMAL, 90)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/SkyBox.dds", TEX_CUBE)), E_FAIL);
	
	// Logics
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TransformCom", CTransform::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CalculatorCom", CCalculator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_AnimatorCom", CAnimator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ColliderCom", CCollider::Create(m_pGraphicDev)), E_FAIL);


	// ItemMgr
	CItemMgr::GetInstance()->Ready_Proto();

	// bulletmgr
	CBulletMgr::GetInstance()->Ready_Proto(m_pGraphicDev);

	// particleMgr
	FAILED_CHECK_RETURN(CParticleMgr::GetInstance()->Ready_Proto(), -1);

	// MonsterMgr
 	CMonsterMgr::GetInstance()->Ready_Proto();

	//NPCMgr
	CNPCMgr::GetInstance()->Ready_Proto();

	// UIMgr
	CUIMgr::GetInstance()->Ready_Proto();

	// Eco Object
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoStone_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject0_0.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoGrass_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject1_%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoTree_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject2_%d.png", TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoJar_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject3_0.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EcoBonfire_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/EcoObject/EcoObject6_%d.png", TEX_NORMAL, 18)), E_FAIL);


	
	m_bFinish = true;

	//lstrcpy(m_szLoading, L"Loading Complete!!!!!!!!!!!");

	return 0;
}

_uint CLoading::Loading_ForBoss(void)
{
	return _uint();
}

_uint CLoading::Loading_ForTool_Scene()
{
	return _uint();
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

	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;

	case LOADING_BOSS:
		// iFlag = pLoading->Loading_ForBoss();
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
