#pragma once
#include "EcoObject.h"

BEGIN(Engine)
class CCubeTex;
class CTransform;
END

class CStaticCamera;
class CRockFall : public CEcoObject
{
private:
	explicit CRockFall(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CRockFall(const CEcoObject& rhs);
	virtual ~CRockFall();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);
	_bool				Is_Player_Around();
	void				Activate();

public:
	static CRockFall*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f, 0.f });
	static CRockFall*	Create(CEcoObject* pEcoObject);
	virtual void		Free(void);
	virtual void		CollisionEvent(CGameObject *pObj);

public:
	_int			Get_Attack() { return m_iAttack; }

private:
	CCubeTex*		m_pBufferCom = nullptr;
	_float			m_fOriginPosY;
	_float			m_fGroundY;
	_float			m_fDist; // Distance between player and rockfall
	_float			m_fRisingSpeed;
	_float			m_fFallSpeed; 
	_float			m_fCurSpeed;
	_bool			m_bFall = true;
	_bool			m_bHit = false;
	_int			m_iAttack = 1000;

	_bool			m_bReady = false;
	CTransform*		m_pPlayerTransCom = nullptr;
	CStaticCamera*	m_pCamera = nullptr;

};

