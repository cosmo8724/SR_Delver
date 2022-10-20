#include "stdafx.h"
#include "Water.h"
#include "Export_Function.h"
#include "StaticCamera.h"

CWater::CWater(LPDIRECT3DDEVICE9 pGraphicDev, WATERTYPE eType)
	:CGameObject(pGraphicDev), m_vDirection({0.f, 0.f, 1.f})
{
	m_eType = eType;
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
	FAILED_CHECK_RETURN(m_pShaderCom->
		Set_RawValue("g_fTime", &m_fTime, sizeof(_float)), 0);


	switch (m_eType)
	{
	case WATER_FLOW1:
	{
		_float fUVSpeed = -0.5f;
		FAILED_CHECK_RETURN(m_pShaderCom->
			Set_RawValue("g_UVSpeed", &fUVSpeed, sizeof(_float)), 0);

		_float fHeight = 0.f;
		FAILED_CHECK_RETURN(m_pShaderCom->
			Set_RawValue("g_WaveHeight", &fHeight, sizeof(_float)), 0);
	}
		break;

	case WATER_FALL1:
	{
		_float fSpeed = -5.0f;
		FAILED_CHECK_RETURN(m_pShaderCom->
			Set_RawValue("g_UVSpeed", &fSpeed, sizeof(_float)), 0);

		_float fHeight = 0.f;
		FAILED_CHECK_RETURN(m_pShaderCom->
			Set_RawValue("g_WaveHeight", &fHeight, sizeof(_float)), 0);
	}

		break;

	case WATER_OCEAN1:
	{
		_float fUVSpeed = 0.f;
		FAILED_CHECK_RETURN(m_pShaderCom->
			Set_RawValue("g_UVSpeed", &fUVSpeed, sizeof(_float)), 0);

		_float fHeight = 0.5f;
		FAILED_CHECK_RETURN(m_pShaderCom->
			Set_RawValue("g_WaveHeight", &fHeight, sizeof(_float)), 0);
	}
		break;
	}


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

	switch (m_eType)
	{
	case WATER_FLOW1:
		{
		}
		break;
	case WATER_FALL1:
		{
			_matrix matRot;
			D3DXMatrixRotationX(&matRot, D3DXToRadian(90.f));
			_matrix matTrans;
			D3DXMatrixTranslation(&matTrans, 25.f, 0.f, 27.f);
			matWorld = matRot * matTrans;
		}
		break;
	case WATER_OCEAN1:
		{
			_matrix matTrans;
			D3DXMatrixTranslation(&matTrans, -3.f, -100.f, -3.f);
			matWorld = matTrans;
		}
		break;
	}

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

	switch (m_eType)
	{
	case WATER_FLOW1:
		pComponent = m_pBufferCom = dynamic_cast<CTerrainTex*>(Clone_Proto(L"Proto_WaterTexCom"));
		NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_WaterTexCom", pComponent });

		pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(L"Proto_ShaderWater"));
		NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_ShaderWater", pComponent });
		break;
	case WATER_FALL1:
		pComponent = m_pBufferCom = dynamic_cast<CTerrainTex*>(Clone_Proto(L"Proto_WaterFallTexCom"));
		NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_WaterFallTexCom", pComponent });

		pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(L"Proto_ShaderWater"));
		NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_ShaderWater", pComponent });
		break;
	case WATER_OCEAN1:
		pComponent = m_pBufferCom = dynamic_cast<CTerrainTex*>(Clone_Proto(L"Proto_OceanTexCom"));
		NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_OceanTexCom", pComponent });

		pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(L"Proto_ShaderWave"));
		NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_ShaderWave", pComponent });
		break;
	}




	

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_TerrainTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TerrainTexture", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });




}

CWater * CWater::Create(LPDIRECT3DDEVICE9 pGraphicDev, WATERTYPE eType)
{
	CWater *	pInstance = new CWater(pGraphicDev, eType);

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
