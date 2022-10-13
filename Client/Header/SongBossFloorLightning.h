#pragma once
#include "Bullet.h"

BEGIN(Engine)

class CRcTex;
class CAnimator;

class CSongBossFloorLightning : public CBullet
{
private:
	explicit CSongBossFloorLightning(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSongBossFloorLightning(const CSongBossFloorLightning& rhs);
	virtual ~CSongBossFloorLightning();

public:
	virtual HRESULT Ready_Object(_int iBulletCount);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual	void	Render_Obejct(void) override;
	 
public:
	static CSongBossFloorLightning*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iBulletCount);
	virtual void			Free(void) override;
	virtual void			Reset() override;

private:
	HRESULT					Add_Component(void);

private:
	void					Billboard();

private:
	CRcTex*					m_pBufferCom = nullptr;
	CAnimator*				m_pAnimtorCom = nullptr;

private:
	_float					m_fFrame = 0.f;

	_bool					m_bReady = false;
	_vec3					m_vPlayerPos = { 0.f,0.f,0.f };

	_uint					m_iBulletCount;

};

END