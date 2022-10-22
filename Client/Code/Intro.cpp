#include "stdafx.h"
#include "Intro.h"
#include "Stage.h"
#include "Export_Function.h"

// Managers
#include "BlockVIBuffer.h"
#include "ParticleMgr.h"
#include "CameraMgr.h"
#include "BulletMgr.h"
#include "ItemMgr.h"
#include "UIMgr.h"

// Obj
#include "Block.h"
#include "Player.h"
#include "SkyBox.h"
#include "EcoObject.h"
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
#include "Loading_Scene.h"

CIntro::CIntro(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CIntro::~CIntro()
{
}

HRESULT CIntro::Ready_Scene(void)
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

_int CIntro::Update_Scene(const _float & fTimeDelta)
{
	_int	iResult = Engine::CScene::Update_Scene(fTimeDelta);

	

	return iResult;
}

void CIntro::LateUpdate_Scene(void)
{
	if (Engine::Key_Down(DIK_F9))
	{
		CCameraMgr::DestroyInstance();
		CItemMgr::DestroyInstance();
		CParticleMgr::DestroyInstance();
		CUIMgr::DestroyInstance();
		CBlockVIBuffer::DestroyInstance();

		CScene*	pLoadingScene = CLoading_Scene::Create(m_pGraphicDev, LOADING_STAGE);
		NULL_CHECK(pLoadingScene);
		Engine::Set_Scene(pLoadingScene);
		return;
	}

	CBlock*	pBlock = nullptr;
	CLayer*	pLayer = m_mapLayer[L"Layer_GameLogic"];
	CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

	for (auto iter = pLayer->Get_mapGameObject()->begin(); iter != pLayer->Get_mapGameObject()->end(); ++iter)
	{
		pBlock = dynamic_cast<CBlock*>(iter->second);

		if (pBlock)
			Engine::CollisionTest(pBlock, pPlayer);
	}

	Engine::CScene::LateUpdate_Scene();
}
void CIntro::Render_Scene(void)
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

HRESULT CIntro::Ready_Layer_Environment(const _tchar * pLayerTag)
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

HRESULT CIntro::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	// Player
	pGameObject = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);

	// Bullet
	//FAILED_CHECK_RETURN(CBulletMgr::GetInstance()->Ready_Clone(pLayer, m_pGraphicDev), E_FAIL);

	// Item
	//CItemMgr::GetInstance()->Add_GameObject(pLayer);

	// Particle
	CParticleMgr::GetInstance()->Add_GameObject(pLayer);

	// Blocks
	{
		string	strPath = "..\\..\\Data\\Map_Intro.dat";
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
			}
			pLayer->Add_GameObject(m_vecObjTags.back(), pCloneObject);
		}
		CloseHandle(hFile);
	}

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CIntro::Ready_Layer_UI(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	// UI
	CUIMgr::GetInstance()->Add_GameObject(pLayer);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CIntro::Ready_Proto(void)
{
	return S_OK;
}

HRESULT CIntro::Ready_Light(void)
{
	// Default
	D3DLIGHT9		tLightInfo0;
	ZeroMemory(&tLightInfo0, sizeof(D3DLIGHT9));

	tLightInfo0.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo0.Diffuse = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f);
	tLightInfo0.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo0.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo0.Direction = _vec3(0.f, -1.f, 0.f);

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo0, LIGHT_STAGE), E_FAIL);

	return S_OK;
}

CIntro * CIntro::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CIntro *	pInstance = new CIntro(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CIntro::Free(void)
{
	for (size_t i = 0; i < m_vecObjTags.size(); i++)
		Safe_Delete_Array(m_vecObjTags[i]);
	m_vecObjTags.clear();

	CScene::Free();
}
