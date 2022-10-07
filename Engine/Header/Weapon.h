#pragma once
#include "Item.h"

BEGIN(Engine)

class CCamera;
class ENGINE_DLL CWeapon : public CItem
{
	enum DURABILITY { DUR_NORMAL, DUR_CRACKED, DUR_BROKEN, DUR_END };

protected:
	explicit CWeapon(LPDIRECT3DDEVICE9 pGraphicDev) : CItem(pGraphicDev) {};
	virtual ~CWeapon() {};

public:
	virtual HRESULT Ready_Object(void) { return S_OK; }
	virtual _int	Update_Object(const _float& fTimeDelta) { return 0; }
	virtual void	LateUpdate_Object(void) {}
	virtual	void	Render_Obejct(void) {}

protected:
	virtual void		Free(void) {}

protected:
	virtual void	LevelUp() {};
	virtual void	Charge(const _float& fTimeDelta) {};
	virtual void	Attack(const _float& fTimeDelta) {};


protected:
	// ����� �������� ������ �ý����� ����.
	_int			m_iLevel = 1;
	_int			m_iHitCnt = 0; // �� �� ���⸦ ����ߴ���
	DURABILITY		m_eDurability = DUR_END;
	CCamera*		m_pCamera = nullptr; // �÷��̾ ������
	CTransform*		m_pCenter = nullptr;

protected:
	_float		m_fPlusSpeed = 0.f;
	_float		m_fFrame = 0.f;
	_bool		m_bReady = false;
	_float		m_fTimeDelta = 0.f;

	_matrix		m_matView, m_matWorld;
	_vec3		m_vPos;
};

END
