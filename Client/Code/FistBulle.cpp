#include "stdafx.h"
#include "..\Header\FistBullet.h"

#include "Export_Function.h"	
#include "BulletMgr.h"
#include "ParticleMgr.h"

CFistBullet::CFistBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
	, m_bReady(false)
	, m_fSpeed(0.f)
{
}

CFistBullet::CFistBullet(const CFistBullet & rhs)
	:CBullet(rhs)
{
}

CFistBullet::~CFistBullet()
{
}

HRESULT CFistBullet::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Scale(0.5f, 0.5f, 0.5f);

	m_fSpeed = 10.f;

	return S_OK;
}

HRESULT CFistBullet::Add_Component(void)
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

	m_pAnimtorCom->Add_Component(L"Proto_FistGreenEffect_Texture");

	return S_OK;
}

_int CFistBullet::Update_Object(const _float & fTimeDelta)
{
	if (!m_bFire)
		return 0;

	int iResult = CGameObject::Update_Object(fTimeDelta);
	m_pAnimtorCom->Play_Animation(fTimeDelta);

	Target(fTimeDelta);

	Add_RenderGroup(RENDER_ALPHA, this);
	
	m_fParticleTime += fTimeDelta;
	m_fLifeTime += fTimeDelta;
	return iResult;
}

void CFistBullet::LateUpdate_Object(void)
{
	if (!m_bFire)
		return;

	if (1.f < m_fLifeTime)
	{
		CParticleMgr::GetInstance()->Set_Info(this, 
			20, 
			0.3f, 
			{ 1.f, 1.f, 1.f },
			1.f, 
			{ 0.f,1.f,0.f,1.f });
		CParticleMgr::GetInstance()->Call_Particle(PTYPE_FIREWORK, TEXTURE_5);
		Reset();
	}

	Billboard();

	CGameObject::LateUpdate_Object();
}

void CFistBullet::Render_Obejct(void)
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
}

void CFistBullet::Billboard()
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
	m_pTransCom->Set_WorldMatrix(&(matBill * matWorld));
}

_int CFistBullet::Target(const _float & fTimeDelta)
{
	if (!m_bReady)
	{
		CTransform*		pFist = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Fist", L"Proto_TransformCom", ID_DYNAMIC));
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
		CParticleMgr::GetInstance()->Set_Info(this, 3, 0.1f,
			_vec3({ 1.f, 1.f, 1.f }), 1.f, D3DXCOLOR{ 0.f, 1.f, 0.f, 1.f });
		CParticleMgr::GetInstance()->Call_Particle(PTYPE_FOUNTAIN, TEXTURE_5);
		m_fParticleTime = 0.f;
	}

	return 0;
}

CFistBullet * CFistBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFistBullet*		pInstance = new CFistBullet(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFistBullet::Free(void)
{
	CGameObject::Free();
}

void CFistBullet::Reset()
{
	m_bFire = false;
	m_bDead = false;
	m_fLifeTime = 0.f;
	m_bReady = false;
	CBulletMgr::GetInstance()->Collect_Obj(m_iIndex, BULLET_M_FIST);
}