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

	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matWorld);

	m_fScaleX = 192.f;
	m_fScaleY = 96.f;

	m_fPosX = 64;
	m_fPosY = WINCY / 4;

	// 스케일 값
	D3DXMatrixScaling(&m_matView, m_fScaleX, m_fScaleY, 1.f);

	// 포지션
	m_matView._41 = m_fTempPosX;
	m_matView._42 = m_fTempPosY;

	// 피킹용 텍스쳐 포지션 설정
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


	return S_OK;
}

_int CInventory::Update_Object(const _float & fTimeDelta)
{
	Mouse_Input(fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_UI, this);

	if (Engine::Get_DIKeyState(DIK_TAB) & 0X80)
	{
		m_matView._41 = m_fPosX;
		m_matView._42 = m_fPosY;
	}
	else
	{
		m_matView._41 = m_fTempPosX;
		m_matView._42 = m_fTempPosY;
	}

	return 0;
}

void CInventory::LateUpdate_Object(void)
{


	Engine::CGameObject::LateUpdate_Object();
}

void CInventory::Render_Obejct(void)
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->Set_Texture(0);	// 텍스처 정보 세팅을 우선적으로 한다.
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
			if (pItem == m_Inventory[i][j])
				return;
			if (nullptr == m_Inventory[i][j])
			{
				m_Inventory[i][j] = pItem;
				CTransform* pTransCom = static_cast<CTransform*>(pItem->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
				pTransCom->Set_Pos(m_InvPosArr[i][j].x, m_InvPosArr[i][j].y, 0.f);
				return;
			}
		}
	}
}

CItem * CInventory::Set_ItemEquip()
{
	if (nullptr != m_pPickedItem)
		return m_pPickedItem;

	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	// 뷰포트 -> 투영
	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	_vec3	vPoint;
	vPoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vPoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vPoint.z = 0.f;

	// 투영 -> 뷰좌표
	_matrix matProj;
	D3DXMatrixOrthoLH(&matProj, WINCX, WINCY, 0.f, 1.f);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vPoint, &vPoint, &matProj);

	ptMouse.x = _long(vPoint.x);
	ptMouse.y = _long(vPoint.y);

	RECT		rc{};

	for (int i = 0; i < m_iMaxRow; ++i)
	{
		for (int j = 0; j < m_iMaxCol; ++j)
		{
			rc.left = long(m_InvPosArr[i][j].x - 0.5f * m_fTileSize);
			rc.top = long(m_InvPosArr[i][j].y + 0.5f * m_fTileSize);
			rc.right = long(m_InvPosArr[i][j].x + 0.5f * m_fTileSize);
			rc.bottom = long(m_InvPosArr[i][j].y - 0.5f * m_fTileSize);

			if (ptMouse.x > rc.left && ptMouse.x < rc.right && ptMouse.y > rc.bottom && ptMouse.y < rc.top)
			{
				// 무기라면 Equip 상태가 되어야 함.
				CWeapon*	pWeapon = dynamic_cast<CWeapon*>(static_cast<CInvImg*>(m_Inventory[i][j])->Get_TargetObj());
				if (pWeapon != nullptr)
				{
					pWeapon->Set_Equipped();
					break;
				}


				// 아이템이라면 소모해야 함.
			}

		}
	}

	return nullptr;
}

void CInventory::Mouse_Input(const _float& fTimeDelta)
{

	if (Engine::Get_DIKeyState(DIK_TAB))
	{
		if (Engine::Is_DoubleClicked(DIM_LB, fTimeDelta))
		{
			m_pPickedItem = Set_ItemEquip();
		}
	}

}
