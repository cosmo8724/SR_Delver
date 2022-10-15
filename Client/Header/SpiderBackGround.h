#pragma once
#include "UI.h"

class CSpiderBackGround : public CUI
{
private:
	explicit CSpiderBackGround(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpiderBackGround();

public:
	virtual HRESULT			Ready_Object(void)						override;
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual void			LateUpdate_Object(void)					override;
	virtual void			Render_Obejct(void)						override;

public:
	void					Set_SpiderRender() { m_bSpider = true; }

private:
	virtual HRESULT			Add_Component(void);

public:
	static CSpiderBackGround*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void				Free(void);

private:
	_matrix					m_matWorld, m_matView;

	// 화면에 그려지는
	_bool					m_bSpider = false;
	_float					m_bSpiderTimeAcc;

	// 흘러내림
	_float					m_fFlowdownSpeed = 0.f;
	_float					m_fFlowdownTimeAcc = 0.f;

	// 투명도
	_int					m_iTransparency = 0;
	_float					m_fTransparencyTimeAcc = 0.f;
};