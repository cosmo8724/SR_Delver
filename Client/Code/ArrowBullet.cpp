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

	float fSize = 1.f;
	m_pTransCom->Set_Scale(fSize, fSize, fSize);

	//// 충돌처리
	//_vec3 vPos, vScale;
	//m_pTransCom->Get_Info(INFO_POS, &vPos);
	//vScale = m_pTransCom->Get_Scale();
	//m_bdBox.vMin = { vPos.x - vScale.x, vPos.y - vScale.y, vPos.z - vScale.z };
	//m_bdBox.vMax = { vPos.x + vScale.x, vPos.y + vScale.y, vPos.z + vScale.z };


	return S_OK;
}

_int CArrowBullet::Update_Object(const _float & fTimeDelta)
{
	if (!m_bFire)
		return 0;

	int iResult = CGameObject::Update_Object(fTimeDelta);

	if (!m_bReady)
	{
		CTransform*		pWeapon = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Arrow0", L"Proto_TransformCom", ID_DYNAMIC));
			//dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
		NULL_CHECK_RETURN(pWeapon, -1);

		_vec3 vPos;
		pWeapon->Get_Info(INFO_POS, &vPos);
		m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);

		CTransform*		pPlayer = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));

		pPlayer->Get_Info(INFO_LOOK, &m_vDirection);
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		
		
		if (m_fSpeed > m_fMaxSpeed)
		{
			m_fSpeed = m_fMaxSpeed;
			m_fSpeedY = m_fMaxSpeed;
		}
		
		//CParticleMgr::GetInstance()->Set_Info(this);
		//CParticleMgr::GetInstance()->Call_Particle(PTYPE_REMAIN, TEXTURE_0);


		m_bReady = true;


	}

	
	_vec3 vMove;
	vMove.x = m_fSpeed * fTimeDelta * m_vDirection.x;

	if (m_vDirection.y < 0.f)
		m_vDirection.y = 0.1f;
	//m_fSpeedY = m_fSpeedY - 1.5f;
	vMove.y = m_fSpeedY *fTimeDelta * m_vDirection.y;

	vMove.z = m_fSpeed * fTimeDelta * m_vDirection.z;
	
	m_pTransCom->Move_Pos(&vMove);





	// 텍스쳐 빌보드
	_matrix matScaleInv, matScale, matRot, matBill, matView, matWorld;
	
	m_pTransCom->Get_WorldMatrix(&matWorld);

	_vec3 vScale = m_pTransCom->Get_Scale();
	D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
	D3DXMatrixInverse(&matScaleInv, 0, &matScale);

	_matrix matRotY, matRotZ, matRotDir;
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(60.f));
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-45.f));

	//_vec3 vec1, vec2;
	//D3DXVec3Normalize(&vec1, &m_vDirection);
	//D3DXVec3Normalize(&vec2, &vMove);

	//_float fRad = D3DXVec3Dot(&vec1, &vec2);

	D3DXMatrixIdentity(&matRotDir);
	//D3DXMatrixRotationZ(&matRotDir, -vMove.y / 180.f);
	//m_fArrowAngle += 0.01f;
	//if (m_fArrowAngle > 30.f)
	//	m_fArrowAngle = 30.f;
	//D3DXMatrixRotationZ(&matRotDir, -D3DXToRadian(40.f));
	
	matRot = matRotY * matRotZ * matRotDir;


	// 빌보드
	D3DXMatrixIdentity(&matBill);
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//memcpy(&matBill, &matView, sizeof(_matrix));
	//memset(&matBill._41, 0, sizeof(_vec3));
	//D3DXMatrixInverse(&matBill, 0, &matBill);


	_matrix newMatWorld = matScale *  matRot * matBill *matScaleInv * matWorld;

	m_pTransCom->Set_WorldMatrix(&newMatWorld);


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


	//// 충돌처리 테스트
	//_vec3 vPos, vScale;
	//_matrix matWorld;
	//m_pTransCom->Get_WorldMatrix(&matWorld);

	//m_pTransCom->Get_Info(INFO_POS, &vPos);
	//vScale = m_pTransCom->Get_Scale();
	//m_bdBox.vMin = { vPos.x - vScale.x, vPos.y - vScale.y, vPos.z - vScale.z };
	//m_bdBox.vMax = { vPos.x + vScale.x, vPos.y + vScale.y, vPos.z + vScale.z };

	//D3DXVec3TransformCoord(&m_bdBox.vMin, &m_bdBox.vMin, &matWorld);
	//D3DXVec3TransformCoord(&m_bdBox.vMax, &m_bdBox.vMax, &matWorld);


	CGameObject::LateUpdate_Object();
}

void CArrowBullet::Render_Obejct(void)
{
	if (!m_bFire || !m_bReady)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->Set_Texture(FtoDw(m_fFrame));

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


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
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	// 텍스쳐 컴객체 컴포넌트
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_ArrowBullet_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ArrowBullet_Texture", pComponent });

	// 월드행렬 컴포넌트
	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	return S_OK;
}

void CArrowBullet::CollisionEvent(CGameObject * pObj)
{
}
