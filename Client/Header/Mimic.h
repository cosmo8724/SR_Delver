#pragma once
#include "Monster.h"

class CMimic : public CMonster
{
	enum STATE { IDLE, ATTACK, HIT, DIE, MOTION_END };

private:
	explicit CMimic(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMimic(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CMimic(const CMonster& rhs);
	virtual ~CMimic();

public:
	virtual HRESULT		Ready_Object(void);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;
	virtual	void		CollisionEvent(CGameObject* pObj)		override;
	virtual	void		InteractEvent() 						override;

private:
	virtual HRESULT		Add_Component(void)						override;
	virtual void		Target_Follow(const _float& fTimeDelta);
	virtual void		OnHit(const _float& fTimeDelta)			override;
	virtual void		Dead()									override;

	void				Motion_Change();

private:
	STATE				m_ePreState;
	STATE				m_eCurState;

	_bool				m_bInteract = false;

	// Timer
	_float				m_fTimeAcc;

public:
	static CMimic*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f, 0.f });
	static CMimic*		Create(CMonster* pMonster);
	virtual void		Free(void);
};