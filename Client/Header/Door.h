#pragma once
#include "EcoObject.h"

BEGIN(Engine)
class CCubeTex;
END
class CDoor : public CEcoObject
{
private:
	explicit CDoor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDoor(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CDoor(const CEcoObject& rhs);
	virtual ~CDoor();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

public:
	static CDoor*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f, 0.f });
	static CDoor*		Create(CEcoObject* pEcoObject);
	virtual void		Free(void);
	virtual void		CollisionEvent(CGameObject *pObj);
	virtual	void		InteractEvent() 	override;

public:
	wstring		Get_String() { return m_str; }

private:
	CCubeTex*	m_pBufferCom = nullptr;
	_bool		m_bReady = false;
	_bool		m_bMove = false;
	wstring		m_str = L"";

	_float		m_fAngle = 0.f;
	_float		m_fAngleSpeed = -1.5f;
	_matrix		m_matWorld;

};

