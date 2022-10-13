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

	m_eID = CAM_OBJECT;

	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);

	return S_OK;
}

_int CObjectCamera::Update_Object(const _float & fTimeDelta)
{
	if (!m_bSwitch)
		return 0;


	Target_Renewal();
	DeadMotion();

	//Revolution();


	_int iExit = CCamera::Update_Object(fTimeDelta);
	return iExit;

}

void CObjectCamera::LateUpdate_Object(void)
{
	if (!m_bSwitch)
		return;

	CCamera::LateUpdate_Object();
}

void CObjectCamera::Target_Renewal(void)
{
	if (nullptr == m_pTarget)
		return;


	m_pTargetTransCom = dynamic_cast<CTransform*>(m_pTarget->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(m_pTargetTransCom);

	_vec3   vPos;
	m_pTargetTransCom->Get_Info(INFO_POS, &vPos);

	_vec3   vLook;
	m_pTargetTransCom->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);

	//m_vEye = vPos	+ m_fEyeDist * vLook;
	m_vAt = vPos+ m_fAtDist	* vLook;

	_matrix matRot, matWorld, matTrans, matPos;
	D3DXMatrixTranslation(&matTrans, m_fEyeDist*vLook.x, m_fEyeDist*vLook.y, m_fEyeDist*vLook.z);
	D3DXMatrixTranslation(&matPos, vPos.x, vPos.y, vPos.z);
	
	D3DXMatrixIdentity(&matRot);
	if (m_fCurAngle < m_fRevAngle)
	{
		m_fCurAngle += 5.f;
		D3DXMatrixRotationY(&matRot, D3DXToRadian(m_fCurAngle));
	}

	matWorld = matTrans * matRot * matPos;

	m_vEye = { 0.f,0.f,0.f };
	D3DXVec3TransformCoord(&m_vEye, &m_vEye, &matWorld);


}

void CObjectCamera::DeadMotion()
{
	if (true == m_bDeadMotion)
	{
		_vec3 vLook;
		m_pTargetTransCom->Get_Info(INFO_LOOK,&vLook);
		D3DXVec3Normalize(&vLook, &vLook);

		_matrix matRot;
		m_fCurAngle += 1.f;
		if (m_fCurAngle >= 89.f)
		{
			m_fCurAngle = 89.f;
			//m_bDeadMotion = false;
		}
		D3DXMatrixRotationAxis(&matRot, &vLook, D3DXToRadian(m_fCurAngle));

		_vec3 vUp = { 0.f, 1.f, 0.f };
		D3DXVec3TransformCoord(&m_vUp, &vUp, &matRot);
		//m_vUp = { -1.f, 0.f, 0.f };
	}
}

void CObjectCamera::Revolution()
{
	//if (m_fCurAngle < m_fRevAngle)
	//{
	//	m_fCurAngle += 0.5f;
	//	D3DXMatrixRotationY(&matRot, D3DXToRadian(m_fCurAngle));
	//}
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
