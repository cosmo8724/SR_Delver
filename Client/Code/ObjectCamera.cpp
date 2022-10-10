#include "stdafx.h"
#include "..\Header\ObjectCamera.h"
#include "Export_Function.h"



CObjectCamera::CObjectCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}

CObjectCamera::~CObjectCamera()
{
}

HRESULT CObjectCamera::Ready_Object(const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
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

_int CObjectCamera::Update_Object(const _float & fTimeDelta)
{
	Target_Renewal();
	_int iExit = CCamera::Update_Object(fTimeDelta);
	return iExit;

}

void CObjectCamera::LateUpdate_Object(void)
{
	CCamera::LateUpdate_Object();
}

void CObjectCamera::Target_Renewal(void)
{
	CTransform*   pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransform);

	_vec3   vPos;
	pPlayerTransform->Get_Info(INFO_POS, &vPos);

	_vec3   vLook;
	pPlayerTransform->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);



		m_vEye = vPos + 0.3f * vLook;
		m_vAt = vPos + vLook;

		//m_vEye = vPos - 5.f * vLook;
		//m_vAt = vPos;

}

CObjectCamera * CObjectCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CObjectCamera*		pInstance = new CObjectCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CObjectCamera::Free(void)
{
	CCamera::Free();
}
