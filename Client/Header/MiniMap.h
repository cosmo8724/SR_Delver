#pragma once
#include "UI.h"
#include "Icon.h"

class CMiniMap :	public CUI
{
private:
	explicit CMiniMap(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMiniMap();

public:
	const vector<CIcon*>&	Get_vecIcon() { return m_vecIcon; }

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual void			LateUpdate_Object(void)					override;
	virtual void			Render_Obejct(void)						override;

	void					Add_Icon(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pObj);

private:
	virtual HRESULT		Add_Component(void);

public:
	static CMiniMap*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);

private:
	_matrix				m_matWorld, m_matView;
	vector<CIcon*>			m_vecIcon;
};