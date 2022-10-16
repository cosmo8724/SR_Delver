#pragma once
#include "UI.h"

class CPlayerInfo : public CUI
{
private:
	explicit CPlayerInfo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayerInfo();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;

public:
	void				Set_OpenInfo() { m_bInfoRender = true; }
	void				Set_CloseInfo() { m_bInfoRender = false; }
	const _bool&		Get_InfoState() { return m_bInfoRender; }

	void				Set_TextCount() { m_iTextCount++; }
	void				Set_Text(vector<wstring> *_sText) { m_vecDialogue = *_sText; }
	void				Reset_TextCount() { m_iTextCount = 0; }

private:
	virtual HRESULT		Add_Component(void);

public:
	static CPlayerInfo*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

private:
	_matrix				m_matView, m_matWorld, m_matProj;

private:
	vector<wstring>		m_vecDialogue;
	_uint				m_iTextCount;

	_bool				m_bInfoRender = false;

	_tchar				m_szLevel[128];
	_tchar				m_szExp[128];
	_tchar				m_szHp[128];
	_tchar				m_szSpeed[128];
	_tchar				m_szHunger[128];
	_tchar				m_szAtk[128];
	_tchar				m_szDef[128];
};