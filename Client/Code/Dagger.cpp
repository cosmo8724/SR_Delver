#include "stdafx.h"
#include "Dagger.h"
#include "Export_Function.h"

CDagger::CDagger(LPDIRECT3DDEVICE9 pGraphicDev)
	:CWeapon(pGraphicDev)
{
	m_ObjTag = L"Dagger";
}

CDagger::CDagger(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	:CWeapon(pGraphicDev)
{
	m_vPos = vPos;
	m_ObjTag = L"Dagger";
	m_eItemType = ITEM_WEAPON;
	m_eWeaponType = WT_MELEE;
}

CDagger::~CDagger()
{
}

HRESULT CDagger::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);
	m_eState = STATE_GROUND;
	return S_OK;
}

_int CDagger::Update_Object(const _float & fTimeDelta)
{
	if (STATE_INV == m_eState)
		return 0;

	int iResult = CWeapon::Update_Object(fTimeDelta);

	if (!m_bReady)
	{
		m_pCamera = dynamic_cast<CCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"StaticCamera"));
		if (nullptr == m_pCamera)
			return 0;
		else
		{
			m_pCenter = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
			if (nullptr == m_pCenter)
				return 0;
			else m_bReady = true;
		}
	}


	_vec3* pPlayerInfo = m_pCenter->Get_InfoAll();

	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	switch (m_eState)
	{
	case STATE_GROUND:
		m_pTransCom->Set_Scale(1.f, 1.f, 1.f);
		m_pTransCom->Revolution(pPlayerInfo, matView, 0.f, m_fTimeDelta, STATE_GROUND);
		//m_pTransCom->Move_Pos(&_vec3({ 0.005f, 0.005f, 0.005f }));
		break;
	case STATE_EQUIP:
		m_pTransCom->Set_Scale(0.3f, 0.3f, 0.3f);


		if (!(Engine::Get_DIKeyState(DIK_TAB) & 0x80))
		{
			Charge(fTimeDelta);
			Attack(fTimeDelta);
		}

		//m_pTransCom->Revolution(pPlayerInfo, matView, 45.f, m_fTimeDelta, STATE_EQUIP);

		if(!m_bAttack)
			m_pTransCom->Item_Motion(m_pGraphicDev, *m_pCenter->Get_WorldMatrixPointer());
		break;
	}

	Add_RenderGroup(RENDER_ALPHA, this);

	m_fTimeDelta = fTimeDelta;

	m_pColliderCom->Calculate_WorldMatrix(*m_pTransCom->Get_WorldMatrixPointer());



	return iResult;

}

void CDagger::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();

	if (STATE_INV == m_eState)
		return;

}

void CDagger::Render_Obejct(void)
{
	if (m_eState == STATE_INV)
		return;


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->Set_Texture(_ulong(m_fFrame));

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

CDagger * CDagger::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CDagger*		pInstance = new CDagger(pGraphicDev, vPos);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDagger::Free(void)
{
	CGameObject::Free();
}

void CDagger::CollisionEvent(CGameObject * pObj)
{
	if (STATE_GROUND == m_eState)
	{
		m_eState = STATE_INV;
		m_pColliderCom->Set_Free(true);
	}
}

HRESULT CDagger::Add_Component(void)
{

	CComponent*		pComponent = nullptr;

	// 버퍼 컴포넌트
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	// 텍스쳐 컴객체 컴포넌트
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Dagger_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Dagger_Texture", pComponent });
	m_textureTag = L"Proto_Dagger_Texture";

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

void CDagger::Charge(const _float & fTimeDelta)
{
	// 차지는 일단 차후
	m_bAttack = true;
}

void CDagger::Attack(const _float & fTimeDelta)
{
	if (!m_bAttack)
		return;

	m_fAttackTime += fTimeDelta;

	m_pTransCom->Item_Motion(m_pGraphicDev, *m_pCenter->Get_WorldMatrixPointer());

}
