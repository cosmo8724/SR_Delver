#include "..\..\Header\Shader.h"
#include "Engine_Include.h"

USING(Engine)

CShader::CShader(LPDIRECT3DDEVICE9 pDevice)
	:CComponent(pDevice)
{
}

CShader::CShader(const CShader & rhs)
	:CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
{
	m_pEffect->AddRef();
}

HRESULT CShader::Ready_Shader(const _tchar * pShaderFilePath)
{
	if (FAILED(D3DXCreateEffectFromFile(m_pGraphicDev, pShaderFilePath,
		nullptr, nullptr, 0, nullptr, &m_pEffect, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CShader::Begin_Shader(_uint iPassIndex)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	m_pEffect->Begin(nullptr, 0);
	m_pEffect->BeginPass(iPassIndex);

	return S_OK;

}

HRESULT CShader::End_Shader()
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	m_pEffect->EndPass();
	m_pEffect->End();

	return S_OK;
}

/*	(1인자)handle에 (2인자)무형의 값(void*)을 던지겠다.
	(3인자) : 던질값의 어디부터 읽을거냐
	(4인자) : 던질값의 크기
*/
HRESULT		CShader::Set_RawValue(D3DXHANDLE hHandle, const void* pData, _uint iLength)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	/* texture와 bool 변수를 제외한 대부분의 데이터는 이 함수를 통해 값을 전달할 수 있다. */
	return m_pEffect->SetRawValue(hHandle, pData, 0, iLength);
}
HRESULT		CShader::Set_Texture(D3DXHANDLE hHandle, IDirect3DBaseTexture9* pTexture)
{
	if (nullptr == m_pEffect)
		return E_FAIL;
	return m_pEffect->SetTexture(hHandle, pTexture);
}



CShader * CShader::Create(LPDIRECT3DDEVICE9 pDevice, const _tchar* pShaderFilePath)
{
	CShader*	pInstance = new CShader(pDevice);

	if (FAILED(pInstance->Ready_Shader(pShaderFilePath)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CShader::Clone(void)
{
	return new CShader(*this);
}

void CShader::Free(void)
{
	Safe_Release(m_pEffect);
	CComponent::Free();
}
