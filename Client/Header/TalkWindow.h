#pragma once
#include "UI.h"

class CTalkWindow : public CUI
{
private:
	explicit CTalkWindow(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTalkWindow();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;

public:
	void				Set_OnText() { m_bText = true; m_bFinished = false; }
	void				Set_OFFText() { m_bText = false; m_bFinished = false; }
	void				Set_TextCount() { m_iTextCount++; }
	void				Set_Text(vector<wstring> *_sText) { m_vecDialogue = *_sText; }
	void				Reset_TextCount() { m_iTextCount = 0; }
	_int				Get_TextCount() { return m_iTextCount; }
	_bool				Is_Finished() { return m_bFinished; }

private:
	virtual HRESULT		Add_Component(void);

public:
	static CTalkWindow*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

private:
	_matrix				m_matView, m_matWorld;
	_float				m_fTempPosX, m_fTempPosY = 2022.f;

private:
	vector<wstring>		m_vecDialogue;
	_uint				m_iTextCount;
	_bool				m_bText = false;
	_bool				m_bFinished = false;
};