#include "stdafx.h"
#include "..\Header\SongBossStun.h"

#include "Export_Function.h"	
#include "BulletMgr.h"
#include "ParticleMgr.h"
#include "SongBoss.h"

CSongBossStun::CSongBossStun(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
	, m_fShakeSpeed(0.f)
{
}

CSongBossStun::CSongBossStun(const CSongBossStun & rhs)
	:CBullet(rhs)
{
}

CSongBossStun::~CSongBossStun()
{
}

HRESULT CSongBossStun::Ready_Object(_int iBulletCount)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Scale(0.2f, 0.2f, 0.2f);
	m_fShakeSpeed = 2.f;
	m_pColliderCom->Set_Free(true);
	m_iBulletCount = iBulletCount;
	return S_OK;
}

HRESULT CSongBossStun::Add_Component(void)
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

	m_pAnimtorCom->Add_Component(L"Proto_MusicNote_Stun_Texture");

	// Collider Component
	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	return S_OK;
}

_int CSongBossStun::Update_Object(const _float & fTimeDelta)
{
	if (!m_bFire)
		return 0;

	int iResult = CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);
	m_pAnimtorCom->Play_Animation(fTimeDelta);
	m_pColliderCom->Calculate_WorldMatrix(*m_pTransCom->Get_WorldMatrixPointer());

	MusicNoteCreatePos();

	m_fLifeTime += fTimeDelta;
	MusicNoteShake(fTimeDelta);
	return iResult;
}

void CSongBossStun::MusicNoteCreatePos()
{
	// 플레이어 주변으로 생기는 음표
	CTransform*	pPlayer = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayer);
	pPlayer->Get_Info(INFO_POS, &m_vPlayerPos); // 플레이어의 좌표를 받아와서

	_float fNotePos = 0.5f;

	if (m_iBulletCount == 0)
		m_pTransCom->Set_Pos(m_vPlayerPos.x + fNotePos, m_vPlayerPos.y - 0.03f, m_vPlayerPos.z + fNotePos);
	else if (m_iBulletCount == 1)
		m_pTransCom->Set_Pos(m_vPlayerPos.x + fNotePos, m_vPlayerPos.y - 0.03f, m_vPlayerPos.z - fNotePos);
	else if (m_iBulletCount == 2)
		m_pTransCom->Set_Pos(m_vPlayerPos.x - fNotePos, m_vPlayerPos.y - 0.03f, m_vPlayerPos.z - fNotePos);
	else if (m_iBulletCount == 3)
		m_pTransCom->Set_Pos(m_vPlayerPos.x - fNotePos, m_vPlayerPos.y - 0.03f, m_vPlayerPos.z + fNotePos);
}

void CSongBossStun::MusicNoteShake(const _float& fTimeDelta)
{
	// 시간초가 다 되어갈 때 음표 좌우로 흔들기
	if (2.5f < m_fLifeTime)
	{
		_vec3 vPlayerPos, vPos;
		CTransform*		pPlayer = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
		NULL_CHECK(pPlayer);
		pPlayer->Get_Info(INFO_POS, &m_vPlayerPos);

		m_pTransCom->Get_Info(INFO_POS, &vPos);

		_vec3 vRight, vUp, vLook;
		vLook = vPlayerPos - vPos;
		D3DXVec3Normalize(&vLook, &vLook);
		vUp = _vec3(0.f, 1.f, 0.f);

		D3DXVec3Cross(&vRight, &vUp, &vLook);

		m_fShakeTimeAcc += m_fShakeTimeAcc;
		if (0.3f < m_fShakeTimeAcc)
		{
			m_fShakeSpeed *= -1;
			m_fShakeTimeAcc = 0.f;
		}

		D3DXVec3Normalize(&vRight, &vRight);
		m_pTransCom->Move_Pos(&(vRight * m_fShakeSpeed * fTimeDelta));
	}
}

void CSongBossStun::LateUpdate_Object(void)
{
	Billboard();

	if (!m_bFire)
		return;

	if (4.f < m_fLifeTime) // 시간초 이내 음표 4개를 부시지 못 하면의 처리
	{
		CSongBoss* pSongBoss = dynamic_cast<CSongBoss*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SongBoss_0"));
		if (pSongBoss->Get_StunCount() != 4)
		{
			m_bRenderOFF = true; // 부서지지 못 한 음표 삭제

			CParticleMgr::GetInstance()->Set_Info(this, 20, 0.3f,
				_vec3({ 1.f, 1.f, 1.f }), 3.f, D3DXCOLOR{ 1.f, 1.f, 1.f, 1.f },
				1.f, false, false);
			CParticleMgr::GetInstance()->Call_Particle(PTYPE_FOUNTAIN, TEXTURE_8);
		}
		
		Reset();
	}

	CGameObject::LateUpdate_Object();
}

void CSongBossStun::Render_Obejct(void)
{
	if (!m_bFire)
		return;

	if (m_bRenderOFF)
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

void CSongBossStun::CollisionEvent(CGameObject * pObj)
{
	if (!m_bOneCheck)
	{
		// 음표를 부시면 카운트 증가
		CSongBoss* pSongBoss = dynamic_cast<CSongBoss*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SongBoss"));
		pSongBoss->Set_StunCount();

		CParticleMgr::GetInstance()->Set_Info(this, 20, 0.3f,
			_vec3({ 1.f, 1.f, 1.f }), 3.f, D3DXCOLOR{ 0.f, 0.f, 0.f, 1.f },
			1.f, false, true);
		CParticleMgr::GetInstance()->Call_Particle(PTYPE_FOUNTAIN, TEXTURE_8);

		m_bRenderOFF = true;
		m_bOneCheck = true;
	}
}

void CSongBossStun::Billboard()
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

CSongBossStun * CSongBossStun::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iBulletCount)
{
	CSongBossStun*		pInstance = new CSongBossStun(pGraphicDev);
	if (FAILED(pInstance->Ready_Object(iBulletCount)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSongBossStun::Free(void)
{
	CGameObject::Free();
}

void CSongBossStun::Reset()
{
	m_bFire = false;
	m_bDead = false;
	m_fLifeTime = 0.f;
	m_fFrame = 0.f;
	m_bReady = false;

	//m_iBulletCount = 0;
	m_bOneCheck = false;
	m_bRenderOFF = false;
	m_fShakeSpeed = 1.5f;
	m_fShakeTimeAcc = 0.f;

	m_pColliderCom->Set_Free(true);
	CBulletMgr::GetInstance()->Collect_Obj(m_iIndex, STUN_SONGBOSS);
}