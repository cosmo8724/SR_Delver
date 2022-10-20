#pragma once
#include "Bullet.h"

BEGIN(Engine)
class CRcTex;
END

// 포물선 형태로 날아갈 Bullet
class CArrowBullet : public CBullet
{
private:
	explicit CArrowBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CArrowBullet(const CArrowBullet& rhs);
	virtual ~CArrowBullet();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual	void	Render_Obejct(void) override;

public:
	static CArrowBullet*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void) override;
	virtual void			Reset() override;

private:
	HRESULT		Add_Component(void);

public:
	void		Plus_Speed(_float fSpeed) { m_fSpeed += fSpeed; m_fSpeedY += fSpeed; }

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

public:
	virtual void	CollisionEvent(CGameObject* pObj) override;
};

