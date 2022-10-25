#include "stdafx.h"
#include "..\Header\Boss.h"
#include "Export_Function.h"
#include "CameraMgr.h"
#include "ItemMgr.h"
#include "ParticleMgr.h"
#include "UIMgr.h"
#include "BlockVIBuffer.h"
#include "BulletMgr.h"

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
}

void CBoss::Render_Scene(void)
{
}

HRESULT CBoss::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CBoss::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CBoss::Ready_Layer_UI(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CBoss::Ready_Proto(void)
{
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
