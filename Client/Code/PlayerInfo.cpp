#include "stdafx.h"
#include "..\Header\PlayerInfo.h"

#include "Export_Function.h"
#include "Player.h"

CPlayerInfo::CPlayerInfo(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
	, m_iTextCount(0)
{
	ZeroMemory(m_szLevel, sizeof(m_szHp));
	ZeroMemory(m_szExp, sizeof(m_szHp));
	ZeroMemory(m_szHp, sizeof(m_szHp));
	ZeroMemory(m_szSpeed, sizeof(m_szHp));
	ZeroMemory(m_szHunger, sizeof(m_szHp));
	ZeroMemory(m_szAtk, sizeof(m_szHp));
	ZeroMemory(m_szDef, sizeof(m_szHp));
}

CPlayerInfo::~CPlayerInfo()
{
}

HRESULT CPlayerInfo::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
	//D3DXMatrixIdentity(&m_matView);
	//D3DXMatrixIdentity(&m_matWorld);

	m_fScaleX = 200.f;
	m_fScaleY = 150.f;

	m_fPosX = 0.f;
	m_fPosY = 0.f;

	m_pTransCom->Set_Scale(m_fScaleX, m_fScaleY, 1.f);
	m_pTransCom->Set_Pos(m_fPosX, m_fPosY, 0.f);

	//// 스케일 값
	//D3DXMatrixScaling(&m_matView, m_fScaleX, m_fScaleY, 1.f);

	//// 포지션
	//m_matView._41 = m_fPosX;
	//m_matView._42 = m_fPosX;

	swprintf_s(m_szLevel,	L"레벨 : %d / %d", 1, 10);
	swprintf_s(m_szExp,		L"경험치 : %d / %d", 0, 100);
	swprintf_s(m_szHp,		L"체력 : %d / %d", 10, 20);
	swprintf_s(m_szSpeed,	L"속력 : %.1f\n", 5.f);
	swprintf_s(m_szHunger,	L"배고픔 : %d / %d", 5, 5);
	swprintf_s(m_szAtk,		L"공격력 : %d", 1);
	swprintf_s(m_szDef,		L"방어력 : %d", 10);

	return S_OK;
}

_int CPlayerInfo::Update_Object(const _float & fTimeDelta)
{
	if (!m_bInfoRender)
		return 0;

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	//PlayerInfo
	CPlayer*	pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	PLAYERINFO tPlayerInfo = pPlayer->Get_PlayerInfo();

	swprintf_s(m_szLevel,	L"레벨 : %d / %d", tPlayerInfo.iLevel, 10);
	swprintf_s(m_szExp,		L"경험치 : %d / %d", tPlayerInfo.iExp, 100);
	swprintf_s(m_szHp,		L"체력 : %d / %d", tPlayerInfo.iHp, tPlayerInfo.iHpMax);
	swprintf_s(m_szSpeed,	L"속력 : %.1f\n", tPlayerInfo.fSpeed);
	swprintf_s(m_szHunger,	L"배고픔 : %d / %d", tPlayerInfo.iHunger, 5);
	swprintf_s(m_szAtk,		L"공격력 : %d", tPlayerInfo.iAtk);
	swprintf_s(m_szDef,		L"방어력 : %d", tPlayerInfo.iDef);

	return 0;
}

void CPlayerInfo::LateUpdate_Object(void)
{
	if (!m_bInfoRender)
		return;

	Engine::CGameObject::LateUpdate_Object();
}

void CPlayerInfo::Render_Obejct(void)
{
	if (!m_bInfoRender)
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

	Render_Font(L"Font_Jinji", m_szLevel,	&_vec2(750, 355), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	Render_Font(L"Font_Jinji", m_szExp,		&_vec2(750, 385), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	Render_Font(L"Font_Jinji", m_szHp,		&_vec2(750, 415), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	Render_Font(L"Font_Jinji", m_szSpeed,	&_vec2(750, 445), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	Render_Font(L"Font_Jinji", m_szHunger,	&_vec2(750, 475), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	Render_Font(L"Font_Jinji", m_szAtk,		&_vec2(750, 505), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	Render_Font(L"Font_Jinji", m_szDef,		&_vec2(750, 535), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

}

HRESULT CPlayerInfo::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// m_pTextureCom	
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PlayerInfo_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_PlayerInfo_Texture", pComponent });

	return S_OK;
}

CPlayerInfo * CPlayerInfo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayerInfo *	pInstance = new CPlayerInfo(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPlayerInfo::Free(void)
{
	CUI::Free();
}
