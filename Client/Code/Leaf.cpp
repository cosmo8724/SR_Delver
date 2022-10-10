#include "stdafx.h"
#include "..\Header\Leaf.h"

#include "Export_Function.h"
#include "BulletMgr.h"

CLeaf::CLeaf(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_OriginalPos(0.f, 0.f, 0.f)
	, m_fTimeAcc(0.f)
	, m_fTeleportingTimeAcc(0.f)
	, m_fBulletTimeAcc(0.f)
{
}


CLeaf::~CLeaf()
{
}

HRESULT CLeaf::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.iHp = 5;
	m_tInfo.iAttack = 1;

	m_OriginalPos = { 10.f, 1.f, 30.f };
	m_pTransCom->Set_Pos(m_OriginalPos.x, m_OriginalPos.y, m_OriginalPos.z);

	m_eCurState = IDLE;

	m_fIdle_Speed = 1.f;
	m_fAttack_Speed = 2.f;

	return S_OK;
}

_int CLeaf::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta * 0.6f);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	m_pAnimtorCom->Play_Animation(fTimeDelta);

	Motion_Change(fTimeDelta);
	//SKillTeleporting(fTimeDelta);

	if(Key_Down(DIK_L))
		CBulletMgr::GetInstance()->Fire(BULLET_M_LEAF);

	return 0;
}

void CLeaf::LateUpdate_Object(void)
{
	CMonster::Billboard();
	Engine::CGameObject::LateUpdate_Object();
}

void CLeaf::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_BOTHINVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x05);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pAnimtorCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CLeaf::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// m_pAnimtorCom
	pComponent = m_pAnimtorCom = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_AnimatorCom"));
	NULL_CHECK_RETURN(m_pAnimtorCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_AnimatorCom", pComponent });

	m_pAnimtorCom->Add_Component(L"Proto_LeafIDLE_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_LeafATTACK_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_LeafHIT_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_LeafDIE_Texture");

	return S_OK;
}

void CLeaf::SKillTeleporting(const _float & fTimeDelta)
{
	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransformCom);

	_vec3		vPlayerPos, vPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	_float fDist = D3DXVec3Length(&(vPlayerPos - vPos));

 	if (fDist < 5.f) // 몬스터 순간 이동
	{
		m_eCurState = ATTACK;

		if (m_pAnimtorCom->Get_Currentframe() >= 12.f && m_pAnimtorCom->Get_Currentframe() < 13.f) // Leaf의 Attack 이미지가 끝날 때 쯤
			Teleporting(vPlayerPos.x, vPlayerPos.z);
	}
	else
	{
		m_eCurState = IDLE;

		if (fDist < 10.f)
		{
			m_fBulletTimeAcc += fTimeDelta;
			if (0.3f < m_fBulletTimeAcc)
			{
				CBulletMgr::GetInstance()->Fire(BULLET_M_LEAF);
				m_fBulletTimeAcc = 0.f;
			}
		}
	}
}

void CLeaf::Teleporting(const _float& fPlayerPosX, const _float& fPlayerPosZ)
{
	int iRandomNum = rand() % 7 + 2; // 몬스터가 랜덤하게 이동할 좌표

	if (fPlayerPosX == (m_OriginalPos.x + iRandomNum) || fPlayerPosZ == (m_OriginalPos.z + iRandomNum) ||
		fPlayerPosX == (m_OriginalPos.x - iRandomNum) || fPlayerPosZ == (m_OriginalPos.z - iRandomNum))
		return; // 만약 몬스터가 이동할 좌표가 플레이어와 같다면 다시 랜덤값을 받는다

	if (iRandomNum % 2 == 0) // 짝수
	{
		if (iRandomNum < 5) // 짝수인데 5보다 크다면
			m_pTransCom->Set_Pos(m_OriginalPos.x - iRandomNum, 1.f, m_OriginalPos.z + iRandomNum);
		else
			m_pTransCom->Set_Pos(m_OriginalPos.x + iRandomNum, 1.f, m_OriginalPos.z + iRandomNum);
	}
	else // 홀수
	{
		if (iRandomNum < 5) // 홀수인데 5보다 크다면
			m_pTransCom->Set_Pos(m_OriginalPos.x + iRandomNum, 1.f, m_OriginalPos.z - iRandomNum);
		else 
			m_pTransCom->Set_Pos(m_OriginalPos.x - iRandomNum, 1.f, m_OriginalPos.z + iRandomNum);
	}
}

void CLeaf::Motion_Change(const _float & fTimeDelta)
{
		if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_pAnimtorCom->Change_Animation(L"Proto_LeafIDLE_Texture");
			break;

		case ATTACK:
			m_pAnimtorCom->Change_Animation(L"Proto_LeafATTACK_Texture");
			break;
		
		case HIT:
			m_pAnimtorCom->Change_Animation(L"Proto_LeafHIT_Texture");
			break;

		case DIE:
			m_pAnimtorCom->Change_Animation(L"Proto_LeafDIE_Texture");
			break;
		}
		m_ePreState = m_eCurState;
	}
}

CLeaf * CLeaf::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLeaf *	pInstance = new CLeaf(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLeaf::Free(void)
{
	CMonster::Free();
}
