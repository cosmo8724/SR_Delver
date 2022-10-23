#pragma once
#include "Bullet.h"

BEGIN(Engine)

class CRcTex;
class CAnimator;

class CLeafBullet : public CBullet
{
private:
	explicit CLeafBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLeafBullet(const CLeafBullet& rhs);
	virtual ~CLeafBullet();

public:
	virtual HRESULT			Ready_Object(void) override;
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual void			LateUpdate_Object(void) override;
	virtual	void			Render_Obejct(void) override;

public:
	static CLeafBullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void) override;
	virtual void			Reset() override;
public:
	void				Set_Target(CGameObject* pObj) { m_pTarget = pObj; }

private:
	void					Billboard();
	_int					Target(const _float& fTimeDelta);

private:
	HRESULT					Add_Component(void);

private:
	CRcTex*					m_pBufferCom = nullptr;
	CAnimator*				m_pAnimtorCom = nullptr;

private:
	_float					m_fSpeed;
	_bool					m_bReady;

	// 회전 테스트
	_matrix					m_matTempRot;
	_float					m_fDistance = 0.0f;
	_float					m_fAngle = 0.f;
	_matrix					m_matWorld;
	_vec3					m_vTrans;
	_vec3					m_vDir;

	CGameObject*			m_pTarget  =  nullptr;
};

END