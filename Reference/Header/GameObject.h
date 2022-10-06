#pragma once

#include "Component.h"
#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject :	public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	CComponent*				Get_Component(const _tchar* pComponentTag, COMPONENTID eID);
	BDBOX*					Get_bdBox() { return &m_bdBox; }
	BDSPHERE*				Get_bdSphere() { return &m_bdSphere; }

public:
	virtual		HRESULT		Ready_Object(void);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Obejct(void);
	virtual		void		CollisionEvent(CGameObject* pObj) {};

private:
	CComponent*		Find_Component(const _tchar* pComponentTag, COMPONENTID eID);

protected:
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	map<const _tchar*, CComponent*>		m_mapComponent[ID_END];

public:
	virtual void	Free(void);

protected:
	BDBOX		m_bdBox;
	BDSPHERE	m_bdSphere;
};

END