#include "stdafx.h"
#include "..\Header\Boss.h"
#include "Export_Function.h"

#include "CameraMgr.h"
#include "ItemMgr.h"
#include "ParticleMgr.h"
#include "UIMgr.h"
#include "BlockVIBuffer.h"
#include "BulletMgr.h"
#include "MonsterMgr.h"
#include "NPCMgr.h"

#include "SkyBox.h"
#include "Player.h"
#include "Block.h"

// Object
#include "Stone.h"
#include "Grass.h"
#include "Tree.h"
#include "Jar.h"
#include "BonFire.h"
#include "Jam.h"
#include "ShortTorch.h"
#include "LongTorch.h"
#include "EcoMush.h"
#include "EcoWeb.h"
#include "Statue.h"
#include "RockFall.h"
#include "TreasureBox.h"

CBoss::CBoss(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CBoss::~CBoss()
{
}

HRESULT CBoss::Ready_Scene(void)
{
	CCameraMgr::GetInstance()->Ready_CameraMgr(m_pGraphicDev);
	CItemMgr::GetInstance()->Ready_ItemMgr(m_pGraphicDev);
	CMonsterMgr::GetInstance()->Ready_MonsterMgr(m_pGraphicDev);
	CParticleMgr::GetInstance()->Ready_ParticleMgr(m_pGraphicDev);
	CUIMgr::GetInstance()->Ready_UI(m_pGraphicDev);

	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Light(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);

	if (!g_bIsTool)
	{
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
	return S_OK;
}

_int CBoss::Update_Scene(const _float & fTimeDelta)
{
	static _bool bBossBGM = false;
	if (!bBossBGM)
	{
		//Engine::PlayBGM(L".mp3", 1.f);	// TODO : Add BGM Here
		bBossBGM = true;
	}

	return Engine::CScene::Update_Scene(fTimeDelta);
}

void CBoss::LateUpdate_Scene(void)
{
	// Player
	CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

	// Player's CollisionGroup
	vector<CGameObject*>*	pCollisionGroup = pPlayer->Get_CollisionGroup();

	// Monster
	vector<CGameObject*>*	pMonsterGroup = CMonsterMgr::GetInstance()->Get_Monster();

	// Item
	vector<CGameObject*>*	pItemGroup = nullptr;
	vector<CGameObject*>*	pWeaponGroup = CItemMgr::GetInstance()->Get_Items(ITEM_WEAPON);

	// Bullets
	vector<CGameObject*>*	pPlayerBulletGroup[4];
	pPlayerBulletGroup[0] = CBulletMgr::GetInstance()->Get_Bullets(BULLET_WAND);
	pPlayerBulletGroup[1] = CBulletMgr::GetInstance()->Get_Bullets(BULLET_ARROW);
	pPlayerBulletGroup[2] = CBulletMgr::GetInstance()->Get_Bullets(BULLET_GREENWAND);
	pPlayerBulletGroup[3] = CBulletMgr::GetInstance()->Get_Bullets(BULLET_REDWAND);

	vector<CGameObject*>*	pMonsterBulletGroup[3];
	pMonsterBulletGroup[0] = CBulletMgr::GetInstance()->Get_Bullets(BULLET_M_FIST);
	pMonsterBulletGroup[1] = CBulletMgr::GetInstance()->Get_Bullets(BULLET_M_LEAF);
	pMonsterBulletGroup[2] = CBulletMgr::GetInstance()->Get_Bullets(BULLET_M_SPIDER);

	vector<CGameObject*>*	pBossBulletGroup[4];
	pBossBulletGroup[0] = CBulletMgr::GetInstance()->Get_Bullets(STUN_SONGBOSS);
	pBossBulletGroup[1] = CBulletMgr::GetInstance()->Get_Bullets(BULLET_SONGBOSS);
	pBossBulletGroup[2] = CBulletMgr::GetInstance()->Get_Bullets(FLOOR_SONGBOSS);
	pBossBulletGroup[3] = CBulletMgr::GetInstance()->Get_Bullets(LIGHTNING_SONGBOSS);
	// ~Bullets

	for (auto& obj : *pCollisionGroup)
	{
		Engine::CollisionAABB(pPlayer, obj);		// Player

		for (auto& weapon : *pWeaponGroup)			// weapon
		{
			Engine::CollisionAABB(obj, weapon);
		}

		for (int i = 0; i < 4; ++i)					// playerBullet	
		{
			for (auto& bullet : *pPlayerBulletGroup[i])
			{
				Engine::CollisionAABB(obj, bullet);
			}
		}
	}

	for (int i = 0; i < ITEM_IMG; ++i)
	{
		vector<CGameObject*>*	pItems = CItemMgr::GetInstance()->Get_Items((ITEMTYPE)i);
		for (auto& item : *pItems)
		{
			Engine::CollisionAABB(pPlayer, item);
		}
	}

	for (auto& monster : *pMonsterGroup)
	{
		Engine::CollisionAABB(pPlayer, monster);	// monster <-> player

		for (auto& weapon : *pWeaponGroup)			// monster <-> weapon
		{
			Engine::CollisionAABB(monster, weapon);
		}

		for (int i = 0; i < 4; ++i)					// monster <-> playerBullet
		{
			for (auto& bullet : *pPlayerBulletGroup[i])
			{
				Engine::CollisionAABB(monster, bullet);
			}
		}
	}

	for (auto& block : m_vecBlocks)
	{
		Engine::CollisionAABB(block, pPlayer);
	}

	// boss bullet 
	for (int i = 0; i < 3; ++i)
	{
		for (auto& bullet : *pBossBulletGroup[i])
		{
			if (i == 0)
			{
				for (auto& weapon : *pWeaponGroup)
				{
					Engine::CollisionAABB(weapon, bullet);
				}
			}
			else
				Engine::CollisionAABB(pPlayer, bullet);
		}
	}

	Engine::CScene::LateUpdate_Scene();
}

void CBoss::Render_Scene(void)
{
}

HRESULT CBoss::Ready_Layer_Environment(const _tchar * pLayerTag)
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

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CBoss::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	// Player
	pGameObject = CPlayer::Create(m_pGraphicDev, _vec3(5.f, 2.f, 5.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);

	// Bullet
	FAILED_CHECK_RETURN(CBulletMgr::GetInstance()->Ready_Clone(pLayer, m_pGraphicDev), E_FAIL);

	// Item
	CItemMgr::GetInstance()->Add_GameObject(pLayer, LOADING_BOSS);

	// Particle
	CParticleMgr::GetInstance()->Add_GameObject(pLayer);

	// Monster
	CMonsterMgr::GetInstance()->Add_GameObject(pLayer, L"..\\..\\Data\\Monsters_Stage.dat");

	// NPC
	CNPCMgr::GetInstance()->Add_GameObject(pLayer);

	// Blocks
	{
		string	strPath = "..\\..\\Data\\Map_Boss5.dat";
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
				m_vecObjTags.push_back(pName);
				pTemp = CBlock::Create(*pBlock);
				pLayer->Add_GameObject(m_vecObjTags.back(), pTemp);

				m_vecBlocks.push_back(pTemp);


			}
		}
		CloseHandle(hFile);
		Safe_Release(pBlock);
	}

	// Eco Object
	{
		string	strPath = "..\\..\\Data\\EcoObject_Stage.dat";
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

			m_vecObjTags.push_back(pName);
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

			case ECO_TREASUREBOX:
				pCloneObject = CTreasureBox::Create(pEcoObject);
				break;
			}
			pLayer->Add_GameObject(m_vecObjTags.back(), pCloneObject);
		}
		CloseHandle(hFile);
	}

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CBoss::Ready_Layer_UI(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	// UI
	CUIMgr::GetInstance()->Add_GameObject(pLayer);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CBoss::Ready_Proto(void)
{
	Engine::Delete_Proto(L"Proto_CubeTexture");
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/Waterfall Skybox.dds", TEX_CUBE)), E_FAIL);

	return S_OK;
}

HRESULT CBoss::Ready_Light(void)
{
	return S_OK;
}

CBoss * CBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss *	pInstance = new CBoss(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss::Free(void)
{
	for (size_t i = 0; i < m_vecObjTags.size(); i++)
		Safe_Delete_Array(m_vecObjTags[i]);
	m_vecObjTags.clear();

	m_vecBlocks.clear();

	CCollisionMgr::DestroyInstance();
	CBulletMgr::DestroyInstance();

	CScene::Free();
}