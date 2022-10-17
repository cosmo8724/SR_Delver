#include "stdafx.h"
#include "..\Header\Inventory.h"
#include "InvImg.h"
#include "Export_Function.h"
#include "Potion.h"
#include "CrossHair.h"
#include "Player.h"
#include "EquipWindow.h"
#include "QuickSlot.h"
#include "ItemMgr.h"

CInventory::CInventory(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
	for (int i = 0; i < m_iMaxRow; ++i)
	{
		for (int j = 0; j < m_iMaxCol; ++j)
		{
			m_Inventory[i][j] = nullptr;
		}
	}
}

CInventory::~CInventory()
{
}

HRESULT CInventory::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
	m_fPosX = WINCX*0.5f + 64.f;
	m_fPosY = WINCY*0.25f;
	m_fScaleX = 192.f;
	m_fScaleY = 96.f;

	m_fTileSize = 32.f * 2;

	_float firstX = m_fPosX - 2.5f * m_fTileSize;
	_float firstY = m_fPosY - 1.0f * m_fTileSize;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 6; ++j)
		{
			m_InvPosArr[i][j] = { firstX + m_fTileSize * j, firstY + m_fTileSize * i };
		}
	}

	return S_OK;
}

_int CInventory::Update_Object(const _float & fTimeDelta)
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

void CInventory::LateUpdate_Object(void)
{
	if (!m_bShow)
		return;

	Engine::CGameObject::LateUpdate_Object();
}

void CInventory::Render_Obejct(void)
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

HRESULT CInventory::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// m_pTextureCom	
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UI_Inventory_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UI_Inventory_Texture", pComponent });

	// calculator
	pComponent = m_pCalCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_CalculatorCom"));
	NULL_CHECK_RETURN(m_pCalCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", pComponent });


	return S_OK;
}

CInventory * CInventory::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CInventory *	pInstance = new CInventory(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CInventory::Free(void)
{
	CUI::Free();
}

void CInventory::Get_Index(int & iRow, int & iCol)
{
	for (int i = 0; i < m_iMaxRow; ++i)
	{
		for (int j = 0; j < m_iMaxCol; ++j)
		{
			if (nullptr == m_Inventory[i][j])
			{
				iRow = i;
				iCol = j;
				return;
			}
		}
	}

	iRow = -1;
	iCol = -1;
}


void CInventory::Set_Inventory(CItem * pItem)
{
	CItem* pImg = nullptr;

	_int iRow, iCol;
	_bool	bOut = false;

	if (nullptr == pItem)
		return;

	// it is for original logic
	if (nullptr != dynamic_cast<CInvImg*>(pItem))
	{
		for (int i = 0; i < m_iMaxRow; ++i)
		{
			for (int j = 0; j < m_iMaxCol; ++j)
			{
				if (nullptr == m_Inventory[i][j])
				{
					m_Inventory[i][j] = pItem;
					static_cast<CInvImg*>(m_Inventory[i][j])->Set_InvPos(m_InvPosArr[i][j].x, m_InvPosArr[i][j].y);
					CTransform* pTransCom = static_cast<CTransform*>(pItem->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
					pTransCom->Set_Pos(m_InvPosArr[i][j].x, m_InvPosArr[i][j].y, 0.f);
					return;
				}
			}
		}
		return;
	}



	// if the item is potion -> Counting
	if (ITEM_POTION == pItem->Get_ItemType())
	{
		for (int i = 0; i < m_iMaxRow; ++i)
		{
			for (int j = 0; j < m_iMaxCol; ++j)
			{
				if (nullptr != m_Inventory[i][j])
				{
					CInvImg* pInvImg = static_cast<CInvImg*>(m_Inventory[i][j]);
					if (ITEM_POTION == pInvImg->Get_TargetObj()->Get_ItemType())
					{
						if (static_cast<CPotion*>(pItem)->Get_TexturId() == static_cast<CPotion*>(pInvImg->Get_TargetObj())->Get_TexturId())
						{
							static_cast<CPotion*>(static_cast<CInvImg*>(m_Inventory[i][j])->Get_TargetObj())->Cal_Cnt(+1);
							return;
						}

					}
				}
				else
				{
					if (!bOut)
					{
						iRow = i; iCol = j;
						bOut = true;
					}
				}
			}
		}

		pImg = static_cast<CItem*>(CItemMgr::GetInstance()->Add_GameObject(L"Layer_UI", pItem->Get_TextureTag(), pItem));
		m_Inventory[iRow][iCol] = pImg;
		static_cast<CInvImg*>(m_Inventory[iRow][iCol])->Set_InvPos(m_InvPosArr[iRow][iCol].x, m_InvPosArr[iRow][iCol].y);
		CTransform* pTransCom = static_cast<CTransform*>(pImg->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
		pTransCom->Set_Pos(m_InvPosArr[iRow][iCol].x, m_InvPosArr[iRow][iCol].y, 0.f);
		return;
	}

	// normal item
	pImg = static_cast<CItem*>(CItemMgr::GetInstance()->Add_GameObject(L"Layer_UI", pItem->Get_TextureTag(), pItem));
	for (int i = 0; i < m_iMaxRow; ++i)
	{
		for (int j = 0; j < m_iMaxCol; ++j)
		{
			if (nullptr == m_Inventory[i][j])
			{
				m_Inventory[i][j] = pImg;
				static_cast<CInvImg*>(m_Inventory[i][j])->Set_InvPos(m_InvPosArr[i][j].x, m_InvPosArr[i][j].y);
				CTransform* pTransCom = static_cast<CTransform*>(pImg->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
				pTransCom->Set_Pos(m_InvPosArr[i][j].x, m_InvPosArr[i][j].y, 0.f);
				return;
			}
		}
	}
}

void  CInventory::Pick()
{
	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	RECT rc = { _long(m_InvPosArr[0][0].x - 32), _long(m_InvPosArr[0][0].y - 32)
		,_long(m_InvPosArr[m_iMaxRow - 1][m_iMaxCol - 1].x + 32), _long(m_InvPosArr[m_iMaxRow - 1][m_iMaxCol - 1].y + 32) };

	if (PtInRect(&rc, ptMouse))
	{

		for (int i = 0; i < m_iMaxRow; ++i)
		{
			for (int j = 0; j < m_iMaxCol; ++j)
			{
				_float fSize = m_fTileSize * 0.5f;
				RECT rcUI = { _long(m_InvPosArr[i][j].x - fSize), _long(m_InvPosArr[i][j].y - fSize)
					, _long(m_InvPosArr[i][j].x + fSize), _long(m_InvPosArr[i][j].y + fSize) };

				if (PtInRect(&rcUI, ptMouse))
				{
					if (nullptr == m_ppPickedItem && nullptr != m_Inventory[i][j])
					{
						m_ppPickedItem = &m_Inventory[i][j];
						static_cast<CInvImg*>(*m_ppPickedItem)->Set_Picked(true);
					}
					else if (nullptr != m_ppPickedItem && nullptr == m_Inventory[i][j])
					{
						m_Inventory[i][j] = *m_ppPickedItem;
						static_cast<CInvImg*>(*m_ppPickedItem)->Set_InvPos(m_InvPosArr[i][j].x, m_InvPosArr[i][j].y);
						static_cast<CInvImg*>(*m_ppPickedItem)->Set_Picked(false);


						(*m_ppPickedItem) = nullptr;
						m_ppPickedItem = nullptr;
					}
					else if (nullptr != m_ppPickedItem&& nullptr != m_Inventory[i][j])
					{
						_vec2 vOrigin = static_cast<CInvImg*>(*m_ppPickedItem)->Get_InvPos();

						CItem* pTemp = m_Inventory[i][j];

						m_Inventory[i][j] = *m_ppPickedItem;
						static_cast<CInvImg*>(*m_ppPickedItem)->Set_InvPos(m_InvPosArr[i][j].x, m_InvPosArr[i][j].y);
						static_cast<CInvImg*>(*m_ppPickedItem)->Set_Picked(false);

						(*m_ppPickedItem) = pTemp;
						static_cast<CInvImg*>(pTemp)->Set_InvPos(vOrigin.x, vOrigin.y);

						m_ppPickedItem = nullptr;
					}


					break;
				}
			}
		}
	}
	else // if it didn't in the inventory box
	{
		CQuickSlot* pSlot = static_cast<CQuickSlot*>(Get_GameObject(L"Layer_UI", L"UI_QuickSlot"));
		if (pSlot->Is_Clicked(ptMouse))
		{
			CItem* pItem = pSlot->Get_Item();
			if (nullptr != m_ppPickedItem && nullptr == pItem)
			{
				pSlot->Set_Item(*m_ppPickedItem);
				(*m_ppPickedItem) = nullptr;
				m_ppPickedItem = nullptr;
			}
			else if (nullptr != m_ppPickedItem && nullptr != pItem)
			{
				_vec2 vOrigin = static_cast<CInvImg*>(*m_ppPickedItem)->Get_InvPos();

				pSlot->Set_Item(*m_ppPickedItem);

				static_cast<CInvImg*>(pItem)->Set_InvPos(vOrigin.x, vOrigin.y);
				static_cast<CInvImg*>(pItem)->Set_State(STATE_QUICK);
				static_cast<CInvImg*>(pItem)->Set_Picked(false);

				(*m_ppPickedItem) = pItem;
				m_ppPickedItem = nullptr;
			}
			else if (nullptr == m_ppPickedItem && nullptr != pItem)
			{
				Set_Inventory(pItem);
				pSlot->Set_Item(nullptr);
			}
		}
	}
}

void CInventory::Set_ItemEquip()
{
	if (nullptr != m_ppPickedItem) // if you picked something, you can't equip anything.
		return;						// It means you have to do double click very well.


	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	RECT rc = { _long(m_InvPosArr[0][0].x - 32), _long(m_InvPosArr[0][0].y - 32)
		,_long(m_InvPosArr[m_iMaxRow - 1][m_iMaxCol - 1].x + 32), _long(m_InvPosArr[m_iMaxRow - 1][m_iMaxCol - 1].y + 32) };

	if (!PtInRect(&rc, ptMouse))
		return;

	for (int i = 0; i < m_iMaxRow; ++i)
	{
		for (int j = 0; j < m_iMaxCol; ++j)
		{
			_float fSize = m_fTileSize * 0.5f;
			RECT rcUI = { _long(m_InvPosArr[i][j].x - fSize), _long(m_InvPosArr[i][j].y - fSize)
				,_long(m_InvPosArr[i][j].x + fSize), _long(m_InvPosArr[i][j].y + fSize) };

			if (PtInRect(&rcUI, ptMouse))
			{
				if (nullptr != m_Inventory[i][j])
				{
					CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

					CItem*	pItem = static_cast<CInvImg*>(m_Inventory[i][j])->Get_TargetObj();
					pItem->Set_Equipped();

					switch (pItem->Get_ItemType())
					{
					case ITEM_WEAPON:
					{
						CItem*		pRight = pPlayer->Get_Right();
						if (nullptr != pRight)
							pRight->Set_State(STATE_INV);

						pPlayer->Set_Right(pItem);

						if (WT_AD == static_cast<CWeapon*>(pItem)->Get_WeaponType())
						{
							CCrossHair* pCrossHair = dynamic_cast<CCrossHair*>(Engine::Get_GameObject(L"Layer_UI", L"UI_CrossHair"));
							pCrossHair->Set_CrossHair(true);
						}
						else
						{
							CCrossHair* pCrossHair = dynamic_cast<CCrossHair*>(Engine::Get_GameObject(L"Layer_UI", L"UI_CrossHair"));
							pCrossHair->Set_CrossHair(false);
						}
					}
					break;
					case ITEM_POTION:
					{
						CPotion* pPotion = static_cast<CPotion*>(pItem);
						pPotion->Cal_Cnt(-1);

						if (0 >= pPotion->Get_Cnt())
						{
							m_Inventory[i][j]->Set_Dead(true);
							m_Inventory[i][j] = nullptr;
						}
					}
					break;
					case ITEM_KEY:
					{
						m_Inventory[i][j]->Set_Dead(true);
						m_Inventory[i][j] = nullptr;
					}
					break;
					case ITEM_FOOD:
					{
						m_Inventory[i][j]->Set_Dead(true);
						m_Inventory[i][j] = nullptr;
					}
					break;
					case ITEM_ARMOR:
					{
						//m_Inventory[i][j]->Set_Dead(true);
						CEquipWindow* pWindow = static_cast<CEquipWindow*>(Engine::Get_GameObject(L"Layer_UI", L"UI_EquipWindow"));
						pWindow->Set_Item(m_Inventory[i][j], ITEM_ARMOR);
						m_Inventory[i][j] = nullptr;
					}
					break;
					case ITEM_SHIELD:
					{
						CItem*		pLeft = pPlayer->Get_Left();
						if (nullptr != pLeft)
							pLeft->Set_State(STATE_INV);

						pPlayer->Set_Left(pItem);

						CEquipWindow* pWindow = static_cast<CEquipWindow*>(Engine::Get_GameObject(L"Layer_UI", L"UI_EquipWindow"));
						pWindow->Set_Item(m_Inventory[i][j], ITEM_SHIELD);
					}
					break;
					case ITEM_HELMAT:
					{
						CEquipWindow* pWindow = static_cast<CEquipWindow*>(Engine::Get_GameObject(L"Layer_UI", L"UI_EquipWindow"));
						pWindow->Set_Item(m_Inventory[i][j], ITEM_HELMAT);
						m_Inventory[i][j] = nullptr;
					}
					break;
					//case ITEM_LEFTHAND:
					//{
					//	CEquipWindow* pWindow = static_cast<CEquipWindow*>(Engine::Get_GameObject(L"Layer_UI", L"UI_EquipWindow"));
					//	pWindow->Set_Item(m_Inventory[i][j], ITEM_LEFTHAND);
					//	m_Inventory[i][j] = nullptr;
					//}
					//break;
					case ITEM_NECKLACE:
					{
						CEquipWindow* pWindow = static_cast<CEquipWindow*>(Engine::Get_GameObject(L"Layer_UI", L"UI_EquipWindow"));
						pWindow->Set_Item(m_Inventory[i][j], ITEM_NECKLACE);
						m_Inventory[i][j] = nullptr;
					}
					break;
					case ITEM_PANTS:
					{
						CEquipWindow* pWindow = static_cast<CEquipWindow*>(Engine::Get_GameObject(L"Layer_UI", L"UI_EquipWindow"));
						pWindow->Set_Item(m_Inventory[i][j], ITEM_PANTS);
						m_Inventory[i][j] = nullptr;
					}
					break;
					case ITEM_RING:
					{
						CEquipWindow* pWindow = static_cast<CEquipWindow*>(Engine::Get_GameObject(L"Layer_UI", L"UI_EquipWindow"));
						pWindow->Set_Item(m_Inventory[i][j], ITEM_RING);
						m_Inventory[i][j] = nullptr;
					}
					break;
					}
				}
			}
		}
	}
}

void CInventory::Mouse_Input(const _float& fTimeDelta)
{
	if (Engine::Get_DIKeyState(DIK_TAB))
	{
		m_bShow = true;
		m_fClickTime += fTimeDelta;

		if (m_fClickTime < 0.5f)
		{
			if (Engine::Mouse_Down(DIM_LB))
				++m_iClickedCnt;
			//if (2 == m_iClickedCnt)
			//{
			//	//MSG_BOX("double");
			//	//Set_ItemEquip();
			//	m_fClickTime = 0.f;
			//	m_iClickedCnt = 0;
			//}
		}
		else if (m_fClickTime > 0.5f)
		{
			if (2 == m_iClickedCnt)		// double click
			{
				Set_ItemEquip();
			}
			if (1 == m_iClickedCnt)	// one click
			{
				Pick();
			}
			m_fClickTime = 0.f;
			m_iClickedCnt = 0;
		}
	}
	else
	{
		if (m_ppPickedItem != nullptr)
		{
			static_cast<CInvImg*>(*m_ppPickedItem)->Set_Picked(false);
			m_ppPickedItem = nullptr;
		}
		m_bShow = false;
	}
}