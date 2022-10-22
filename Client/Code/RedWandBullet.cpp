#include "stdafx.h"
#include "..\Header\RedWandBullet.h"

#include "Export_Function.h"	
#include "BulletMgr.h"
#include "ParticleMgr.h"

CRedWandBullet::CRedWandBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
	, m_fSpeed(20.f)
{
	//m_pGraphicDev->AddRef();
	memset(&m_bdBox, 0, sizeof(BDBOX));
	memset(&m_bdSphere, 0, sizeof(BDSPHERE));
}

CRedWandBullet::CRedWandBullet(const CRedWandBullet & rhs)
	:CBullet(rhs)
	, m_fSpeed(rhs.m_fSpeed)
{
	//m_pGraphicDev->AddRef();
	memcpy(&m_bdBox, &rhs.m_bdBox, sizeof(BDBOX));
	memcpy(&m_bdSphere, &rhs.m_bdSphere, sizeof(BDSPHERE));
}

CRedWandBullet::~CRedWandBullet()
{
}

HRESULT CRedWandBullet::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	float fSize = 0.05f;
	m_pTransCom->Set_Scale(fSize, fSize, fSize);

	// 충돌처리
	_vec3 vPos, vScale;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	vScale = m_pTransCom->Get_Scale();
	m_bdBox.vMin = { vPos.x - vScale.x, vPos.y - vScale.y, vPos.z - vScale.z };
	m_bdBox.vMax = { vPos.x + vScale.x, vPos.y + vScale.y, vPos.z + vScale.z };


	return S_OK;
}

HRESULT CRedWandBullet::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	// 버퍼 컴포넌트
	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Clone_Proto(L"Proto_CubeTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTexCom", pComponent });

	// 텍스쳐 컴객체 컴포넌트
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_RedBullet_CubeTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RedBullet_CubeTexture", pComponent });

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

void CRedWandBullet::CollisionEvent(CGameObject * pObj)
{
	CParticleMgr::GetInstance()->Set_Info(this, 1, 0.5f, { 1.f, 1.f, 1.f },
		3.f, { 1.f, 0.f, 0.f, 1.f });
	CParticleMgr::GetInstance()->Call_Particle(PTYPE_FOUNTAIN, TEXTURE_13);

	m_bCollision = true;
	//Reset();
}

_int CRedWandBullet::Update_Object(const _float & fTimeDelta)
{
	if (!m_bFire)
		return 0;

	if (!m_bReady)
	{
		CTransform*		pPlayer = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
		NULL_CHECK_RETURN(pPlayer, -1);

		_vec3 vPos, vRight, vUp, vLook;
		pPlayer->Get_Info(INFO_POS, &vPos);
		pPlayer->Get_Info(INFO_RIGHT, &vRight);
		pPlayer->Get_Info(INFO_UP, &vUp);
		pPlayer->Get_Info(INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vRight, &vRight);
		D3DXVec3Normalize(&vUp, &vUp);
		D3DXVec3Normalize(&vLook, &vLook);

		_vec3 vTrans = vPos + 0.25f * vRight + 0.8f * vLook;

		m_pTransCom->Set_Pos(vTrans.x, vTrans.y, vTrans.z);

		pPlayer->Get_Info(INFO_LOOK, &m_vDirection);
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);

		m_bReady = true;
	}

	int iResult = CGameObject::Update_Object(fTimeDelta);

	m_fLifeTime += fTimeDelta;
	m_fParticleTime += fTimeDelta;

	// Cube Scale
	if (m_bCollision)
	{
		m_fDestroyTime += fTimeDelta;
		m_fScaleDelta += 0.01f;
		m_pTransCom->Set_Scale(m_fScaleDelta, m_fScaleDelta, m_fScaleDelta);

		Add_RenderGroup(RENDER_ALPHA, this);
		return;
	}

	m_fSpeed = 10.f;
	m_pTransCom->Move_Pos(&(m_fSpeed * fTimeDelta * m_vDirection));
	if (0.1f < m_fParticleTime)
	{
		CParticleMgr::GetInstance()->Set_Info(this, 2, 0.1f,
			_vec3({ 1.f, 1.f, 1.f }), 1.f, D3DXCOLOR{ 1.f, 0.f, 0.f, 1.f }, 1.f);
		CParticleMgr::GetInstance()->Call_Particle(PTYPE_TRACER, TEXTURE_13);
		m_fParticleTime = 0.f;
	}

	Add_RenderGroup(RENDER_ALPHA, this);

	m_pColliderCom->Calculate_WorldMatrix(*m_pTransCom->Get_WorldMatrixPointer());
	
	return iResult;
}

void CRedWandBullet::LateUpdate_Object(void)
{
	if (!m_bFire || !m_bReady)
		return;

	// 아무데도 충돌안해도 일정 시간 후 리셋
	if (1.f < m_fLifeTime && !m_bCollision)
	{
		CParticleMgr::GetInstance()->Set_Info(this, 10, 0.3f, { 1.f, 1.f, 1.f },
			1.f, { 1.f, 0.f, 0.f, 1.f });
		CParticleMgr::GetInstance()->Call_Particle(PTYPE_FIREWORK, TEXTURE_13);
		Reset();
	}

	if (m_bCollision && 1.f < m_fDestroyTime)
	{
		Reset();
	}


	// 충돌처리 테스트
	_vec3 vPos, vScale;
	_matrix matWorld;
	m_pTransCom->Get_WorldMatrix(&matWorld);

	m_pTransCom->Get_Info(INFO_POS, &vPos);
	vScale = m_pTransCom->Get_Scale();
	m_bdBox.vMin = { vPos.x - vScale.x, vPos.y - vScale.y, vPos.z - vScale.z };
	m_bdBox.vMax = { vPos.x + vScale.x, vPos.y + vScale.y, vPos.z + vScale.z };

	//D3DXVec3TransformCoord(&m_bdBox.vMin, &m_bdBox.vMin, &matWorld);
	//D3DXVec3TransformCoord(&m_bdBox.vMax, &m_bdBox.vMax, &matWorld);


	CGameObject::LateUpdate_Object();
}

void CRedWandBullet::Render_Obejct(void)
{
	if (!m_bFire || !m_bReady)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(40, 255, 255, 255));
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	/*m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000001);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);*/

	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pTextureCom->Set_Texture(FtoDw(m_fFrame));

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);


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

CRedWandBullet * CRedWandBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRedWandBullet*		pInstance = new CRedWandBullet(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CRedWandBullet::Free(void)
{
	CGameObject::Free();
	//Safe_Release(m_pGraphicDev);
}

void CRedWandBullet::Reset()
{
	m_bFire = false;
	m_bDead = false;
	m_fLifeTime = 0.f;
	m_fFrame = 0.f;
	m_bReady = false;
	m_fScaleDelta = 0.5f;

	m_bCollision = false;
	m_fDestroyTime = 0.f;
	m_fScaleDelta = 0.5f;

	m_pColliderCom->Set_Free(true);
	m_pTransCom->Set_Pos(-1000.f, -1000.f, -1000.f);
	CBulletMgr::GetInstance()->Collect_Obj(m_iIndex, BULLET_REDWAND);
}

