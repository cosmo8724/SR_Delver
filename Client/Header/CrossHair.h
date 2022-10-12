#pragma once
#include "UI.h"

class CCrossHair : public CUI
{
private:
	explicit CCrossHair(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCrossHair();

public:
	virtual HRESULT			Ready_Object(void)						override;
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual void			LateUpdate_Object(void)					override;
	virtual void			Render_Obejct(void)						override;

public:
	void					Set_CrossHair(_bool bWeapon) { m_bWeapon = bWeapon; }

private:
	virtual HRESULT			Add_Component(void);

public:
	static CCrossHair*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);

private:
	_matrix					m_matWorld, m_matView;

	_bool					m_bWeapon = false;
};