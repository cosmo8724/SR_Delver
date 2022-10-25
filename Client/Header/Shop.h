#pragma once
#include "UI.h"

class CShop : public CUI
{
private:
	explicit CShop(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShop();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;

private:
	virtual HRESULT		Add_Component(void);

public:
	static CShop*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void	Set_Open(_bool bOpen) { m_bOpen = bOpen; }
	_int	Get_Selected() { return m_iFrame; }

private:
	_matrix			m_matWorld, m_matView, m_matProj;
	_bool			m_bOpen = false;

	_int			m_iRow = 3;
	_int			m_iCol = 1;
	RECT			m_ShopArr[3][1];

	_int 			m_iFrame = 0;

	_int			m_bSelecting = false;

};

