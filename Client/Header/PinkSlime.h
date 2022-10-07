#pragma once
#include "Monster.h"

class CPinkSlime : public CMonster
{
	enum STATE { IDLE, ATTACK, HIT, DIE, MOTION_END };

private:
	explicit CPinkSlime(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPinkSlime(const CPinkSlime& rhs);
	virtual ~CPinkSlime();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;

private:
	virtual HRESULT		Add_Component(void) override;
	virtual void		Target_Follow(const _float& fTimeDelta);

	void				Motion_Change(const _float& fTimeDelta);

	void				Scale_Change();

private:
	CGameObject*		pGameObject = nullptr;

private:
	STATE				m_ePreState;
	STATE				m_eCurState;

	_float				m_fScale = 2.f;
	_float				m_fHeight = 2.f;

	_float				m_fTimeAcc;

	_float				m_fTime = 0.f; 
	_bool				m_bClone = false;

public:
	static CPinkSlime*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CPinkSlime*	Create(const CPinkSlime& rhs);
	virtual void		Free(void);
};


