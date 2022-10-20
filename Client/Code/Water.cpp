#include "stdafx.h"
#include "Water.h"
#include "Export_Function.h"
#include "StaticCamera.h"

CWater::CWater(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev), m_vDirection({0.f, 0.f, 1.f})
{
}

CWater::~CWater()
{
}

HRESULT CWater::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pTransCom->Set_Pos(10.f, 1.3f, 10.f);
	//m_pTransCom->Set_Angle(D3DXToRadian(90.f), 0.f, 0.f);


	//m_pTransCom->Set_Angle(D3DXToRadian(112.f), 0.f, 0.f);

	return S_OK;
}

_int CWater::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_fTime += fTimeDelta;
	FAILED_CHECK(m_pShaderCom->
		Set_RawValue("g_fTime", &m_fTime, sizeof(_float)), 0);

	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CWater::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();

}

void CWater::Render_Obejct(void)
{
	/*
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	//D3DMATERIAL9		tMtrl;
	//ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	//tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tMtrl.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	//tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	//tMtrl.Power = 0.f;

	//m_pGraphicDev->SetMaterial(&tMtrl);

	m_pTextureCom->Set_Texture(7);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	*/

	// Shader Test
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	_matrix		IMatrix = *D3DXMatrixIdentity(&IMatrix);
	_matrix matWorld , matView, matProj;

	//matView = matProj = IMatrix;


	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);


	D3DXMatrixTranspose(&matWorld, &matWorld);
	D3DXMatrixTranspose(&matView, &matView);
	D3DXMatrixTranspose(&matProj, &matProj);
	//CStaticCamera* pCam = dynamic_cast<CStaticCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"StaticCamera"));
	//if (nullptr != pCam)
	//{
	//	pCam->Get_ViewMatrix(matView);
	//	pCam->Get_ProjectMatrix(matProj);
	//}


	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &matWorld, sizeof(_matrix))))
		return;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &matView, sizeof(_matrix))))
		return;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &matProj, sizeof(_matrix))))
		return;

	m_pTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", 7);


	m_pShaderCom->Begin_Shader(0); // pass 0

	m_pBufferCom->Render_Buffer();

	m_pShaderCom->End_Shader();

}

HRESULT CWater::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CTerrainTex*>(Clone_Proto(L"Proto_TerrainTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TerrainTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_TerrainTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TerrainTexture", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(L"Proto_ShaderWater"));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ShaderWater", pComponent });


}

CWater * CWater::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWater *	pInstance = new CWater(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CWater::Free(void)
{
	CGameObject::Free();
}
