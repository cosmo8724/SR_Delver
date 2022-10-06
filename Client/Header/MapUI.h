#pragma once
#include "UI.h"

class CMapUI : public CUI
{
private:
	explicit CMapUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMapUI();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual void			LateUpdate_Object(void)					override;
	virtual void			Render_Obejct(void)						override;

private:
	virtual HRESULT		Add_Component(void);

public:
	static CMapUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);
};