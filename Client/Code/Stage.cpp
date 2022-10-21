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

// Manager
#include "ItemMgr.h"
#include "ParticleMgr.h"
#include "CameraMgr.h"
#include "BlockVIBuffer.h"
#include "MonsterMgr.h"
#include "NPCMgr.h"
#include "UIMgr.h"

// EcoObject
#include "Stone.h"
#include "Grass.h"
#include "Tree.h"
#include "Jar.h"
#include "BonFire.h"
#include "ObjectCamera.h"
#include "LongTorch.h"
#include "RockFall.h"
#include "Jam.h"
#include "ShortTorch.h"
#include "EcoMush.h"
#include "EcoWeb.h"
#include "Statue.h"

#include "Water.h"

#include "Cat.h"

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
	CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
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
			Engine::CollisionTest(pBlock, pPlayer);
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

	// Player / Monster
	vector<CGameObject*>* pMonster = CMonsterMgr::GetInstance()->Get_Monster();
	for (auto& monster : *pMonster)
		Engine::CollisionAABB(pPlayer, monster);

	// Player / MonsterBullet
	vector<CGameObject*>*	pMonsterBullet = CBulletMgr::GetInstance()->Get_Bullets(BULLET_M_FIST);
	for (auto& bullet : *pMonsterBullet)
		Engine::CollisionAABB(pPlayer, bullet);

	pMonsterBullet = CBulletMgr::GetInstance()->Get_Bullets(BULLET_M_LEAF);
	for (auto& bullet : *pMonsterBullet)
		Engine::CollisionAABB(pPlayer, bullet);

	pMonsterBullet = CBulletMgr::GetInstance()->Get_Bullets(BULLET_M_SPIDER);
	for (auto& bullet : *pMonsterBullet)
		Engine::CollisionAABB(pPlayer, bullet);

	pMonsterBullet = CBulletMgr::GetInstance()->Get_Bullets(BULLET_SONGBOSS);
	for (auto& bullet : *pMonsterBullet)
 		Engine::CollisionAABB(pPlayer, bullet);

	pMonsterBullet = CBulletMgr::GetInstance()->Get_Bullets(FLOOR_SONGBOSS);
	for (auto& bullet : *pMonsterBullet)
		Engine::CollisionAABB(pPlayer, bullet);

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
	//pSour4 = Engine::Get_GameObject(L"Layer_GameLogic", L"Jar_2");
	//pSour5 = Engine::Get_GameObject(L"Layer_GameLogic", L"Jar_3");

	for (auto& weapon : *pItems)
	{
		for (auto& monster : *pMonster)
			Engine::CollisionAABB(monster, weapon);

		// 보스 스킬, 무기
		vector<CGameObject*>*	pMonsterullets = CBulletMgr::GetInstance()->Get_Bullets(STUN_SONGBOSS);
		for (auto& bullet : *pMonsterullets)
			Engine::CollisionAABB(bullet, weapon);

		// Eco
		//Engine::CollisionAABB(pSour, weapon);

		//Engine::CollisionAABB(pSour2, weapon);

		//Engine::CollisionAABB(pSour3, weapon);

		//Engine::CollisionAABB(pSour4, weapon);

		//Engine::CollisionAABB(pSour5, weapon);
	}

	// Bullet 테스트
	vector<CGameObject*>*	pPlayerBullets = CBulletMgr::GetInstance()->Get_Bullets(BULLET_WAND);
	for (auto& bullet : *pPlayerBullets)
	{
		for (auto& monster : *pMonster)
			Engine::CollisionAABB(monster, bullet);

		// Eco
		pSour = Engine::Get_GameObject(L"Layer_GameLogic", L"Jar");
		//Engine::CollisionTest(pSour, bullet);

		//Engine::CollisionTest(pSour2, bullet);

		//Engine::CollisionTest(pSour3, bullet);

		//Engine::CollisionTest(pSour4, bullet);

		//Engine::CollisionTest(pSour5, bullet);
	}

	pPlayerBullets = CBulletMgr::GetInstance()->Get_Bullets(BULLET_ARROW);
	for (auto& bullet : *pPlayerBullets)
	{
		for (auto& monster : *pMonster)
			Engine::CollisionAABB(monster, bullet);

		// Eco
		pSour = Engine::Get_GameObject(L"Layer_GameLogic", L"Jar");
		//Engine::CollisionTest(pSour, bullet);

		//Engine::CollisionTest(pSour2, bullet);

		//Engine::CollisionTest(pSour3, bullet);

		//Engine::CollisionTest(pSour4, bullet);

		//Engine::CollisionTest(pSour5, bullet);
	}

	pPlayerBullets = CBulletMgr::GetInstance()->Get_Bullets(BULLET_GREENWAND);
	for (auto& bullet : *pPlayerBullets)
	{
		for (auto& monster : *pMonster)
			Engine::CollisionAABB(monster, bullet);
	}

	pPlayerBullets = CBulletMgr::GetInstance()->Get_Bullets(BULLET_REDWAND);
	for (auto& bullet : *pPlayerBullets)
	{
		for (auto& monster : *pMonster)
			Engine::CollisionAABB(monster, bullet);
	}


	// Player의 CollisionGroup
	vector<CGameObject*>* pCollisionGroup = pPlayer->Get_CollisionGroup();
	for (auto& obj : *pCollisionGroup)
	{
		Engine::CollisionAABB(obj, pPlayer);
	}


	
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
	//pGameObject = CTerrain::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);
	


	// Water Flow
	pGameObject = CWater::Create(m_pGraphicDev, WATER_FLOW1);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WaterFlow1", pGameObject), E_FAIL);

	// Water Fall
	pGameObject = CWater::Create(m_pGraphicDev, WATER_FALL1);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WaterFall1", pGameObject), E_FAIL);

	// Ocean
	pGameObject = CWater::Create(m_pGraphicDev, WATER_OCEAN1);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WATEROCEAN1", pGameObject), E_FAIL);

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
	//CItemMgr::GetInstance()->Add_GameObject(pLayer, L"Arrow", ITEM_WEAPON, _vec3({ 30.f, 1.f, 30.f }));
	//CItemMgr::GetInstance()->Add_GameObject(pLayer, L"Dagger", ITEM_WEAPON, _vec3({ 40.f, 1.f, 40.f }));
	//CItemMgr::GetInstance()->Add_GameObject(pLayer, L"Wand", ITEM_WEAPON, _vec3({ 35.f, 1.f, 35.f }));
	//CItemMgr::GetInstance()->Add_GameObject(pLayer, L"Key", ITEM_KEY, _vec3({ 5.f, 1.f, 10.f }));
	CItemMgr::GetInstance()->Add_GameObject(pLayer);


	// Particle
 	CParticleMgr::GetInstance()->Add_GameObject(pLayer);

	// Monster
	CMonsterMgr::GetInstance()->Add_GameObject(pLayer);

	// NPC
	CNPCMgr::GetInstance()->Add_GameObject(pLayer);

	// EcoObject
	pGameObject = CJar::Create(m_pGraphicDev, _vec3({30.f, 1.f, 40.f}));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Jar", pGameObject), E_FAIL);

	pGameObject = CRockFall::Create(m_pGraphicDev, _vec3({ -6.f, 7.f, -50.f }));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RockFall", pGameObject), E_FAIL);

	pGameObject = CBonFire::Create(m_pGraphicDev, _vec3({ 107.f, -38.f, 77.f }));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Bonfire", pGameObject), E_FAIL);

	//pGameObject = CLongTorch::Create(m_pGraphicDev, _vec3({ 6.f, 0.9f, 5.f }));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Torch2", pGameObject), E_FAIL);

	// Cat
	pGameObject = CCat::Create(m_pGraphicDev, _vec3({ -6.f, 2.f, -40.f }));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cat", pGameObject), E_FAIL);


	// Blocks
	{
		string	strPath = "..\\..\\Data\\Map_Test7.dat";
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
		string	strPath = "..\\..\\Data\\EcoObject_Intro.dat";
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

			case ECO_BONFIRE:
				pCloneObject = CBonFire::Create(pEcoObject);
				break;

			case ECO_JAM:
				pCloneObject = CJam::Create(pEcoObject);
				break;

			case ECO_TORCH2:
				pCloneObject = CShortTorch::Create(pEcoObject);
				break;

			case ECO_TORCH1:
				pCloneObject = CLongTorch::Create(pEcoObject);
				break;

			case ECO_MUSHROOM:
				pCloneObject = CEcoMush::Create(pEcoObject);
				break;

			case ECO_WEB:
				pCloneObject = CEcoWeb::Create(pEcoObject);
				break;

			case ECO_STATUE:
				pCloneObject = CStatue::Create(pEcoObject);
				break;

			case ECO_ROCKFALL:
				pCloneObject = CRockFall::Create(pEcoObject);
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

	// UI
	CUIMgr::GetInstance()->Add_GameObject(pLayer);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Proto(void)
{
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

	_int	iWidth, iDepth, iInterval;
	CImGuiMgr::GetInstance()->Get_MapWidth(&iWidth, &iDepth, &iInterval);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WaterTexCom", CTerrainTex::Create(m_pGraphicDev, 20, 17, iInterval, _vec3(20,0,11))), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WaterFallTexCom", CTerrainTex::Create(m_pGraphicDev, 7, 100 , iInterval, _vec3(0, 0,0))), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_OceanTexCom", CTerrainTex::Create(m_pGraphicDev, 110, 105, iInterval, _vec3(0, 0, 0))), E_FAIL);


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
	// Default
	D3DLIGHT9		tLightInfo0;
	ZeroMemory(&tLightInfo0, sizeof(D3DLIGHT9));

	tLightInfo0.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo0.Diffuse = D3DXCOLOR(0.1f, 0.1f,0.1f,0.1f);
	tLightInfo0.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo0.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo0.Direction  = _vec3(0.f, -1.f, 0.f);

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo0, LIGHT_STAGE), E_FAIL);


	// Player
	D3DLIGHT9		tLightInfo1;
	ZeroMemory(&tLightInfo1, sizeof(D3DLIGHT9));
	tLightInfo1.Type		= D3DLIGHT_POINT;
	tLightInfo1.Diffuse		= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo1.Specular	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo1.Ambient		= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo1.Position	= _vec3(7.f, 2.f, 5.f);
	tLightInfo1.Attenuation0 = 0.00001f;
	tLightInfo1.Range = 300.f;
	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo1, LIGHT_PLAYER), E_FAIL);
	m_pGraphicDev->LightEnable(LIGHT_PLAYER, FALSE);


	// Lantern
	D3DLIGHT9		tLightInfo2;
	ZeroMemory(&tLightInfo2, sizeof(D3DLIGHT9));
	tLightInfo2.Type = D3DLIGHT_POINT;
	tLightInfo2.Diffuse = D3DXCOLOR(1.f, 0.6f, 0.f, 1.f);
	tLightInfo2.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo2.Ambient = D3DXCOLOR(1.f, 0.6f, 0.f, 1.f);
	tLightInfo2.Position = _vec3(7.f, 2.f, 5.f);
	tLightInfo2.Attenuation0 = 0.00001f;
	tLightInfo2.Range = 5.f;
	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo2, LIGHT_LANTERN), E_FAIL);
	m_pGraphicDev->LightEnable(LIGHT_LANTERN, FALSE);


	// Wand
	D3DLIGHT9		tLightInfo3;
	ZeroMemory(&tLightInfo3, sizeof(D3DLIGHT9));
	tLightInfo3.Type = D3DLIGHT_POINT;
	tLightInfo3.Diffuse = D3DXCOLOR(0.5f, 0.f, 0.5f, 1.f);
	tLightInfo3.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo3.Ambient = D3DXCOLOR(0.5f, 0.f, 0.5f, 1.f);
	tLightInfo3.Position = _vec3(7.f, 2.f, 5.f);
	tLightInfo3.Range = 0.5f;
	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo3, LIGHT_WAND), E_FAIL);
	m_pGraphicDev->LightEnable(LIGHT_WAND, FALSE);

	//D3DLIGHT9	tLightInfo2;
	//ZeroMemory(&tLightInfo2, sizeof(D3DLIGHT9));

	//tLightInfo2.Type = D3DLIGHT_POINT;
	//tLightInfo2.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo2.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo2.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo2.Position = _vec3(10.f, 3.f, 9.f);
	//tLightInfo2.Range = 3.f;

	//FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo2, 1), E_FAIL);
	//
	return S_OK;
}