#include "stdafx.h"
#include "..\Header\SpiderBackGround.h"
#include "Export_Function.h"

CSpiderBackGround::CSpiderBackGround(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
	, m_bSpiderTimeAcc(0.f)
{
}

CSpiderBackGround::~CSpiderBackGround()
{
}

HRESULT CSpiderBackGround::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matWorld);

	m_fScaleX = WINCX;
	m_fScaleY = WINCY;

	m_fPosX = 0.f;
	m_fPosY = 0.f;

	D3DXMatrixScaling(&m_matView, m_fScaleX, m_fScaleY, 1.f);

	m_matView._41 = m_fPosX;
	m_matView._42 = m_fPosY;

	m_fFlowdownSpeed = 0.7f;
	m_iTransparency = 255;

	return S_OK;
}

_int CSpiderBackGround::Update_Object(const _float & fTimeDelta)
{
	if (!m_bSpider)
		return 0;

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	// 거미줄 흘러내리는
	m_fFlowdownTimeAcc += fTimeDelta;
	if (0.1f < m_fFlowdownTimeAcc)
	{
		m_fPosY -= m_fFlowdownSpeed;
		m_matView._41 = m_fPosX;
		m_matView._42 = m_fPosY;
		m_fFlowdownTimeAcc = 0.f;
	}
	
	m_bSpiderTimeAcc += fTimeDelta;
	if (2.f < m_bSpiderTimeAcc) // 거미줄 지워주기
	{
		m_fTransparencyTimeAcc += fTimeDelta;
		if (0.05f < m_fTransparencyTimeAcc)
		{
			m_iTransparency -= 10; // 투명도가 연해지는 숫자 > 10
			m_fTransparencyTimeAcc = 0.f;

			if (0 >= m_iTransparency)
			{
				m_fPosY = 0.f;
				m_bSpiderTimeAcc = 0.f;
				m_iTransparency = 255;
				m_bSpider = false;
			}
		}
	}

	return 0;
}

void CSpiderBackGround::LateUpdate_Object(void)
{
	if (!m_bSpider)
		return;

	Engine::CGameObject::LateUpdate_Object();
}

void CSpiderBackGround::Render_Obejct(void)
{
	if (!m_bSpider)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iTransparency, 0, 0, 0));
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->Set_Texture(0);	// 텍스처 정보 세팅을 우선적으로 한다.
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CSpiderBackGround::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// m_pTextureCom	
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UI_SpiderBackground_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UI_SpiderBackground_Texture", pComponent });

	return S_OK;
}

CSpiderBackGround * CSpiderBackGround::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSpiderBackGround*	pInstance = new CSpiderBackGround(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSpiderBackGround::Free(void)
{
	CUI::Free();
}
