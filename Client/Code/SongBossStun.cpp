#include "stdafx.h"
#include "..\Header\SongBossStun.h"

#include "Export_Function.h"	
#include "BulletMgr.h"

CSongBossStun::CSongBossStun(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
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

	m_iBulletCount = iBulletCount;
	return S_OK;
}

HRESULT CSongBossStun::Add_Component(void)
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

	m_pAnimtorCom->Add_Component(L"Proto_MusicNote_Stun_Texture");

	return S_OK;
}

_int CSongBossStun::Update_Object(const _float & fTimeDelta)
{
	if (!m_bFire)
		return 0;

	int iResult = CGameObject::Update_Object(fTimeDelta);
	m_pAnimtorCom->Play_Animation(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);

	// �÷��̾� �ֺ����� ����� ��ǥ
	CTransform*		pPlayer = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayer, -1);
	pPlayer->Get_Info(INFO_POS, &m_vPlayerPos); // �÷��̾��� ��ǥ�� �޾ƿͼ�

	_float fNotePos = 0.5f;

	if (m_iBulletCount == 0)
		m_pTransCom->Set_Pos(m_vPlayerPos.x + fNotePos, m_vPlayerPos.y - 0.03f, m_vPlayerPos.z + fNotePos);
	else if (m_iBulletCount == 1)
		m_pTransCom->Set_Pos(m_vPlayerPos.x + fNotePos, m_vPlayerPos.y - 0.03f, m_vPlayerPos.z - fNotePos);
	else if (m_iBulletCount == 2)
		m_pTransCom->Set_Pos(m_vPlayerPos.x - fNotePos, m_vPlayerPos.y - 0.03f, m_vPlayerPos.z - fNotePos);
	else if (m_iBulletCount == 3)
		m_pTransCom->Set_Pos(m_vPlayerPos.x - fNotePos, m_vPlayerPos.y - 0.03f, m_vPlayerPos.z + fNotePos);
	else
		return 0;

	// ��ǥ �¿�� ����

	
	m_fLifeTime += fTimeDelta;
	return iResult;
}

void CSongBossStun::LateUpdate_Object(void)
{
	Billboard();

	if (!m_bFire)
		return;

	//if (10.f < m_fLifeTime)
	//	Reset();

	CGameObject::LateUpdate_Object();
}

void CSongBossStun::Render_Obejct(void)
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

void CSongBossStun::Billboard()
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
	CBulletMgr::GetInstance()->Collect_Obj(m_iIndex, STUN_SONGBOSS);
}