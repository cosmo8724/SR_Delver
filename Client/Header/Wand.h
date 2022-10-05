#pragma once
#include "Weapon.h"

class CWand : public CWeapon
{
private:
	explicit CWand(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWand(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual ~CWand();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual	void	Render_Obejct(void) override;

public:
	static CWand*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void	Free(void) override;

protected:
	virtual void	LevelUp() override;

private:
	HRESULT		Add_Component(void);

public:
	virtual void	CollisionEvent(CGameObject* pObj);

private:
	_float		m_fFrame = 0.f;
	_bool		m_bReady = false;
	_float		m_fTimeDelta = 0.f;

private:
	_matrix		m_matView, m_matWorld;
	_vec3		m_vPos;


};

