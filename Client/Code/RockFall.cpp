#include "stdafx.h"
#include "..\Header\RockFall.h"
#include "Export_Function.h"
#include "CullingMgr.h"
#include "StaticCamera.h"
#include "Player.h"

CRockFall::CRockFall(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	:CEcoObject(pGraphicDev)
{
	m_eType			= ECO_ROCKFALL;
	m_vPos			= vPos;
	m_fDist			= 10.f;
	m_fRisingSpeed	= 0.07f;
	m_fFallSpeed	= 0.5f;
	m_fCurSpeed		= m_fFallSpeed;
	m_fOriginPosY	= vPos.y;
	m_fGroundY = 0.5f;
}

CRockFall::CRockFall(const CEcoObject & rhs)
	:CEcoObject(rhs)
{
}

CRockFall::~CRockFall()
{
}

HRESULT CRockFall::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (!m_bClone)
		m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	m_pTransCom->Set_Scale(0.4f, 1.f, 1.f);

	return S_OK;
}

_int CRockFall::Update_Object(const _float & fTimeDelta)
{
	CEcoObject::Update_Object(fTimeDelta);

	if (!m_bReady)
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
		m_pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
		m_pCamera = dynamic_cast<CStaticCamera*>(Get_GameObject(L"Layer_Environment", L"StaticCamera"));

		if (nullptr != m_pPlayerTransCom &&
			nullptr != m_pCamera)
		{
			m_bReady = true;
			pPlayer->Add_CollisionGroup(this);
		}
		else
			return 0;
	}
	
	Activate(); // move up and down

	if (Is_Player_Around() && m_bHit)
	{
		m_pCamera->Shake_Camera(0.5f, 1);
	}


	return 0;
}

void CRockFall::LateUpdate_Object(void)
{
	if(CCullingMgr::GetInstance()->Is_Inside(this))
		Add_RenderGroup(RENDER_ALPHA, this);

	//Billboard();
	CEcoObject::LateUpdate_Object();
}

void CRockFall::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	// ���İ� ��ȭ
	//m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iTransparency, 0, 0, 0));
	//m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	m_pTextureCom->Set_Texture(m_iTexture);
	this->m_pBufferCom->Render_Buffer();

	// ���İ� ��ȭ ����
	//m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	CEcoObject::Render_Obejct();	// collider ���

}

HRESULT CRockFall::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = this->m_pBufferCom = dynamic_cast<CCubeTex*>(Clone_Proto(L"Proto_CubeTexCom"));
	NULL_CHECK_RETURN(this->m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTexCom", pComponent });

	if (!m_bClone)
	{
		pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
		NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });
	}

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_RockFall_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RockFall_Texture", pComponent });


	return S_OK;
}

_bool CRockFall::Is_Player_Around()
{
	_float fDist = D3DXVec3Length(&_vec3(m_pPlayerTransCom->Get_Pos() - m_pTransCom->Get_Pos()));

	if (m_fDist > fDist)
		return true;

	return false;
}

void CRockFall::Activate()
{
	_float fY = m_pTransCom->Get_Pos().y;

	if (m_fOriginPosY <= fY)
	{
		m_fCurSpeed = -m_fFallSpeed;
		m_bFall = true;
	}
	else if (true == m_bFall && m_fGroundY >= fY)
	{
		m_fCurSpeed = m_fRisingSpeed;
		m_bFall = false;
		m_bHit = true;
	}
	else if (fY > m_fGroundY)
		m_bHit = false;

	m_pTransCom->Plus_PosY(m_fCurSpeed);

}

CRockFall * CRockFall::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CRockFall *	pInstance = new CRockFall(pGraphicDev, vPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CRockFall * CRockFall::Create(CEcoObject * pEcoObject)
{
	CRockFall *	pInstance = new CRockFall(*pEcoObject);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CRockFall::Free(void)
{
	CEcoObject::Free();
}

void CRockFall::CollisionEvent(CGameObject * pObj)
{
}