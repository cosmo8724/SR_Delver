#pragma once
#include "EcoObject.h"

BEGIN(Engine)
class CCubeTex;
class CTransform;
END


class CKnifeTrap : public CEcoObject
{
private:
	explicit CKnifeTrap(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CKnifeTrap(const CEcoObject& rhs);
	virtual ~CKnifeTrap();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);
	_bool				Is_Player_Around();
	void				Activate(const _float& fTimeDelta);

public:
	static CKnifeTrap*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f, 0.f });
	static CKnifeTrap*	Create(CEcoObject* pEcoObject);
	virtual void		Free(void);
	virtual void		CollisionEvent(CGameObject *pObj);

public:
	_int			Get_Attack() { return m_iAttack; }
	void			Set_GroundY(_float y) { m_fGroundY = y; }
	void			Set_TopY(_float y) { m_fTopPosY = y; }


private:
	_float			m_fDist; // Distance between player and rockfall
	_float			m_fTopPosY;
	_float			m_fGroundY;
	_float			m_fSpeed;
	_float			m_fCurSpeed;
	_bool			m_bFall = true;
	_int			m_iAttack = 2;
	_bool			m_bReady = false;
	CTransform*		m_pPlayerTransCom = nullptr;
	_bool			m_bActivate = false;

	_float			m_bChargeTime; // = 0.f;
	_bool			m_bCharge = false;

};

