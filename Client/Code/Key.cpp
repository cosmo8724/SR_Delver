#include "stdafx.h"
#include "Key.h"
#include "Export_Function.h"
#include "CullingMgr.h"


CKey::CKey(LPDIRECT3DDEVICE9 pGraphicDev)
	:CItem(pGraphicDev)
{
	D3DXMatrixIdentity(&m_matWorld);
	m_ObjTag = L"Key";
}

CKey::CKey(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	:CItem(pGraphicDev)
{
	m_vPos = vPos;
	D3DXMatrixIdentity(&m_matWorld);
	m_ObjTag = L"Key";
	m_eItemType = ITEM_KEY;
	m_str = L"<Key>\nType:Item\nKeep it well";
	m_fHeight = 0.8f;
}

CKey::~CKey()
{
}

HRESULT CKey::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);
	m_eState = STATE_POP;
	m_pColliderCom->Set_Free(true);
	m_eItemType = ITEM_KEY;
	return S_OK;

}

_int CKey::Update_Object(const _float & fTimeDelta)
{
	if (m_eState == STATE_INV)
		return 0;

	if (m_bDead)
		return OBJ_DEAD;

	int iResult = CItem::Update_Object(fTimeDelta);

	switch (m_eState)
	{
	case STATE_GROUND:
		m_pColliderCom->Set_Free(false);
		m_pTransCom->Set_Scale(0.5f, 0.5f, 0.5f);
		break;
	case STATE_POP:
		_float fSpeed = fTimeDelta;
		m_fHeightNow += fSpeed;
		if (m_fHeightNow < m_fHeight)
		{

			m_pTransCom->Move_Pos(&_vec3{ 0.f, fSpeed, 0.f });
		}
		else
			m_eState = STATE_GROUND;

		break;
	}


	m_pColliderCom->Calculate_WorldMatrix(*m_pTransCom->Get_WorldMatrixPointer());

	return iResult;
}

void CKey::LateUpdate_Object(void)
{
	if (m_eState != STATE_GROUND)
		return;

	if (CCullingMgr::GetInstance()->Is_Inside(this))
		Add_RenderGroup(RENDER_ALPHA, this);

	Billboard();
	CGameObject::LateUpdate_Object();
}

void CKey::Render_Obejct(void)
{
	if (m_eState != STATE_GROUND)
		return;

 	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

#ifdef _DEBUG
	// Collider
	m_pGraphicDev->SetTransform(D3DTS_WORLD,
		&(m_pColliderCom->Get_WorldMatrix()));
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pColliderCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif
}

HRESULT CKey::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	// 버퍼 컴포넌트
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	// 텍스쳐 컴객체 컴포넌트
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Key_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Key_Texture", pComponent });
	m_textureTag = L"Proto_Key_Texture";

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

CKey * CKey::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CKey*	pInstance = new CKey(pGraphicDev, vPos);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CKey::Free(void)
{
	CGameObject::Free();
}

void CKey::CollisionEvent(CGameObject * pObj)
{
	int i = 0;
}

void CKey::InteractEvent()
{
	if (STATE_GROUND == m_eState)
	{
		//CMonster* pMonster = dynamic_cast<CMonster*>(pObj);
		//if (pMonster)
		//	return;

		m_eState = STATE_INV;
		m_pColliderCom->Set_Free(true);
	}
}
