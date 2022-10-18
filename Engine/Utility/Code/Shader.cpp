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

	m_pEffect->End();

	return S_OK;
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
