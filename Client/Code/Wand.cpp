#include "stdafx.h"
#include "..\Header\Wand.h"
#include "Export_Function.h"

CWand::CWand(LPDIRECT3DDEVICE9 pGraphicDev)
	: CWeapon(pGraphicDev)
{
	m_ObjTag = L"Wand";
}

CWand::CWand(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	: CWeapon(pGraphicDev)
{
	m_vPos = vPos;
	m_ObjTag = L"Wand";
}

CWand::~CWand()
{
}

HRESULT CWand::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pTransCom->Set_Scale(0.3f, 0.3f, 0.3f);
	//m_pTransCom->Set_Pos(1.f, 2.f, 3.f);
	//_vec3 vPos = { 20.f, 1.f, 20.f };
	//m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);

	m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	m_eState = STATE_GROUND;

	// 충돌처리 테스트
	_vec3 vScale;
	_matrix matWorld;
	m_pTransCom->Get_WorldMatrix(&matWorld);

	vScale = m_pTransCom->Get_Scale();
	m_bdBox.vMin = { m_vPos.x - vScale.x, m_vPos.y - vScale.y, m_vPos.z - vScale.z };
	m_bdBox.vMax = { m_vPos.x + vScale.x, m_vPos.y + vScale.y, m_vPos.z + vScale.z };

	//NULL_CHECK(m_pTarget);

	return S_OK;
}

HRESULT CWand::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	// 버퍼 컴포넌트
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	// 텍스쳐 컴객체 컴포넌트
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_WandTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_WandTexture", pComponent });
	m_textureTag = L"Proto_WandTexture";

	// 월드행렬 컴포넌트
	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	return S_OK;

}

void CWand::CollisionEvent(CGameObject * pObj)
{
	m_eState = STATE_INV;
	m_pTransCom->Set_Pos(-1000.f, -1000.f, -1000.f);


	// 충돌처리 테스트
	_vec3 vScale, vPos;
	_matrix matWorld;
	m_pTransCom->Get_WorldMatrix(&matWorld);

	vScale = m_pTransCom->Get_Scale();
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	m_bdBox.vMin = { vPos.x - vScale.x, vPos.y - vScale.y, vPos.z - vScale.z };
	m_bdBox.vMax = { vPos.x + vScale.x, vPos.y + vScale.y, vPos.z + vScale.z };

}


_int CWand::Update_Object(const _float & fTimeDelta)
{
	int iResult = CGameObject::Update_Object(fTimeDelta);


	if (STATE_INV == m_eState)
		return 0;

	if (STATE_INV == m_eState)
	{
		//D3DXMatrixIdentity(&m_matView);
		//D3DXMatrixIdentity(&m_matWorld);

		//_float m_fScaleX = 32.f;
		//_float m_fScaleY = 32.f;

		//// 스케일 값
		//D3DXMatrixScaling(&m_matView, m_fScaleX, m_fScaleY, 1.f);

		//// 포지션
		//_vec3 vPos;
		//m_pTransCom->Get_Info(INFO_POS, &vPos);
		//m_matView._41 = vPos.x;
		//m_matView._42 = vPos.y;
		//Add_RenderGroup(RENDER_UI, this);
	}
	else
		Add_RenderGroup(RENDER_ALPHA, this);

	m_fTimeDelta = fTimeDelta;

	return iResult;
}

void CWand::LateUpdate_Object(void)
{
	if (STATE_INV == m_eState)
		return;

	if (!m_bReady)
	{
		m_pCamera = dynamic_cast<CCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"StaticCamera"));
		if (nullptr == m_pCamera)
			return;
		else
		{
			m_pCenter = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
			if (nullptr == m_pCenter)
				return;
			else m_bReady = true;
		}
	}
	_vec3* pPlayerInfo = m_pCenter->Get_InfoAll();

	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	switch (m_eState)
	{
	case STATE_GROUND:
		m_pTransCom->Revolution(pPlayerInfo, matView, 0.f, m_fTimeDelta, STATE_GROUND);
		break;
	case STATE_INV:
		//m_pTransCom->Revolution(pPlayerInfo, matView, 80.f, m_fTimeDelta);
		break;
	case STATE_EQUIP:
		m_pTransCom->Revolution(pPlayerInfo, matView, 45.f, m_fTimeDelta, STATE_EQUIP);

		break;
	}


	CGameObject::LateUpdate_Object();
}

void CWand::Render_Obejct(void)
{
	if (STATE_INV == m_eState)
		return;

	if (m_eState == STATE_INV)
	{
		//if (Engine::Get_DIKeyState(DIK_TAB) & 0X80)
		//{
		//	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
		//	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
		//}
		//else
		//	return;

	}
	else
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->Set_Texture(_ulong(m_fFrame));

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CWand * CWand::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CWand*		pInstance = new CWand(pGraphicDev, vPos);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CWand::Free(void)
{
	CGameObject::Free();
}

void CWand::LevelUp()
{
}



