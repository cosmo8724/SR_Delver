#pragma once
#include "Equipment.h"

class CShield : public CEquipment
{
private:
	explicit CShield(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual ~CShield();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual	void	Render_Obejct(void) override;

public:
	static CShield*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void	Free(void) override;
	virtual void	CollisionEvent(CGameObject* pObj);

private:
	HRESULT		Add_Component(void);

public:
	virtual void	Charge(const _float& fTimeDelta);
	virtual void	Attack(const _float& fTimeDelta);


private:
	_vec3			m_vPos;
	_float			m_fTimeDelta = 0.f;
	_bool			m_bReady = false;
	CTransform*		m_pCenter = nullptr;
	_float			m_fFrame = 0.f;
};

