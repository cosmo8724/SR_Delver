#pragma once
#include "Monster.h"

class CLeaf : public CMonster
{
	enum STATE { IDLE, ATTACK, HIT, DIE, MOTION_END };

private:
	explicit CLeaf(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLeaf(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual ~CLeaf();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;
	virtual	void		CollisionEvent(CGameObject* pObj)		override;

private:
	virtual HRESULT		Add_Component(void)						override;
	virtual void		OnHit(const _float& fTimeDelta)			override;
	virtual void		Dead()									override;

	void				SKillTeleporting(const _float& fTimeDelta);
	void				Teleporting(const _float& fPlayerPosX, const _float& fPlayerPosZ);

	void				Motion_Change();

private:
	STATE				m_ePreState;
	STATE				m_eCurState;

	_vec3				m_OriginalPos;

	// Timer
	_float				m_fTimeAcc ;
	_float				m_fTeleportingTimeAcc;
	_float				m_fBulletTimeAcc;

public:
	static CLeaf*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void		Free(void);
};
