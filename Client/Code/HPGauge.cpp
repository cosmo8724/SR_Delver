#include "stdafx.h"
#include "..\Header\HPGauge.h"

#include "Export_Function.h"
#include "Player.h"

CHPGauge::CHPGauge(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
	, m_iHp(0)
	, m_iMinusHp(0)
	, m_iMaxHp(0)
	, m_iMinHp(0)
{
	ZeroMemory(m_szHpNumber, sizeof(m_szHpNumber));
}

CHPGauge::~CHPGauge()
{
}

HRESULT CHPGauge::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matWorld);

	/////////// 고정할 값들
	m_fScaleX = 120.f;
	m_fScaleY = 30.f;

	m_fPosX = WINCX / -2.5 - 57.f / 64.f * 120.f;
	m_fPosY = WINCY / -2.5;

	// 스케일 값
	D3DXMatrixScaling(&m_matView, m_fScaleX, m_fScaleY, 1.f);

	// 포지션
	m_matView._41 = m_fPosX;
	m_matView._42 = m_fPosY;

	return S_OK;
}

_int CHPGauge::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	//PlayerInfo
	CPlayer*	pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	tPlayerInfo = pPlayer->Get_PlayerInfo();

	// ▶ 입력하는 숫자가 게이지와 나눠질 값
	m_iHp = tPlayerInfo.iHp;
	m_iMaxHp = tPlayerInfo.iHpMax;

	m_fScaleX = _float(2.f * 120.f * m_iHp / m_iMaxHp);
	//m_fPosX = -((120.f - m_fScaleX) * 0.5f + 640.f) - 57.f / 128.f;

	D3DXMatrixScaling(&m_matView, m_fScaleX, m_fScaleY, 1.f);

	m_matView._41 = m_fPosX;
	m_matView._42 = m_fPosY;

	// 폰트
	swprintf_s(m_szHpNumber, L"%d / %d", m_iHp, m_iMaxHp);

	return 0;
}

void CHPGauge::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CHPGauge::Render_Obejct(void)
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

	Render_Font(L"Font_Jinji", m_szHpNumber, &_vec2(145, 802), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

HRESULT CHPGauge::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// m_pTextureCom	
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UI_HPGauge_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UI_HPGauge_Texture", pComponent });

	return S_OK;
}

CHPGauge * CHPGauge::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHPGauge *	pInstance = new CHPGauge(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CHPGauge::Free(void)
{
	CUI::Free();
}
