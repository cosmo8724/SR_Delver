#pragma once
#include "GameObject.h"
#include "Engine_Include.h"
#include "Export_Function.h"

BEGIN(Engine)

class CTransform;
class CCubeTex;
class CTexture;
class CCalculator;

END

class CBlock : public CGameObject
{
public:
	explicit CBlock(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBlock(const CBlock& rhs);
	virtual ~CBlock();

public:
	void				SetParentBlock(CBlock* pParentBlock) { m_pParentBlock = pParentBlock; }
	void				SetBlock() { m_bSet = true; }
	void				SetChanging() { m_bChanging = !m_bChanging; }
	const _bool&		Get_IsSet() { return m_bSet; }
	_bool				IsClone() { return m_bClone; }
	_float				Get_Height() { return m_bdBox.vMax.y; }
	void				SetTextureIndex(_int Num) { m_iTexture = Num; }
	void				PlusTexture() { if (m_iTexture < m_pTextureCom->Get_FrameEnd()) m_iTexture++; }
	void				MinusTexture() { if (m_iTexture > 0) m_iTexture--; }
	_int				GetTextureIndex() { return m_iTexture; }
	void				SetBlockType(BLOCKTYPE eType) { m_eCurrentType = eType; }

public:
	virtual HRESULT Ready_Object(_vec3* vPos = nullptr);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	HRESULT			Add_Component(void);
	void				Chase_MousePT();
	HRESULT			Change_BlockType();

public:
	CBlock*			m_pParentBlock = nullptr;

	CCubeTex*		m_pBufferCom = nullptr;
	CTransform*		m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;

	BLOCKTYPE		m_eCurrentType = BLOCK_CAVE;
	BLOCKTYPE		m_eLastType = BLOCKTYPE_END;

	_bool				m_bSet = false;
	_bool				m_bClone = false;
	_bool				m_bChanging = false;
	_bool				m_bCreateIcon = false;

	_int				m_iTexture = 0;
	_float				m_fScale = 1.f;

public:
	static CBlock*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* vPos = nullptr);
	static CBlock*	Create(const CBlock& rhs);
	virtual void	Free(void);
};