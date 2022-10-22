#pragma once
#include "Component.h"
#include "Texture.h"
#include "GameObject.h"

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CAnimator : public CComponent
{
private:
	explicit CAnimator(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAnimator(const CAnimator& rhs);
	virtual ~CAnimator();

public:
	const _float&		Get_Currentframe() { return m_fFrame; }
	void				Set_Frame() { m_fFrame = 0.f; }

	// 애니메이션 만들기, 실행 시키기, 바꾸기

public:
	HRESULT		Ready_Animator();

	HRESULT		Add_Component(const _tchar * pTectureTag);

	void		Play_Animation(const _float& fTimeDelta, const _float & fTimeSpeed = 1.f);
	void		Change_Animation(const _tchar * pTectureTag);
	void		Set_Texture();
	void		Set_FrameTexture(const _float fFrame = 1);
	void		Set_UITexture(const _float& fTextEnd);

private: // Key : Tag, Velaue : TextComponent
	map<const _tchar*, CTexture*>		m_mapAnimation;

public:
	static	CAnimator*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone(void);
	virtual void			Free(void);

private:
	CTexture*				m_pCurAnim = nullptr;

private:
	_float					m_fFrame;
};

END