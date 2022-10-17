#pragma once
#include "Bullet.h"

BEGIN(Engine)
class CRcTex;
END

class CExplodeBullet : public CBullet
{
private:
	explicit CExplodeBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CExplodeBullet(const CExplodeBullet& rhs);
	virtual ~CExplodeBullet();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual	void	Render_Obejct(void) override;

public:
	static CExplodeBullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void) override;
	virtual void			Reset() override;

private:
	HRESULT		Add_Component(void);
public:
	virtual void	CollisionEvent(CGameObject* pObj) override;

private:
	CRcTex*		m_pBufferCom = nullptr;
	_float		m_fFrame = 0.f;
	_float		m_fSpeed;
	_bool		m_bReady = false;
	_vec3		m_vDirection = { 0.f,0.f,0.f };

	//추가 변수
	_float		m_fMinSpeed;
	_float		m_fMaxSpeed;
	_float		m_fSpeedY;
	_float		m_fArrowAngle = 0.f;

	_matrix		m_matWorld;
	_float		m_fParticleTime = 0.f;
};

