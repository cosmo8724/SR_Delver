#include "stdafx.h"
#include "Shop.h"
#include "Export_Function.h"

CShop::CShop(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CShop::~CShop()
{
}

HRESULT CShop::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
	m_fPosX = WINCX*0.5f;
	m_fPosY = WINCY*0.5f;
	m_fScaleX = 300.f;
	m_fScaleY = 120.f;

	_float	fHeight = 36.f * 2;
	_vec2 vFirst = { m_fPosX, m_fPosY- fHeight};

	for (_int i = 0; i < m_iRow; ++i)
	{
		for (_int j = 0; j < m_iCol; ++j)
		{
			_vec2 vCenter = { vFirst.x, vFirst.y + fHeight * i };

			// LT RB
			m_ShopArr[i][j] = { _long(vCenter.x - m_fScaleX), _long(vCenter.y - fHeight*0.7f)
								, _long(vCenter.x + m_fScaleX * 0.5f), _long(vCenter.y + fHeight*0.2f) };
		}
	}


	return S_OK;
}

_int CShop::Update_Object(const _float & fTimeDelta)
{
	m_pTransCom->Set_Scale(m_fScaleX, m_fScaleY, 1.f);
	m_pTransCom->Set_Pos(m_fPosX - WINCX * 0.5f, -m_fPosY + WINCY * 0.5f, 0.f);


	//Mouse_Input(fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);


	if (m_bOpen)
	{
		Engine::StopSound(SOUND_UI);
		Engine::Play_Sound(L"UI_Shop.mp3", SOUND_UI, 1.f);

		Engine::Add_RenderGroup(RENDER_UI, this);
	}

	if (m_bOpen)
	{
		POINT		ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		RECT rc = { m_ShopArr[0][0].left, m_ShopArr[0][0].top,
			m_ShopArr[2][0].right, m_ShopArr[2][0].bottom };


		if (!PtInRect(&rc, ptMouse) && !m_bSelecting)
		{
			m_iFrame = 0;
			return 0;
		}

		// Wand
		if (PtInRect(&m_ShopArr[0][0], ptMouse))
		{
			m_iFrame = 1;
			if (Mouse_Down(DIM_LB))
			{
				m_bSelecting = true;
			}
			return 0;
		}

		// Alcohol
		if (PtInRect(&m_ShopArr[1][0], ptMouse))
		{
			m_iFrame = 2;
			if (Mouse_Down(DIM_LB))
			{
				m_bSelecting = true;
			}
			return 0;
		}

		// Lantern
		if (PtInRect(&m_ShopArr[2][0], ptMouse))
		{
			m_iFrame = 3;
			if (Mouse_Down(DIM_LB))
			{
				m_bSelecting = true;
			}
			return 0;
		}





	}



	return 0;
}

void CShop::LateUpdate_Object(void)
{
	if (!m_bOpen)
		return;

	Engine::CGameObject::LateUpdate_Object();
}

void CShop::Render_Obejct(void)
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
}

HRESULT CShop::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// m_pTextureCom	
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UI_ShopWindow_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UI_ShopWindow_Texture", pComponent });

	// calculator
	//pComponent = m_pCalCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_CalculatorCom"));
	//NULL_CHECK_RETURN(m_pCalCom, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", pComponent });


	return S_OK;
}

CShop * CShop::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShop *	pInstance = new CShop(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CShop::Free(void)
{
	CUI::Free();

}
