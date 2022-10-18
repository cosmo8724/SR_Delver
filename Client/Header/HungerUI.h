#pragma once
#include "UI.h"

BEGIN(Engine)
	class CAnimator;
END
class CHungerUI : public CUI
{
private:
	explicit CHungerUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHungerUI();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;

private:
	virtual HRESULT		Add_Component(void);

public:
	static CHungerUI*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

private:
	CAnimator*			m_pAnimtorCom = nullptr;

private:
	_matrix				m_matProj, m_matView, m_matOrtho, m_matWorld;
	_float				m_fFrame = 0.f;

	wstring			m_textureTag;
};

