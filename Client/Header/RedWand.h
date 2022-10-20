#pragma once
#include "Weapon.h"

BEGIN(Engine)
class CShader;
END

class CRedWand : public CWeapon
{
private:
	explicit CRedWand(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRedWand(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual ~CRedWand();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual	void	Render_Obejct(void) override;

public:
	static CRedWand*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void	Free(void) override;

protected:
	virtual void	LevelUp() override;

private:
	HRESULT		Add_Component(void);

public:
	virtual void	CollisionEvent(CGameObject* pObj);
	virtual void	Charge(const _float& fTimeDelta);
	virtual void	Attack(const _float& fTimeDelta);

private:
	CShader*		m_pShaderCom = nullptr;
};

