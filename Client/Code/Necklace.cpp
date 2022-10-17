#include "stdafx.h"
#include "Necklace.h"
#include "CullingMgr.h"
#include "Export_Function.h"

CNecklace::CNecklace(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int texId)
	:CEquipment(pGraphicDev)
{
	D3DXMatrixIdentity(&m_matWorld);
	m_vPos = vPos;
	m_ObjTag = L"Necklace";
	m_iTextureType = texId;
}

CNecklace::~CNecklace()
{
}

HRESULT CNecklace::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	m_eState = STATE_GROUND;
	m_tInfo.iDef = 10;
	m_eItemType = ITEM_NECKLACE;
	return S_OK;
}

_int CNecklace::Update_Object(const _float & fTimeDelta)
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

void CNecklace::LateUpdate_Object(void)
{
	if (m_eState != STATE_GROUND)
		return;

	if (CCullingMgr::GetInstance()->Is_Inside(this))
		Add_RenderGroup(RENDER_ALPHA, this);

	Billboard();
	CGameObject::LateUpdate_Object();

}

void CNecklace::Render_Obejct(void)
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

HRESULT CNecklace::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	// ���� ������Ʈ
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	// �ؽ��� �İ�ü ������Ʈ
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Necklace_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Necklace_Texture", pComponent });
	m_textureTag = L"Proto_Necklace_Texture";

	// ������� ������Ʈ
	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// Collider Component
	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	return S_OK;
}

CNecklace * CNecklace::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int texId)
{
	CNecklace*	pInstance = new CNecklace(pGraphicDev, vPos, texId);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CNecklace::Free(void)
{
	CGameObject::Free();
}

void CNecklace::CollisionEvent(CGameObject * pObj)
{
	if (STATE_GROUND == m_eState)
	{
		m_eState = STATE_INV;
		m_pColliderCom->Set_Free(true);
	}
}