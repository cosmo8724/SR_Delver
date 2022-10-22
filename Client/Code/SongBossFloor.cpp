#include "stdafx.h"
#include "..\Header\SongBossFloor.h"

#include "Export_Function.h"	
#include "BulletMgr.h"
#include "ParticleMgr.h"

CSongBossFloor::CSongBossFloor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
	, m_iBulletCount(0)
	, m_iTransparency(0)
	, m_fTransparencyTimeAcc(0.f)
{
}

CSongBossFloor::CSongBossFloor(const CSongBossFloor & rhs)
	:CBullet(rhs)
{
}

CSongBossFloor::~CSongBossFloor()
{
}

HRESULT CSongBossFloor::Ready_Object(_int iBulletCount)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.iAttack = 5;
	m_pTransCom->Set_Scale(1.f, 1.f, 1.f);
	m_pTransCom->Rotation(ROT_X, 45.555f);

	m_iBulletCount = iBulletCount;
	m_iTransparency = 40;

	return S_OK;
}

HRESULT CSongBossFloor::Add_Component(void)
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

	m_pAnimtorCom->Add_Component(L"Proto_MusicNote_Floor_Texture");

	// Collider Component
	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	return S_OK;
}

_int CSongBossFloor::Update_Object(const _float & fTimeDelta)
{
	if (!m_bFire)
		return 0;

	int iResult = CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);
	m_pAnimtorCom->Play_Animation(fTimeDelta * 0.5f);

	MusicNoteCreatePos();

	m_fLifeTime += fTimeDelta;
	StartLightning(fTimeDelta);
	return iResult;
}

void CSongBossFloor::MusicNoteCreatePos()
{
	// �÷��̾� �ֺ����� ����� ��ǥ
	CTransform*		pPlayer = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayer);
	pPlayer->Get_Info(INFO_POS, &m_vPlayerPos); // �÷��̾��� ��ǥ�� �޾ƿͼ�

	if (!m_bReady)
	{
		_float fNotePos = 3.f;
		_float fNotePosY = -65.8f;

		if (m_iBulletCount == 0)
			m_pTransCom->Set_Pos(m_vPlayerPos.x, fNotePosY, m_vPlayerPos.z + fNotePos);
		else if (m_iBulletCount == 1)
			m_pTransCom->Set_Pos(m_vPlayerPos.x + fNotePos, fNotePosY, m_vPlayerPos.z);
		else if (m_iBulletCount == 2)
			m_pTransCom->Set_Pos(m_vPlayerPos.x - fNotePos, fNotePosY, m_vPlayerPos.z);
		else if (m_iBulletCount == 3)
			m_pTransCom->Set_Pos(m_vPlayerPos.x, fNotePosY, m_vPlayerPos.z - fNotePos);
		else if (m_iBulletCount == 4)
			m_pTransCom->Set_Pos(m_vPlayerPos.x, fNotePosY, m_vPlayerPos.z);

		m_bReady = true;
	}
}

void CSongBossFloor::StartLightning(const _float& fTimeDelta)
{
	m_fTransparencyTimeAcc += fTimeDelta;
	// ������ ����
	if (0.1f < m_fTransparencyTimeAcc)
	{
		m_iTransparency += 10; // �������� �������� ���� > 10
		m_fTransparencyTimeAcc = 0.f;
		
//#ifdef SH_Debug
		//cout << m_iTransparency << endl;
//#endif
		
		if (m_iTransparency >= 240)
		{
			m_bStartLightning = true;
			m_iTransparency = 40;
		}
	}

	if (m_bStartLightning)
	{
		CParticleMgr::GetInstance()->Set_Info(this, 3, 0.5f,
			_vec3({ 1.f, 1.f, 1.f }), 1.f, D3DXCOLOR{ 1.f, 1.f, 0.f, 1.f });
		CParticleMgr::GetInstance()->Call_Particle(PTYPE_FOUNTAIN, TEXTURE_9);

		m_pColliderCom->Set_Free(false);
		m_pColliderCom->Calculate_WorldMatrix(*m_pTransCom->Get_WorldMatrixPointer());
	}
}

void CSongBossFloor::LateUpdate_Object(void)
{
	if (!m_bFire)
		return;

	if (2.5f < m_fLifeTime)
		Reset();

	CGameObject::LateUpdate_Object();
}

void CSongBossFloor::Render_Obejct(void)
{
	if (!m_bFire)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iTransparency, 0, 0, 0));
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pAnimtorCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
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

CSongBossFloor * CSongBossFloor::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iBulletCount)
{
	CSongBossFloor*		pInstance = new CSongBossFloor(pGraphicDev);
	if (FAILED(pInstance->Ready_Object(iBulletCount)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSongBossFloor::Free(void)
{
	CGameObject::Free();
}

void CSongBossFloor::Reset()
{
	m_bFire = false;
	m_bDead = false;
	m_fLifeTime = 0.f;
	m_fFrame = 0.f;
	m_bReady = false;

	m_iTransparency = 40;
	m_bStartLightning = false;
	m_fTransparencyTimeAcc = 0.f;

	m_pColliderCom->Set_Free(true);
	CBulletMgr::GetInstance()->Collect_Obj(m_iIndex, FLOOR_SONGBOSS);
}