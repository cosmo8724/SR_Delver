#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CAnimator;

class ENGINE_DLL CMonster : public CGameObject
{
public:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonster(const CMonster& rhs);
	virtual ~CMonster();

public:
	_int				Get_MonsterHp() { return m_tInfo.iHp; }
	_int				Get_MonsterAttack() { return m_tInfo.iAttack; }
	_int				Get_MonsterExp() { return m_tInfo.iExp; }

protected:
	virtual HRESULT		Ready_Object(void);
	virtual _int		Update_Object(const _float& fTimeDelta);
	virtual void		LateUpdate_Object(void) {};
	virtual void		Render_Obejct(void);

protected:
	void				Set_KnockBack(_float fHeight) { m_bKnockBack = true; m_fHeight = fHeight; }

protected:
	virtual HRESULT		Add_Component(void);

	virtual void		Target_Follow(const _float& fTimeDelta) {};
	virtual void		OnHit(const _float& fTimeDelta) {};
	virtual void		Dead() {};

	virtual void		Billboard();
	virtual void		KnockBack(const _float& fTimeDeleta, _float fKnockBackSpeed = 20.f);

public:
	virtual void		Free(void);
	static CMonster*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	MONSTERTYPE			m_eType = MOB_END;
	_vec3				m_vPos;

protected:
	_bool				m_bClone = false;
	_float				m_fIdle_Speed;
	_float				m_fAttack_Speed;

	MONTSERINFO			m_tInfo;

	wstring				m_ObjTag;
	wstring				m_textureTag;

	_float				m_fHeight = 0.f;

	// Hit
	_bool				m_bHit = false;
	_float				m_fHitTimeAcc = 0.f;

	// Dead
	_bool				m_bOneCheck = false;
	_bool				m_bRenderOFF = false;
	_float				m_fRenderOFFTimeAcc = 0.1f;

	// MiniMap
	_bool				m_bCreateIcon = false;

protected:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CAnimator*			m_pAnimtorCom = nullptr;

	_matrix				m_matOldBill;

private:
	_bool				m_bKnockBack = false;
	_float				m_fBSpeed = 0.1f;
	_float				m_fBSpeed0 = 0.1f;
	_float				m_fBAccel = 0.007f;
	_float				m_fBTimeDelta = 0.f;
};

END