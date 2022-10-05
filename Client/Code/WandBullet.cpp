#include "stdafx.h"
#include "..\Header\WandBullet.h"

#include "Export_Function.h"	
//#include "EffectMgr.h"
//#include "ExploEffect.h"

CWandBullet::CWandBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
{
	//m_pGraphicDev->AddRef();
	memset(&m_bdBox, 0, sizeof(BDBOX));
	memset(&m_bdSphere, 0, sizeof(BDSPHERE));
}

CWandBullet::CWandBullet(const CWandBullet & rhs)
	:CBullet(rhs)
{
	//m_pGraphicDev->AddRef();
	memcpy(&m_bdBox, &rhs.m_bdBox, sizeof(BDBOX));
	memcpy(&m_bdSphere, &rhs.m_bdSphere, sizeof(BDSPHERE));
}

CWandBullet::~CWandBullet()
{
}

HRESULT CWandBullet::Ready_Object(void)
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

HRESULT CWandBullet::Add_Component(void)
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

void CWandBullet::CollisionEvent(CGameObject * pObj)
{
	//_vec3 vPos;
	//m_pTransCom->Get_Info(INFO_POS, &vPos);

	//CGameObject*		pGameObject = nullptr;
	//pGameObject =		CExploEffect::Create(m_pGraphicDev, vPos);
	//NULL_CHECK(pGameObject);


	////CEffectMgr::GetInstance()->Add_Object(L"ExploEffect", pGameObject); 
	//	// 첫 하나의 이펙트를 넣어주면 복사하여 랜덤 개수/위치/크기의 오브젝트를 복사생성하여 레이어에 추가한다.
	//CLayer*		pLayer = Engine::Get_Layer(L"Layer_Effect");
	//NULL_CHECK(pLayer);
	//if (E_FAIL == pLayer->Add_GameObject(L"Layer_Effect", pGameObject))
	//{
	//	MSG_BOX("Add_Effect_Fail");
	//}

	Reset();
}

_int CWandBullet::Update_Object(const _float & fTimeDelta)
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
		m_bReady = true;
	}


	m_pTransCom->Move_Pos(&(m_fSpeed * fTimeDelta * m_vDirection));

	int iResult = CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_ALPHA, this);

	m_fLifeTime += fTimeDelta;

	return iResult;
}

void CWandBullet::LateUpdate_Object(void)
{
	if (!m_bFire)
		return;

	// 아무데도 충돌안해도 일정 시간 후 리셋
	if (3.f < m_fLifeTime)
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

void CWandBullet::Render_Obejct(void)
{
	if (!m_bFire)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pTextureCom->Set_Texture(FtoDw(m_fFrame));

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);


	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

CWandBullet * CWandBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWandBullet*		pInstance = new CWandBullet(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CWandBullet::Free(void)
{
	CGameObject::Free();
	//Safe_Release(m_pGraphicDev);
}

void CWandBullet::Reset()
{
	m_bFire = false;
	m_bDead = false;
	m_fLifeTime = 0.f;
	m_fFrame = 0.f;
	m_bReady = false;
}

