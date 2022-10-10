#include "stdafx.h"
#include "..\Header\Inventory.h"
#include "InvImg.h"
#include "Export_Function.h"
#include "Potion.h"

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
	for (int i = 0; i < m_iMaxRow; ++i)
	{
		for (int j = 0; j < m_iMaxCol; ++j)
		{
			//if (pItem == m_Inventory[i][j])
			//	return;
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
}

void  CInventory::Pick()
{
	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	for (int i = 0; i < m_iMaxRow; ++i)
	{
		for (int j = 0; j < m_iMaxCol; ++j)
		{
			_float fSize = m_fTileSize * 0.5f;
			RECT rcUI = { _long(m_InvPosArr[i][j].x - fSize), _long(m_InvPosArr[i][j].y - fSize)
				, _long(m_InvPosArr[i][j].x + fSize), _long(m_InvPosArr[i][j].y + fSize) };

			if (PtInRect(&rcUI, ptMouse))
			{
				if (nullptr == m_ppPickedItem && nullptr != m_Inventory[i][j]) // 선택한 아이템이 없는 상황에서 아이템 클릭
				{
					m_ppPickedItem = &m_Inventory[i][j];
					static_cast<CInvImg*>(*m_ppPickedItem)->Set_Picked(true);
				}
				else if (nullptr != m_ppPickedItem && nullptr == m_Inventory[i][j]) // 선택한 아이템이 있는 상황에서 빈 칸을 클릭
				{
					m_Inventory[i][j] = *m_ppPickedItem;
					static_cast<CInvImg*>(*m_ppPickedItem)->Set_InvPos(m_InvPosArr[i][j].x, m_InvPosArr[i][j].y); // 위치 변경
					static_cast<CInvImg*>(*m_ppPickedItem)->Set_Picked(false);	// 고정

																				// 선택한 아이템이 있던 기존 타일은 null
					(*m_ppPickedItem) = nullptr;
					m_ppPickedItem = nullptr;
				}
				else if (nullptr != m_ppPickedItem&& nullptr != m_Inventory[i][j]) // 선택한 아이템이 있는 상황에서 아이템을 클릭했을 때
				{
					_vec2 vOrigin = static_cast<CInvImg*>(*m_ppPickedItem)->Get_InvPos();

					CItem* pTemp = m_Inventory[i][j];

					m_Inventory[i][j] = *m_ppPickedItem;
					static_cast<CInvImg*>(*m_ppPickedItem)->Set_InvPos(m_InvPosArr[i][j].x, m_InvPosArr[i][j].y); // 위치 변경
					static_cast<CInvImg*>(*m_ppPickedItem)->Set_Picked(false);	// 고정

					(*m_ppPickedItem) = pTemp;
					static_cast<CInvImg*>(pTemp)->Set_InvPos(vOrigin.x, vOrigin.y); // 위치 변경

					m_ppPickedItem = nullptr;
				}

				break;
			}
		}
	}
}

void CInventory::Set_ItemEquip()
{
	if (nullptr != m_ppPickedItem) // 선택한 아이템이 있다면 더블클릭 시 실행되지 않게 하자.
		return;


	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

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


					CItem*	pItem = static_cast<CInvImg*>(m_Inventory[i][j])->Get_TargetObj();
					pItem->Set_Equipped();
					switch (pItem->Get_ItemType())
					{
					case ITEM_WEAPON:
						if (nullptr != m_pEquipped)
							static_cast<CInvImg*>(m_pEquipped)->Get_TargetObj()->Set_State(STATE_INV);
						m_pEquipped = m_Inventory[i][j];
						break;
					case ITEM_POTION:	// 해당 인벤토리아이템을 삭제해야 한다.
					{
						m_Inventory[i][j]->Set_Dead(true);
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