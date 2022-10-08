#include "stdafx.h"
#include "..\Header\Inventory.h"
#include "InvImg.h"
#include "Export_Function.h"

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

	/*
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matWorld);

	m_fScaleX = 192.f;
	m_fScaleY = 96.f;

	m_fPosX = 64;
	m_fPosY = WINCY / 4;

	// scale
	D3DXMatrixScaling(&m_matView, m_fScaleX, m_fScaleY, 1.f);

	// position
	m_matView._41 = m_fTempPosX;
	m_matView._42 = m_fTempPosY;

	// texture position for mouse picking
	m_fInvWidth = m_fScaleX * 2.f;
	m_fInvHeight = m_fScaleY * 2.f;
	m_fTileSize = 32.f * 2;

	_float firstX = m_fPosX - 2.5f * m_fTileSize;
	_float firstY = m_fPosY + 1.0f * m_fTileSize;


	for (int i = 0; i < 3; ++i)
	{
	for (int j = 0; j < 6; ++j)
	{
	m_InvPosArr[i][j] = { firstX + m_fTileSize * j, firstY - m_fTileSize*i };
	}
	}
	*/

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
	m_fPosX = WINCX*0.5f + 64.f;
	m_fPosY = WINCY*0.25f;
	m_fScaleX = 192.f;
	m_fScaleY = 96.f;

	m_fTileSize = 32.f * 2;

	_float firstX = m_fPosX - 2.5f * m_fTileSize;
	_float firstY = m_fPosY - 1.0f * m_fTileSize;


	// m_fPosX - WINCX * 0.5f, -m_fPosY
	//firstX = firstX - WINCX * 0.5f;
	//firstY = -firstY + WINCY * 0.5f;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 6; ++j)
		{
			m_InvPosArr[i][j] = { firstX + m_fTileSize * j, firstY + m_fTileSize * i };
		}
	}



	_vec3 vPos = m_InvPosArr[2][5];
	//RECT		rcUI = { _long(m_fPosX - m_fScaleX * 0.5f), _long(m_fPosY - m_fScaleY * 0.5f)
	//	, _long(m_fPosX + m_fScaleX * 0.5f), _long(m_fPosY + m_fScaleY * 0.5f) };

	//POINT		ptMouse;
	//GetCursorPos(&ptMouse);
	//ScreenToClient(g_hWnd, &ptMouse);


	//if (PtInRect(&rcUI, ptMouse))
	//{
	//	MSG_BOX("�浹");
	//}





	return S_OK;
}

_int CInventory::Update_Object(const _float & fTimeDelta)
{
	if (Engine::Get_DIKeyState(DIK_TAB) & 0x80)
		m_bShow = true;
	else
		m_bShow = false;

	//if (!m_bShow)
	//	return 0;
	m_pTransCom->Set_Scale(m_fScaleX, m_fScaleY, 1.f);
	m_pTransCom->Set_Pos(m_fPosX - WINCX * 0.5f, -m_fPosY + WINCY * 0.5f, 0.f);


	Mouse_Input(fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);



	Engine::Add_RenderGroup(RENDER_UI, this);

	//if (Engine::Get_DIKeyState(DIK_TAB) & 0X80)
	//{
	//	m_matView._41 = m_fPosX;
	//	m_matView._42 = m_fPosY;
	//}
	//else
	//{
	//	m_matView._41 = m_fTempPosX;
	//	m_matView._42 = m_fTempPosY;
	//}


	return 0;
}

void CInventory::LateUpdate_Object(void)
{
	if (!m_bShow)
		return;


	//RECT		rcUI = { _long(m_fPosX - m_fScaleX), _long(m_fPosY - m_fScaleY)
	//	, _long(m_fPosX + m_fScaleX), _long(m_fPosY + m_fScaleY) };

	//POINT		ptMouse;
	//GetCursorPos(&ptMouse);
	//ScreenToClient(g_hWnd, &ptMouse);


	//if (PtInRect(&rcUI, ptMouse))
	//{
	//	MSG_BOX("�浹");
	//}

	//RECT		rcUI = { _long(m_InvPosArr[2][5].x - m_fTileSize*0.5f), _long(m_InvPosArr[2][5].y - m_fTileSize*0.5f)
	//	, _long(m_InvPosArr[2][5].x + m_fTileSize*0.5f), _long(m_InvPosArr[2][5].y + m_fTileSize*0.5f) };

	//POINT		ptMouse;
	//GetCursorPos(&ptMouse);
	//ScreenToClient(g_hWnd, &ptMouse);


	//if (PtInRect(&rcUI, ptMouse))
	//{
	//	MSG_BOX("�浹");
	//}






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
				if (nullptr == m_ppPickedItem && nullptr != m_Inventory[i][j]) // ������ �������� ���� ��Ȳ���� ������ Ŭ��
				{
					m_ppPickedItem = &m_Inventory[i][j];
					static_cast<CInvImg*>(*m_ppPickedItem)->Set_Picked(true);
				}
				else if (nullptr != m_ppPickedItem && nullptr == m_Inventory[i][j]) // ������ �������� �ִ� ��Ȳ���� �� ĭ�� Ŭ��
				{
					m_Inventory[i][j] = *m_ppPickedItem;
					static_cast<CInvImg*>(*m_ppPickedItem)->Set_InvPos(m_InvPosArr[i][j].x, m_InvPosArr[i][j].y); // ��ġ ����
					static_cast<CInvImg*>(*m_ppPickedItem)->Set_Picked(false);	// ����

																				// ������ �������� �ִ� ���� Ÿ���� null
					(*m_ppPickedItem) = nullptr;
					m_ppPickedItem = nullptr;
				}
				else if (nullptr != m_ppPickedItem&& nullptr != m_Inventory[i][j]) // ������ �������� �ִ� ��Ȳ���� �������� Ŭ������ ��
				{
					_vec2 vOrigin = static_cast<CInvImg*>(*m_ppPickedItem)->Get_InvPos();

					CItem* pTemp = m_Inventory[i][j];

					m_Inventory[i][j] = *m_ppPickedItem;
					static_cast<CInvImg*>(*m_ppPickedItem)->Set_InvPos(m_InvPosArr[i][j].x, m_InvPosArr[i][j].y); // ��ġ ����
					static_cast<CInvImg*>(*m_ppPickedItem)->Set_Picked(false);	// ����

					(*m_ppPickedItem) = pTemp;
					static_cast<CInvImg*>(pTemp)->Set_InvPos(vOrigin.x, vOrigin.y); // ��ġ ����

					m_ppPickedItem = nullptr;
				}

				break;
			}
		}
	}
}

void CInventory::Set_ItemEquip()
{
	if (nullptr != m_ppPickedItem) // ������ �������� �ִٸ� ����Ŭ�� �� ������� �ʰ� ����.
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
					// if the item type is lantern / armor / weapon -> equipped 
					CWeapon*	pWeapon = dynamic_cast<CWeapon*>(static_cast<CInvImg*>(m_Inventory[i][j])->Get_TargetObj());
					if (pWeapon != nullptr)
					{
						pWeapon->Set_Equipped();
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
				Set_ItemEquip();		// else if there's a picked item	&&	double clicked item		-> item equip swapped 
			}
			if (1 == m_iClickedCnt)	// one click
			{
				Pick();	 // if there's no picked item		&&	clicked item -> item picked (stick to mouse pointer)
						 // else if there's a picked item	&&	clicked item -> swap

			}
			m_fClickTime = 0.f;
			m_iClickedCnt = 0;
		}
	}
	else	// if you didn't press the Tab Key -> there's no picked item;(picked item will be sticked to the mouse pointer)
	{
		if (m_ppPickedItem != nullptr)
		{
			static_cast<CInvImg*>(*m_ppPickedItem)->Set_Picked(false);
			m_ppPickedItem = nullptr;
		}
		m_bShow = false;
	}
}
