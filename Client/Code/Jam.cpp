#include "stdafx.h"
#include "..\Header\Jam.h"
#include "Export_Function.h"

CJam::CJam(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	:CEcoObject(pGraphicDev)
{
	m_eType = ECO_JAM;
	m_vPos = vPos;
}

CJam::CJam(const CEcoObject & rhs)
	: CEcoObject(rhs)
{
}

CJam::~CJam()
{
}

HRESULT CJam::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (!m_bClone)
		m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	return S_OK;
}

_int CJam::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	CEcoObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_ALPHA, this);

	return OBJ_NOEVENT;
}

void CJam::LateUpdate_Object(void)
{
	if (m_bDead)
		return;

	Billboard();
	CEcoObject::LateUpdate_Object();
}

void CJam::Render_Obejct(void)
{
	if (m_bDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	// 알파값 변화
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iTransparency, 0, 0, 0));
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	m_pTextureCom->Set_Texture(m_iTexture);
	m_pBufferCom->Render_Buffer();

	// 알파값 변화 해제
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	CEcoObject::Render_Obejct();	// collider 출력
}

HRESULT CJam::Add_Component(void)
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_EcoJam_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_EcoJam_Texture", pComponent });


	return S_OK;
}

CJam * CJam::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CJam *	pInstance = new CJam(pGraphicDev, vPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CJam * CJam::Create(CEcoObject * pEcoObject)
{
	CJam *	pInstance = new CJam(*pEcoObject);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CJam::Free(void)
{
	CEcoObject::Free();

}
