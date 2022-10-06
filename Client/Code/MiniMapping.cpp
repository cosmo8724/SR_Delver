#include "stdafx.h"
#include "..\Header\MiniMapping.h"
#include "Export_Function.h"
#include "Terrain.h"
#include "Block.h"
#include "Player.h"
#include "..\..\Engine\Header\Monster.h"

USING(Engine)

CMiniMapping::CMiniMapping(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}


CMiniMapping::~CMiniMapping()
{
}

HRESULT CMiniMapping::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	FAILED_CHECK_RETURN(Set_Color(), E_FAIL);

	return S_OK;
}

_int CMiniMapping::Update_Object(const _float & fTimeDelta)
{
	return _int();
}

void CMiniMapping::LateUpdate_Object(void)
{
}

void CMiniMapping::Render_Obejct(void)
{
}

HRESULT CMiniMapping::Add_Component(void)
{
	return E_NOTIMPL;
}

HRESULT CMiniMapping::Set_Color()
{
	CTerrain*		pTerrain = dynamic_cast<CTerrain*>(m_pObject);
	if (pTerrain)
	{
		m_Color = { 0.3f, 0.3f, 0.3f, 1.f };
	}

	CBlock*		pBlock = dynamic_cast<CBlock*>(m_pObject);
	if (pBlock)
	{
		m_Color = { 0.8f, 0.8f, 0.8f, 1.f };
	}

	CPlayer*		pPlayer = dynamic_cast<CPlayer*>(m_pObject);
	if (pPlayer)
	{
		m_Color = { 0.f, 0.f, 1.f, 1.f };
	}

	Engine::CMonster*	pMonster = dynamic_cast<Engine::CMonster*>(m_pObject);
	if (pMonster)
	{
		m_Color = { 1.f, 0.f, 0.f, 1.f };
	}

	//FAILED_CHECK()

	return S_OK;
}

CMiniMapping * CMiniMapping::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject * pObj)
{
	CMiniMapping*		pInstance = new CMiniMapping(pGraphicDev, pObj);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMiniMapping::Free(void)
{
}
