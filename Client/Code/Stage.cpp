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
#include "CrossHair.h"
#include "HitBackGround.h"

// Font
#include "HPGauge.h"
#include "TalkWindow.h"

// Manager
#include "ItemMgr.h"
#include "ParticleMgr.h"
#include "CameraMgr.h"
#include "BlockVIBuffer.h"

// EcoObject
#include "Stone.h"
#include "Grass.h"
#include "Tree.h"
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


	// 플레이어와 아이템
	for (int i = 0; i < ITEM_IMG; ++i)
	{
		vector<CGameObject*>*	pItems = CItemMgr::GetInstance()->Get_Items((ITEMTYPE)i);
		for (auto& item : *pItems)
		{
			Engine::CollisionAABB(pPlayer, item);
		}
	}

	// 무기와 환경요소
	vector<CGameObject*>* pItems = CItemMgr::GetInstance()->Get_Items(ITEM_WEAPON);
	CGameObject* pSour = nullptr;
	CGameObject* pSour2 = nullptr;
	CGameObject* pSour3 = nullptr;
	CGameObject* pSour4 = nullptr;
	CGameObject* pSour5 = nullptr;

	pSour = Engine::Get_GameObject(L"Layer_GameLogic", L"Jar");
	pSour2 = Engine::Get_GameObject(L"Layer_GameLogic", L"Jar_0");
	pSour3 = Engine::Get_GameObject(L"Layer_GameLogic", L"Jar_1");
	pSour4 = Engine::Get_GameObject(L"Layer_GameLogic", L"Jar_2");
	pSour5 = Engine::Get_GameObject(L"Layer_GameLogic", L"Jar_3");

	for (auto& weapon : *pItems)
	{
		// sh_Monster Test
		pSour = Engine::Get_GameObject(L"Layer_GameLogic", L"GreenSlime");
		Engine::CollisionTest(pSour, weapon);

		pSour = Engine::Get_GameObject(L"Layer_GameLogic", L"PinkSlime");
		Engine::CollisionTest(pSour, weapon);

		//pSour = Engine::Get_GameObject(L"Layer_GameLogic", L"PinkSlime_0");
		//Engine::CollisionTest(pSour, weapon);

		//pSour = Engine::Get_GameObject(L"Layer_GameLogic", L"PinkSlime_1");
		//Engine::CollisionTest(pSour, weapon);

		//pSour = Engine::Get_GameObject(L"Layer_GameLogic", L"PinkSlime_2");
		//Engine::CollisionTest(pSour, weapon);

		// Eco
		Engine::CollisionAABB(pSour, weapon);

		Engine::CollisionAABB(pSour2, weapon);

		Engine::CollisionAABB(pSour3, weapon);

		Engine::CollisionAABB(pSour4, weapon);

		Engine::CollisionAABB(pSour5, weapon);
	}

	// Bullet 테스트
	vector<CGameObject*>*	pPlayerBullets = CBulletMgr::GetInstance()->Get_Bullets(BULLET_WAND);
	for (auto& bullet : *pPlayerBullets)
	{
		pSour = Engine::Get_GameObject(L"Layer_GameLogic", L"Fist"); // sh_확인 필요
		Engine::CollisionTest(pSour, bullet);

		// Eco
		pSour = Engine::Get_GameObject(L"Layer_GameLogic", L"Jar");
		Engine::CollisionTest(pSour, bullet);

		Engine::CollisionTest(pSour2, bullet);

		Engine::CollisionTest(pSour3, bullet);

		Engine::CollisionTest(pSour4, bullet);

		Engine::CollisionTest(pSour5, bullet);
	}

	pPlayerBullets = CBulletMgr::GetInstance()->Get_Bullets(BULLET_ARROW);
	for (auto& bullet : *pPlayerBullets)
	{
		// sh_Monster Test
		pSour = Engine::Get_GameObject(L"Layer_GameLogic", L"GreenSlime");
		Engine::CollisionTest(pSour, bullet);
		Engine::CollisionTest(pSour, pPlayer); // 몬스터 플레이어

		pSour = Engine::Get_GameObject(L"Layer_GameLogic", L"BrownBat");
		Engine::CollisionTest(pSour, bullet);

		pSour = Engine::Get_GameObject(L"Layer_GameLogic", L"Fist");
		Engine::CollisionTest(pSour, bullet);

		// Eco
		pSour = Engine::Get_GameObject(L"Layer_GameLogic", L"Jar");
		Engine::CollisionTest(pSour, bullet);

		Engine::CollisionTest(pSour2, bullet);

		Engine::CollisionTest(pSour3, bullet);

		Engine::CollisionTest(pSour4, bullet);

		Engine::CollisionTest(pSour5, bullet);
	}


	//// Monster Collider
	//pSour = Engine::Get_GameObject(L"Layer_GameLogic", L"GreenSlime");
	//for (auto& bullet : *CBulletMgr::GetInstance()->Get_Bullets(BULLET_WAND))
	//	Engine::CollisionTest(pSour, bullet);
	////for (auto& bullet : *CBulletMgr::GetInstance()->Get_Bullets(BULLET_ARROW))
	////	Engine::CollisionTest(pSour, bullet);

	

	//pSour = Engine::Get_GameObject(L"Layer_GameLogic", L"PinkSlime");
	//for (auto& bullet : *CBulletMgr::GetInstance()->Get_Bullets(BULLET_WAND))
	//	Engine::CollisionTest(pSour, bullet);
	////for (auto& bullet : *CBulletMgr::GetInstance()->Get_Bullets(BULLET_ARROW))
	////	Engine::CollisionTest(pSour, bullet);

	//Engine::CollisionTest(pSour, pPlayer);




	
	Engine::CScene::LateUpdate_Scene();
}

void CStage::Render_Scene(void)
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

HRESULT CStage::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	
	CGameObject*		pGameObject = nullptr;

	// CameraMgr
	CCameraMgr::GetInstance()->Add_GameObject(pLayer);

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
	CItemMgr::GetInstance()->Add_GameObject(pLayer, L"Dagger", ITEM_WEAPON, _vec3({ 40.f, 1.f, 40.f }));
	CItemMgr::GetInstance()->Add_GameObject(pLayer, L"Wand", ITEM_WEAPON, _vec3({ 35.f, 1.f, 35.f }));
	CItemMgr::GetInstance()->Add_GameObject(pLayer, L"Key", ITEM_KEY, _vec3({ 5.f, 1.f, 10.f }));

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
	{
		string	strPath = "..\\..\\Data\\Map.dat";
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
	}

	// Eco Object
	{
		string	strPath = "..\\..\\Data\\EcoObject.dat";
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
		CEcoObject* pEcoObject = CEcoObject::Create(m_pGraphicDev);
		CEcoObject* pCloneObject = nullptr;
		CTransform*	pTransCom = dynamic_cast<CTransform*>(pEcoObject->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));

		while (true)
		{
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

			TCHAR*	pName = new TCHAR[dwStrByte];
			ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

			ReadFile(hFile, &pEcoObject->m_eType, sizeof(ECOOBJTYPE), &dwByte, nullptr);
			ReadFile(hFile, &pTransCom->m_vInfo[INFO_RIGHT], sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &pTransCom->m_vInfo[INFO_UP], sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &pTransCom->m_vInfo[INFO_LOOK], sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &pTransCom->m_vInfo[INFO_POS], sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &pTransCom->m_vAngle, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &pTransCom->m_vScale, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &pTransCom->m_matWorld, sizeof(_matrix), &dwByte, nullptr);
			_int	iTexture = 0;
			ReadFile(hFile, &iTexture, sizeof(_int), &dwByte, nullptr);
			pEcoObject->Set_CurrentTexture(iTexture);

			if (0 == dwByte)
			{
				Safe_Release(pEcoObject);
				Safe_Delete_Array(wpPath);
				Safe_Delete_Array(pName);
				break;
			}

			vecObjTags.push_back(pName);
			switch (pEcoObject->m_eType)
			{
			case ECO_STONE:
				pCloneObject = CStone::Create(pEcoObject);
				break;

			case ECO_GRASS:
				pCloneObject = CGrass::Create(pEcoObject);
				break;

			case ECO_TREE:
				pCloneObject = CTree::Create(pEcoObject);
				break;

			case ECO_JAR:
				pCloneObject = CJar::Create(pEcoObject);
				break;

			case ECO_JAM:
				break;
			}
			pLayer->Add_GameObject(vecObjTags.back(), pCloneObject);
		}
		CloseHandle(hFile);
	}
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

	// UI_CrossHair
	pGameObject = CCrossHair::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_CrossHair", pGameObject), E_FAIL);

	// UI_HitBackGround
	pGameObject = CHitBackGround::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_HitBackGround", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Proto(void)
{
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
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexCom", CTerrainTex::Create(m_pGraphicDev, 20, 20, iInterval)), E_FAIL);

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
