#include "stdafx.h"
#include "ArrowBullet.h"
#include "Export_Function.h"
#include "BulletMgr.h"
#include "ParticleMgr.h"

CArrowBullet::CArrowBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
	, m_fMinSpeed(10.f)
	, m_fMaxSpeed(20.f)
{
	memset(&m_bdBox, 0, sizeof(BDBOX));
	memset(&m_bdSphere, 0, sizeof(BDSPHERE));
	m_fSpeed = m_fMinSpeed;
	m_fSpeedY = m_fMinSpeed;
}

CArrowBullet::CArrowBullet(const CArrowBullet & rhs)
	:CBullet(rhs)
{
	memcpy(&m_bdBox, &rhs.m_bdBox, sizeof(BDBOX));
	memcpy(&m_bdSphere, &rhs.m_bdSphere, sizeof(BDSPHERE));
	m_fMinSpeed = rhs.m_fMinSpeed;
	m_fMaxSpeed = rhs.m_fMaxSpeed;
	m_fSpeed = rhs.m_fSpeed;
	m_fSpeedY = rhs.m_fSpeedY;

}

CArrowBullet::~CArrowBullet()
{
}

HRESULT CArrowBullet::Ready_Object(void)
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

_int CArrowBullet::Update_Object(const _float & fTimeDelta)
{
	if (!m_bFire)
		return 0;

	if (!m_bReady)
	{
		CTransform*		pPlayer = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
		NULL_CHECK_RETURN(pPlayer, -1);

		_vec3 vPos;
		pPlayer->Get_Info(INFO_POS, &vPos);
		m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);

		pPlayer->Get_Info(INFO_LOOK, &m_vDirection);
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		
		
		if (m_fSpeed > m_fMaxSpeed)
		{
			m_fSpeed = m_fMaxSpeed;
			m_fSpeedY = m_fMaxSpeed;
		}
		
		CParticleMgr::GetInstance()->Set_Info(this);
		CParticleMgr::GetInstance()->Call_Particle(PTYPE_REMAIN, TEXTURE_0);


		m_bReady = true;


	}

	_vec3 vMove;
	vMove.x = m_fSpeed * fTimeDelta * m_vDirection.x;

	if (m_vDirection.y < 0.f)
		m_vDirection.y = 0.1f;
	m_fSpeedY = m_fSpeedY - 1.5f;
	vMove.y = m_fSpeedY *fTimeDelta * m_vDirection.y;

	vMove.z = m_fSpeed * fTimeDelta * m_vDirection.z;
	m_pTransCom->Move_Pos(&vMove);



	int iResult = CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_ALPHA, this);

	m_fLifeTime += fTimeDelta;

	return iResult;
}

void CArrowBullet::LateUpdate_Object(void)
{
	if (!m_bFire || !m_bReady)
		return;

	// 아무데도 충돌안해도 일정 시간 후 리셋함
	if (2.f < m_fLifeTime)
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

void CArrowBullet::Render_Obejct(void)
{
	if (!m_bFire || !m_bReady)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->Set_Texture(FtoDw(m_fFrame));

	m_pBufferCom->Render_Buffer();

}

CArrowBullet * CArrowBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CArrowBullet*		pInstance = new CArrowBullet(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CArrowBullet::Free(void)
{
	CGameObject::Free();
}

void CArrowBullet::Reset()
{
	m_bFire = false;
	m_bDead = false;
	m_fLifeTime = 0.f;
	m_fFrame = 0.f;
	m_bReady = false;
	m_fSpeed = m_fMinSpeed;
	m_fSpeedY = m_fMinSpeed;

	CBulletMgr::GetInstance()->Collect_Obj(m_iIndex, BULLET_ARROW);
}

HRESULT CArrowBullet::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	// 버퍼 컴포넌트
	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Clone_Proto(L"Proto_CubeTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTexCom", pComponent });

	// 텍스쳐 컴객체 컴포넌트
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_CubeTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTexture", pComponent });

	// 월드행렬 컴포넌트
	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	return S_OK;
}

void CArrowBullet::CollisionEvent(CGameObject * pObj)
{
}
