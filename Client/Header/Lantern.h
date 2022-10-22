#pragma once
#include "Equipment.h"

class CLantern : public CEquipment
{
private:
	explicit CLantern(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual ~CLantern();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual	void	Render_Obejct(void) override;

public:
	static CLantern*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void		Free(void) override;
	virtual void		CollisionEvent(CGameObject* pObj);

private:
	HRESULT		Add_Component(void);

private:
	_vec3			m_vPos;
	_float			m_fTimeDelta = 0.f;
	_bool			m_bReady = false;
	CTransform*		m_pCenter = nullptr;
	_float			m_fFrame = 0.f;
};

