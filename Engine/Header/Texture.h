#pragma once
#include "Component.h"

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CTexture : public CComponent
{
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture();

public:
	HRESULT		Ready_Texture(const _tchar* pPath, TEXTUREID eType, const _uint& iCnt);
	void		Set_Texture(const _uint& iIndex = 0);		// 텍스처 그리기 관련 함수
	void		Set_Texture(class CShader* pShaderCom, D3DXHANDLE hHandle, const _uint& iIndex = 0);
	
	const vector<IDirect3DBaseTexture9*>	Get_Texture() { return m_vecTexture; }
	_uint		Get_FrameEnd() { return m_iFrameEnd - 1; }

private:
	vector<IDirect3DBaseTexture9*>		m_vecTexture;
	_uint								m_iFrameEnd;

public:
	static	CTexture*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, TEXTUREID eType, const _uint& iCnt = 1);
	virtual CComponent*Clone(void);
	virtual void			Free(void);
};

END