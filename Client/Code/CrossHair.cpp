#include "stdafx.h"
#include "..\Header\CrossHair.h"
#include "Export_Function.h"

CCrossHair::CCrossHair(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}


CCrossHair::~CCrossHair()
{
}

HRESULT CCrossHair::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matWorld);

	m_fScaleX = 32.f;
	m_fScaleY = 32.f;

	m_fPosX = 0.f;
	m_fPosY = 0.f;

	D3DXMatrixScaling(&m_matView, m_fScaleX, m_fScaleY, 1.f);

	m_matView._41 = m_fPosX;
	m_matView._42 = m_fPosY;

	return S_OK;
}

_int CCrossHair::Update_Object(const _float & fTimeDelta)
{
	if (!m_bWeapon)
		return 0;

	Engine::CGameObject::Update_Object(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CCrossHair::LateUpdate_Object(void)
{
	if (!m_bWeapon)
		return;

	Engine::CGameObject::LateUpdate_Object();
}

void CCrossHair::Render_Obejct(void)
{
	if (!m_bWeapon)
		return;

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

HRESULT CCrossHair::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// m_pTextureCom	
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UI_CrossHair_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UI_CrossHair_Texture", pComponent });

	return S_OK;
}

CCrossHair * CCrossHair::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCrossHair*	pInstance = new CCrossHair(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CCrossHair::Free(void)
{
	CUI::Free();
}
