#include "stdafx.h"
#include "..\Header\EquipWindow.h"

#include "Export_Function.h"

CEquipWindow::CEquipWindow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CEquipWindow::~CEquipWindow()
{
}

HRESULT CEquipWindow::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matWorld);

	m_fScaleX = 64.f;
	m_fScaleY = 96.f;

	m_fPosX = WINCX / -8;
	m_fPosY = WINCY / 4;

	// ������ ��
	D3DXMatrixScaling(&m_matView, m_fScaleX, m_fScaleY, 1.f);

	// ������
	m_matView._41 = m_fPosX;
	m_matView._42 = m_fPosY;

	return S_OK;
}

_int CEquipWindow::Update_Object(const _float & fTimeDelta)
{
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

void CEquipWindow::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CEquipWindow::Render_Obejct(void)
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->Set_Texture(0);	// �ؽ�ó ���� ������ �켱������ �Ѵ�.
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
