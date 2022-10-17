#pragma once
#include "UI.h"

BEGIN(Engine)
class CItem;
class CCalculator;
END
class CInventory : public CUI
{
private:
	explicit CInventory(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CInventory();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;

private:
	virtual HRESULT		Add_Component(void);

public:
	static CInventory*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void	Get_Index(int& iRow, int& iCol);
	void	Set_Inventory(CItem* pItem);
	void	Pick();
	void	Set_ItemEquip();

	CItem** Get_PickedItem() { return m_ppPickedItem; }
	void	Set_PickedItemNull() {
		(*m_ppPickedItem) = nullptr;
		m_ppPickedItem = nullptr;
	}

private:
	void		Mouse_Input(const _float& fTimeDelta);

private:
	_matrix		m_matView, m_matWorld;

private:
	// 인벤토리 피킹용 변수
	_float			m_fInvWidth = 0.f;
	_float			m_fInvHeight = 0.f;
	_float			m_fTileSize = 0.f;
	_vec2			m_InvPosArr[3][6];
	CItem*			m_Inventory[3][6];
	_int			m_iMaxRow = 3;
	_int			m_iMaxCol = 6;

private:
	CCalculator*	m_pCalCom = nullptr;
	CItem**			m_ppPickedItem = nullptr;

	// 추가 변수
private:
	_float			m_fClickTime = 0.f;
	_int			m_iClickedCnt = 0;
	_bool			m_bShow = false;
	_matrix			m_matProj;
	CItem*			m_pEquipped = nullptr;

};

