#include "stdafx.h"
#include "..\Header\SongBossBullet.h"

#include "Export_Function.h"	
#include "BulletMgr.h"
#include "ParticleMgr.h"

CSongBossBullet::CSongBossBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
	, m_fSpeed(0.f)
{
}

CSongBossBullet::CSongBossBullet(const CSongBossBullet & rhs)
	:CBullet(rhs)
{
}

CSongBossBullet::~CSongBossBullet()
{
}

HRESULT CSongBossBullet::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.iAttack = 3;
	m_fSpeed = 20.f;

	m_pTransCom->Set_Scale(0.5f, 0.5f, 0.5f);
	
	return S_OK;
}

HRESULT CSongBossBullet::Add_Component(void)
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

	m_pAnimtorCom->Add_Component(L"Proto_MusicNote_Bullet_Texture");

	// Collider Component
	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	return S_OK;
}

_int CSongBossBullet::Update_Object(const _float & fTimeDelta)
{
	if (!m_bFire)
		return 0;

	int iResult = CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);
	m_pAnimtorCom->Play_Animation(fTimeDelta);
	m_pColliderCom->Calculate_WorldMatrix(*m_pTransCom->Get_WorldMatrixPointer());

	Target(fTimeDelta);

	if (!m_bOneCheck)
	{
		//CParticleMgr::GetInstance()->Set_Info(this, 30, 0.5f,
		//	_vec3({ 1.f, 1.f, 1.f }), 3.f, D3DXCOLOR{ 0.1f, 0.1f, 0.1f, 1.f },
		//	1.f, false, true);
		//CParticleMgr::GetInstance()->Call_Particle(PTYPE_FOUNTAIN, TEXTURE_8);



		m_bOneCheck = true;
	}

	//if (!m_bReady)
	//{
	//	CTransform*		pFist = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"SongBoss", L"Proto_TransformCom", ID_DYNAMIC));
	//	NULL_CHECK_RETURN(pFist, -1);

	//	CTransform*		pPlayer = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	//	NULL_CHECK_RETURN(pPlayer, -1);

	//	pFist->Get_Info(INFO_POS, &vParentPos);
	//	m_pTransCom->Set_Pos(vParentPos.x, vParentPos.y, vParentPos.z);

	//	pPlayer->Get_Info(INFO_POS, &m_vPlayerPos);
	//	m_vPlayerPos.y -= 0.01f;

	//	m_bReady = true;
	//}

	//_vec3	vDir;
	//vDir = m_vPlayerPos - vParentPos;
	//D3DXVec3Normalize(&vDir, &vDir);
	//vDir *= m_fSpeed * fTimeDelta;

	//m_pTransCom->Move_Pos(&vDir);

	m_fLifeTime += fTimeDelta;
	return iResult;
}

void CSongBossBullet::LateUpdate_Object(void)
{
	Billboard();

	if (!m_bFire)
		return;

	if (3.f < m_fLifeTime)
		Reset();

	CGameObject::LateUpdate_Object();
}

void CSongBossBullet::Render_Obejct(void)
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

void CSongBossBullet::Billboard()
{
	// 빌보드
	_matrix		matWorld, matView, matBill;
	D3DXMatrixIdentity(&matBill);

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	// 현재 지금 이 코드는 문제가 없지만 나중에 문제가 될 수 있음
	m_matWorld = matBill * m_matWorld;
	//m_pTransCom->Set_WorldMatrix(&(matBill * matWorld));
}

_int CSongBossBullet::Target(const _float & fTimeDelta)
{
	// Bullet의 부모가 될 Leaf의 Transform
	// Leaf가 Player를 바라보는 방향을 위해 Leaf, Player
	_vec3	vParentPos, vPlayerPos;
	CTransform*		pParent = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"SongBoss", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pParent, -1);

	CTransform*		pPlayer = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayer, -1);

	pPlayer->Get_Info(INFO_POS, &vPlayerPos);
	pParent->Get_Info(INFO_POS, &vParentPos);
	m_pTransCom->Set_Pos(vParentPos.x, vParentPos.y + 2.f, vParentPos.z); // Bullet의 시작위치

	_vec3 vDir, vDistance;
	vDir = vPlayerPos - vParentPos; // 몬스터가 플레이어를 바라보는 방향 벡터
	D3DXVec3Normalize(&vDir, &vDir); // 단위 벡터로 변경
	D3DXVec3Normalize(&vDistance, &vDistance);

	if (!m_bReady) // 처음 들어 왔을 때 한 번만 받는다
	{
		m_vTrans = vParentPos;
		m_bReady = true;
	}

	_matrix matScale, matRot, matTrans, matRev, matWorld;
	// 스
	D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.5f);

	// 이
	D3DXMatrixTranslation(&matTrans, vDistance.x * 0.7f, vDistance.y * 0.7f, vDistance.z * 0.7f);

	// 공
	m_fAngle += 0.1f;
	if (m_fAngle > 360.f)
		m_fAngle -= 360.f;
	//m_fAngle = 0.1f;
	//m_fAngle = m_fAngle % 360.f;
	D3DXMatrixRotationAxis(&matRev, &vDir, m_fAngle);

	//matWorld = matTrans * matRev;

	_matrix matParent;
	pParent->Get_WorldMatrix(&matParent);
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

CSongBossBullet * CSongBossBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSongBossBullet*		pInstance = new CSongBossBullet(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSongBossBullet::Free(void)
{
	CGameObject::Free();
}

void CSongBossBullet::Reset()
{
	m_bFire = false;
	m_bDead = false;
	m_fLifeTime = 0.f;
	m_fFrame = 0.f;
	m_bReady = false;
	m_bOneCheck = false;
	m_pColliderCom->Set_Free(false);
	CBulletMgr::GetInstance()->Collect_Obj(m_iIndex, BULLET_SONGBOSS);
}