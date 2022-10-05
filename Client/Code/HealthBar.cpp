#include "stdafx.h"
#include "..\Header\HealthBar.h"

#include "Export_Function.h"

CHealthBar::CHealthBar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CHealthBar::~CHealthBar()
{
}

HRESULT CHealthBar::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matWorld);

	m_fScaleX = 120.f;
	m_fScaleY = 30.f;

	m_fPosX = WINCX / -2.5;
	m_fPosY = WINCY / -2.5;

	// ������ ��
	D3DXMatrixScaling(&m_matView, m_fScaleX, m_fScaleY, 1.f);

	// ������
	m_matView._41 = m_fPosX;
	m_matView._42 = m_fPosY;

	//D3DXMatrixOrthoLH(&m_matOrtho, WINCX, WINCY, 0.f, 1.f);

	return S_OK;
}

_int CHealthBar::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CHealthBar::LateUpdate_Object(void)
{

	Engine::CGameObject::LateUpdate_Object();
}

void CHealthBar::Render_Obejct(void)
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matOrtho);

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

HRESULT CHealthBar::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// m_pTextureCom	
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UI_HealthBar_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UI_HealthBar_Texture", pComponent });

	return S_OK;
}

CHealthBar * CHealthBar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHealthBar *	pInstance = new CHealthBar(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CHealthBar::Free(void)
{
	CUI::Free();
}
