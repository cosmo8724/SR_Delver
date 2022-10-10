#pragma once
#include "Bullet.h"
BEGIN(Engine)
class CCubeTex;
END
class CWandBullet : public CBullet
{
private:
	explicit CWandBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWandBullet(const CWandBullet& rhs);
	virtual ~CWandBullet();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual	void	Render_Obejct(void) override;

public:
	static CWandBullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void) override;
	virtual void			Reset() override;

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

	// 충돌처리용
private:
	CCubeTex*	m_pBDBuffer = nullptr;
public:
	virtual void	CollisionEvent(CGameObject* pObj) override;


};