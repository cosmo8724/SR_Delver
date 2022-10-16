#include "stdafx.h"
#include "..\Header\SkeletonGhost.h"

#include "Export_Function.h"
#include "MiniMap.h"

#include "Player.h"
#include "ParticleMgr.h"
#include "ItemMgr.h"

CSkeletonGhost::CSkeletonGhost(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_fHeight(0.f)
	, m_fTimeAcc(0.f)
	, m_fAttackTimeAcc(0.f)
	, m_fHpMinusTimeAcc(0.f)
{
	m_ObjTag = L"SkeletonGhost";
}

CSkeletonGhost::CSkeletonGhost(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	: CMonster(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_fHeight(0.f)
	, m_fTimeAcc(0.f)
	, m_fAttackTimeAcc(0.f)
	, m_fHpMinusTimeAcc(0.f)
{
	m_vPos = vPos;
	m_ObjTag = L"SkeletonGhost";
}

CSkeletonGhost::~CSkeletonGhost()
{
}

HRESULT CSkeletonGhost::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.iHp = 3;
	m_tInfo.iAttack = 1;

	m_fHeight = m_vPos.y; // 3.f;
	m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	m_eCurState = IDLE;

	m_fIdle_Speed = 1.f;
	m_fAttack_Speed = 2.f;

	return S_OK;
}

_int CSkeletonGhost::Update_Object(const _float & fTimeDelta)
{
	if (!m_bCreateIcon)
	{
		CMiniMap* pMiniMap = dynamic_cast<CMiniMap*>(Engine::Get_GameObject(L"Layer_UI", L"UI_MiniMap"));
		pMiniMap->Add_Icon(m_pGraphicDev, this);
		m_bCreateIcon = true;
	}
	Engine::CMonster::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	m_pAnimtorCom->Play_Animation(fTimeDelta);
	Motion_Change();

	if (0 >= m_tInfo.iHp)
	{
		Dead();
		m_fRenderOFFTimeAcc += fTimeDelta;
		if (1.5f < m_fRenderOFFTimeAcc)
		{
			m_bRenderOFF = true;
			m_fRenderOFFTimeAcc = 0.f;
		}
		return OBJ_DEAD;
	}

	OnHit(fTimeDelta);

	if (!m_bHit)
		Target_Follow(fTimeDelta);
	return 0;
}

void CSkeletonGhost::LateUpdate_Object(void)
{
	//Billboard();
	CMonster::Billboard();
	Engine::CGameObject::LateUpdate_Object();
}

void CSkeletonGhost::Render_Obejct(void)
{
	if (!m_bRender)
		return;

	if (!m_bRenderOFF)
		CMonster::Render_Obejct();

	//if(m_bCircle)
	//	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	//else
	//	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//m_pAnimtorCom->Set_Texture();
	//m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//CMonster::Render_Obejct();
}

HRESULT CSkeletonGhost::Add_Component(void)
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

	// Collider Component
	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	m_pAnimtorCom->Add_Component(L"Proto_SkeletonGhostIDLE_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_SkeletonGhostATTACK_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_SkeletonGhostHIT_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_SkeletonGhostDIE_Texture");

	return S_OK;
}

void CSkeletonGhost::Target_Follow(const _float & fTimeDelta)
{
	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransformCom);

	_vec3		vPlayerPos, vPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	// MonsterLook -> Player
	_vec3 vRight, vUp, vLook;
	vLook = vPlayerPos - vPos;
	m_pTransCom->Set_Look(&vLook);

	// Distance Attack
	_float fDist = D3DXVec3Length(&(vPlayerPos - vPos));

	if (fDist < 5.f)
	{
		m_fHpMinusTimeAcc += fTimeDelta;
		if (2.f < m_fHpMinusTimeAcc) // 2초마다 플레이어 체력 감소
		{
			CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
			pPlayer->Set_HpMinus();

			m_fHpMinusTimeAcc = 0.f;
		}

		m_bCircle = true;
		Circle();
	}
	else
		m_bCircle = false;


	if (!m_bCircle && fDist < 15.f)
	{
		m_pTransCom->ChangeHeight_Target(&vPlayerPos, m_fHeight, m_fAttack_Speed, fTimeDelta);
	}
}

void CSkeletonGhost::Circle()
{
	if (!m_bCircle)
		return;

	_vec3	vPlayerPos;
	CTransform*		pPlayer = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayer);

	pPlayer->Get_Info(INFO_POS, &vPlayerPos);
	//m_pTransCom->Set_Pos(vPlayerPos.x, vPlayerPos.y, vPlayerPos.z);

	_vec3 vDir, vDistance;
	vDir = { 0.f, 1.f, 0.f };
	//D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&vDistance, &vDistance);

	_matrix matTrans, matRev;

	// ��
	D3DXMatrixTranslation(&matTrans, vDistance.x * 5.f, vDistance.y * -2.5f, vDistance.z * 2.f);

	// ��
	m_fAngle += 0.03f;
	if (m_fAngle > 360.f)
		m_fAngle = 0.03f;
	D3DXMatrixRotationAxis(&matRev, &vDir, m_fAngle);

	_matrix matParent;
	pPlayer->Get_WorldMatrix(&matParent);

	_matrix mat;
	D3DXMatrixTranslation(&mat, matParent._41, matParent._42, matParent._43);

	matParent = matRev * mat;

	_matrix mat1;
	mat1 = matTrans * matParent;

	m_matWorld = mat1;
	m_pTransCom->Set_Pos(m_matWorld._41, m_matWorld._42, m_matWorld._43);
}

void CSkeletonGhost::Billboard()
{
	_matrix		matWorld, matView, matBill;
	D3DXMatrixIdentity(&matBill);

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	m_matWorld = matBill * m_matWorld;
}

void CSkeletonGhost::OnHit(const _float & fTimeDelta)
{
	if (!m_bHit)
		return;

	if (!m_bOneCheck)
	{
		m_eCurState = HIT;
		CMonster::KnockBack(fTimeDelta, m_fHeight, 20.f);
		m_bOneCheck = true;
	}

	m_fHitTimeAcc += fTimeDelta;
	if (0.7f < m_fHitTimeAcc) // 0.7 > Monster Hit Time
	{
		// MinusHp
		CPlayer*	pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
		m_tInfo.iHp -= pPlayer->Get_PlayerAttack();

		// Initialization
		m_eCurState = IDLE;
		m_bHit = false;
		m_bOneCheck = false;
		m_fHitTimeAcc = 0.f;
	}
}

void CSkeletonGhost::Dead()
{
	if (m_bDead) // �� ���� �����
		return;

	m_eCurState = DIE;
	m_pTransCom->Set_Y(1.f);

	CParticleMgr::GetInstance()->Set_Info(this,
		50,
		0.1f,
		{ 0.5f, 0.5f, 0.5f },
		1.f,
		{ 0.f, 0.3f, 1.f, 1.f });
	CParticleMgr::GetInstance()->Call_Particle(PTYPE_FOUNTAIN, TEXTURE_5);

	CItemMgr::GetInstance()->Add_RandomObject(L"Layer_GameLogic", L"Potion", ITEM_POTION, m_pTransCom->Get_Pos());

	m_pColliderCom->Set_Free(true);
	m_bDead = true;
}

void CSkeletonGhost::CollisionEvent(CGameObject * pObj)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);
	if (pPlayer != pObj)
	{
		m_bHit = true;
		m_bRender = true;
	}
}

void CSkeletonGhost::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_pAnimtorCom->Change_Animation(L"Proto_SkeletonGhostIDLE_Texture");
			break;

		case ATTACK:
			m_pAnimtorCom->Change_Animation(L"Proto_SkeletonGhostATTACK_Texture");
			break;

		case HIT:
			m_pAnimtorCom->Change_Animation(L"Proto_SkeletonGhostHIT_Texture");
			break;

		case DIE:
			m_pAnimtorCom->Change_Animation(L"Proto_SkeletonGhostDIE_Texture");
			break;
		}
		m_ePreState = m_eCurState;
	}
}

CSkeletonGhost * CSkeletonGhost::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CSkeletonGhost *	pInstance = new CSkeletonGhost(pGraphicDev, vPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSkeletonGhost::Free(void)
{
	CMonster::Free();
}
