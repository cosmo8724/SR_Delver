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
#include "GreenSpider.h"
// Boss
#include "SongBoss.h"

IMPLEMENT_SINGLETON(CMonsterMgr)

CMonsterMgr::CMonsterMgr()
{
}

CMonsterMgr::~CMonsterMgr()
{
	Free();
}

void CMonsterMgr::Ready_MonsterMgr(LPDIRECT3DDEVICE9 pGraphicDev)
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
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlueBatATTACK_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/BlueBat/Attack/BlueBat%d.png", TEX_NORMAL, 12)), E_FAIL);
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

	// SkeletonGhost_Texture 
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SkeletonGhostIDLE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/SkeletonGhost/Idle/SkeletonGhost%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SkeletonGhostATTACK_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/SkeletonGhost/Attack/SkeletonGhost%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SkeletonGhostHIT_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/SkeletonGhost/Hit/SkeletonGhost%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SkeletonGhostDIE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/SkeletonGhost/Die/SkeletonGhost%d.png", TEX_NORMAL, 3)), E_FAIL);

	// GreenSpider_Texture 
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_GreenSpiderIDLE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/GreenSpider/Idle/GreenSpider%d.png", TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_GreenSpiderATTACK_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/GreenSpider/Attack/GreenSpider%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_GreenSpiderHIT_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/GreenSpider/Hit/GreenSpider%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_GreenSpiderDIE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/GreenSpider/Die/GreenSpider%d.png", TEX_NORMAL, 3)), E_FAIL);

	////////////// Boss
	// SongBoss_Texture 
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SongBossMOVE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/SongBoss/Move/SongBoss%d.png", TEX_NORMAL, 16)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SongBossIDLE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/SongBoss/Idle/SongBoss%d.png", TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SongBossATTACK_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/SongBoss/Attack/SongBoss%d.png", TEX_NORMAL, 12)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SongBossHIT_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/SongBoss/Hit/SongBoss%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SongBossDIE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/SongBoss/Die/SongBoss%d.png", TEX_NORMAL, 10)), E_FAIL);

	return S_OK;
}

HRESULT CMonsterMgr::Add_GameObject(CLayer * pLayer)
{
	CGameObject*		pGameObject = nullptr;

	//////////// Monster
	//// GreenSlime
	//pGameObject = CGreenSlime::Create(m_pGraphicDev, _vec3( 15.f, 1.f, 15.f ));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GreenSlime", pGameObject), E_FAIL);
	//m_vecMonster.push_back(pGameObject);

	//// PinkSlime
	//pGameObject = CPinkSlime::Create(m_pGraphicDev, _vec3(7.f, 2.f, 10.f));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PinkSlime", pGameObject), E_FAIL);
	//m_vecMonster.push_back(pGameObject);

	// Fist
	//pGameObject = CFist::Create(m_pGraphicDev, _vec3(3.f, 1.f, 15.f));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Fist", pGameObject), E_FAIL);
	//m_vecMonster.push_back(pGameObject);

	//// BlueBat
	//pGameObject = CBlueBat::Create(m_pGraphicDev, _vec3(15.f, 1.f, 5.f));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BlueBat", pGameObject), E_FAIL);
	//m_vecMonster.push_back(pGameObject);

	//// BrownBat
	//pGameObject = CBrownBat::Create(m_pGraphicDev, _vec3(15.f, 3.f, 15.f));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BrownBat", pGameObject), E_FAIL);
	//m_vecMonster.push_back(pGameObject);

	//// Stick
	//pGameObject = CStick::Create(m_pGraphicDev, _vec3(15.f, 1.f, 10.f));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Stick", pGameObject), E_FAIL);
	//m_vecMonster.push_back(pGameObject);

	//// Leaf
	//pGameObject = CLeaf::Create(m_pGraphicDev, _vec3(5.f, 1.f, 10.f));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Leaf", pGameObject), E_FAIL);
	//m_vecMonster.push_back(pGameObject);

	//// SkeletonGhost
	//pGameObject = CSkeletonGhost::Create(m_pGraphicDev, _vec3(20.f, 3.f, 20.f));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkeletonGhost", pGameObject), E_FAIL);
	//m_vecMonster.push_back(pGameObject);

	//// GreenSpider
	//pGameObject = CGreenSpider::Create(m_pGraphicDev, _vec3(5.f, 1.f, 10.f));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GreenSpider", pGameObject), E_FAIL);
	//m_vecMonster.push_back(pGameObject);

	////////////Boss
	// SongBoss
	pGameObject = CSongBoss::Create(m_pGraphicDev, _vec3(10.f, 1.f, 10.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SongBoss", pGameObject), E_FAIL);
	m_vecMonster.push_back(pGameObject);

	return E_NOTIMPL;
}

inline void CMonsterMgr::Free(void)
{
	m_vecMonster.swap(vector<CGameObject*>());
	Safe_Release(m_pGraphicDev);
}