#pragma once
#include "Weapon.h"

class CArrow : public CWeapon
{
private:
	explicit CArrow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CArrow(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual ~CArrow();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual	void	Render_Obejct(void) override;

public:
	static CArrow*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void	Free(void) override;

private:
	HRESULT		Add_Component(void);
	void		Set_StateEquip() { m_eState = STATE_EQUIP; }

public:
	virtual void	CollisionEvent(CGameObject* pObj);
	virtual void	Charge(const _float& fTimeDelta);
	virtual void	Attack(const _float& fTimeDelta);
private:
	_float		m_fFrame = 0.f;
	_bool		m_bReady = false;
	_float		m_fTimeDelta = 0.f;

private:
	_matrix		m_matView, m_matWorld;
	_vec3		m_vPos;

	// 추가변수
private:
	_float		m_fPlusSpeed = 0.f;
};

