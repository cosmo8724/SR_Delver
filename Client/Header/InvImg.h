#pragma once
#include "Item.h"

// Stage의 아이템들을 획득 시, 인벤토리에 생기는 이미지 객체
// 아이템들과 연동되어 있어야 한다.
class CInvImg : public CItem
{
private:
	explicit CInvImg(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInvImg(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* texTag, CItem* pObj);
	virtual ~CInvImg();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;

private:
	virtual HRESULT		Add_Component(void);

public:
	static CInvImg*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* texTag, CItem* pObj);
	virtual void		Free(void);

public:
	CItem*				Get_TargetObj() { return m_pObj; }
	void				Set_Picked(_bool bPicked) { m_bPicked = bPicked; }
	void				Set_InvPos(_float fX, _float fY) { m_fPosX = fX; m_fPosY = fY; }
	_vec2				Get_InvPos() { return _vec2({ m_fPosX, m_fPosY }); }


private:
	_vec2				m_vPos;

	CItem*				m_pObj;		// 실제로 설정값들을 가지고 있는 객체
	wstring				m_TexTag;
	_matrix				m_matView, m_matWorld;
	_bool				m_bOn = false;
	_float				m_fFrame = 0.f;

	const _tchar*		m_texTag;
	_bool				m_bPicked = false;

	_matrix				m_matProj;
	_float				m_fPosX, m_fPosY, m_fScaleX, m_fScaleY;


};

