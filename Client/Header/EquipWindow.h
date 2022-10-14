#pragma once
#include "UI.h"

BEGIN(Engine)
class CItem;
END

class CEquipWindow : public CUI
{
private:
	explicit CEquipWindow(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEquipWindow();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;

private:
	virtual HRESULT		Add_Component(void);

public:
	static CEquipWindow*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

private:
	void		Mouse_Input(const _float& fTimeDelta);

public:
	void		Set_Item(CItem* pItem, ITEMTYPE eType);

private:
	_matrix				m_matView, m_matWorld, m_matProj;

private:
	_float					m_fTempPosX, m_fTempPosY = 2022.f;

	_int			m_iMaxRow = 3;
	_int			m_iMaxCol = 2;
	_vec2			m_PosArr[3][2];
	CItem*			m_Window[3][2];
	// helmet	leftHand
	// armor	neckglace
	// pants	ring

	_float			m_fTileSize;
	_bool			m_bShow = false;


};

