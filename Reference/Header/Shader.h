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
	
	/* 이제부터 셰이더로 렌더링한다. */
	HRESULT		Begin_Shader(_uint iPassIndex);
	
	/* 셰이더로 렌더링하는 순간
	정점버퍼의 렌더함수를 호출한다면
	Transform에 대한 기능은 사라진다.*/
	
	HRESULT		End_Shader();

public:
	static	CShader*		Create(LPDIRECT3DDEVICE9 pDevice, const _tchar* pShaderFilePath);
	virtual CComponent*		Clone(void);
private:
	virtual void Free(void);

private:
	/* 외부에서 작성된 셰이더 코드파일 결과를 여기로 받아온다. 
		즉, 해당 셰이더 코드를 객체화 한 것이다. */
	LPD3DXEFFECT	m_pEffect = nullptr;
};

END