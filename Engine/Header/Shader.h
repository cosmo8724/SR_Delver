#pragma once
#include "Component.h"

BEGIN(Engine)

/* 외부에서 작성된 셰이더코드(파일)을 번역하여 객체(ID3DXEffect)화 시킨다. */
class ENGINE_DLL CShader : public CComponent
{
public:
	explicit CShader(LPDIRECT3DDEVICE9 pDevice);
	explicit CShader(const CShader& rhs);
	virtual ~CShader() = default;

public:
	HRESULT		Ready_Shader(const _tchar* pShaderFilePath);
	HRESULT		Begin_Shader(_uint iPassIndex);	
	HRESULT		End_Shader();

public:
	HRESULT		Set_RawValue(D3DXHANDLE hHandle, const void* pData, _uint iLength);
	HRESULT		Set_Texture(D3DXHANDLE hHandle, IDirect3DBaseTexture9* pTexture);
private:
	LPD3DXEFFECT	m_pEffect = nullptr;


public:
	static	CShader*		Create(LPDIRECT3DDEVICE9 pDevice, const _tchar* pShaderFilePath);
	virtual CComponent*		Clone(void);
private:
	virtual void Free(void);


};

END