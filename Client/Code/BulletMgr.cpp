#include "stdafx.h"
#include "..\Header\BulletMgr.h"
//#include "Bullet.h"

#include "Export_Function.h"
#include <string>

#include "WandBullet.h"
#include "FistBullet.h"
#include "SongBossBullet.h"
#include "SongBosStun.h"
#include "SongBosFloor.h"
#include "ArrowBullet.h"
#include "LeafBullet.h"

IMPLEMENT_SINGLETON(CBulletMgr)

CBulletMgr::CBulletMgr()
{
	//for (int i = 0; i < BULLET_END; ++i)
	//{
	//	m_CurIdx[i] = 0;
	//}

	m_MaxIdx[BULLET_WAND] = 20;
	m_MaxIdx[BULLET_M_FIST] = 5;
	m_MaxIdx[BULLET_SONGBOSS] = 5;
	m_MaxIdx[STUN_SONGBOSS] = 4; // ENUM 선언위치 변경 X
	m_MaxIdx[FLOOR_SONGBOSS] = 5; // ENUM 선언위치 변경 X
	m_MaxIdx[BULLET_ARROW] = 10;
	m_MaxIdx[BULLET_M_LEAF] = 1;

	for (int bulletId = 0; bulletId < BULLET_END; ++bulletId)
	{
		for (int Idx = 0; Idx < m_MaxIdx[bulletId]; ++Idx)
		{
			m_IdxQue[bulletId].push(Idx);
		}
	}

	//for (int i = 0; i < m_MaxIdx[BULLET_WAND]; ++i)
	//	m_IdxQue[BULLET_WAND].push(i);
}

CBulletMgr::~CBulletMgr()
{
	Free();
}

HRESULT CBulletMgr::Ready_Proto(LPDIRECT3DDEVICE9 pGraphicDev)
{
	// Loading 씬에 넣을 bullet 원본 컴포넌트
	// Fist
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FistGreenEffect_Texture", CTexture::Create(pGraphicDev, L"../Bin/Resource/Texture/Monster/Monster_Effect/Fist_GreenEffect/GreenEffect%d.png", TEX_NORMAL, 15)), E_FAIL);
	// SongBoss
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MusicNote_Bullet_Texture", CTexture::Create(pGraphicDev, L"../Bin/Resource/Texture/Monster/Monster_Effect/SongBoss_Bullet/SongBoss%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MusicNote_Stun_Texture", CTexture::Create(pGraphicDev, L"../Bin/Resource/Texture/Monster/Monster_Effect/SongBoss_Stun/Stun.png", TEX_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MusicNote_Floor_Texture", CTexture::Create(pGraphicDev, L"../Bin/Resource/Texture/Monster/Monster_Effect/SongBoss_Floor/Floor.png", TEX_NORMAL, 1)), E_FAIL);
	// Leaf
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Leaf_Bullet_Texture", CTexture::Create(pGraphicDev, L"../Bin/Resource/Texture/Monster/Monster_Effect/Leaf_Bullet/Leaf_Bullet%d.png", TEX_NORMAL, 7)), E_FAIL);

	// ArrowBullet
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ArrowBullet_Texture", CTexture::Create(pGraphicDev, L"../Bin/Resource/Texture/Item/Bullet/ArrowBullet.png", TEX_NORMAL)), E_FAIL);



	return S_OK;
}

HRESULT CBulletMgr::Ready_Clone(CLayer* pLayer, LPDIRECT3DDEVICE9 pGraphicDev)
{
	//pGraphicDev->AddRef();
	CGameObject*		pGameObject = nullptr;

	// wandbullet
	wstring*	objTags = new wstring[m_MaxIdx[BULLET_WAND]];
	m_vecObjTags[BULLET_WAND].push_back(objTags);

	//wchar_t*	obgTags = new wchar_t[20];

	for (int i = 0; i < m_MaxIdx[BULLET_WAND]; ++i)
	{
		pGameObject = CWandBullet::Create(pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		objTags[i] = L"WandBullet";
		//wstring objTag(L"WandBullet");
		wchar_t index[10];
		_itow_s(i, index, 10);
		objTags[i] += index;
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(objTags[i].c_str(), pGameObject), E_FAIL);

		m_vecObjPool[BULLET_WAND].push_back(pGameObject);
	}

	// Fistbullet
	objTags = nullptr;
	objTags = new wstring[m_MaxIdx[BULLET_M_FIST]];
	m_vecObjTags[BULLET_M_FIST].push_back(objTags);

	for (int i = 0; i < m_MaxIdx[BULLET_M_FIST]; ++i)
	{
		pGameObject = CFistBullet::Create(pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		objTags[i] = L"FistBullet";
		wchar_t index[10];
		_itow_s(i, index, 10);
		objTags[i] += index;
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(objTags[i].c_str(), pGameObject), E_FAIL);

		m_vecObjPool[BULLET_M_FIST].push_back(pGameObject);
	}

	// SongBoss_bullet
	objTags = nullptr;
	objTags = new wstring[m_MaxIdx[BULLET_SONGBOSS]];
	m_vecObjTags[BULLET_SONGBOSS].push_back(objTags);

	for (int i = 0; i < m_MaxIdx[BULLET_SONGBOSS]; ++i)
	{
		pGameObject = CSongBossBullet::Create(pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		objTags[i] = L"SongBoss_Bullet";
		wchar_t index[10];
		_itow_s(i, index, 10);
		objTags[i] += index;
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(objTags[i].c_str(), pGameObject), E_FAIL);

		m_vecObjPool[BULLET_SONGBOSS].push_back(pGameObject);
	}

	// SongBoss_Stun
	objTags = nullptr;
	objTags = new wstring[m_MaxIdx[STUN_SONGBOSS]];
	m_vecObjTags[STUN_SONGBOSS].push_back(objTags);

	for (int i = 0; i < m_MaxIdx[STUN_SONGBOSS]; ++i)
	{
		pGameObject = CSongBosStun::Create(pGraphicDev, i);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		objTags[i] = L"SongBoss_Stun";
		wchar_t index[10];
		_itow_s(i, index, 10);
		objTags[i] += index;
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(objTags[i].c_str(), pGameObject), E_FAIL);

		m_vecObjPool[STUN_SONGBOSS].push_back(pGameObject);
	}

	// SongBoss_Floor
	objTags = nullptr;
	objTags = new wstring[m_MaxIdx[FLOOR_SONGBOSS]];
	m_vecObjTags[FLOOR_SONGBOSS].push_back(objTags);

	for (int i = 0; i < m_MaxIdx[FLOOR_SONGBOSS]; ++i)
	{
		pGameObject = CSongBosFloor::Create(pGraphicDev, i);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		objTags[i] = L"SongBoss_Floor";
		wchar_t index[10];
		_itow_s(i, index, 10);
		objTags[i] += index;
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(objTags[i].c_str(), pGameObject), E_FAIL);

		m_vecObjPool[FLOOR_SONGBOSS].push_back(pGameObject);
	}

	// Bullet_arrow
	objTags = nullptr;
	objTags = new wstring[m_MaxIdx[BULLET_ARROW]];
	m_vecObjTags[BULLET_ARROW].push_back(objTags);

	for (int i = 0; i < m_MaxIdx[BULLET_ARROW]; ++i)
	{
		pGameObject = CArrowBullet::Create(pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		objTags[i] = L"ArrowBullet";
		wchar_t index[10];
		_itow_s(i, index, 10);
		objTags[i] += index;
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(objTags[i].c_str(), pGameObject), E_FAIL);

		m_vecObjPool[BULLET_ARROW].push_back(pGameObject);
	}

	// Bullet_Leaf
	objTags = nullptr;
	objTags = new wstring[m_MaxIdx[BULLET_M_LEAF]];
	m_vecObjTags[BULLET_M_LEAF].push_back(objTags);

	for (int i = 0; i < m_MaxIdx[BULLET_M_LEAF]; ++i)
	{
		pGameObject = CLeafBullet::Create(pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		objTags[i] = L"Leaf_Bullet";
		wchar_t index[10];
		_itow_s(i, index, 10);
		objTags[i] += index;
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(objTags[i].c_str(), pGameObject), E_FAIL);

		m_vecObjPool[BULLET_M_LEAF].push_back(pGameObject);
	}

	return S_OK;
}


void CBulletMgr::Collect_Obj(_int iIdx, BULLETID eID)
{
	m_IdxQue[eID].push(iIdx);
}

CGameObject * CBulletMgr::Reuse_Obj(const D3DXVECTOR3 & vPos, const D3DXVECTOR3 & vDir)
{

	//	CGameObject*		pBullet = nullptr;
	//
	//	if (m_ObjectPool.empty())
	//	{
	//		pBullet = CBullet::Create(vPos, vDir);
	//		++m_iCount;
	//
	//#ifdef _DEBUG
	//		cout << "총알 개수 : " << m_iCount << endl;
	//#endif // _DEBUG
	//
	//	}
	//
	//	else
	//	{
	//		pBullet = m_ObjectPool.front();
	//		m_ObjectPool.pop_front();
	//	}
	//
	//	dynamic_cast<CBullet*>(pBullet)->Set_Pos(vPos);
	//	dynamic_cast<CBullet*>(pBullet)->Set_Dir(vDir);
	//
	//	return pBullet;
	return nullptr;
}

void CBulletMgr::Fire(BULLETID _eID)
{
	_int iIdx = -1;
	if (!m_IdxQue[_eID].empty())
	{
		iIdx = m_IdxQue[_eID].front();
		
		static_cast<CBullet*>(m_vecObjPool[_eID][iIdx])->Set_Fire(true);
		static_cast<CBullet*>(m_vecObjPool[_eID][iIdx])->Set_Index(iIdx);

		m_IdxQue[_eID].pop();
	}
	//m_CurIdx[_eID] = (m_CurIdx[_eID] + 1) % m_MaxIdx[_eID];
}


bool CBulletMgr::Is_Fired(CGameObject * pObj)
{
	return static_cast<CBullet*>(pObj)->Is_Fired();
}

void CBulletMgr::Pre_Setting(BULLETID eID, _float fSet)
{
	_int iIdx = -1;

	switch (eID)
	{
	case BULLET_ARROW:	// 화살의 경우 차징에 따라 날아가는 스피드가 달라진다..
		if (!m_IdxQue[eID].empty())
		{
			iIdx = m_IdxQue[eID].front();
			static_cast<CArrowBullet*>(m_vecObjPool[eID][iIdx])->Plus_Speed(fSet);
		}
		break;
	}
}

void CBulletMgr::Free()
{
	for (int i = 0; i < BULLET_END; ++i)
	{
		for (auto& tag : m_vecObjTags[i])
		{
			delete[] tag;
			tag = nullptr;
		}
		m_vecObjTags[i].swap(vector<wstring*>());

		m_vecObjPool[i].swap(vector<CGameObject*>());
	}

}
