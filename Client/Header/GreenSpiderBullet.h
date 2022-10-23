#pragma once
#include "Bullet.h"

BEGIN(Engine)

class CRcTex;
class CAnimator;

class CGreenSpiderBullet : public CBullet
{
private:
	explicit CGreenSpiderBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGreenSpiderBullet(const CGreenSpiderBullet& rhs);
	virtual ~CGreenSpiderBullet();

public:
	virtual HRESULT			Ready_Object(void)						override;
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual void			LateUpdate_Object(void)					override;
	virtual	void			Render_Obejct(void)						override;
	void					CollisionEvent(CGameObject* pObj)		override;

public:
	static CGreenSpiderBullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void) override;
	virtual void			Reset() override;

private:
	HRESULT					Add_Component(void);

private:
	void					Billboard();
	_int					Target(const _float& fTimeDelta);

public:
	void				Set_Target(CGameObject* pObj) { m_pTarget = pObj; }

private:
	CRcTex*					m_pBufferCom = nullptr;
	CAnimator*				m_pAnimtorCom = nullptr;

private:
	_float					m_fSpeed;
	_bool					m_bReady;
	_vec3					vPos, m_vPlayerPos = { 0.f,0.f,0.f };

	_float					m_fParticleTime = 0.f;

	CGameObject*			m_pTarget = nullptr;


};

END