#include "stdafx.h"
#include "Shield.h"
#include "Export_Function.h"

CShield::CShield(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	:CEquipment(pGraphicDev)
{
	D3DXMatrixIdentity(&m_matWorld);
	m_ObjTag = L"Shield";
	m_vPos = vPos;
	m_eItemType = ITEM_SHIELD;

}

CShield::~CShield()
{
}

HRESULT CShield::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	m_eState = STATE_GROUND;
	m_tInfo.iDef = 5;
	return S_OK;
}

_int CShield::Update_Object(const _float & fTimeDelta)
{

	int iResult = CItem::Update_Object(fTimeDelta);

	if (!m_bReady)
	{
		m_pCenter = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
		NULL_CHECK_RETURN(m_pCenter, -1);
		m_bReady = true;
	}

	_vec3* pPlayerInfo = m_pCenter->Get_InfoAll();

	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	switch (m_eState)
	{
	case STATE_GROUND:
		m_pTransCom->Set_Scale(0.6f, 0.6f, 0.6f);
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


		if (!m_bAttack)
		{
			m_pColliderCom->Set_Free(true);
			m_pTransCom->Item_LeftMotion(m_pGraphicDev, *m_pCenter->Get_WorldMatrixPointer());
		}
		break;
	}



	m_fTimeDelta = fTimeDelta;
	m_pColliderCom->Calculate_WorldMatrix(*m_pTransCom->Get_WorldMatrixPointer());



	return iResult;
}

void CShield::LateUpdate_Object(void)
{
	if (STATE_INV != m_eState)
		Add_RenderGroup(RENDER_ALPHA, this);

	// 0(ground) 1(inv) 2(equip) 3(quick)

	CGameObject::LateUpdate_Object();
}

void CShield::Render_Obejct(void)
{		


	if (m_eState == STATE_INV)
		return;
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																								
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

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

CShield * CShield::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CShield*		pInstance = new CShield(pGraphicDev, vPos);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CShield::Free(void)
{
	CGameObject::Free();

}

void CShield::CollisionEvent(CGameObject * pObj)
{
	//if (STATE_GROUND == m_eState)
	//{
	//	CMonster* pMonster = dynamic_cast<CMonster*>(pObj);
	//	if (pMonster)
	//		return;

	//	m_eState = STATE_INV;
	//	m_pColliderCom->Set_Free(true);
	//}
}

HRESULT CShield::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	// ���� ������Ʈ
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	// �ؽ��� �İ�ü ������Ʈ
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Shield_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Shield_Texture", pComponent });
	m_textureTag = L"Proto_Shield_Texture";

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

void CShield::Charge(const _float & fTimeDelta)
{
	if (Engine::Key_Down(DIK_P))
	{
		if (!m_bAttack)
			m_bAttack = true;

		m_pColliderCom->Set_Free(false);
		m_pTransCom->Prepare_Attack();

	}
}

void CShield::Attack(const _float & fTimeDelta)
{
	if (!m_bAttack)
		return;

	if (m_pTransCom->Item_Defense(m_pGraphicDev, *m_pCenter->Get_WorldMatrixPointer()))
	{
		m_bAttack = false;
		m_pColliderCom->Set_Free(true);
	}

}
