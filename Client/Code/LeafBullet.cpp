#include "stdafx.h"
#include "..\Header\LeafBullet.h"

#include "Export_Function.h"	
#include "BulletMgr.h"

CLeafBullet::CLeafBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
	, m_bReady(false)
{
}

CLeafBullet::CLeafBullet(const CLeafBullet & rhs)
	:CBullet(rhs)
{
}

CLeafBullet::~CLeafBullet()
{
}

HRESULT CLeafBullet::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.iAttack = 1;
	m_pColliderCom->Set_Free(true);
	return S_OK;
}

HRESULT CLeafBullet::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	// 버퍼 컴포넌트
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	// 월드행렬 컴포넌트
	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// m_pAnimtorCom
	pComponent = m_pAnimtorCom = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_AnimatorCom"));
	NULL_CHECK_RETURN(m_pAnimtorCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_AnimatorCom", pComponent });

	m_pAnimtorCom->Add_Component(L"Proto_Leaf_Bullet_Texture");

	// Collider Component
	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	return S_OK;
}

_int CLeafBullet::Update_Object(const _float & fTimeDelta)
{
	if (!m_bFire)
		return 0;

	int iResult = CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);
	m_pAnimtorCom->Play_Animation(fTimeDelta);
	m_pColliderCom->Calculate_WorldMatrix(*m_pTransCom->Get_WorldMatrixPointer());

	Target(fTimeDelta);
	
	m_fLifeTime += fTimeDelta;
	return iResult;
}

void CLeafBullet::LateUpdate_Object(void)
{
	Billboard();

	if (!m_bFire)
		return;

	if (3.f < m_fLifeTime)
		Reset();

	CGameObject::LateUpdate_Object();
}

void CLeafBullet::Render_Obejct(void)
{
	if (!m_bFire)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
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

void CLeafBullet::Billboard()
{
	// 빌보드
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

	_vec3 vScale = m_pTransCom->Get_Scale();
	_matrix matScale, matScaleInv;
	D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
	D3DXMatrixInverse(&matScaleInv, 0, &matScale);

	m_matWorld = matBill *matWorld;
	m_pTransCom->Set_WorldMatrix(&m_matWorld);

	//// 빌보드
	//_matrix		matWorld, matView, matBill;
	//D3DXMatrixIdentity(&matBill);

	//m_pTransCom->Get_WorldMatrix(&matWorld);
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	//matBill._11 = matView._11;
	//matBill._13 = matView._13;
	//matBill._31 = matView._31;
	//matBill._33 = matView._33;

	//D3DXMatrixInverse(&matBill, 0, &matBill);

	//m_matWorld = matBill * m_matWorld;
	////m_pTransCom->Set_WorldMatrix(&(matBill * matWorld));
}

_int CLeafBullet::Target(const _float & fTimeDelta)
{
	///// 플레이어 머리를 위쪽을 도는 나뭇잎
	//_vec3	vPlayerPos;
	//CTransform*		pPlayer = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	//NULL_CHECK_RETURN(pPlayer, -1);

	//pPlayer->Get_Info(INFO_POS, &vPlayerPos);
	//m_pTransCom->Set_Pos(vPlayerPos.x, vPlayerPos.y , vPlayerPos.z);

	//_vec3 vDir, vDistance;
	//vDir = { 0.f, 1.f, 0.f };
	////D3DXVec3Normalize(&vDir, &vDir); // 단위 벡터로 변경
	//D3DXVec3Normalize(&vDistance, &vDistance);

	//_matrix matTrans, matRev;

	//// 이
	//D3DXMatrixTranslation(&matTrans, vDistance.x * 1.5f, vDistance.y * -2.f, vDistance.z * 1.5f);

	//// 공
	//m_fAngle += 0.1f;
	//if (m_fAngle > 360.f)
	//	m_fAngle = 0.1f;
	//D3DXMatrixRotationAxis(&matRev, &vDir, m_fAngle);

	//_matrix matParent;
	//pPlayer->Get_WorldMatrix(&matParent);

	//_matrix mat;
	//D3DXMatrixTranslation(&mat, matParent._41, matParent._42, matParent._43);

	//matParent = matRev * mat;

	//_matrix mat1;
	//mat1 = matTrans * matParent; // 공전하는 행렬

	//m_matWorld = mat1;

	// Bullet의 부모가 될 Leaf의 Transform
	// Leaf가 Player를 바라보는 방향을 위해 Leaf, Player
	_vec3	vPos, vPlayerPos;
	CTransform*		pLeaf = static_cast<CTransform*>(m_pTarget->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pLeaf, -1);

	CTransform*		pPlayer = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayer, -1);

	pPlayer->Get_Info(INFO_POS, &vPlayerPos);
	pLeaf->Get_Info(INFO_POS, &vPos);
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z); // Bullet의 시작위치

	_vec3 vDir, vDistance;
	vDir = vPlayerPos - vPos; // 몬스터가 플레이어를 바라보는 방향 벡터
	D3DXVec3Normalize(&vDir, &vDir); // 단위 벡터로 변경
	D3DXVec3Normalize(&vDistance, &vDistance);

	if (!m_bReady) // 처음 들어 왔을 때 한 번만 받는다
	{
		m_vTrans = vPos;
		m_vDir = vDir;
		m_bReady = true;
	}

	_matrix matScale, matRot, matTrans, matRev, matWorld;
	// 스
	D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.5f);

	// 이
	D3DXMatrixTranslation(&matTrans, 0.7f, 0.7f, 0.7f);

	// 공
	m_fAngle += 0.2f;
	if (m_fAngle > 360.f)
		m_fAngle -= 360.f;
		//m_fAngle = 0.1f;
	//m_fAngle = m_fAngle % 360.f;
	D3DXMatrixRotationAxis(&matRev, &vDir, m_fAngle);
	
	//matWorld = matTrans * matRev;

	_matrix matParent;
	pLeaf->Get_WorldMatrix(&matParent);	
	_matrix mat;
	D3DXMatrixTranslation(&mat, matParent._41, matParent._42, matParent._43);

	matParent = matScale * matRev;// *mat;

	_matrix mat1;
	mat1 = matTrans * matParent; // 공전하는 행렬

	//m_pTransCom->Set_WorldMatrix(&matWorld);

	_matrix mat2;

	if (m_bReady)
	{
		m_vTrans += vDir * 0.1f;
		D3DXMatrixTranslation(&mat2, m_vTrans.x, m_vTrans.y, m_vTrans.z);
	}
	m_matWorld = mat1 * mat2;

	m_pTransCom->Set_Pos(m_matWorld._41, m_matWorld._42, m_matWorld._43);
	return 0;
}

CLeafBullet * CLeafBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLeafBullet*		pInstance = new CLeafBullet(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLeafBullet::Free(void)
{
	CGameObject::Free();
}

void CLeafBullet::Reset()
{
	m_bFire = false;
	m_bDead = false;
	m_fLifeTime = 0.f;
	m_bReady = false;
	m_pColliderCom->Set_Free(true);
	CBulletMgr::GetInstance()->Collect_Obj(m_iIndex, BULLET_M_LEAF);
}