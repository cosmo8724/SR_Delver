#include "stdafx.h"
#include "..\Header\GreenSpiderBullet.h"

#include "Export_Function.h"	
#include "BulletMgr.h"
#include "ParticleMgr.h"
#include "Player.h"
#include "SpiderBackGround.h"


CGreenSpiderBullet::CGreenSpiderBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
	, m_bReady(false)
	, m_fSpeed(0.f)
{
}

CGreenSpiderBullet::CGreenSpiderBullet(const CGreenSpiderBullet & rhs)
	:CBullet(rhs)
{
}

CGreenSpiderBullet::~CGreenSpiderBullet()
{
}

HRESULT CGreenSpiderBullet::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.iAttack = 1;
	m_fSpeed = 10.f;

	m_pTransCom->Set_Scale(0.3f, 0.3f, 0.3f);

	return S_OK;
}

HRESULT CGreenSpiderBullet::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	// ���� ������Ʈ
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	// ������� ������Ʈ
	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// m_pAnimtorCom
	pComponent = m_pAnimtorCom = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_AnimatorCom"));
	NULL_CHECK_RETURN(m_pAnimtorCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_AnimatorCom", pComponent });

	m_pAnimtorCom->Add_Component(L"Proto_GreenSpider_Bullet_Texture");

	// Collider Component
	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	return S_OK;
}

_int CGreenSpiderBullet::Update_Object(const _float & fTimeDelta)
{
	if (!m_bFire)
		return 0;

	int iResult = CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);
	m_pAnimtorCom->Play_Animation(fTimeDelta);
	m_pColliderCom->Calculate_WorldMatrix(*m_pTransCom->Get_WorldMatrixPointer());

	Target(fTimeDelta);
	
	m_fParticleTime += fTimeDelta;
	m_fLifeTime += fTimeDelta;
	return iResult;
}

void CGreenSpiderBullet::LateUpdate_Object(void)
{
	Billboard();

	if (!m_bFire)
		return;

	if (1.f < m_fLifeTime)
	{
		CParticleMgr::GetInstance()->Set_Info(this, 3, 0.05f,
			_vec3({ 1.f, 1.f, 1.f }), 1.f, D3DXCOLOR{ 1.f, 1.f, 1.f, 1.f });
		CParticleMgr::GetInstance()->Call_Particle(PTYPE_FOUNTAIN, TEXTURE_5); 
		
		Reset();
	}

	CGameObject::LateUpdate_Object();
}

void CGreenSpiderBullet::Render_Obejct(void)
{
	if (!m_bFire)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
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

void CGreenSpiderBullet::Billboard()
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
}

_int CGreenSpiderBullet::Target(const _float & fTimeDelta)
{
	if (!m_bReady)
	{
		CTransform*		pFist = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"GreenSpider_0", L"Proto_TransformCom", ID_DYNAMIC));
		NULL_CHECK_RETURN(pFist, -1);

		CTransform*		pPlayer = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
		NULL_CHECK_RETURN(pPlayer, -1);

		pFist->Get_Info(INFO_POS, &vPos);
		m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);

		pPlayer->Get_Info(INFO_POS, &m_vPlayerPos);
		m_vPlayerPos.y -= 0.01f;

		m_bReady = true;
	}

	_vec3	vDir;
	vDir = m_vPlayerPos - vPos;
	D3DXVec3Normalize(&vDir, &vDir);
	vDir *= m_fSpeed * fTimeDelta;
	m_pTransCom->Move_Pos(&vDir);

	if (0.1f < m_fParticleTime)
	{
		CParticleMgr::GetInstance()->Set_Info(this, 3, 0.05f,
			_vec3({ 1.f, 1.f, 1.f }), 1.f, D3DXCOLOR{ 1.f, 1.f, 1.f, 1.f });
		CParticleMgr::GetInstance()->Call_Particle(PTYPE_FOUNTAIN, TEXTURE_5);
		m_fParticleTime = 0.f;
	}

	return 0;
}

void CGreenSpiderBullet::CollisionEvent(CGameObject* pObj)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	CSpiderBackGround* pSpiderBackGround = dynamic_cast<CSpiderBackGround*>(Engine::Get_GameObject(L"Layer_UI", L"UI_SpiderBackGround"));
	if (pPlayer == pObj)
	{
		pPlayer->Set_Slow();
		pSpiderBackGround->Set_SpiderRender();
	}
}

CGreenSpiderBullet * CGreenSpiderBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGreenSpiderBullet*		pInstance = new CGreenSpiderBullet(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGreenSpiderBullet::Free(void)
{
	CGameObject::Free();
}

void CGreenSpiderBullet::Reset()
{
	m_bFire = false;
	m_bDead = false;
	m_fLifeTime = 0.f;
	m_bReady = false;
	m_pColliderCom->Set_Free(true);
	CBulletMgr::GetInstance()->Collect_Obj(m_iIndex, BULLET_M_SPIDER);
}