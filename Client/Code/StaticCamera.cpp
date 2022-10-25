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
	const _float& fAspect,
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

	m_eID = CAM_STATIC;

	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);

	m_bSwitch = true;

	m_fWaveSpeed = 0.5f;

	return S_OK;
}

Engine::_int CStaticCamera::Update_Object(const _float& fTimeDelta)
{
	if (!m_bSwitch)
	{
		m_fFov += 0.05f;
		if (m_fFov >= D3DXToRadian(75.f))
			m_fFov = D3DXToRadian(75.f);
		return 0;
	}


	Key_Input(fTimeDelta);
	ShakeY(fTimeDelta);
	Shake_Camera(fTimeDelta);
	Wave_Camera(fTimeDelta);
	Player_Dead(fTimeDelta);
	Target_Renewal();

	_int iExit = CCamera::Update_Object(fTimeDelta);

	return iExit;
}

void CStaticCamera::LateUpdate_Object(void)
{
	if (!m_bSwitch)
		return;


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
	if (m_bFree || Get_DIKeyState(DIK_TAB) & 0x80)
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

	if (Key_Down(DIK_Z))
		m_bFPS = !m_bFPS;
}

void CStaticCamera::Target_Renewal(void)
{
	if (m_bShakeY || m_bShake || m_bPlayerDead)
		return;

	m_pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(m_pPlayerTransCom);

	_vec3   vPos;
	m_pPlayerTransCom->Get_Info(INFO_POS, &vPos);

	_vec3   vLook;
	m_pPlayerTransCom->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);

	// 1��Ī
	if (m_bFPS)
	{
		m_vEye = vPos + 0.3f * vLook;
		m_vAt = vPos + vLook;
	}
	// 3��Ī
	else
	{
		m_vEye = vPos - 5.f * vLook;
		m_vAt = vPos;
	}
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

void CStaticCamera::Shake_Camera(const _float & fTimeDelta)
{
	if (!m_bShake)
		return;

	m_fShakeTimeNow += fTimeDelta;
	float fRand=0.f;
	if (m_fShakeTime > m_fShakeTimeNow)
	{
		fRand = (rand() % (m_iShakePower * 2) - m_iShakePower*0.5f) * 0.03f;
		m_vAt.y += fRand;
	}
	else
	{
		m_fShakeTimeNow = 0.f;
		m_bShake = false;
	}


	_vec3   vPos;
	m_pPlayerTransCom->Get_Info(INFO_POS, &vPos);

	_vec3   vLook;
	m_pPlayerTransCom->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);
	

	vLook.y += fRand;

	m_vEye = vPos + 0.3f * vLook;
	m_vAt = vPos + vLook;


}

void CStaticCamera::Wave_Camera(const _float & fTimeDelta)
{
	if (!m_bWave)
		return;

	m_fWaveTimeNow += fTimeDelta;

	if (m_fWaveTimeNow < m_fWaveTime)
	{
		_matrix matView;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixInverse(&matView, 0, &matView);

		_vec3 vRight, vUp, vLook;
		D3DXVec3Normalize(&vRight, &_vec3(matView._11, matView._12, matView._13));
		D3DXVec3Normalize(&vUp, &_vec3(matView._21, matView._22, matView._23));
		D3DXVec3Normalize(&vLook, &_vec3(matView._31, matView._32, matView._33));

		//_vec3   vLook;
		//m_pPlayerTransCom->Get_Info(INFO_LOOK, &vLook);
		//D3DXVec3Normalize(&vLook, &vLook);

		_vec3 vAxis = m_vAxis.x * vRight + m_vAxis.y * vUp + m_vAxis.z * vLook;

		m_fWaveAngleNow += m_fWaveSpeed;
		if (m_fWaveAngleNow < -m_fWaveAngle || m_fWaveAngleNow > m_fWaveAngle)
			m_fWaveSpeed *= -1;
		
		_matrix matRot;
		D3DXMatrixRotationAxis(&matRot, &vAxis, D3DXToRadian(m_fWaveAngleNow));

		_vec3 Up = { 0.f, 1.f, 0.f };
		D3DXVec3TransformCoord(&m_vUp, &Up, &matRot);
	}
	else
	{
		m_bWave = false;
		m_fWaveAngleNow = 0.f;
		m_fWaveTimeNow = 0.f;
		m_vUp = { 0.f,1.f,0.f };
	}

}

void CStaticCamera::Player_Dead(const _float & fTimeDelta)
{
	if (!m_bPlayerDead)
		return;

	//m_fDeadTime += fTimeDelta;

	if (m_fDeadAngle < 80.f)
	{
		_matrix viewInv;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &viewInv);
		D3DXMatrixInverse(&viewInv, 0, &viewInv);

		_vec3 vLook;
		D3DXVec3Normalize(&vLook, &_vec3(m_matView._31, m_matView._32, m_matView._33));

		_vec3 vAxis = vLook;

		m_fDeadAngle += 1.f;

		_matrix matRot;
		D3DXMatrixRotationAxis(&matRot, &vAxis, D3DXToRadian(m_fDeadAngle));

		_vec3 Up = { 0.f, 1.f, 0.f };
		D3DXVec3TransformCoord(&m_vUp, &Up, &matRot);
	}


}
