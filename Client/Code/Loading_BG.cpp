#include "stdafx.h"
#include "Loading_BG.h"

CLoading_BG::CLoading_BG(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CLoading_BG::~CLoading_BG()
{
}

HRESULT CLoading_BG::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CLoading_BG::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CLoading_BG::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CLoading_BG::Render_Obejct(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	_matrix		IMatrix = *D3DXMatrixIdentity(&IMatrix);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &IMatrix, sizeof(_matrix))))
		return;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &IMatrix, sizeof(_matrix))))
		return;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &IMatrix, sizeof(_matrix))))
		return;

	m_pTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", 0);


	m_pShaderCom->Begin_Shader(0); // pass 0

	m_pBufferCom->Render_Buffer();

	m_pShaderCom->End_Shader();


	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

HRESULT CLoading_BG::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_LoadingTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_LoadingTexture", pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(L"Proto_ShaderRect"));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ShaderRect", pComponent });

	return S_OK;
}

CLoading_BG * CLoading_BG::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLoading_BG *	pInstance = new CLoading_BG(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLoading_BG::Free(void)
{
	CGameObject::Free();
}
