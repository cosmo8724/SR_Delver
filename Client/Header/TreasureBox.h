#pragma once
#include "EcoObject.h"

class CTreasureBox : 	public CEcoObject
{
private:
	explicit CTreasureBox(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTreasureBox(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CTreasureBox(const CEcoObject& rhs);
	virtual ~CTreasureBox();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual void			LateUpdate_Object(void) override;
	virtual void			Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

public:
	static CTreasureBox*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f, 0.f });
	static CTreasureBox*		Create(CEcoObject* pEcoObject);
	virtual void					Free(void);
	virtual void					CollisionEvent(CGameObject *pObj);
	virtual	void					InteractEvent() 	override;

private:
	_bool		m_bReady = false;
};
