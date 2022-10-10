#pragma once
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRcTex;
	class CTexture;
	class CCalculator;	// 여기에 선언하지 않으면 형식지정자가 없다는 에러가 뜰 수 있다.
	class CItem;
}
class CBlock;

class CPlayer : public CGameObject
{
	enum PLAYER_STATE { PLAYER_GROUND, PLAYER_ON_BLOCK, PLAYER_JUMP, PLAYER_STATE_END };

private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;


private:
	HRESULT				Add_Component(void);
	void				Key_Input(const _float& fTimeDelta);
	void				Mouse_Move(void);
	void				Mouse_Click(const _float& fTimeDelta);
	void				Set_OnTerrain(void);
	void				Jump(const _float& fTimeDelta);
	_float				Get_Height();
	void				CollisionEvent(CGameObject * pOtherObj);
	
public:
	void				Set_HpPlus() { m_tInfo.iHp += 1; }

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;

	CBlock*		m_pCurrentBlock = nullptr;
	_bool		m_bBlockChanged = true;

	PLAYER_STATE	m_eState = PLAYER_GROUND;

	_vec3				m_vDirection;
	_float				m_fSpeed = 5.f;
	_float				m_fScale = 1.f;

	// 점프 관련 변수
	_bool				m_bJump = false;
	_float				m_fJSpeed; 		// 점프 속도
	_float				m_fJSpeed0; 	// 점프 초기 속도
	_float				m_fAccel;		// 중력가속도
	_float				m_fJTimeDelta = 0.f;

	// 마우스 입력 관련 변수
	_float				m_fLBClick = 0.f;
	_float				m_fTimeDelta = 0.f;

	// 장착템 관련 변수
	CItem*				m_pRight	= nullptr;	// 오른손템
	CItem*				m_pLeft		= nullptr;	// 왼손템

	PLAYERINFO			m_tInfo;

public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

};

