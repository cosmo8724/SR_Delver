#include "stdafx.h"
#include "..\Header\StaticCamera.h"

#include "Export_Function.h"

CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
	, m_fPosY(1.f)
	, m_fShakeYSpeed(0.02f)
	, m_fShakeYTimeAcc(0.f)
	, m_fNoShakeYTimeAcc(0.f)
{
}


CStaticCamera::~CStaticCamera()
{
}

HRESULT CStaticCamera::Ready_Object(const _vec3* pEye, 
	const _vec3* pAt, 
	const _vec3* pUp,
	const _float& fFov,
	const _float& fAspect ,
	const _float& fNear,
	const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);

	return S_OK;
}

Engine::_int CStaticCamera::Update_Object(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);
	ShakeY(fTimeDelta);
	Target_Renewal();

	_int iExit = CCamera::Update_Object(fTimeDelta);

	return iExit;
}

void CStaticCamera::LateUpdate_Object(void)
{
	if (true == m_bFix)
	{
		Mouse_Fix();
	}
	CCamera::LateUpdate_Object();
}

void CStaticCamera::Free(void)
{
	CCamera::Free();
}

CStaticCamera* CStaticCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov /*= D3DXToRadian(60.f)*/, const _float& fAspect /*= (float)WINCX / WINCY*/, const _float& fNear /*= 0.1f*/, const _float& fFar /*= 1000.f*/)
{
	CStaticCamera*		pInstance = new CStaticCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CStaticCamera::Key_Input(const _float& fTimeDelta)
{
	if (Get_DIKeyState(DIK_TAB) & 0x80)
		m_bFix = false;
	else
		m_bFix = true;

	/*if (Get_DIKeyState(DIK_W) & 0x80)
		m_fDistance -= fTimeDelta * m_fSpeed;

	if(Get_DIKeyState(DIK_S) & 0x80)
		m_fDistance += fTimeDelta * m_fSpeed;

	if (Get_DIKeyState(DIK_D) & 0x80)
		m_fAngle -= D3DXToRadian(180.f) * fTimeDelta;

	if (Get_DIKeyState(DIK_A) & 0x80)
		m_fAngle += D3DXToRadian(180.f) * fTimeDelta;*/

	if (Get_DIMouseState(DIM_RB))
	{
		m_fFov -= 0.025f;
		if (m_fFov <= D3DXToRadian(30.f))
			m_fFov = D3DXToRadian(30.f);
	}
	else
	{
		m_fFov += 0.05f;
		if (m_fFov >= D3DXToRadian(75.f))
			m_fFov = D3DXToRadian(75.f);
	}
}

void CStaticCamera::Target_Renewal(void)
{
	if (m_bShakeY)
		return;

	CTransform*   pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransform);

	_vec3   vPos;
	pPlayerTransform->Get_Info(INFO_POS, &vPos);

	_vec3   vLook;
	pPlayerTransform->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);

	// 1ÀÎÄª
	//m_vEye = vPos + 0.3f * vLook;
	//m_vAt = vPos + vLook;

	// 3ÀÎÄª
	m_vEye = vPos - 5.f * vLook;
	m_vAt = vPos;
}

void CStaticCamera::ShakeY(const _float & fTimeDelta)
{
	if (!m_bShakeY)
		return;

	m_fShakeYTimeAcc += fTimeDelta;
	m_fPosY += m_fShakeYSpeed;
	Set_Eye(m_fPosY);
	if (0.1f < m_fShakeYTimeAcc)
	{
		m_fShakeYSpeed *= -1.f;
		m_fShakeYTimeAcc = 0.f;
	}

	m_fNoShakeYTimeAcc += fTimeDelta;
	if (0.5f < m_fNoShakeYTimeAcc)
	{
		m_bShakeY = false;
		m_fNoShakeYTimeAcc = 0.f;
	}
}

void CStaticCamera::Mouse_Fix(void)
{
	POINT pt{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}
