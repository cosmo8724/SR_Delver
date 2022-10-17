#include "stdafx.h"
#include "Ring.h"
#include "CullingMgr.h"
#include "Export_Function.h"

CRing::CRing(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int texId)
	:CEquipment(pGraphicDev)
{
	D3DXMatrixIdentity(&m_matWorld);
	m_vPos = vPos;
	m_ObjTag = L"Ring";
	m_iTextureType = texId;
}

CRing::~CRing()
{
}

HRESULT CRing::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	m_eState = STATE_GROUND;
	m_tInfo.iDef = 10;
	m_eItemType = ITEM_RING;
	return S_OK;
}

_int CRing::Update_Object(const _float & fTimeDelta)
{
	if(m_eState == STATE_INV)
		return 0;

	if (m_bDead)
		return OBJ_DEAD;

	int iResult = CItem::Update_Object(fTimeDelta);

	if (STATE_GROUND == m_eState)
	{
		m_pTransCom->Set_Scale(0.5f, 0.5f, 0.5f);
		m_pTransCom->Set_Y(m_vPos.y - 0.3f);
	}

	m_pColliderCom->Calculate_WorldMatrix(*m_pTransCom->Get_WorldMatrixPointer());

	return iResult;
}

void CRing::LateUpdate_Object(void)
{
	if (m_eState != STATE_GROUND)
		return;

	if (CCullingMgr::GetInstance()->Is_Inside(this))
		Add_RenderGroup(RENDER_ALPHA, this);

	Billboard();
	CGameObject::LateUpdate_Object();

}

void CRing::Render_Obejct(void)
{
	if (m_eState != STATE_GROUND)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->Set_Texture(m_iTextureType);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

#ifdef _DEBUG
	// Collider
	m_pGraphicDev->SetTransform(D3DTS_WORLD,
		&(m_pColliderCom->Get_WorldMatrix()));
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pColliderCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif
}

HRESULT CRing::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	// 버퍼 컴포넌트
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	// 텍스쳐 컴객체 컴포넌트
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Ring_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Ring_Texture", pComponent });
	m_textureTag = L"Proto_Ring_Texture";

	// 월드행렬 컴포넌트
	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// Collider Component
	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	return S_OK;
}

CRing * CRing::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int texId)
{
	CRing*	pInstance = new CRing(pGraphicDev, vPos, texId);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CRing::Free(void)
{
	CGameObject::Free();
}

void CRing::CollisionEvent(CGameObject * pObj)
{
	if (STATE_GROUND == m_eState)
	{
		m_eState = STATE_INV;
		m_pColliderCom->Set_Free(true);
	}
}
