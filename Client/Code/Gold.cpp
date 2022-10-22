#include "stdafx.h"
#include "Gold.h"

#include "Export_Function.h"
#include "Player.h"
#include "CullingMgr.h"

CGold::CGold(LPDIRECT3DDEVICE9 pGraphicDev)
	:CItem(pGraphicDev)
{
	D3DXMatrixIdentity(&m_matWorld);
	m_ObjTag = L"Gold";
}

CGold::CGold(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	:CItem(pGraphicDev)
{
	m_vPos = vPos;
	D3DXMatrixIdentity(&m_matWorld);
	m_ObjTag = L"Gold";
	m_eItemType = ITEM_GOLD;

}

CGold::CGold(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int _eType)
	:CItem(pGraphicDev)
{
	m_vPos = vPos;
	D3DXMatrixIdentity(&m_matWorld);
	m_ObjTag = L"Gold";
	m_iTextureIdx = _eType;
}

CGold::~CGold()
{
}

HRESULT CGold::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	m_eState = STATE_GROUND;
	m_eItemType = ITEM_GOLD;
	return S_OK;
}

_int CGold::Update_Object(const _float & fTimeDelta)
{
	if (m_eState == STATE_INV)
		return 0;

	if (m_bDead)
		return OBJ_DEAD;

	int iResult = CItem::Update_Object(fTimeDelta);

	if (STATE_GROUND == m_eState)
	{
		m_pTransCom->Set_Scale(0.5f, 0.5f, 0.5f);
		m_pTransCom->Set_Y(m_vPos.y - 0.5f);
	}


	m_pColliderCom->Calculate_WorldMatrix(*m_pTransCom->Get_WorldMatrixPointer());

	return iResult;
}

void CGold::LateUpdate_Object(void)
{
	if (m_eState != STATE_GROUND)
		return;

	if (CCullingMgr::GetInstance()->Is_Inside(this))
		Add_RenderGroup(RENDER_ALPHA, this);

	Billboard();
	CGameObject::LateUpdate_Object();
}

void CGold::Render_Obejct(void)
{
	if (m_eState != STATE_GROUND)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->Set_Texture(m_iTextureIdx);

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

HRESULT CGold::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	// 버퍼 컴포넌트
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	// 텍스쳐 컴객체 컴포넌트
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Gold_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Gold_Texture", pComponent });
	m_textureTag = L"Proto_Gold_Texture";

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


CGold * CGold::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int _eType)
{
	CGold*	pInstance = new CGold(pGraphicDev, vPos, _eType);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CGold::Free(void)
{
	CGameObject::Free();
}

void CGold::CollisionEvent(CGameObject * pObj)
{
	if (STATE_GROUND == m_eState)
	{
		/*
		GOLD_0 : 2
		GOLD_1 : 4
		GOLD_2 : 6
		GOLD_3 : 8
		*/
		CMonster* pMonster = dynamic_cast<CMonster*>(pObj);
		if (pMonster)
			return;

		CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
		m_iTextureIdx = (m_iTextureIdx + 1) * 2;
		pPlayer->Set_GoldPlus(m_iTextureIdx);

		m_eState = STATE_INV;
		m_pColliderCom->Set_Free(true);
	}
}