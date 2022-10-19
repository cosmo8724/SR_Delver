#pragma once
#include "Component.h"


BEGIN(Engine)

/* �ܺο��� �ۼ��� ���̴��ڵ�(����)�� �����Ͽ� ��ü(ID3DXEffect)ȭ ��Ų��. */
class ENGINE_DLL CShader : public CComponent
{
public:
	explicit CShader(LPDIRECT3DDEVICE9 pDevice);
	explicit CShader(const CShader& rhs);
	virtual ~CShader() = default;

public:
	HRESULT		Ready_Shader(const _tchar* pShaderFilePath);
	
	/* �������� ���̴��� �������Ѵ�. */
	HRESULT		Begin_Shader(_uint iPassIndex);
	
	/* ���̴��� �������ϴ� ����
	���������� �����Լ��� ȣ���Ѵٸ�
	Transform�� ���� ����� �������.*/
	
	HRESULT		End_Shader();

public:
	static	CShader*		Create(LPDIRECT3DDEVICE9 pDevice, const _tchar* pShaderFilePath);
	virtual CComponent*		Clone(void);
private:
	virtual void Free(void);

private:
	/* �ܺο��� �ۼ��� ���̴� �ڵ����� ����� ����� �޾ƿ´�. 
		��, �ش� ���̴� �ڵ带 ��üȭ �� ���̴�. */
	LPD3DXEFFECT	m_pEffect = nullptr;
};

END