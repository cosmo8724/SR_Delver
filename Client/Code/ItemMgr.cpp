#include "stdafx.h"
#include "ItemMgr.h"
#include "Export_Function.h"
#include "Arrow.h"
#include "Wand.h"
#include "GreenWand.h"
#include "RedWand.h"
#include "InvImg.h"
#include "Potion.h"
#include "Dagger.h"
#include "Key.h"
#include "Armor.h"
#include "Shield.h"
#include "Helmat.h"
#include "LeftHand.h"
#include "Necklace.h"
#include "Pants.h"
#include "Ring.h"
#include "Food.h"
#include "Gold.h"
#include "Lantern.h"

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
	// Weapon
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Wand1Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Weapon/Wand/wand%d.png", TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_GreenWand1Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Weapon/GreenWand/GreenWand%d.png", TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RadWand1Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Weapon/RadWand/RadWand.png", TEX_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Arrow1Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Weapon/Arrow/Arrow1/Arrow1_%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Dagger_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Weapon/Dagger/Dagger%d.png", TEX_NORMAL, 4)), E_FAIL);

	// Potion
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Potion_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Potion/Potion%d.png", TEX_NORMAL, 5)), E_FAIL);
	// Food
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Food_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Food/Food%d.png", TEX_NORMAL, 5)), E_FAIL);
	// Gold
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Gold_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Gold/Gold_%d.png", TEX_NORMAL, 4)), E_FAIL);

	// Key
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Key_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/key.png", TEX_NORMAL)), E_FAIL);
	
	// Equipment
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Armor_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Equipment/armor%d.png", TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Helmet_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Equipment/helmet%d.png", TEX_NORMAL,2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Pants_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Equipment/pants%d.png", TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Necklace_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Equipment/necklace%d.png", TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Ring_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Equipment/ring%d.png", TEX_NORMAL, 2)), E_FAIL);

	// Shield
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Shield_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Equipment/shield%d.png", TEX_NORMAL,1)), E_FAIL);

	// Lantern
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Lantern_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Equipment/lantern%d.png", TEX_NORMAL, 1)), E_FAIL);

	
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
	else if (objName == L"GreenWand")
	{
		m_vecItemObjTags[eType].push_back(szObjTag);

		CGameObject* pGameObject = CGreenWand::Create(m_pGraphicDev, vPos);
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

HRESULT CItemMgr::Add_GameObject(CLayer * pLayer, LOADINGID eID)
{
	CGameObject* pGameObject = nullptr;
	
	if (eID == LOADING_STAGE)
	{
		//Arrow
		pGameObject = CArrow::Create(m_pGraphicDev, _vec3({ 10.f, 2.f, 10.f }));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Arrow", pGameObject), E_FAIL);
		m_vecItemPool[ITEM_WEAPON].push_back(pGameObject);

		//Dagger
		pGameObject = CDagger::Create(m_pGraphicDev, _vec3({ 9.f, 2.f, 7.f }));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Dagger", pGameObject), E_FAIL);
		m_vecItemPool[ITEM_WEAPON].push_back(pGameObject);

		//Wand
		pGameObject = CWand::Create(m_pGraphicDev, _vec3({ 13.f, 2.f, 3.f }));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Wand", pGameObject), E_FAIL);
		m_vecItemPool[ITEM_WEAPON].push_back(pGameObject);

		// GreenWand
		pGameObject = CGreenWand::Create(m_pGraphicDev, _vec3({ 13.f, 2.f, 4.f }));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GreenWand", pGameObject), E_FAIL);
		m_vecItemPool[ITEM_WEAPON].push_back(pGameObject);

		// RadWand
		pGameObject = CRedWand::Create(m_pGraphicDev, _vec3({ 13.f, 2.f, 5.f }));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RadWand", pGameObject), E_FAIL);
		m_vecItemPool[ITEM_WEAPON].push_back(pGameObject);

		//Key
		//pGameObject = CKey::Create(m_pGraphicDev, _vec3({ 5.f, 1.f, 10.f }));
		//NULL_CHECK_RETURN(pGameObject, E_FAIL);
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Key", pGameObject), E_FAIL);
		//m_vecItemPool[ITEM_KEY].push_back(pGameObject);

		// Armor
		pGameObject = CArmor::Create(m_pGraphicDev, _vec3({ 7.f, 2.f, 3.f }), 0);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Armor", pGameObject), E_FAIL);
		m_vecItemPool[ITEM_ARMOR].push_back(pGameObject);

		//pGameObject = CArmor::Create(m_pGraphicDev, _vec3({ 13.f, 1.f, 10.f }), 1);
		//NULL_CHECK_RETURN(pGameObject, E_FAIL);
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Armor2", pGameObject), E_FAIL);
		//m_vecItemPool[ITEM_ARMOR].push_back(pGameObject);

		// Shield
		pGameObject = CShield::Create(m_pGraphicDev, _vec3({ 13.f, 2.f, 10.f }));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Shield", pGameObject), E_FAIL);
		m_vecItemPool[ITEM_ARMOR].push_back(pGameObject);

		// Helmat
		pGameObject = CHelmat::Create(m_pGraphicDev, _vec3({ 7.f, 2.f, 5.f }), 0);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Helmat", pGameObject), E_FAIL);
		m_vecItemPool[ITEM_HELMAT].push_back(pGameObject);

		pGameObject = CHelmat::Create(m_pGraphicDev, _vec3({ 7.f, 2.f, 6.f }), 1);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Helmat2", pGameObject), E_FAIL);
		m_vecItemPool[ITEM_HELMAT].push_back(pGameObject);

		// Necklace
		pGameObject = CNecklace::Create(m_pGraphicDev, _vec3({ 7.f, 2.f, 7.f }), 0);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Necklace", pGameObject), E_FAIL);
		m_vecItemPool[ITEM_NECKLACE].push_back(pGameObject);

		pGameObject = CNecklace::Create(m_pGraphicDev, _vec3({ 7.f, 2.f, 8.f }), 1);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Necklace2", pGameObject), E_FAIL);
		m_vecItemPool[ITEM_NECKLACE].push_back(pGameObject);

		// Pants
		pGameObject = CPants::Create(m_pGraphicDev, _vec3({ 7.f, 2.f, 9.f }), 0);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pants", pGameObject), E_FAIL);
		m_vecItemPool[ITEM_PANTS].push_back(pGameObject);

		pGameObject = CPants::Create(m_pGraphicDev, _vec3({ 7.f, 2.f, 10.f }), 1);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pants2", pGameObject), E_FAIL);
		m_vecItemPool[ITEM_PANTS].push_back(pGameObject);

		// Ring
		pGameObject = CRing::Create(m_pGraphicDev, _vec3({ 7.f, 2.f, 11.f }), 0);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Ring", pGameObject), E_FAIL);
		m_vecItemPool[ITEM_RING].push_back(pGameObject);

		pGameObject = CRing::Create(m_pGraphicDev, _vec3({ 7.f, 2.f, 12.f }), 1);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Ring2", pGameObject), E_FAIL);
		m_vecItemPool[ITEM_RING].push_back(pGameObject);

		// Food
		pGameObject = CFood::Create(m_pGraphicDev, _vec3({ 11.f, 2.f, 5.f }), 0);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Food", pGameObject), E_FAIL);
		m_vecItemPool[ITEM_FOOD].push_back(pGameObject);

		pGameObject = CFood::Create(m_pGraphicDev, _vec3({ 11.f, 2.f, 6.f }), 1);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Food2", pGameObject), E_FAIL);
		m_vecItemPool[ITEM_FOOD].push_back(pGameObject);

		pGameObject = CFood::Create(m_pGraphicDev, _vec3({ 11.f, 2.f, 7.f }), 2);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Food3", pGameObject), E_FAIL);
		m_vecItemPool[ITEM_FOOD].push_back(pGameObject);

		pGameObject = CFood::Create(m_pGraphicDev, _vec3({ 11.f, 2.f, 8.f }), 3);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Food4", pGameObject), E_FAIL);
		m_vecItemPool[ITEM_FOOD].push_back(pGameObject);

		pGameObject = CFood::Create(m_pGraphicDev, _vec3({ 11.f, 2.f, 9.f }), 4);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Food5", pGameObject), E_FAIL);
		m_vecItemPool[ITEM_FOOD].push_back(pGameObject);

		pGameObject = CPotion::Create(m_pGraphicDev, _vec3({ 20.f, 2.f, 20.f }), POTION_0);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"POTION_0", pGameObject), E_FAIL);
		m_vecItemPool[ITEM_POTION].push_back(pGameObject);

		pGameObject = CPotion::Create(m_pGraphicDev, _vec3({ 22.f, 2.f, -22.f }), POTION_0);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"POTION_1", pGameObject), E_FAIL);
		m_vecItemPool[ITEM_POTION].push_back(pGameObject);

		pGameObject = CLantern::Create(m_pGraphicDev, _vec3({ 10.f, 2.f, 4.f }));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lantern", pGameObject), E_FAIL);
		m_vecItemPool[ITEM_LANTERN].push_back(pGameObject);
	}


	return S_OK;
}

CGameObject* CItemMgr::Add_GameObject(const _tchar * pLayerTag, wstring texTag, CItem * pObj)
{
	// ������Ʈ ��
	TCHAR*	szObjTag = new TCHAR[MAX_PATH];
	wsprintf(szObjTag, L"InvImg%d", m_vecItemObjTags[ITEM_IMG].size());
	m_vecItemObjTags[ITEM_IMG].push_back(szObjTag);

	// �ؽ��� ������Ʈ��
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
	else if (ITEM_FOOD == pObj->Get_ItemType())
	{
		iTex = static_cast<CFood*>(pObj)->Get_TexturId();
		static_cast<CInvImg*>(pGameObject)->Set_Frame(iTex);
	}
	else if (ITEM_GOLD == pObj->Get_ItemType())
	{
		iTex = static_cast<CGold*>(pObj)->Get_TexturId();
		static_cast<CInvImg*>(pGameObject)->Set_Frame(iTex);
	}
	else if (ITEM_ARMOR == pObj->Get_ItemType())
	{
		iTex = static_cast<CArmor*>(pObj)->Get_TexturId();
		static_cast<CInvImg*>(pGameObject)->Set_Frame(iTex);
	}
	else if (ITEM_HELMAT == pObj->Get_ItemType())
	{
		iTex = static_cast<CHelmat*>(pObj)->Get_TexturId();
		static_cast<CInvImg*>(pGameObject)->Set_Frame(iTex);
	}
	else if (ITEM_LEFTHAND == pObj->Get_ItemType())
	{
		iTex = static_cast<CLeftHand*>(pObj)->Get_TexturId();
		static_cast<CInvImg*>(pGameObject)->Set_Frame(iTex);
	}
	else if (ITEM_NECKLACE == pObj->Get_ItemType())
	{
		iTex = static_cast<CNecklace*>(pObj)->Get_TexturId();
		static_cast<CInvImg*>(pGameObject)->Set_Frame(iTex);
	}
	else if (ITEM_PANTS == pObj->Get_ItemType())
	{
		iTex = static_cast<CPants*>(pObj)->Get_TexturId();
		static_cast<CInvImg*>(pGameObject)->Set_Frame(iTex);
	}
	else if (ITEM_RING == pObj->Get_ItemType())
	{
		iTex = static_cast<CRing*>(pObj)->Get_TexturId();
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
	_int iRandomNum = rand() % 10;
	if (iRandomNum % 2)
		return S_OK;

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
	else if (objName == L"Food")
	{
		m_vecItemObjTags[eType].push_back(szObjTag);

		_int iTex = rand() % FOOD_END;

		CGameObject* pGameObject = CFood::Create(m_pGraphicDev, vPos, iTex);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		CLayer* pLayer = Engine::Get_Layer(pLayerTag);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(szObjTag, pGameObject), E_FAIL);

		m_vecItemPool[ITEM_FOOD].push_back(pGameObject);
	}
	else if (objName == L"Key")
	{
		m_vecItemObjTags[eType].push_back(szObjTag);

		CGameObject* pGameObject = CKey::Create(m_pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		CLayer* pLayer = Engine::Get_Layer(pLayerTag);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(szObjTag, pGameObject), E_FAIL);

		m_vecItemPool[ITEM_KEY].push_back(pGameObject);
	}
	else if (objName == L"Gold")
	{
		m_vecItemObjTags[eType].push_back(szObjTag);

		_int iTex = rand() % GOLD_END;

		CGameObject* pGameObject = CGold::Create(m_pGraphicDev, vPos, iTex);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		CLayer* pLayer = Engine::Get_Layer(pLayerTag);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(szObjTag, pGameObject), E_FAIL);

		m_vecItemPool[ITEM_GOLD].push_back(pGameObject);
	}

	else
	{
		delete[] szObjTag;
		szObjTag = nullptr;
	}



	return S_OK;
}

CItem * CItemMgr::Add_GameObject_Shop(const _tchar * pLayerTag, const _tchar * objTag, ITEMTYPE eType, _vec3 vPos)
{

	wstring tag = objTag;
	tag += L"%d";

	TCHAR   *   szObjTag = new TCHAR[MAX_PATH];
	wsprintf(szObjTag, objTag);
	_tcscat_s(szObjTag, MAX_PATH, L"%d");
	wsprintf(szObjTag, tag.c_str(), m_vecItemObjTags[eType].size());


	wstring objName = objTag;
	if (objName == L"RadWand")
	{
		m_vecItemObjTags[eType].push_back(szObjTag);

		CItem* pGameObject = CRedWand::Create(m_pGraphicDev, vPos);
		if (nullptr == pGameObject)
			MSG_BOX("레드완드 생성실패");

		CLayer* pLayer = Engine::Get_Layer(pLayerTag);

		if (E_FAIL == pLayer->Add_GameObject(szObjTag, pGameObject))
			MSG_BOX("레이어 불러오기실패");

		m_vecItemPool[ITEM_WEAPON].push_back(pGameObject);

		return pGameObject;
	}

	else if (objName == L"Potion")
	{
		m_vecItemObjTags[eType].push_back(szObjTag);

		CItem* pGameObject = CPotion::Create(m_pGraphicDev, vPos, 0);
		if (nullptr == pGameObject)
			MSG_BOX("포션 생성실패");

		CLayer* pLayer = Engine::Get_Layer(pLayerTag);

		if (E_FAIL == pLayer->Add_GameObject(szObjTag, pGameObject))
			MSG_BOX("레이어 불러오기실패");

		m_vecItemPool[ITEM_POTION].push_back(pGameObject);

		return pGameObject;

	}

	else if (objName == L"Lantern")
	{
		m_vecItemObjTags[eType].push_back(szObjTag);

		CItem* pGameObject = CLantern::Create(m_pGraphicDev, vPos);
		if (nullptr == pGameObject)
			MSG_BOX("랜턴 생성실패");

		CLayer* pLayer = Engine::Get_Layer(pLayerTag);

		if (E_FAIL == pLayer->Add_GameObject(szObjTag, pGameObject))
			MSG_BOX("레이어 불러오기실패");

		m_vecItemPool[ITEM_LANTERN].push_back(pGameObject);

		return pGameObject;

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
	// ����
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
