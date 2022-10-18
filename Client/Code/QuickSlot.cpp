#include "stdafx.h"
#include "..\Header\QuickSlot.h"

#include "Export_Function.h"
#include "InvImg.h"
#include "Player.h"
#include "CrossHair.h"

CQuickSlot::CQuickSlot(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
	for (int i = 0; i < m_iMaxRow; ++i)
	{
		for (int j = 0; j < m_iMaxCol; ++j)
		{
			m_Slot[i][j] = nullptr;
		}
	}
}

CQuickSlot::~CQuickSlot()
{
}

HRESULT CQuickSlot::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
	m_fPosX = WINCX*0.5f;
	m_fPosY = m_fTileSize * 0.5f;
	m_fScaleX = 160.f;
	m_fScaleY = 32.f;

	_float firstX = m_fPosX - 2.f * m_fTileSize;
	_float firstY = m_fPosY;

	for (int i = 0; i < m_iMaxRow; ++i)
	{
		for (int j = 0; j < m_iMaxCol; ++j)
		{
			m_PosArr[i][j] = { firstX + m_fTileSize * j, firstY + m_fTileSize * i };
		}
	}

	return S_OK;
}

_int CQuickSlot::Update_Object(const _float & fTimeDelta)
{
	//if (Engine::Get_DIKeyState(DIK_TAB) & 0x80)
	//	m_bShow = true;
	//else
	//	m_bShow = false;

	m_pTransCom->Set_Scale(m_fScaleX, m_fScaleY, 1.f);
	m_pTransCom->Set_Pos(m_fPosX - WINCX * 0.5f, -m_fPosY + WINCY * 0.5f, 0.f);


	//Mouse_Input(fTimeDelta);
	Key_Input(fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CQuickSlot::LateUpdate_Object(void)
{
	//if (!m_bShow)
	//	return;

	Engine::CGameObject::LateUpdate_Object();
}

void CQuickSlot::Render_Obejct(void)
{
	//if (!m_bShow)
	//	return;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	_matrix		ViewMatrix;
	ViewMatrix = *D3DXMatrixIdentity(&ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

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

HRESULT CQuickSlot::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// m_pTextureCom	
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UI_QuickSlot_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UI_QuickSlot_Texture", pComponent });

	return S_OK;
}

void CQuickSlot::Key_Input(const _float & fTimeDelta)
{
	CItem* pRight = nullptr;
	_int iKey = -1;


	if (Key_Down(iKey=DIK_1) || Key_Down(iKey = DIK_2) || Key_Down(iKey = DIK_3) || Key_Down(iKey = DIK_4) || Key_Down(iKey = DIK_5))
	{
		CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
		CItem*		pItem = pPlayer->Get_Right();


		if (nullptr != m_Slot[0][iKey -2])
		{
			pRight = static_cast<CInvImg*>(m_Slot[0][iKey - 2])->Get_TargetObj();
			pRight->Set_Equipped();
			if (ITEM_WEAPON == pRight->Get_ItemType())
			{
				if (pRight == pItem)
					return;
				if (nullptr != pItem && ITEM_WEAPON == pItem->Get_ItemType())
					pItem->Set_State(STATE_INV);

				pPlayer->Set_Right(pRight);
				if (WT_AD == static_cast<CWeapon*>(pRight)->Get_WeaponType())
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
			else
			{
				m_Slot[0][iKey - 2]->Set_Dead(true);
				m_Slot[0][iKey - 2] = nullptr;
			}
		}
	}
	

}

_bool CQuickSlot::Is_Clicked(POINT ptMouse)
{
	for (int j = 0; j < m_iMaxCol; ++j)
	{
		RECT rcUI = { _long(m_PosArr[0][j].x - 32.f), _long(m_PosArr[0][j].y - 32.f)
			, _long(m_PosArr[0][j].x + 32.f), _long(m_PosArr[0][j].y + 32.f) };

		if (PtInRect(&rcUI, ptMouse))
		{
			m_iRow = 0, m_iCol = j;
			return true;
		}
	}
	return false;
}

void CQuickSlot::Set_Item(CItem * pItem)
{
	if (nullptr == pItem)
		m_Slot[0][m_iCol]->Set_State(STATE_INV);

	m_Slot[0][m_iCol] = pItem;

	if (nullptr != pItem)
	{
		static_cast<CInvImg*>(pItem)->Set_InvPos(m_PosArr[0][m_iCol].x, m_PosArr[0][m_iCol].y);
		static_cast<CInvImg*>(pItem)->Set_Picked(false);
		static_cast<CInvImg*>(pItem)->Set_State(STATE_QUICK);

	}

}

CQuickSlot * CQuickSlot::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CQuickSlot *	pInstance = new CQuickSlot(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CQuickSlot::Free(void)
{
	CUI::Free();
}
