#pragma once
#include "UI.h"
class CInventory;
class CShop;
class CMyButton : public CUI
{
private:
	explicit CMyButton(LPDIRECT3DDEVICE9 pGraphicDev, _vec2 vPos, wstring str);
	virtual ~CMyButton();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;

private:
	virtual HRESULT		Add_Component(void);

public:
	static CMyButton*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec2 vPos, wstring str);
	virtual void		Free(void);

private:
	_matrix			m_matWorld, m_matView, m_matProj;
	_int 			m_iFrame = 0;
	_bool			m_bShow = false;
	_vec2			m_vPos;

	RECT			m_Rect;
	_bool			m_bReady = false;
	CInventory*		m_pInv = nullptr;
	wstring			m_str;
	CShop*			m_pShop = nullptr;

	_float			m_fTime = 0.f;
};

