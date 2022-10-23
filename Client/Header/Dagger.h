#pragma once
#include "Weapon.h"

class CDagger : public CWeapon
{
private:
	explicit CDagger(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDagger(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual ~CDagger();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual	void	Render_Obejct(void) override;

public:
	static CDagger*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void	Free(void) override;
	virtual void	CollisionEvent(CGameObject* pObj);
	virtual void	InteractEvent();

private:
	HRESULT		Add_Component(void);

public:
	virtual void	Charge(const _float& fTimeDelta);
	virtual void	Attack(const _float& fTimeDelta);

private:
	_float		m_fAttackTime = 0.f;
	_bool		m_bAttackPt = false;
	_bool		m_bParticle = false;




};

