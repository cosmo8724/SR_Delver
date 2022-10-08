#pragma once
#include "UI.h"

class CIcon : public CUI
{
private:
	explicit CIcon(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pObj);
	virtual ~CIcon();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual void			LateUpdate_Object(void)					override;
	virtual void			Render_Obejct(void)						override;

private:
	virtual HRESULT		Add_Component(void);
	void					RotateByPlayer();

public:
	static CIcon*			Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pObj);
	virtual void			Free(void);

private:
	_matrix				m_matWorld, m_matView;
	CGameObject*		m_pParentObj = nullptr;
	OBJECTTYPE			m_eType = OBJ_END;
	_bool					m_bOnMinimap = true;
};

