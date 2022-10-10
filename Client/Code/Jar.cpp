#include "stdafx.h"
#include "..\Header\Jar.h"
#include "Export_Function.h"

CJar::CJar(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEcoObject(pGraphicDev)
{
}

CJar::CJar(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	:CEcoObject(pGraphicDev)
{
	m_vPos = vPos;
}

CJar::~CJar()
{
}

HRESULT CJar::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	return S_OK;
}

_int CJar::Update_Object(const _float & fTimeDelta)
{
	CEcoObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CJar::LateUpdate_Object(void)
{
	CEcoObject::LateUpdate_Object();
}

void CJar::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	CEcoObject::Render_Obejct();	// collider Ãâ·Â
}

HRESULT CJar::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_EcoObject0_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_EcoObject0_Texture", pComponent });


	return S_OK;
}

CJar * CJar::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CJar *	pInstance = new CJar(pGraphicDev, vPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CJar::Free(void)
{
	CEcoObject::Free();
}
