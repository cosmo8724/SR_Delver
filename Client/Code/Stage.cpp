#include "stdafx.h"
#include "..\Header\Stage.h"

#include "Export_Function.h"
#include "ImGuiMgr.h"
#include "SkyBox.h"
#include "Effect.h"
#include "BulletMgr.h"
#include "CollisionMgr.h"
#include "GameObject.h"
#include "Block.h"

// Monster
#include "GreenSlime.h"
#include "PinkSlime.h"
#include "Fist.h"
#include "BlueBat.h"
#include "BrownBat.h"
#include "Stick.h"
#include "Leaf.h"
#include "SkeletonGhost.h"
// Boss
#include "SongBoss.h"

// NPC
#include "Dog.h"
#include "Grandfather.h"
#include "GuitarMan.h"
#include "HoodMan.h"
#include "SmileMan.h"
#include "WinkMan.h"

// UI
#include "HealthBar.h"
#include "HPGauge.h"
#include "QuickSlot.h"
#include "Inventory.h"
#include "EquipWindow.h"
#include "MapUI.h"
#include "MiniMap.h"

// Font
#include "HPGauge.h"
#include "TalkWindow.h"

// Manager
#include "ItemMgr.h"
#include "ParticleMgr.h"

// EcoObject
#include "Jar.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}


CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene(void)
{
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Light(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
			
	return S_OK;
}

_int CStage::Update_Scene(const _float & fTimeDelta)
{
	return Engine::CScene::Update_Scene(fTimeDelta);
}

void CStage::LateUpdate_Scene(void)
{
	CBlock* pBlock = nullptr;
	CLayer*	pLayer = m_mapLayer[L"Layer_GameLogic"];
	//CGameObject* pSour = Engine::Get_GameObject(L"Layer_GameLogic", L"GreenSlime");
	CGameObject*	pPlayer = Engine::Get_GameObject(L"Layer_GameLogic", L"Player");
	//vector<CGameObject*>*	pPlayerBullets = CBulletMgr::GetInstance()->Get_Bullets(BULLET_WAND);

	for (auto iter = pLayer->Get_mapGameObject()->begin(); iter != pLayer->Get_mapGameObject()->end(); ++iter)
	{
		pBlock = dynamic_cast<CBlock*>(iter->second);

		if (pBlock)
		{
			//for (auto& bullet : *pPlayerBullets)
			//{
			//	if (CBulletMgr::GetInstance()->Is_Fired(bullet))
			//	{
			//		//CCollisionMgr::GetInstance()->CollisionAABB(pSour, bullet);

			//		CCollisionMgr::GetInstance()->CollisionAABB(pBlock, bullet);
			//	}
			//}
			//CCollisionMgr::GetInstance()->CollisionSphere(pPlayer, pBlock);
			//CCollisionMgr::GetInstance()->CollisionAABB(pPlayer, pBlock);
			Engine::CollisionTest(pPlayer, pBlock);
		}
	}

	// Collider 테스트(Player - 아이템)
	// Collider 테스트( 플레이어와 Arrow0,Arrow1,Wand2)
	//CGameObject* pDest = nullptr;

	//for (int i = 0; i < 2; ++i)
	//{
	//	wstring obj = L"Arrow";
	//	wchar_t index[10];
	//	_itow_s(i, index, 10);
	//	obj += index;

	//	pDest = Engine::Get_GameObject(L"Layer_GameLogic", obj.c_str());
	//	//Engine::CollisionAABB(pSour, pDest);
	//	Engine::CollisionTest(pPlayer, pDest);
	//}

	//pDest = Engine::Get_GameObject(L"Layer_GameLogic", L"Wand2");
	////Engine::CollisionAABB(pSour, pDest);
	//Engine::CollisionTest(pPlayer, pDest);
	// ~Collider 테스트

	// 플레이어와 아이템
	vector<CGameObject*>*	pItems = CItemMgr::GetInstance()->Get_Items();
	for (auto& item : *pItems)
	{
		Engine::CollisionAABB(pPlayer, item);
	}

	// Bullet 테스트
	CGameObject* pSour = nullptr;
	vector<CGameObject*>*	pPlayerBullets = CBulletMgr::GetInstance()->Get_Bullets(BULLET_WAND);
	for (auto& bullet : *pPlayerBullets)
	{
		pSour = Engine::Get_GameObject(L"Layer_GameLogic", L"GreenSlime");
		Engine::CollisionTest(pSour, bullet);

		pSour = Engine::Get_GameObject(L"Layer_GameLogic", L"Jar");
		Engine::CollisionTest(pSour, bullet);


	}


	// song bullet 이랑 테스트 하고 싶다면
	//for (auto& bullet : *CBulletMgr::GetInstance()->Get_Bullets(BULLET_SONGBOSS))
	//{

	//}
	
	Engine::CScene::LateUpdate_Scene();
}

void CStage::Render_Scene(void)
{

}

HRESULT CStage::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	
	CGameObject*		pGameObject = nullptr;

	// DynamicCamera
	/*pGameObject = CDynamicCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);*/

	// StaticCamera
	pGameObject = CStaticCamera::Create(m_pGraphicDev, &_vec3(0.f, 20.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StaticCamera", pGameObject), E_FAIL);

	// skybox
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);

	// Terrain
	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);
	
	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	// Player
	pGameObject = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);

	// Bullet
	FAILED_CHECK_RETURN(CBulletMgr::GetInstance()->Ready_Clone(pLayer, m_pGraphicDev), E_FAIL);

	// Item
	CItemMgr::GetInstance()->Add_GameObject(pLayer, L"Arrow", ITEM_WEAPON, _vec3({ 30.f, 1.f, 30.f }));
	CItemMgr::GetInstance()->Add_GameObject(pLayer, L"Arrow", ITEM_WEAPON, _vec3({ 40.f, 1.f, 40.f }));
	CItemMgr::GetInstance()->Add_GameObject(pLayer, L"Wand", ITEM_WEAPON, _vec3({ 35.f, 1.f, 35.f }));
	
	// Particle
 	CParticleMgr::GetInstance()->Add_GameObject(pLayer);


	//////////// Monster
	// GreenSlime
	pGameObject = CGreenSlime::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GreenSlime", pGameObject), E_FAIL);

	// PinkSlime
	pGameObject = CPinkSlime::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PinkSlime", pGameObject), E_FAIL);

	// Fist
	pGameObject = CFist::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Fist", pGameObject), E_FAIL);

	// BlueBat
	pGameObject = CBlueBat::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BlueBat", pGameObject), E_FAIL);

	// BrownBat
	pGameObject = CBrownBat::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BrownBat", pGameObject), E_FAIL);

	// Stick
	pGameObject = CStick::Create(m_pGraphicDev, 1);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Stick", pGameObject), E_FAIL);

	// Leaf
	pGameObject = CLeaf::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Leaf", pGameObject), E_FAIL);

	// SkeletonGhost
	pGameObject = CSkeletonGhost::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkeletonGhost", pGameObject), E_FAIL);

	//////////////Boss
	// SongBoss
	pGameObject = CSongBoss::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SongBoss", pGameObject), E_FAIL);

	//////////////////NPC
	// Dog
	pGameObject = CDog::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Dog", pGameObject), E_FAIL);

	// Grandfather
	pGameObject = CGrandfather::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Grandfather", pGameObject), E_FAIL);

	// GuitarMan
	pGameObject = CGuitarMan::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GuitarMan", pGameObject), E_FAIL);

	// HoodMan
	pGameObject = CHoodMan::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"HoodMan", pGameObject), E_FAIL);

	// SmileMan
	pGameObject = CSmileMan::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SmileMan", pGameObject), E_FAIL);

	// WinkMan
	pGameObject = CWinkMan::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WinkMan", pGameObject), E_FAIL);

	// EcoObject
	pGameObject = CJar::Create(m_pGraphicDev, _vec3({30.f, 1.f, 40.f}));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Jar", pGameObject), E_FAIL);

	// Blocks
	string	strPath = "..\\Bin\\Resource\\Map_SH.dat";
	const char* pPath = strPath.c_str();
	int iLength = strlen(pPath) + 1;
	TCHAR* wpPath = new TCHAR[iLength];
	size_t	Temp;
	mbstowcs_s(&Temp, wpPath, iLength, pPath, iLength);

	HANDLE	hFile = CreateFile(wpPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;
	DWORD	dwStrByte = 0;
	CBlock*	pBlock = CBlock::Create(m_pGraphicDev);
	CGameObject*	pTemp = nullptr;

	while (true)
	{
		if (pBlock)
		{
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

			TCHAR*	pName = new TCHAR[dwStrByte];
			ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

			ReadFile(hFile, &pBlock->m_pTransCom->m_vInfo[INFO_RIGHT], sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &pBlock->m_pTransCom->m_vInfo[INFO_UP], sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &pBlock->m_pTransCom->m_vInfo[INFO_LOOK], sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &pBlock->m_pTransCom->m_vInfo[INFO_POS], sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &pBlock->m_pTransCom->m_vAngle, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &pBlock->m_pTransCom->m_vScale, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &pBlock->m_pTransCom->m_matWorld, sizeof(_matrix), &dwByte, nullptr);
			ReadFile(hFile, &pBlock->m_eCurrentType, sizeof(BLOCKTYPE), &dwByte, nullptr);
			ReadFile(hFile, &pBlock->m_iTexture, sizeof(_int), &dwByte, nullptr);
			ReadFile(hFile, &pBlock->m_fScale, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &pBlock->m_bSet, sizeof(_bool), &dwByte, nullptr);

			if (0 == dwByte)
			{
				Safe_Release(pBlock);
				Safe_Delete_Array(wpPath);
				Safe_Delete_Array(pName);
				break;
			}
			vecObjTags.push_back(pName);
			pTemp = CBlock::Create(*pBlock);
			pLayer->Add_GameObject(vecObjTags.back(), pTemp);
		}
	}
	CloseHandle(hFile);
	Safe_Release(pBlock);
	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	// UI_HPGauge
	pGameObject = CHPGauge::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_HPGauge", pGameObject), E_FAIL);

	// UI_HealthBar
	pGameObject = CHealthBar::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_HealthBar", pGameObject), E_FAIL);

	// UI_QuickSlot
	pGameObject = CQuickSlot::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_QuickSlot", pGameObject), E_FAIL);

	// UI_Inventory
	pGameObject = CInventory::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Inventory", pGameObject), E_FAIL);

	// UI_EquipWindow
	pGameObject = CEquipWindow::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_EquipWindow", pGameObject), E_FAIL);

	// NPC_TalkWindow
	pGameObject = CTalkWindow::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_TalkWindow", pGameObject), E_FAIL);

	// UI_MAP
	pGameObject = CMapUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Map", pGameObject), E_FAIL);

	// UI_MiniMap
	pGameObject = CMiniMap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_MiniMap", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Proto(void)
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Tile/textures_%d.png", TEX_NORMAL, 18)), E_FAIL);
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

	return S_OK;
}

CStage * CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage *	pInstance = new CStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStage::Free(void)
{
	for (size_t i = 0; i < vecObjTags.size(); i++)
		Safe_Delete_Array(vecObjTags[i]);
	vecObjTags.clear();

	CCollisionMgr::DestroyInstance();
	CBulletMgr::DestroyInstance();

	CScene::Free();
}

HRESULT CStage::Ready_Light(void)
{
	D3DLIGHT9		tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type		= D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Direction  = _vec3(0.f, -1.f, 1.f);

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}
