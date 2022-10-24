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
#include "Mimic.h"
// Boss
#include "SongBoss.h"
#include "Jellyfish.h"

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

	// Mimic_Texture 
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MimicIDLE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Mimic/Idle/Mimic%d.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MimicATTACK_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Mimic/Attack/Mimic%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MimicHIT_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Mimic/Hit/Mimic%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MimicDIE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Mimic/Die/Mimic%d.png", TEX_NORMAL, 3)), E_FAIL);

	////////////// Boss
	// SongBoss_Texture 
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SongBossMOVE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/SongBoss/Move/SongBoss%d.png", TEX_NORMAL, 16)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SongBossIDLE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/SongBoss/Idle/SongBoss%d.png", TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SongBossATTACK_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/SongBoss/Attack/SongBoss%d.png", TEX_NORMAL, 12)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SongBossHIT_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/SongBoss/Hit/SongBoss%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SongBossDIE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/SongBoss/Die/SongBoss%d.png", TEX_NORMAL, 10)), E_FAIL);

	// JellyfishBoss_Texture 
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_JellyfishBossIDLE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Jellyfish/Idle/Jellyfish_%d.png", TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_JellyfishBossATTACK_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Jellyfish/Attack/Jellyfish_%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_JellyfishBossHIT_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Jellyfish/Hit/Jellyfish_%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_JellyfishBossDIE_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Jellyfish/Die/Jellyfish_%d.png", TEX_NORMAL, 8)), E_FAIL);

	return S_OK;
}

HRESULT CMonsterMgr::Add_GameObject(CLayer * pLayer, const _tchar* szFilePath)
{
	CGameObject*		pGameObject = nullptr;


	pGameObject = CFist::Create(m_pGraphicDev, _vec3({ 22, 2, 14.f }));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"fistttt", pGameObject), E_FAIL);
	m_vecMonster.push_back(pGameObject);


	if (!lstrcmp(szFilePath, L""))
	{
		//////////// Monster
		// GreenSlime
		pGameObject = CGreenSlime::Create(m_pGraphicDev, _vec3(8.f, -22.f, 95.f));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GreenSlime", pGameObject), E_FAIL);
		m_vecMonster.push_back(pGameObject);

		//pGameObject = CGreenSlime::Create(m_pGraphicDev, _vec3(18.f, 2.f, 12.f));
		//NULL_CHECK_RETURN(pGameObject, E_FAIL);
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GreenSlime1", pGameObject), E_FAIL);
		//m_vecMonster.push_back(pGameObject);

		//pGameObject = CGreenSlime::Create(m_pGraphicDev, _vec3(12.f, 2.f, 11.f));
		//NULL_CHECK_RETURN(pGameObject, E_FAIL);
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GreenSlime2", pGameObject), E_FAIL);
		//m_vecMonster.push_back(pGameObject);

		//pGameObject = CGreenSlime::Create(m_pGraphicDev, _vec3(7.f, 2.f, 10.f));
		//NULL_CHECK_RETURN(pGameObject, E_FAIL);
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GreenSlime3", pGameObject), E_FAIL);
		//m_vecMonster.push_back(pGameObject);







		// 200 ���� ������ �Ŷ�	
		/*for (int i = 0; i < 200; i++)
		{
			wstring tag = L"GreenSlime_";
			tag += i;
			TCHAR   *   szObjTag = new TCHAR[MAX_PATH];
			wsprintf(szObjTag, tag.c_str());

			pGameObject = CGreenSlime::Create(m_pGraphicDev, _vec3((8.f + i) / 3.f, -22.f, 95.f));
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(szObjTag, pGameObject), E_FAIL);
			m_vecMonster.push_back(pGameObject);
		}*/

		// PinkSlime
		pGameObject = CPinkSlime::Create(m_pGraphicDev, _vec3(94.f, -95.f, 17.f), SEPARATION_ONE);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PinkSlime", pGameObject), E_FAIL);
		m_vecMonster.push_back(pGameObject);

		// Fist
		pGameObject = CFist::Create(m_pGraphicDev, _vec3(44.f, -30.f, 57.f));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Fist", pGameObject), E_FAIL);
		m_vecMonster.push_back(pGameObject);

		// BlueBat
		pGameObject = CBlueBat::Create(m_pGraphicDev, _vec3(59.f, -30.f, 69.f));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BlueBat", pGameObject), E_FAIL);
		m_vecMonster.push_back(pGameObject);

		// BrownBat
		pGameObject = CBrownBat::Create(m_pGraphicDev, _vec3(-7.f, -57.f, 21.f));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BrownBat", pGameObject), E_FAIL);
		m_vecMonster.push_back(pGameObject);

		pGameObject = CBrownBat::Create(m_pGraphicDev, _vec3(-10.f, -57.f, 13.f));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BrownBat1", pGameObject), E_FAIL);
		m_vecMonster.push_back(pGameObject);

		// Stick
		pGameObject = CStick::Create(m_pGraphicDev, _vec3(45.f, -78.f, 22.f));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Stick", pGameObject), E_FAIL);
		m_vecMonster.push_back(pGameObject);

		pGameObject = CStick::Create(m_pGraphicDev, _vec3(24.f, -74.f, 50.f));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Stick1", pGameObject), E_FAIL);
		m_vecMonster.push_back(pGameObject);

		// Leaf
		//pGameObject = CLeaf::Create(m_pGraphicDev, _vec3(50.f, -30.f, 62.f));
		pGameObject = CLeaf::Create(m_pGraphicDev, _vec3(15.f, 2.f, 15.f));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Leaf", pGameObject), E_FAIL);
		m_vecMonster.push_back(pGameObject);

		// SkeletonGhost
		pGameObject = CSkeletonGhost::Create(m_pGraphicDev, _vec3(1.f, -63.f, -33.f));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkeletonGhost", pGameObject), E_FAIL);
		m_vecMonster.push_back(pGameObject);

		pGameObject = CSkeletonGhost::Create(m_pGraphicDev, _vec3(20.f, 2.f, 15.f));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkeletonGhost1", pGameObject), E_FAIL);
		m_vecMonster.push_back(pGameObject);

		// GreenSpider
		pGameObject = CGreenSpider::Create(m_pGraphicDev, _vec3(36.f, -70.f, -26.f));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GreenSpider", pGameObject), E_FAIL);
		m_vecMonster.push_back(pGameObject);

		pGameObject = CGreenSpider::Create(m_pGraphicDev, _vec3(45.f, -70.f, -9.f));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GreenSpider1", pGameObject), E_FAIL);
		m_vecMonster.push_back(pGameObject);

		// Mimic
		pGameObject = CMimic::Create(m_pGraphicDev, _vec3(77.f, -16.f, -7.f));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Mimic", pGameObject), E_FAIL);
		m_vecMonster.push_back(pGameObject);

		//////////Boss
		// SongBoss
		pGameObject = CSongBoss::Create(m_pGraphicDev, _vec3(-22.f, -63.5f, -23.f));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SongBoss", pGameObject), E_FAIL);
		m_vecMonster.push_back(pGameObject);

		// Jellyfish
		pGameObject = CJellyfish::Create(m_pGraphicDev, _vec3(16.f, 2.f, 16.f));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Jellyfish", pGameObject), E_FAIL);
		m_vecMonster.push_back(pGameObject);
	}
	else
	{
		HANDLE	hFile = CreateFile(szFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return E_FAIL;

		DWORD	dwByte = 0;
		DWORD	dwStrByte = 0;
		Engine::CMonster* pMonster = Engine::CMonster::Create(m_pGraphicDev);
		Engine::CMonster* pCloneObject = nullptr;
		CTransform*	pTransCom = dynamic_cast<CTransform*>(pMonster->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));

		while (true)
		{
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

			TCHAR*	pName = new TCHAR[dwStrByte];
			ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

			ReadFile(hFile, &pTransCom->m_vInfo[INFO_RIGHT], sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &pTransCom->m_vInfo[INFO_UP], sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &pTransCom->m_vInfo[INFO_LOOK], sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &pTransCom->m_vInfo[INFO_POS], sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &pTransCom->m_vAngle, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &pTransCom->m_vScale, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &pTransCom->m_matWorld, sizeof(_matrix), &dwByte, nullptr);
			ReadFile(hFile, &pMonster->m_vPos, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &pMonster->m_eType, sizeof(MONSTERTYPE), &dwByte, nullptr);

			if (0 == dwByte)
			{
				Safe_Release(pMonster);
				Safe_Delete_Array(pName);
				break;
			}

			m_vecObjTags.push_back(pName);
			switch (pMonster->m_eType)
			{
			case MOB_GREENSLIME:
				pCloneObject = CGreenSlime::Create(pMonster);
				break;

			case MOB_PINKSLIME:
				pCloneObject = CPinkSlime::Create(pMonster);
				break;

			case MOB_FIST:
				pCloneObject = CFist::Create(pMonster);
				break;

			case MOB_BLUEBAT:
				pCloneObject = CBlueBat::Create(pMonster);
				break;

			case MOB_BROWNBAT:
				pCloneObject = CBrownBat::Create(pMonster);
				break;

			case MOB_STICK:
				pCloneObject = CStick::Create(pMonster);
				break;

			case MOB_LEAF:
				pCloneObject = CLeaf::Create(pMonster);
				break;

			case MOB_SKELETONGHOST:
				pCloneObject = CSkeletonGhost::Create(pMonster);
				break;

			case MOB_GREENSPIDER:
				pCloneObject = CGreenSpider::Create(pMonster);
				break;

			case MOB_MIMIC:
				pCloneObject = CMimic::Create(pMonster);
				break;

			case MOB_OCTOPUS:
				/*pCloneObject = CStatue::Create(pMonster);
				m_iOctopusCnt++;*/
				break;

			case MOB_SONGBOSS:
				pCloneObject = CSongBoss::Create(pMonster);
				break;
			}

			pLayer->Add_GameObject(m_vecObjTags.back(), pCloneObject);
			m_vecMonster.push_back(pCloneObject);
		}
		CloseHandle(hFile);





	}
	return S_OK;
}

inline void CMonsterMgr::Free(void)
{
	for (size_t i = 0; i < m_vecObjTags.size(); i++)
		Safe_Delete_Array(m_vecObjTags[i]);
	m_vecObjTags.clear();

	m_vecMonster.swap(vector<CGameObject*>());
	Safe_Release(m_pGraphicDev);
}