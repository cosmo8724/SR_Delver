#pragma once
#include "Monster.h"

class CLeaf : public CMonster
{
	enum STATE { IDLE, ATTACK, HIT, DIE, MOTION_END };

private:
	explicit CLeaf(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLeaf();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;

private:
	virtual HRESULT		Add_Component(void) override;
	
	void				SKillTeleporting(const _float& fTimeDelta);
	void				Teleporting(const _float& fPlayerPosX, const _float& fPlayerPosZ);

	void				Motion_Change(const _float& fTimeDelta);

private:
	STATE				m_ePreState;
	STATE				m_eCurState;

	_vec3				m_OriginalPos;

	// Timer
	_float				m_fTimeAcc ;
	_float				m_fTeleportingTimeAcc;
	_float				m_fBulletTimeAcc;

public:
	static CLeaf*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);
};
