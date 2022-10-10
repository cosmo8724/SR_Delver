#pragma once
#include "Camera.h"

class CObjectCamera : public CCamera
{
private:
	explicit CObjectCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CObjectCamera();

public:
	HRESULT Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp,
							const _float& fFov,
							const _float& fAspect,
							const _float& fNear,
							const _float& fFar);
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;

private:
	void		Target_Renewal(void);

private:
	_float			m_fDistance		= 10.f;
	_float			m_fSpeed		= 10.f;
	_float			m_fAngle		= 0.f;
	//_bool			m_bSwitch		= false;

public:
	static CObjectCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov = D3DXToRadian(60.f),
		const _float& fAspect = (float)WINCX / WINCY,
		const _float& fNear = 0.1f,
		const _float& fFar = 1000.f);

private:
	virtual void Free(void) override;
};

