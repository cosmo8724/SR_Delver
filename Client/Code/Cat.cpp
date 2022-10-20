#include "stdafx.h"
#include "..\Header\Cat.h"

#include "Export_Function.h"

#include "Player.h"

CCat::CCat(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
{
}

CCat::CCat(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	: CGameObject(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
{
	m_vPos = vPos;
}


CCat::~CCat()
{
}

HRESULT CCat::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tHp = 10;
	m_tAttack = 1;
	m_fSpeed = 5.f;

	m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	m_eCurState = IDLE;

	return S_OK;
}

_int CCat::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_NONALPHA, this);

	m_pAnimtorCom->Play_Animation(fTimeDelta * 0.5);
	Motion_Change();

	//CPlayer*	pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));	
	
	//// 플레이어가 이동하면 따라서 이동하기
	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	_vec3 vPlayerRight, vPlayerUp, vPlayerLook, vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_RIGHT, &vPlayerRight);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
	pPlayerTransformCom->Get_Info(INFO_UP, &vPlayerUp);
	pPlayerTransformCom->Get_Info(INFO_LOOK, &vPlayerLook);
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);
	D3DXVec3Normalize(&vPlayerUp, &vPlayerUp);
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);

	_vec3 vTrans = vPlayerPos + 1.f * vPlayerRight + 0.8f * vPlayerLook;
	m_pTransCom->Set_Pos(vTrans.x, vTrans.y, vTrans.z);

	_matrix		matRight;
	pPlayerTransformCom->Get_Info(INFO_LOOK, &m_vDirection);
	D3DXMatrixRotationAxis(&matRight, &vPlayerLook, D3DXToRadian(120.f));
	D3DXVec3TransformNormal(&m_vDirection, &vPlayerLook, &matRight);

	m_fSpeed = 10.f;
	m_pTransCom->Move_Pos(&(m_fSpeed * fTimeDelta * m_vDirection));

	//// 부모에 자식 곱해주기
	//_matrix matPlayerWorld, matMWorld, matWorld;
	//pPlayerTransformCom->Get_WorldMatrix(&matPlayerWorld);
	//m_pTransCom->Get_WorldMatrix(&matMWorld);

	//matWorld = matPlayerWorld * matMWorld;
	//m_pTransCom->Set_WorldMatrix(&matWorld);









	//_vec3	vPlayerPos;
	//CTransform*		pPlayer = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	//NULL_CHECK(pPlayer);

	//pPlayer->Get_Info(INFO_POS, &vPlayerPos);
	////m_pTransCom->Set_Pos(vPlayerPos.x, vPlayerPos.y, vPlayerPos.z);

	//_vec3 vDir, vDistance;
	//vDir = { 0.f, 1.f, 0.f };
	////D3DXVec3Normalize(&vDir, &vDir);
	//D3DXVec3Normalize(&vDistance, &vDistance);

	//_matrix matTrans, matRev;

	//// ��
	//D3DXMatrixTranslation(&matTrans, vDistance.x * 5.f, vDistance.y, vDistance.z * 2.f);

	//// ��
	//m_fAngle += 0.03f;
	//if (m_fAngle > 360.f)
	//	m_fAngle = 0.03f;
	//D3DXMatrixRotationAxis(&matRev, &vDir, m_fAngle);

	//_matrix matParent;
	//pPlayer->Get_WorldMatrix(&matParent);

	//_matrix mat;
	//D3DXMatrixTranslation(&mat, matParent._41, matParent._42, matParent._43);

	//matParent = matRev * mat;

	//_matrix mat1;
	//mat1 = matTrans * matParent;

	//m_matWorld = mat1;
	//m_pTransCom->Set_Pos(m_matWorld._41, m_matWorld._42, m_matWorld._43);








	// 일정시간 플레이어가 이동하지 않으면 IDlE 상태
	// 플레이어가 몬스터가 일정 범위 안 으로 들어오면 몬스터 공격
	// 데미지 입었을 때 Hit
	// 체력이 0이되면 Die





	return 0;
}

void CCat::LateUpdate_Object(void)
{
	Billboard();
	Engine::CGameObject::LateUpdate_Object();
}

void CCat::Render_Obejct(void)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pAnimtorCom->Set_Texture();
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

HRESULT CCat::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pAnimtorCom = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_AnimatorCom"));
	NULL_CHECK_RETURN(m_pAnimtorCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_AnimatorCom", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	m_pAnimtorCom->Add_Component(L"Proto_CatIDLE_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_CatWALKING_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_CatHILL_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_CatATTACK_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_CatHIT_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_CatDIE_Texture");

	return S_OK;
}

void CCat::Billboard()
{
	_matrix		matWorld, matView, matBill;
	D3DXMatrixIdentity(&matBill);

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);


	if (matView._21 > 0.f)
	{
		matBill = m_matOldBill;
	}
	else
	{
		D3DXMatrixIdentity(&matBill);
		matBill._11 = matView._11;
		matBill._13 = matView._13;
		matBill._31 = matView._31;
		matBill._33 = matView._33;

		m_matOldBill = matBill;
	}

	D3DXMatrixInverse(&matBill, 0, &matBill);

	m_pTransCom->Set_WorldMatrix(&(matBill * matWorld));
}

void CCat::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_pAnimtorCom->Change_Animation(L"Proto_CatIDLE_Texture");
			break;

		case WALKING:
			m_pAnimtorCom->Change_Animation(L"Proto_CatWALKING_Texture");
			break;

		case HILL:
			m_pAnimtorCom->Change_Animation(L"Proto_CatHILL_Texture");
			break;

		case ATTACK:
			m_pAnimtorCom->Change_Animation(L"Proto_CatATTACK_Texture");
			break;

		case HIT:
			m_pAnimtorCom->Change_Animation(L"Proto_CatHIT_Texture");
			break;

		case DIE:
			m_pAnimtorCom->Change_Animation(L"Proto_CatDIE_Texture");
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CCat::CollisionEvent(CGameObject * pObj)
{
}

CCat * CCat::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CCat *	pInstance = new CCat(pGraphicDev, vPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCat::Free(void)
{
	CGameObject::Free();
}
