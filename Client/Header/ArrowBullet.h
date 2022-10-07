#pragma once
#include "Bullet.h"

BEGIN(Engine)
class CCubeTex;
END

// ������ ���·� ���ư� Bullet
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
	static CArrowBullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void) override;
	virtual void			Reset() override;

private:
	HRESULT		Add_Component(void);

public:
	void		Plus_Speed(_float fSpeed) { m_fSpeed += fSpeed; m_fSpeedY += fSpeed; }

private:
	_float		m_fFrame = 0.f;
	_float		m_fSpeed;
	_bool		m_bReady = false;
	_vec3		m_vDirection = { 0.f,0.f,0.f };

	//�߰� ����
	_float		m_fMinSpeed;
	_float		m_fMaxSpeed;
	_float		m_fSpeedY;

	// �浹ó����
private:
	CCubeTex*	m_pBDBuffer = nullptr;
public:
	virtual void	CollisionEvent(CGameObject* pObj) override;
};
