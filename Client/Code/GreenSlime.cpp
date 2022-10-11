#include "stdafx.h"
#include "..\Header\GreenSlime.h"

#include "Export_Function.h"
#include "WandBullet.h"
#include "MiniMap.h"

// 충돌
#include "Player.h"
#include "BulletMgr.h"

CGreenSlime::CGreenSlime(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
	, m_ePreState(MOTION_END)
	, m_eCurState(MOTION_END)
	, m_fTimeAcc(0.f)
{
}

CGreenSlime::~CGreenSlime()
{
}

HRESULT CGreenSlime::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.iHp = 2;
	m_tInfo.iAttack = 1;

	m_pTransCom->Set_Pos(15.f, 1.f, 15.f);

	m_eCurState = IDLE;

	m_fIdle_Speed = 1.f;
	m_fAttack_Speed = 2.f;

	return S_OK;
}

_int CGreenSlime::Update_Object(const _float & fTimeDelta)
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
	//// 애니메이션 변화
	//m_fFrame += m_pTextureCom->Get_FrameEnd()  * fTimeDelta;

	//if (m_fFrame >= m_pTextureCom->Get_FrameEnd())
	//	m_fFrame = 0;

	Motion_Change(fTimeDelta);

	if (0 >= m_tInfo.iHp)
	{
		m_eCurState = DIE;
		return OBJ_DEAD;
	}

	Hit(fTimeDelta);
	
	if (!m_bHit)
	{
		Target_Follow(fTimeDelta);
	}	
	return 0;
}

void CGreenSlime::LateUpdate_Object(void)
{
	CMonster::Billboard();
	Engine::CGameObject::LateUpdate_Object();
}

void CGreenSlime::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pAnimtorCom->Set_Texture();
	//m_pTextureCom->Set_Texture((_ulong)m_fFrame);	// 텍스처 정보 세팅을 우선적으로 한다.
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	CMonster::Render_Obejct();
}

HRESULT CGreenSlime::Add_Component(void)
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

	m_pAnimtorCom->Add_Component(L"Proto_GreenSlimeIDLE_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_GreenSlimeATTACK_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_GreenSlimeHIT_Texture");
	m_pAnimtorCom->Add_Component(L"Proto_GreenSlimeDIE_Texture");

	//// m_pTextureCom	
	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_GreenSlimeIDLE_Texture"));
	//NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Proto_GreenSlimeIDLE_Texture", pComponent });

	//pComponent = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_GreenSlimeATTACK_Texture"));
	//NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Proto_GreenSlimeATTACK_Texture", pComponent });

	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_GreenSlimeHIT_Texture"));
	//NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Proto_GreenSlimeHIT_Texture", pComponent });

	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_GreenSlimeDIE_Texture"));
	//NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Proto_GreenSlimeDIE_Texture", pComponent });
	/////////

	return S_OK;
}

void CGreenSlime::Target_Follow(const _float & fTimeDelta)
{
	// 플레이어 따라가기
	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransformCom);

	_vec3		vPlayerPos, vPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	_float fDist = D3DXVec3Length(&(vPlayerPos - vPos));

	if (fDist < 8.f)
	{
		m_eCurState = ATTACK;
		m_pTransCom->Chase_Target(&vPlayerPos, m_fAttack_Speed, fTimeDelta);
	}
	else
	{
		m_eCurState = IDLE;

		_vec3 vRight, vUp, vLook;
		vLook = vPlayerPos - vPos;
		D3DXVec3Normalize(&vLook, &vLook);
		vUp = _vec3(0.f, 1.f, 0.f);

		D3DXVec3Cross(&vRight, &vUp, &vLook);

		m_fTimeAcc += fTimeDelta;
		if (2.f < m_fTimeAcc)
		{
			m_fIdle_Speed *= -1;
			m_fTimeAcc = 0.f;
		}

		D3DXVec3Normalize(&vRight, &vRight);
		m_pTransCom->Move_Pos(&(vRight * m_fIdle_Speed * fTimeDelta));
		
		//// 몬스터 좌우로 이동하기
		//_vec3		vRight;
		//m_pTransCom->Get_Info(INFO_RIGHT, &vRight);

		//m_fTimeAcc += fTimeDelta;
		//if (2.f < m_fTimeAcc)
		//{
		//	m_fIdle_Speed *= -1;
		//	m_fTimeAcc = 0.f;
		//}

		//D3DXVec3Normalize(&vRight, &vRight);
		//m_pTransCom->Move_Pos(&(vRight * m_fIdle_Speed * fTimeDelta));
	}
}

void CGreenSlime::Hit(const _float & fTimeDelta)
{
	if (!m_bHit)
		return;

	m_eCurState = HIT;

	m_fHitTimeAcc += fTimeDelta;
	if (1.f < m_fHitTimeAcc)
	{
		m_tInfo.iHp--;
		m_bHit = false;
		m_fHitTimeAcc = 0.f;
	}
}

void CGreenSlime::CollisionEvent(CGameObject * pObj)
{
	//CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);
	CGameObject*	pPlayer = Engine::Get_GameObject(L"Layer_GameLogic", L"Player");
	if (nullptr != pPlayer)
		m_bHit = true;

	for (auto& bullet : *CBulletMgr::GetInstance()->Get_Bullets(BULLET_WAND))
	{
		if (nullptr != bullet)
			m_bHit = true;
	}


}

void CGreenSlime::Motion_Change(const _float& fTimeDelta)
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_pAnimtorCom->Change_Animation(L"Proto_GreenSlimeIDLE_Texture");
			//m_pTextureCom = static_cast<CTexture*>(Find_Component(L"Proto_GreenSlimeIDLE_Texture", ID_STATIC));
			break;

		case ATTACK:
			m_pAnimtorCom->Change_Animation(L"Proto_GreenSlimeATTACK_Texture");
			//m_pTextureCom = static_cast<CTexture*>(Find_Component(L"Proto_GreenSlimeATTACK_Texture", ID_STATIC));
			break;

		case HIT:
			m_pAnimtorCom->Change_Animation(L"Proto_GreenSlimeHIT_Texture");
			//m_pTextureCom = static_cast<CTexture*>(Find_Component(L"Proto_GreenSlimeHIT_Texture", ID_STATIC));
			break;

		case DIE:
			m_pAnimtorCom->Change_Animation(L"Proto_GreenSlimeDIE_Texture");
			//m_pTextureCom = static_cast<CTexture*>(Find_Component(L"Proto_GreenSlimeDIE_Texture", ID_STATIC));
			break;
		}
		m_ePreState = m_eCurState;
	}
}

CGreenSlime * CGreenSlime::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGreenSlime *	pInstance = new CGreenSlime(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGreenSlime::Free(void)
{
	CMonster::Free();
}
