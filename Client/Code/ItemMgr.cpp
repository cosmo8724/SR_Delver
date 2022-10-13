#include "stdafx.h"
#include "ItemMgr.h"
#include "Export_Function.h"
#include "Arrow.h"
#include "Wand.h"
#include "InvImg.h"
#include "Potion.h"
#include "Dagger.h"
#include "Key.h"

IMPLEMENT_SINGLETON(CItemMgr)

CItemMgr::CItemMgr()
{
}


CItemMgr::~CItemMgr()
{
	Free();
}

void CItemMgr::Ready_ItemMgr(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();
}

HRESULT CItemMgr::Ready_Proto()
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Wand1Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Weapon/Wand/wand%d.png", TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Arrow1Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Weapon/Arrow/Arrow1/Arrow1_%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Potion_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Potion/Potion%d.png", TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Dagger_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Weapon/Dagger/Dagger%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Key_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/key.png", TEX_NORMAL)), E_FAIL);

	return S_OK;
}

HRESULT CItemMgr::Ready_Clone(CLayer * pLayer)
{
	return S_OK;
}

HRESULT CItemMgr::Add_GameObject(const _tchar* pLayerTag, const _tchar* objTag, ITEMTYPE eType, _vec3 vPos)
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

		m_vecItemPool[ITEM_WEAPON].push_back(pGameObject);
	}

	else
	{
		delete[] szObjTag;
		szObjTag = nullptr;
	}

	return S_OK;
}


HRESULT CItemMgr::Add_GameObject(CLayer * pLayer, const _tchar * objTag, ITEMTYPE eType, _vec3 vPos)
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

		m_vecItemPool[eType].push_back(pGameObject);
	}
	else if (objName == L"Wand")
	{
		m_vecItemObjTags[eType].push_back(szObjTag);

		CGameObject* pGameObject = CWand::Create(m_pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		FAILED_CHECK_RETURN(pLayer->Add_GameObject(szObjTag, pGameObject), E_FAIL);

		m_vecItemPool[eType].push_back(pGameObject);
	}
	else if (objName == L"Dagger")
	{
		m_vecItemObjTags[eType].push_back(szObjTag);

		CGameObject* pGameObject = CDagger::Create(m_pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		FAILED_CHECK_RETURN(pLayer->Add_GameObject(szObjTag, pGameObject), E_FAIL);

		m_vecItemPool[eType].push_back(pGameObject);
	}
	else if (objName == L"Key")
	{
		m_vecItemObjTags[eType].push_back(szObjTag);

		CGameObject* pGameObject = CKey::Create(m_pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		FAILED_CHECK_RETURN(pLayer->Add_GameObject(szObjTag, pGameObject), E_FAIL);

		m_vecItemPool[eType].push_back(pGameObject);
	}
	else
	{
		delete[] szObjTag;
		szObjTag = nullptr;
	}

	return S_OK;
}

CGameObject* CItemMgr::Add_GameObject(const _tchar * pLayerTag, wstring texTag, CItem * pObj)
{
	// 오브젝트 명
	TCHAR*	szObjTag = new TCHAR[MAX_PATH];
	wsprintf(szObjTag, L"InvImg%d", m_vecItemObjTags[ITEM_IMG].size());
	m_vecItemObjTags[ITEM_IMG].push_back(szObjTag);

	// 텍스쳐 컴포넌트명
	TCHAR   *   szTexTag = new TCHAR[MAX_PATH];
	wsprintf(szTexTag, texTag.c_str());
	m_vecItemTextureTags.push_back(szTexTag);

	_int iTex;
	CGameObject* pGameObject = CInvImg::Create(m_pGraphicDev, szTexTag, pObj);

	if (ITEM_POTION == pObj->Get_ItemType())
	{
		iTex = static_cast<CPotion*>(pObj)->Get_TexturId();
		static_cast<CInvImg*>(pGameObject)->Set_Frame(iTex);
	}


	NULL_CHECK_RETURN(pGameObject, nullptr);

	CLayer* pLayer = Engine::Get_Layer(pLayerTag);
	if (E_FAIL == pLayer->Add_GameObject(szObjTag, pGameObject))
		return nullptr;
	
	m_vecItemPool[ITEM_IMG].push_back(pGameObject);


	return pGameObject;
}

HRESULT CItemMgr::Add_RandomObject(const _tchar * pLayerTag, const _tchar * objTag, ITEMTYPE eType, _vec3 vPos)
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

		m_vecItemPool[ITEM_WEAPON].push_back(pGameObject);
	}

	else if (objName == L"Potion")
	{
		m_vecItemObjTags[eType].push_back(szObjTag);

		_int iTex = rand() % POTION_END;

		CGameObject* pGameObject = CPotion::Create(m_pGraphicDev, vPos, iTex);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		CLayer* pLayer = Engine::Get_Layer(pLayerTag);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(szObjTag, pGameObject), E_FAIL);

		m_vecItemPool[ITEM_POTION].push_back(pGameObject);
	}

	else
	{
		delete[] szObjTag;
		szObjTag = nullptr;
	}



	return S_OK;
}

inline void CItemMgr::Free(void)
{
	// 무기
	for (int i = 0; i < ITEM_END; ++i)
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

	for (int i = 0; i < ITEM_END; ++i)
	{
		m_vecItemPool[i].swap(vector<CGameObject*>());
	}



	Safe_Release(m_pGraphicDev);

}
