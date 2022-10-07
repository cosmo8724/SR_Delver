#include "stdafx.h"
#include "..\Header\SongBossBullet.h"

#include "Export_Function.h"	

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

	m_pTransCom->Set_Scale(0.5f, 0.5f, 0.5f);

	m_fSpeed = 5.f;

	return S_OK;
}

HRESULT CSongBossBullet::Add_Component(void)
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

	m_pAnimtorCom->Add_Component(L"Proto_MusicNote_Bullet_Texture");

	return S_OK;
}

_int CSongBossBullet::Update_Object(const _float & fTimeDelta)
{
	if (!m_bFire)
		return 0;

	int iResult = CGameObject::Update_Object(fTimeDelta);

	m_pAnimtorCom->Play_Animation(fTimeDelta);

	if (!m_bReady)
	{
		CTransform*		pFist = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"SongBoss", L"Proto_TransformCom", ID_DYNAMIC));
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

	Add_RenderGroup(RENDER_ALPHA, this);
	
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
		Reset();
	}

	CGameObject::LateUpdate_Object();
}

void CSongBossBullet::Render_Obejct(void)
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

void CSongBossBullet::Billboard()
{
	// ������
	_matrix		matWorld, matView, matBill;
	D3DXMatrixIdentity(&matBill);

	m_pTransCom->Get_WorldMatrix(&matWorld);




	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	// ���� ���� �� �ڵ�� ������ ������ ���߿� ������ �� �� ����
	m_pTransCom->Set_WorldMatrix(&(matBill * matWorld));
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
}