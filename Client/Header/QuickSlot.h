#pragma once
#include "UI.h"

BEGIN(Engine)
class CItem;
END

class CQuickSlot : public CUI
{
private:
	explicit CQuickSlot(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CQuickSlot();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;

private:
	virtual HRESULT		Add_Component(void);
public:
	static CQuickSlot*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);


private:
	void				Key_Input(const _float& fTimeDelta);

public:
	_bool				Is_Clicked(POINT ptMouse);
	CItem*				Get_Item() { return m_Slot[m_iRow][m_iCol];	}
	void				Set_Item(CItem* pItem);


private:
	_matrix			m_matView, m_matWorld, m_matProj;
	_float			m_fTileSize = 64.f;
	_vec2			m_PosArr[1][5];
	CItem*			m_Slot[1][5];
	_int			m_iMaxRow = 1;
	_int			m_iMaxCol = 5;
	//_bool			m_bShow = false;
	_int			m_iRow, m_iCol;

};

