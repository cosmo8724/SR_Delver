#pragma once

#include "Camera.h"
#include "Engine_Include.h"

BEGIN(Engine)
class CTransform;
END

class CStaticCamera : public Engine::CCamera
{
private:
	explicit CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStaticCamera();

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
		m_vUp = { 0.f, 1.f, 0.f };
	}


public:
	void		Set_ShakeY() { m_bShakeY = true; }
	void		Shake_Camera(_float fTime, _int iPower)
	{
		m_bShake = true;
		m_fShakeTime = fTime;
		m_iShakePower = iPower;
	}
	void		Wave_Camera(_float fTime, _vec3 vAxis, _float fAngle)
	{
		m_bWave = true;
		m_fWaveTime = fTime;
		m_fWaveAngle = fAngle;
		m_vAxis = vAxis;
	}
	void		Set_PlayerDead(_bool bPlayerDead) { m_bPlayerDead = bPlayerDead; }
	void		Set_Free(_bool bFree) { m_bFree = bFree; }

private:
	void		Key_Input(const _float& fTimeDelta);
	void		Target_Renewal(void);
	void		ShakeY(const _float& fTimeDelta);
	void		Mouse_Fix(void);
	void		Shake_Camera(const _float& fTimeDelta);
	void		Wave_Camera(const _float& fTimeDelta);
	void		Player_Dead(const _float& fTimeDelta);


private:
	_float			m_fDistance = 10.f;
	_float			m_fSpeed = 10.f;
	_float			m_fAngle = 0.f;

	_bool			m_bFPS = true;

	// ShakeY客 包访等 函荐
	_bool		m_bShakeY = false;
	_float		m_fPosY;
	_float		m_fShakeYSpeed;

	_float		m_fShakeYTimeAcc;
	_float		m_fNoShakeYTimeAcc;
	_bool		m_bFix = true;

	// Shake Camera 客 包访等 函荐
	_bool		m_bShake = false;
	_float		m_fShakeTime = 0.f;
	_float		m_fShakeTimeNow = 0.f;
	_int		m_iShakePower = 0;

	// Wave Camera 
	_bool		m_bWave = false;
	_float		m_fWaveSpeed = 0.5f;
	_float		m_fWaveAngleNow = 0.f;
	_float		m_fWaveAngle = 0.f;
	_float		m_fWaveTime = 0.f;
	_float		m_fWaveTimeNow = 0.f;
	_vec3		m_vAxis;

	// Player Dead Motion
	_bool		m_bPlayerDead = false;
	_float		m_fDeadAngle = 0.f;
	_float		m_fDeadTime = 0.f;
	
	
	_bool		m_bFree = false;



	// Releated to Motion
	CTransform* m_pPlayerTransCom = nullptr;

public:
	static CStaticCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
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

