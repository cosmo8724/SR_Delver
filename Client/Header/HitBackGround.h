#pragma once
#include "UI.h"

class CHitBackGround : public CUI
{
private:
	explicit CHitBackGround(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHitBackGround();

public:
	virtual HRESULT			Ready_Object(void)						override;
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual void			LateUpdate_Object(void)					override;
	virtual void			Render_Obejct(void)						override;

public:
	void					Set_HitBackGround(_bool bHit) { m_bHit = bHit; }

private:
	virtual HRESULT			Add_Component(void);

public:
	static CHitBackGround*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);

private:
	_matrix					m_matWorld, m_matView;

	_bool					m_bHit = false;
};