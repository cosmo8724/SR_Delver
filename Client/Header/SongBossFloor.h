#pragma once
#include "Bullet.h"

BEGIN(Engine)

class CRcTex;
class CAnimator;

class CSongBossFloor : public CBullet
{
private:
	explicit CSongBossFloor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSongBossFloor(const CSongBossFloor& rhs);
	virtual ~CSongBossFloor();

public:
	virtual HRESULT Ready_Object(_int iBulletCount);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual	void	Render_Obejct(void) override;
	 
public:
	static CSongBossFloor*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iBulletCount);
	virtual void			Free(void) override;
	virtual void			Reset() override;

private:
	HRESULT					Add_Component(void);

private:
	CRcTex*					m_pBufferCom = nullptr;
	CAnimator*				m_pAnimtorCom = nullptr;

private:
	_float		m_fFrame = 0.f;

	_bool		m_bReady = false;
	_vec3		m_vPlayerPos = { 0.f,0.f,0.f };

	_uint		m_iBulletCount;
	_uint		m_iTransparency;

	// Timer
	_float		m_fTransparencyTimeAcc;

};

END