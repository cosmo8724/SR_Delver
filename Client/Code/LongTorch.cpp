#include "stdafx.h"
#include "..\Header\LongTorch.h"
#include "Export_Function.h"

CLongTorch::CLongTorch(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	:CEcoObject(pGraphicDev)
{
	m_eType = ECO_TORCH2;
	m_vPos = vPos;
}

CLongTorch::CLongTorch(const CEcoObject & rhs)
	: CEcoObject(rhs)
{
}

CLongTorch::~CLongTorch()
{
}

HRESULT CLongTorch::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (!m_bClone)
		m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	m_pTransCom->Set_Scale(1.f, 1.3f, 1.0f);

	return S_OK;
}

_int CLongTorch::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	CEcoObject::Update_Object(fTimeDelta);

	m_fFrame += fTimeDelta * 10.f;
	_int iFrameEnd = m_pTextureCom->Get_FrameEnd();
	if (iFrameEnd <= m_fFrame)
		m_fFrame = 0.f;

	Add_RenderGroup(RENDER_ALPHA, this);

	return OBJ_NOEVENT;
}

void CLongTorch::LateUpdate_Object(void)
{
	if (m_bDead)
		return;

	Billboard();
	CEcoObject::LateUpdate_Object();
}

void CLongTorch::Render_Obejct(void)
{
	if (m_bDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	m_pTextureCom->Set_Texture((_ulong)m_fFrame);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);


	CEcoObject::Render_Obejct();	// collider Ãâ·Â
}

HRESULT CLongTorch::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	if (!m_bClone)
	{
		pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
		NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });
	}

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_EcoTorch2_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_EcoTorch2_Texture", pComponent });

	return S_OK;
}

CLongTorch * CLongTorch::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CLongTorch *	pInstance = new CLongTorch(pGraphicDev, vPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CLongTorch * CLongTorch::Create(CEcoObject * pEcoObject)
{
	CLongTorch *	pInstance = new CLongTorch(*pEcoObject);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLongTorch::Free(void)
{
	CEcoObject::Free();
}
