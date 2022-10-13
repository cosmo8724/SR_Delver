#include "stdafx.h"
#include "CameraMgr.h"
#include "DynamicCamera.h"
#include "StaticCamera.h"
#include "ObjectCamera.h"
#include "Export_Function.h"

IMPLEMENT_SINGLETON(CCameraMgr)

CCameraMgr::CCameraMgr()
{
}


CCameraMgr::~CCameraMgr()
{
	Free();
}

void CCameraMgr::Ready_CameraMgr(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();
}

HRESULT CCameraMgr::Add_GameObject(CLayer * pLayer)
{
	CGameObject*		pGameObject = nullptr;

	// DynamicCamera
	pGameObject = CDynamicCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);
	m_vecCamera.push_back(static_cast<CCamera*>(pGameObject));

	// StaticCamera
	pGameObject = CStaticCamera::Create(m_pGraphicDev, &_vec3(0.f, 20.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StaticCamera", pGameObject), E_FAIL);
	m_vecCamera.push_back(static_cast<CCamera*>(pGameObject));

	// ObjectCamera
	pGameObject = CObjectCamera::Create(m_pGraphicDev, &_vec3(0.f, 20.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ObjectCamera", pGameObject), E_FAIL);
	m_vecCamera.push_back(static_cast<CCamera*>(pGameObject));


	return S_OK;
}

void CCameraMgr::Change_Camera(CAMERAID _eCur, CAMERAID _eChange)
{
	m_vecCamera[_eCur]->Set_Switch(false);
	m_vecCamera[_eCur]->Reset();
	m_vecCamera[_eChange]->Set_Switch(true);
	m_CurId = _eChange;

}

void CCameraMgr::Change_Camera(CAMERAID _eChange)
{
	m_vecCamera[m_CurId]->Set_Switch(false);
	m_vecCamera[m_CurId]->Reset();

	m_CurId = _eChange;
	m_vecCamera[_eChange]->Set_Switch(true);

}

void CCameraMgr::Set_Camera(CGameObject * pObj, _float fEyeDist, _float fAtDist)
{
	m_vecCamera[m_CurId]->Set_Target(pObj);
	m_vecCamera[m_CurId]->Set_Dist(fEyeDist, fAtDist);
}

void CCameraMgr::Action_Camera(_float fAngle)
{
	m_vecCamera[m_CurId]->Set_RevAngle(fAngle);
}

void CCameraMgr::Action_PlayerDie()
{
	// release previous camera
	m_vecCamera[m_CurId]->Set_Switch(false);
	m_vecCamera[m_CurId]->Reset();

	m_CurId = CAM_OBJECT;
	m_vecCamera[m_CurId]->Set_Switch(true);
	static_cast<CObjectCamera*>(m_vecCamera[m_CurId])->Set_DeadMotionTrue();

}

inline void CCameraMgr::Free(void)
{
	m_vecCamera.swap(vector<CCamera*>());
	Safe_Release(m_pGraphicDev);
}
