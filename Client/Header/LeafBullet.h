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

private:
	void					Billboard();
	_int					Target(const _float& fTimeDelta);
	void					Rotation(const _float& fTimeDelta);

private:
	HRESULT					Add_Component(void);

private:
	CRcTex*					m_pBufferCom = nullptr;
	CAnimator*				m_pAnimtorCom = nullptr;

private:
	_float					m_fSpeed;
	_bool					m_bReady;
	_vec3					vPos, m_vPlayerPos = { 0.f,0.f,0.f };

	// 회전 테스트
	_matrix					m_matRot;
};

END