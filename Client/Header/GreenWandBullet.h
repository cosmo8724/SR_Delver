#pragma once
#include "Bullet.h"

BEGIN(Engine)
class CCubeTex;
END

class CGreenWandBullet : public CBullet
{
private:
	explicit CGreenWandBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGreenWandBullet(const CGreenWandBullet& rhs);
	virtual ~CGreenWandBullet();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual	void	Render_Obejct(void) override;

public:
	static CGreenWandBullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void) override;
	virtual void			Reset() override;

public:
	void		Set_Angle(_float fAngle) { m_fBulletAngle = fAngle; }

private:
	HRESULT		Add_Component(void);

private:
	CCubeTex*	m_pBufferCom = nullptr;
	_float		m_fFrame = 0.f;
	_float		m_fSpeed;// = 10.f;
	_bool		m_bReady = false;
	_vec3		m_vDirection = { 0.f,0.f,0.f };
	
	_float		m_fParticleTime = 0.f;
	_bool		m_bExplode = false;

	_float		m_fBulletAngle = 0.f;

	// 충돌처리용
private:
	CCubeTex*		m_pBDBuffer = nullptr;
public:
	virtual void	CollisionEvent(CGameObject* pObj) override;


};