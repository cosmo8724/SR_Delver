#pragma once
#include "Camera.h"

BEGIN(Engine)
class CTransform;
END

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
	virtual	void Reset() {
		CCamera::Reset();
		m_bDeadMotion = false;
		m_vUp = { 0.f, 1.f, 0.f };
		m_pTargetTransCom = nullptr;
	}


private:
	void		Target_Renewal(void);
	void		DeadMotion();

public:
	void		Set_DeadMotionTrue() { m_bDeadMotion = true; }

public:
	virtual void		Set_RevAngle(_float fAngle) { m_fRevAngle = fAngle; }
	virtual void		Revolution();

private:
	_float			m_fDistance		= 10.f;
	_float			m_fSpeed		= 10.f;
	_float			m_fAngle		= 0.f;

	CTransform* m_pTargetTransCom = nullptr;

	_bool		m_bDeadMotion = false;

public:
	static CObjectCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov = D3DXToRadian(75.f),
		const _float& fAspect = (float)WINCX / WINCY,
		const _float& fNear = 0.1f,
		const _float& fFar = 1000.f);

private:
	virtual void Free(void) override;
};

