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

	m_pTransCom->Set_Scale(1.f, 1.f, 1.f);
	
	return S_OK;
}

HRESULT CSongBossBullet::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pAnimtorCom = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_AnimatorCom"));
	NULL_CHECK_RETURN(m_pAnimtorCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_AnimatorCom", pComponent });

	m_pAnimtorCom->Add_Component(L"Proto_MusicNote_Bullet_Texture");

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
		CParticleMgr::GetInstance()->Set_Info(this, 30, 0.5f,
			_vec3({ 1.f, 1.f, 1.f }), 3.f, D3DXCOLOR{ 1.f, 1.f, 1.f, 1.f },
			1.f, false, true);
		CParticleMgr::GetInstance()->Call_Particle(PTYPE_FOUNTAIN, TEXTURE_8);

		m_bOneCheck = true;
	}

	m_fLifeTime += fTimeDelta;
	return iResult;
}

void CSongBossBullet::LateUpdate_Object(void)
{
	Billboard();

	if (!m_bFire)
		return;

	if (3.f < m_fLifeTime)
	{
		CParticleMgr::GetInstance()->Set_Info(this, 30, 0.5f,
			_vec3({ 1.f, 1.f, 1.f }), 3.f, D3DXCOLOR{ 1.f, 1.f, 1.f, 1.f },
			1.f, false, true);
		CParticleMgr::GetInstance()->Call_Particle(PTYPE_FOUNTAIN, TEXTURE_8);

		Reset();
	}

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
}

_int CSongBossBullet::Target(const _float & fTimeDelta)
{
	_vec3	vParentPos, vPlayerPos;
	CTransform*		pParent = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"SongBoss", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pParent, -1);

	CTransform*		pPlayer = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayer, -1);

	pPlayer->Get_Info(INFO_POS, &vPlayerPos);
	pParent->Get_Info(INFO_POS, &vParentPos);
	m_pTransCom->Set_Pos(vParentPos.x, vParentPos.y + 2.f, vParentPos.z); 

	_vec3 vDir, vDistance;
	vDir = vPlayerPos - vParentPos; 
	D3DXVec3Normalize(&vDir, &vDir); 
	D3DXVec3Normalize(&vDistance, &vDistance);

	if (!m_bReady)
	{
		m_vTrans = vParentPos;
		m_bReady = true;
	}

	_matrix matScale, matRot, matTrans, matRev, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	D3DXMatrixTranslation(&matTrans, vDistance.x * 0.7f, vDistance.y * 0.7f, vDistance.z * 0.7f);

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
	mat1 = matTrans * matParent; 

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
	m_pColliderCom->Set_Free(true);
	CBulletMgr::GetInstance()->Collect_Obj(m_iIndex, BULLET_SONGBOSS);
}