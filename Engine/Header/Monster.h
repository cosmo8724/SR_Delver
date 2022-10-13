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
	virtual ~CMonster();

public:
	_int				Get_MonsterAttack() { return m_tInfo.iAttack; }

protected:
	virtual HRESULT		Ready_Object(void) { return S_OK; };
	virtual _int		Update_Object(const _float& fTimeDelta);
	virtual void		LateUpdate_Object(void) {};
	virtual void		Render_Obejct(void);

protected:
	void				Set_KnockBack() { m_bKnockBack = true; }

protected:
	virtual HRESULT		Add_Component(void) PURE;

	virtual void		Target_Follow(const _float& fTimeDelta) {};
	virtual void		OnHit(const _float& fTimeDelta) {};
	virtual void		Dead() {};

	virtual void		Billboard();
	virtual void		KnockBack(const _float& fTimeDeleta, _float fHeight = 1.f, _float fKnockBackSpeed = 20.f);

public:
	virtual void		Free(void);

protected:
	_float				m_fIdle_Speed;
	_float				m_fAttack_Speed;

	MONTSERINFO			m_tInfo;

	wstring				m_ObjTag;
	wstring				m_textureTag;

	_float				m_fHeight = 1.f;

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

private:
	// 넉백 관련 변수
	_bool				m_bKnockBack = false;
	_float				m_fBSpeed = 0.1f;
	_float				m_fBSpeed0 = 0.1f;
	_float				m_fBAccel = 0.01f;
	_float				m_fBTimeDelta = 0.f;
};

END