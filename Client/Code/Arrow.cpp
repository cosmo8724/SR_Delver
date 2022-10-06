#include "stdafx.h"
#include "Arrow.h"
#include "Export_Function.h"
#include "BulletMgr.h"
#include "ParticleMgr.h"

CArrow::CArrow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CWeapon(pGraphicDev)
{
	m_ObjTag = L"Arrow";
}

CArrow::CArrow(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	: CWeapon(pGraphicDev)
{
	m_vPos = vPos;
	m_ObjTag = L"Arrow";
}

CArrow::~CArrow()
{
}

HRESULT CArrow::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);
	m_eState = STATE_GROUND;

	_vec3 vScale;
	_matrix matWorld;
	m_pTransCom->Get_WorldMatrix(&matWorld);

	vScale = m_pTransCom->Get_Scale();
	m_bdBox.vMin = { m_vPos.x - vScale.x, m_vPos.y - vScale.y, m_vPos.z - vScale.z };
	m_bdBox.vMax = { m_vPos.x + vScale.x, m_vPos.y + vScale.y, m_vPos.z + vScale.z };

	return S_OK;
}

_int CArrow::Update_Object(const _float & fTimeDelta)
{
	int iResult = CGameObject::Update_Object(fTimeDelta);

	if (STATE_INV == m_eState)
		return iResult;


	if (!m_bReady)
	{
		m_pCamera = dynamic_cast<CCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"StaticCamera"));
		if (nullptr == m_pCamera)
			return 0;
		else
		{
			m_pCenter = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
			if (nullptr == m_pCenter)
				return 0;
			else m_bReady = true;
		}
	}


	_vec3* pPlayerInfo = m_pCenter->Get_InfoAll();

	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	switch (m_eState)
	{
	case STATE_GROUND:
		m_pTransCom->Set_Scale(1.f, 1.f, 1.f);
		m_pTransCom->Revolution(pPlayerInfo, matView, 0.f, m_fTimeDelta, STATE_GROUND);
		break;
	case STATE_EQUIP:
		if (!(Engine::Get_DIKeyState(DIK_TAB) & 0x80))
		{
			Charge(fTimeDelta);
			Attack(fTimeDelta);
		}

		m_pTransCom->Set_Scale(0.3f, 0.3f, 0.3f);
		m_pTransCom->Revolution(pPlayerInfo, matView, 45.f, m_fTimeDelta, STATE_EQUIP);
		break;
	}

	Add_RenderGroup(RENDER_ALPHA, this);

	m_fTimeDelta = fTimeDelta;

	return iResult;
}

void CArrow::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();

	if (STATE_INV == m_eState)
		return;

}

void CArrow::Render_Obejct(void)
{
	if (m_eState == STATE_INV)
		return;


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->Set_Texture(_ulong(m_fFrame));

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CArrow * CArrow::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CArrow*		pInstance = new CArrow(pGraphicDev, vPos);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CArrow::Free(void)
{
	CGameObject::Free();
}

HRESULT CArrow::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	// 버퍼 컴포넌트
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	// 텍스쳐 컴객체 컴포넌트
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Arrow1Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Arrow1Texture", pComponent });
	m_textureTag = L"Proto_Arrow1Texture";

	// 월드행렬 컴포넌트
	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	return S_OK;

}

void CArrow::CollisionEvent(CGameObject * pObj)
{
	if (STATE_GROUND == m_eState)
	{
		m_eState = STATE_INV;
		m_pTransCom->Set_Pos(-1000.f, -1000.f, -1000.f);
		m_vPos = { -1000.f, -1000.f, -1000.f };

		_vec3 vScale;
		_matrix matWorld;
		m_pTransCom->Get_WorldMatrix(&matWorld);

		vScale = m_pTransCom->Get_Scale();
		m_bdBox.vMin = { m_vPos.x - vScale.x, m_vPos.y - vScale.y, m_vPos.z - vScale.z };
		m_bdBox.vMax = { m_vPos.x + vScale.x, m_vPos.y + vScale.y, m_vPos.z + vScale.z };
	}

}

void CArrow::Charge(const _float & fTimeDelta)
{
	_int frameEnd = m_pTextureCom->Get_FrameEnd();

	if (m_bCharge)
	{
		m_fFrame += frameEnd * fTimeDelta;

		if (!m_bParticleCall)
		{
			CParticleMgr::GetInstance()->Set_Info(this);
			CParticleMgr::GetInstance()->Call_Particle(PTYPE_SPOT, TEXTURE_0);
			m_bParticleCall = true;
		}


		if (m_fFrame >= frameEnd)
			m_fFrame = (_float)frameEnd;

		if (5.f < m_fChargeTime)
		{
			m_fFrame = 0.f;
			m_fChargeTime = 0.f;
			m_bCharge = false;
			m_bAttack = true;
		}

	}

	if (Engine::Get_DIMouseState(DIM_LB) & 0x80)	// 왼쪽 버튼이 눌렸다.
	{
		m_bClick = true;
		m_fChargeTime += m_fTimeDelta;

		if (m_fChargeTime > 0.3f)	// 차징상황이라면
		{
			m_bClick = false;		// 클릭이 아니라 차징이다.
			m_bCharge = true;
		}
	}
	else
	{
		if (true == m_bClick)	// 왼쪽 버튼이 눌려있지는 않지만 이전에 눌렸었다면
		{
			m_fFrame += frameEnd * fTimeDelta * 3.f;	// 단순 공격을 한다.
			if (m_fFrame >= frameEnd)
			{
				m_fFrame = 0.f;
				m_bClick = false;
				m_bAttack = true;
			}
		}
		else if (true == m_bCharge)  // 차지 중인 상황이었다면 차징공격을 한다.
		{
			m_fFrame = 0.f;
			m_bCharge = false;
			m_fChargeTime = 0.f;
			m_bAttack = true;
			m_bParticleCall = false;
		}
	}
}

void CArrow::Attack(const _float & fTimeDelta)
{
	if (true == m_bAttack)
	{
		// 총알 발사가 좀?
		CBulletMgr::GetInstance()->Fire(BULLET_ARROW);
		m_bAttack = false;
	}
}
