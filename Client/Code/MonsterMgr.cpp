#include "stdafx.h"
#include "MonsterMgr.h"

#include "Export_Function.h"

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

#include "SongBoss.h"

#include "Arrow.h"
#include "Wand.h"
#include "InvImg.h"

IMPLEMENT_SINGLETON(CMonsterMgr)

CMonsterMgr::CMonsterMgr()
{
}

CMonsterMgr::~CMonsterMgr()
{
	Free();
}

void CMonsterMgr::Ready_ItemMgr(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();
}

HRESULT CMonsterMgr::Ready_Proto()
{
	///////// Monster
	// GreenSlime_Texture 
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_GreenSlimeIDLE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/GreenSlime/Idle/GreenSlime%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_GreenSlimeATTACK_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/GreenSlime/Attack/GreenSlime%d.png", TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_GreenSlimeHIT_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/GreenSlime/Hit/GreenSlime%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_GreenSlimeDIE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/GreenSlime/Die/GreenSlime%d.png", TEX_NORMAL, 3)), E_FAIL);

	// PinkSlime_Texture 
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PinkSlimeIDLE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/PinkSlime/Idle/PinkSlime%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PinkSlimeATTACK_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/PinkSlime/Attack/PinkSlime%d.png", TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PinkSlimeHIT_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/PinkSlime/Hit/PinkSlime%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PinkSlimeDIE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/PinkSlime/Die/PinkSlime%d.png", TEX_NORMAL, 12)), E_FAIL);

	// Fist_Texture 
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FistIDLE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Fist/Idle/Fist%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FistATTACK_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Fist/Attack/Fist%d.png", TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FistHIT_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Fist/Hit/Fist%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FistDIE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Fist/Die/Fist%d.png", TEX_NORMAL, 3)), E_FAIL);

	// BlueBat_Texture 
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlueBatIDLE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/BlueBat/Idle/BlueBat%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlueBatATTACK_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/BlueBat/Attack/BlueBat%d.png", TEX_NORMAL, 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlueBatHIT_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/BlueBat/Hit/BlueBat%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlueBatDIE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/BlueBat/Die/BlueBat%d.png", TEX_NORMAL, 3)), E_FAIL);

	// BrownBat_Texture 
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BrownBatIDLE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/BrownBat/Idle/BrownBat%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BrownBatATTACK_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/BrownBat/Attack/BrownBat%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BrownBatHIT_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/BrownBat/Hit/BrownBat%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BrownBatDIE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/BrownBat/Die/BrownBat%d.png", TEX_NORMAL, 2)), E_FAIL);

	// Stick_Texture 
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StickIDLE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Stick/Idle/Stick%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StickANGER_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Stick/Anger/Stick%d.png", TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StickATTACK_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Stick/Attack/Stick%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StickHIT_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Stick/Hit/Stick%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StickDIE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Stick/Die/Stick%d.png", TEX_NORMAL, 3)), E_FAIL);

	// Leaf_Texture 
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LeafIDLE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Leaf/Idle/Leaf%d.png", TEX_NORMAL, 4)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LeafLEAF_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Leaf/Leaf/Leaf%d.png", TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LeafATTACK_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Leaf/Attack/Leaf%d.png", TEX_NORMAL, 14)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LeafHIT_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Leaf/Hit/Leaf%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LeafDIE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Leaf/Die/Leaf%d.png", TEX_NORMAL, 3)), E_FAIL);

	////////////// Boss
	// SongBoss_Texture 
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SongBossMOVE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/SongBoss/Move/SongBoss%d.png", TEX_NORMAL, 16)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SongBossIDLE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/SongBoss/Idle/SongBoss%d.png", TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SongBossATTACK_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/SongBoss/Attack/SongBoss%d.png", TEX_NORMAL, 12)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SongBossHIT_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/SongBoss/Hit/SongBoss%d.png", TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SongBossDIE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/SongBoss/Die/SongBoss%d.png", TEX_NORMAL, 10)), E_FAIL);

	return S_OK;
}

HRESULT CMonsterMgr::Ready_Clone(CLayer * pLayer)
{
	return S_OK;
}

HRESULT CMonsterMgr::Add_GameObject(const _tchar* pLayerTag, const _tchar* objTag, ITEMTYPE eType, _vec3 vPos)
{
	wstring tag = objTag;
	tag += L"%d";

	TCHAR   *   szObjTag = new TCHAR[MAX_PATH];
	wsprintf(szObjTag, objTag);
	_tcscat_s(szObjTag, MAX_PATH, L"%d");
	wsprintf(szObjTag, tag.c_str(), m_vecItemObjTags[eType].size());


	wstring objName = objTag;
	if (objName == L"Arrow")
	{
		m_vecItemObjTags[eType].push_back(szObjTag);

		CGameObject* pGameObject = CArrow::Create(m_pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		CLayer* pLayer = Engine::Get_Layer(pLayerTag);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(szObjTag, pGameObject), E_FAIL);

		m_vecItemPool.push_back(pGameObject);
	}

	else
	{
		delete[] szObjTag;
		szObjTag = nullptr;
	}

	return S_OK;
}


HRESULT CMonsterMgr::Add_GameObject(CLayer * pLayer, const _tchar * objTag, ITEMTYPE eType, _vec3 vPos)
{
	wstring tag = objTag;
	tag += L"%d";

	TCHAR   *   szObjTag = new TCHAR[MAX_PATH];
	wsprintf(szObjTag, objTag);
	_tcscat_s(szObjTag, MAX_PATH, L"%d");
	wsprintf(szObjTag, tag.c_str(), m_vecItemObjTags[eType].size());

	wstring objName = objTag;
	if (objName == L"Arrow")
	{
		m_vecItemObjTags[eType].push_back(szObjTag);

		CGameObject* pGameObject = CArrow::Create(m_pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		FAILED_CHECK_RETURN(pLayer->Add_GameObject(szObjTag, pGameObject), E_FAIL);

		m_vecItemPool.push_back(pGameObject);
	}
	else if (objName == L"Wand")
	{
		m_vecItemObjTags[eType].push_back(szObjTag);

		CGameObject* pGameObject = CWand::Create(m_pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		FAILED_CHECK_RETURN(pLayer->Add_GameObject(szObjTag, pGameObject), E_FAIL);

		m_vecItemPool.push_back(pGameObject);
	}

	else
	{
		delete[] szObjTag;
		szObjTag = nullptr;
	}

	return S_OK;
}

CGameObject* CMonsterMgr::Add_GameObject(const _tchar * pLayerTag, wstring texTag, CItem * pObj)
{
	// 오브젝트 명
	TCHAR*	szObjTag = new TCHAR[MAX_PATH];
	wsprintf(szObjTag, L"InvImg%d", m_vecItemObjTags[ITEM_IMG].size());
	m_vecItemObjTags[ITEM_IMG].push_back(szObjTag);

	// 텍스쳐 컴포넌트명
	TCHAR   *   szTexTag = new TCHAR[MAX_PATH];
	wsprintf(szTexTag, texTag.c_str());
	m_vecItemTextureTags.push_back(szTexTag);

	CGameObject* pGameObject = CInvImg::Create(m_pGraphicDev, szTexTag, pObj);
	NULL_CHECK_RETURN(pGameObject, nullptr);

	CLayer* pLayer = Engine::Get_Layer(pLayerTag);
	if (E_FAIL == pLayer->Add_GameObject(szObjTag, pGameObject))
		return nullptr;

	m_vecItemPool.push_back(pGameObject);

	return pGameObject;
}

inline void CMonsterMgr::Free(void)
{
	// 무기
	for (int i = 0; i < WEAPON_END; ++i)
	{
		for (auto& tag : m_vecItemObjTags[i])
		{
			delete[] tag;
			tag = nullptr;
		}
		m_vecItemObjTags[i].clear();
		m_vecItemObjTags[i].swap(vector<TCHAR*>());
	}

	for (auto& tag : m_vecItemTextureTags)
	{
		delete[] tag;
		tag = nullptr;
	}
	m_vecItemTextureTags.clear();
	m_vecItemTextureTags.swap(vector<TCHAR*>());


	m_vecItemPool.swap(vector<CGameObject*>());

	Safe_Release(m_pGraphicDev);

}
