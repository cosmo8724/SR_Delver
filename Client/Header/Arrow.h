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

public:
	virtual void	CollisionEvent(CGameObject* pObj);
	virtual void	Charge(const _float& fTimeDelta);
	virtual void	Attack(const _float& fTimeDelta);

};

