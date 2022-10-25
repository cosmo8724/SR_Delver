#pragma once
#include "UI.h"

class CEndUI : public CUI
{
private:
	explicit CEndUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEndUI();

public:
	virtual HRESULT			Ready_Object(void)						override;
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual void			LateUpdate_Object(void)					override;
	virtual void			Render_Obejct(void)						override;

public:
	void					Set_EndUI() { m_bEndUI = true; }

private:
	virtual HRESULT			Add_Component(void);

public:
	static CEndUI*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void				Free(void);

private:
	_matrix					m_matWorld, m_matView;

	_bool					m_bEndUI = false;
	_float					m_EndUITimeAcc = 0.f;
};