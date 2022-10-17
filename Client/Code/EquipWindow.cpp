#include "stdafx.h"
#include "..\Header\EquipWindow.h"

#include "Export_Function.h"
#include "InvImg.h"
#include "Inventory.h"
#include "Player.h"

CEquipWindow::CEquipWindow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
	for (int i = 0; i < m_iMaxRow; ++i)
	{
		for (int j = 0; j < m_iMaxCol; ++j)
		{
			m_Window[i][j] = nullptr;
		}
	}
}

CEquipWindow::~CEquipWindow()
{
}

HRESULT CEquipWindow::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
	m_fPosX = WINCX*0.25f*1.5f;
	m_fPosY = WINCY*0.25f;
	m_fScaleX = 64.f;
	m_fScaleY = 96.f;

	m_fTileSize = 64.f;

	_float firstX = m_fPosX - 0.5f * m_fTileSize;
	_float firstY = m_fPosY - 1.0f * m_fTileSize;

	for (int i = 0; i < m_iMaxRow; ++i)
	{
		for (int j = 0; j < m_iMaxCol; ++j)
		{
			m_PosArr[i][j] = { firstX + m_fTileSize * j, firstY + m_fTileSize * i };
		}
	}
	return S_OK;
}

_int CEquipWindow::Update_Object(const _float & fTimeDelta)
{
	if (Engine::Get_DIKeyState(DIK_TAB) & 0x80)
		m_bShow = true;
	else
		m_bShow = false;

	m_pTransCom->Set_Scale(m_fScaleX, m_fScaleY, 1.f);
	m_pTransCom->Set_Pos(m_fPosX - WINCX * 0.5f, -m_fPosY + WINCY * 0.5f, 0.f);

	Mouse_Input(fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;

}

void CEquipWindow::LateUpdate_Object(void)
{
	if (!m_bShow)
		return;

	//POINT		ptMouse;
	//GetCursorPos(&ptMouse);
	//ScreenToClient(g_hWnd, &ptMouse);

	//_float fSize = 32.f;
	//RECT rcUI = { _long(m_PosArr[2][1].x - fSize), _long(m_PosArr[2][1].y - fSize)
	//	, _long(m_PosArr[2][1].x + fSize), _long(m_PosArr[2][1].y + fSize) };

	//if (PtInRect(&rcUI, ptMouse))
	//{
	//	MSG_BOX("ȭ��");
	//}

	Engine::CGameObject::LateUpdate_Object();



}

void CEquipWindow::Render_Obejct(void)
{
	if (!m_bShow)
		return;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	_matrix		ViewMatrix;
	ViewMatrix = *D3DXMatrixIdentity(&ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CEquipWindow::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// m_pTextureCom	
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UI_EquipWindow_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UI_EquipWindow_Texture", pComponent });

	return S_OK;
}

CEquipWindow * CEquipWindow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEquipWindow *	pInstance = new CEquipWindow(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEquipWindow::Free(void)
{
	CUI::Free();
}

void CEquipWindow::Set_Item(CItem * pItem, ITEMTYPE eType)
{
	switch (eType)
	{
	case ITEM_ARMOR:
	{
		if (m_Window[1][0] != nullptr)
		{
			CInventory* pInv = static_cast<CInventory*>(Engine::Get_GameObject(L"Layer_UI", L"UI_Inventory"));
			pInv->Set_Inventory(m_Window[1][0]);

			CItem* pItem = static_cast<CInvImg*>(m_Window[1][0])->Get_TargetObj();
			CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
			pPlayer->Set_Info(pItem->Get_ItemInfo(), -1);
		}

		m_Window[1][0] = pItem; // InvImg
		static_cast<CInvImg*>(pItem)->Set_InvPos(m_PosArr[1][0].x, m_PosArr[1][0].y);
		CItem* pItem = static_cast<CInvImg*>(m_Window[1][0])->Get_TargetObj();
		CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
		pPlayer->Set_Info(pItem->Get_ItemInfo(), 1);
	}
	break;
	case ITEM_SHIELD:
	{
		if (m_Window[0][1] != nullptr)
		{
			CInventory* pInv = static_cast<CInventory*>(Engine::Get_GameObject(L"Layer_UI", L"UI_Inventory"));
			pInv->Set_Inventory(m_Window[0][1]);

			CItem* pItem = static_cast<CInvImg*>(m_Window[0][1])->Get_TargetObj();
			CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
			pPlayer->Set_Info(pItem->Get_ItemInfo(), -1);
		}
		m_Window[0][1] = pItem; // InvImg
		static_cast<CInvImg*>(pItem)->Set_InvPos(m_PosArr[0][1].x, m_PosArr[0][1].y);
		CItem* pItem = static_cast<CInvImg*>(m_Window[0][1])->Get_TargetObj();
		CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
		pPlayer->Set_Info(pItem->Get_ItemInfo(), 1);
	}
	break;			
	case ITEM_HELMAT:
	{
		if (m_Window[0][0] != nullptr)
		{
			CInventory* pInv = static_cast<CInventory*>(Engine::Get_GameObject(L"Layer_UI", L"UI_Inventory"));
			pInv->Set_Inventory(m_Window[0][0]);

			CItem* pItem = static_cast<CInvImg*>(m_Window[0][0])->Get_TargetObj();
			CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
			pPlayer->Set_Info(pItem->Get_ItemInfo(), -1);
		}
		m_Window[0][0] = pItem; // InvImg
		static_cast<CInvImg*>(pItem)->Set_InvPos(m_PosArr[0][0].x, m_PosArr[0][0].y);
		CItem* pItem = static_cast<CInvImg*>(m_Window[0][0])->Get_TargetObj();
		CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
		pPlayer->Set_Info(pItem->Get_ItemInfo(), 1);
	}
	break;
	case ITEM_NECKLACE:
	{
		if (m_Window[1][1] != nullptr)
		{
			CInventory* pInv = static_cast<CInventory*>(Engine::Get_GameObject(L"Layer_UI", L"UI_Inventory"));
			pInv->Set_Inventory(m_Window[1][1]);

			CItem* pItem = static_cast<CInvImg*>(m_Window[1][1])->Get_TargetObj();
			CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
			pPlayer->Set_Info(pItem->Get_ItemInfo(), -1);
		}

		m_Window[1][1] = pItem; // InvImg
		static_cast<CInvImg*>(pItem)->Set_InvPos(m_PosArr[1][1].x, m_PosArr[1][1].y);
		CItem* pItem = static_cast<CInvImg*>(m_Window[1][1])->Get_TargetObj();
		CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
		pPlayer->Set_Info(pItem->Get_ItemInfo(), 1);
	}
	break;
	case ITEM_PANTS:
	{
		if (m_Window[2][0] != nullptr)
		{
			CInventory* pInv = static_cast<CInventory*>(Engine::Get_GameObject(L"Layer_UI", L"UI_Inventory"));
			pInv->Set_Inventory(m_Window[2][0]);

			CItem* pItem = static_cast<CInvImg*>(m_Window[2][0])->Get_TargetObj();
			CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
			pPlayer->Set_Info(pItem->Get_ItemInfo(), -1);
		}

		m_Window[2][0] = pItem; // InvImg
		static_cast<CInvImg*>(pItem)->Set_InvPos(m_PosArr[2][0].x, m_PosArr[2][0].y);
		CItem* pItem = static_cast<CInvImg*>(m_Window[2][0])->Get_TargetObj();
		CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
		pPlayer->Set_Info(pItem->Get_ItemInfo(), 1);
	}
	break;

	case ITEM_RING:
	{
		if (m_Window[2][1] != nullptr)
		{
			CInventory* pInv = static_cast<CInventory*>(Engine::Get_GameObject(L"Layer_UI", L"UI_Inventory"));
			pInv->Set_Inventory(m_Window[2][1]);

			CItem* pItem = static_cast<CInvImg*>(m_Window[2][1])->Get_TargetObj();
			CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
			pPlayer->Set_Info(pItem->Get_ItemInfo(), -1);
		}

		m_Window[2][1] = pItem; // InvImg
		static_cast<CInvImg*>(pItem)->Set_InvPos(m_PosArr[2][1].x, m_PosArr[2][1].y);
		CItem* pItem = static_cast<CInvImg*>(m_Window[2][1])->Get_TargetObj();
		CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
		pPlayer->Set_Info(pItem->Get_ItemInfo(), 1);
	}
	break;

	}
}

void CEquipWindow::Mouse_Input(const _float & fTimeDelta)
{
}
