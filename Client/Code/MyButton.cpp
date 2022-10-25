#include "stdafx.h"
#include "..\Header\MyButton.h"
#include "Export_Function.h"
#include "Shop.h"
#include "Inventory.h"
#include "ItemMgr.h"
#include "RedWand.h"
#include "Lantern.h"
#include "Potion.h"
#include "EquipWindow.h"
#include "Player.h"
#include "StaticCamera.h"
#include "TalkWindow.h"
CMyButton::CMyButton(LPDIRECT3DDEVICE9 pGraphicDev, _vec2 vPos, wstring str)
	: CUI(pGraphicDev)
{
	m_vPos = vPos;
	m_str = str;
}

CMyButton::~CMyButton()
{
}

HRESULT CMyButton::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
	m_fPosX = m_vPos.x;
	m_fPosY = m_vPos.y;

	m_fScaleX = 30.f;
	m_fScaleY = 17.f;


	m_Rect = { _long(m_fPosX - m_fScaleX),  _long(m_fPosY - m_fScaleY),
		_long(m_fPosX + m_fScaleX),  _long(m_fPosY + m_fScaleY) };

	return S_OK;
}

_int CMyButton::Update_Object(const _float & fTimeDelta)
{

	if (!m_bReady)
	{
		m_pTransCom->Set_Scale(m_fScaleX, m_fScaleY, 1.f);
		m_pTransCom->Set_Pos(m_fPosX - WINCX * 0.5f, -m_fPosY + WINCY * 0.5f, 0.f);

		m_pInv = dynamic_cast<CInventory*>(Get_GameObject(L"Layer_UI", L"UI_Inventory"));
		m_pShop = dynamic_cast<CShop*>(Get_GameObject(L"Layer_UI", L"UI_Shop"));
		m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

		if ((nullptr != m_pInv) && (nullptr != m_pShop) &&(nullptr != m_pPlayer))
			m_bReady = true;
		else
			return 0;
	}

	if (!m_pInv->Is_Open())
		return 0;
	Engine::Add_RenderGroup(RENDER_UI, this);

	//Mouse_Input(fTimeDelta);
	Engine::CGameObject::Update_Object(fTimeDelta);


	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_int iSelected = 0;
	//m_fTime += fTimeDelta;
	if (PtInRect(&m_Rect, ptMouse))
	{
		m_iFrame = 1;

		if (m_str == L"Buy")
		{
			if (Mouse_Down(DIM_LB))
			{
				m_fTime = 0.f;
				iSelected = m_pShop->Get_Selected();

				CTransform* pPlayer = static_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));

				switch (iSelected)
				{
				case 1:
				{
					if (g_iMoney >= 20)
					{
						m_pPlayer->Cal_Money(-20); // �ð���...
						CRedWand* pItem = static_cast<CRedWand*>(CItemMgr::GetInstance()->Add_GameObject_Shop(L"Layer_GameLogic", L"RadWand", ITEM_WEAPON, pPlayer->Get_Pos()));
						pItem->Set_State(STATE_INV);
						m_pInv->Set_Inventory(pItem);
					}
				}
				break;
				case 2:
				{
					if (g_iMoney >= 15)
					{
						m_pPlayer->Cal_Money(-15); 
						CPotion* pItem = static_cast<CPotion*>(CItemMgr::GetInstance()->Add_GameObject_Shop(L"Layer_GameLogic", L"Potion", ITEM_POTION, pPlayer->Get_Pos()));
						pItem->Set_State(STATE_INV);
						m_pInv->Set_Inventory(pItem);
					}
				}
				break;
				case 3:
				{
					if (g_iMoney >= 10)
					{
						m_pPlayer->Cal_Money(-10);
						CLantern* pItem = static_cast<CLantern*>(CItemMgr::GetInstance()->Add_GameObject_Shop(L"Layer_GameLogic", L"Lantern", ITEM_LANTERN, pPlayer->Get_Pos()));
						pItem->Set_State(STATE_INV);
						m_pInv->Set_Inventory(pItem);
					}

				}
				break;
				}
			}

		}
		else if (m_str == L"Close")
		{
			if (Mouse_Down(DIM_LB))
			{
				m_pShop->Set_Open(false);
				m_pInv->Set_Open(false);
				CEquipWindow* pEquip = static_cast<CEquipWindow*>(Engine::Get_GameObject(L"Layer_UI", L"UI_EquipWindow"));
				pEquip->Set_Open(false);

				CStaticCamera* pCam = static_cast<CStaticCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"StaticCamera"));
				pCam->Set_Free(false);

				CTalkWindow* pTalkWindow = dynamic_cast<CTalkWindow*>(Engine::Get_GameObject(L"Layer_UI", L"UI_TalkWindow"));
				pTalkWindow->Set_OFFText();
				pTalkWindow->Reset_TextCount();
			}
		}
	
	}
	else
		m_iFrame = 0;
	

	return 0;
}

void CMyButton::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();

}

void CMyButton::Render_Obejct(void)
{
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

	m_pTextureCom->Set_Texture(m_iFrame);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	Render_Font(L"Font_Jinji", m_str.c_str(), &_vec2(m_fPosX-17.f, m_fPosY-7.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

}

HRESULT CMyButton::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// m_pTextureCom	
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UI_Button_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UI_Button_Texture", pComponent });


	return S_OK;
}

CMyButton * CMyButton::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec2 vPos, wstring str)
{
	CMyButton *	pInstance = new CMyButton(pGraphicDev, vPos, str);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMyButton::Free(void)
{
	CUI::Free();
}
