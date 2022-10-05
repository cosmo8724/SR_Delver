#include "stdafx.h"
#include "..\Header\MapUI.h"


CMapUI::CMapUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}


CMapUI::~CMapUI()
{
}

HRESULT CMapUI::Ready_Object(void)
{
	return E_NOTIMPL;
}

_int CMapUI::Update_Object(const _float & fTimeDelta)
{
	return _int();
}

void CMapUI::LateUpdate_Object(void)
{
}

void CMapUI::Render_Obejct(void)
{
}

HRESULT CMapUI::Add_Component(void)
{
	return E_NOTIMPL;
}

CMapUI * CMapUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CMapUI::Free(void)
{
}
