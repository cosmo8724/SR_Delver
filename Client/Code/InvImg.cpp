#include "stdafx.h"
#include "InvImg.h"
#include "Export_Function.h"

CInvImg::CInvImg(LPDIRECT3DDEVICE9 pGraphicDev)
	: CItem(pGraphicDev)
	, m_vPos(0.f, 0.f)
	, m_pObj(nullptr)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matWorld);
}

CInvImg::CInvImg(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* texTag, CItem* pObj)
	: CItem(pGraphicDev)
	, m_pObj(pObj)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matWorld);
	m_texTag = texTag;
}

CInvImg::~CInvImg()
{
}

HRESULT CInvImg::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");
	//if (nullptr == pLayer)
	//	MSG_BOX("CInvImg - Layer_UI 생성 실패");
	//wchar_t	num[10];
	//_itow_s(CDynamicObjMgr::GetInstance()->invImgTagCnt, num, 10);
	//wstring str = L"InvImg";
	//str = str + num;
	//CDynamicObjMgr::GetInstance()->invImgTagCnt++;
	//wchar_t	num[10];
	//_int iIndex = CDynamicObjMgr::GetInstance()->InvImgObjTag.size();
	//_itow_s(iIndex, num, 10);
	//wstring str = L"InvImg";
	//str = str + num;
	//CDynamicObjMgr::GetInstance()->InvImgObjTag.push_back(str);
	//if (E_FAIL == pLayer->Add_GameObject(CDynamicObjMgr::GetInstance()->InvImgObjTag[iIndex].c_str(), this))
	//	MSG_BOX("CInvImg - Add_GameObject 실패");

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_fScaleX = 32.f;
	m_fScaleY = 32.f;

	return S_OK;
}

_int CInvImg::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return 0;

	if (Engine::Get_DIKeyState(DIK_TAB) & 0X80)
		m_bOn = true;
	else
		m_bOn = false;

	if (!m_bOn)
		return 0;

	m_pTransCom->Set_Scale(m_fScaleX, m_fScaleY, 1.f);

	_vec3 vPos;
	if (!m_bPicked)
		vPos = { m_fPosX, m_fPosY, 0.f };
	else
	{
		POINT	ptMouse{};

		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		//// viewport -> projection 
		//D3DVIEWPORT9		ViewPort;
		//ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
		//m_pGraphicDev->GetViewport(&ViewPort);

		//_vec3	vPoint;
		//vPoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
		//vPoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
		//vPoint.z = 0.f;

		//// projection -> view space
		//_matrix matProj;
		//D3DXMatrixOrthoLH(&matProj, WINCX, WINCY, 0.f, 1.f);
		//D3DXMatrixInverse(&matProj, nullptr, &matProj);
		//D3DXVec3TransformCoord(&vPoint, &vPoint, &matProj);

		//ptMouse.x = _long(vPoint.x);
		//ptMouse.y = _long(vPoint.y);

		vPos = { (_float)ptMouse.x,(_float)ptMouse.y, 0.f };
	}
	m_pTransCom->Set_Pos(vPos.x - WINCX * 0.5f, -vPos.y + WINCY * 0.5f, 0.f);


	int iResult = CItem::Update_Object(fTimeDelta);




	/*
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matWorld);

	_float m_fScaleX = 32.f;
	_float m_fScaleY = 32.f;

	// 스케일 값
	D3DXMatrixScaling(&m_matView, m_fScaleX, m_fScaleY, 1.f);

	// 포지션
	_vec3 vPos;

	if(!m_bPicked)
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	else
	{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	// viewport -> projection
	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	_vec3	vPoint;
	vPoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vPoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vPoint.z = 0.f;

	// projection -> view space
	_matrix matProj;
	D3DXMatrixOrthoLH(&matProj, WINCX, WINCY, 0.f, 1.f);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vPoint, &vPoint, &matProj);

	ptMouse.x = _long(vPoint.x);
	ptMouse.y = _long(vPoint.y);

	vPos = { (_float)ptMouse.x,(_float)ptMouse.y, 0.f };
	}

	m_matView._41 = vPos.x;
	m_matView._42 = vPos.y;
	*/


	Add_RenderGroup(RENDER_UI, this);

	return iResult;

}

void CInvImg::LateUpdate_Object(void)
{
	if (m_bDead)
		return;

	if (!m_bOn)
		return;

	CGameObject::LateUpdate_Object();

}

void CInvImg::Render_Obejct(void)
{
	if (m_bDead)
		return;

	if (!m_bOn)
		return;

	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	_matrix		ViewMatrix;
	ViewMatrix = *D3DXMatrixIdentity(&ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->Set_Texture(_ulong(m_fFrame));

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

HRESULT CInvImg::Add_Component(void)
{

	CComponent*	pComponent = nullptr;

	// 버퍼 컴포넌트
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	// 월드행렬 컴포넌트
	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// 텍스쳐 컴객체 컴포넌트
	//_int iIndex = CDynamicObjMgr::GetInstance()->InvImgTexComTag.size();
	//CDynamicObjMgr::GetInstance()->InvImgTexComTag.push_back(m_TexTag);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(m_texTag));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ m_texTag, pComponent });

	return S_OK;
}

CInvImg * CInvImg::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* texTag, CItem* pObj)
{
	CInvImg*	pInstance = new CInvImg(pGraphicDev, texTag, pObj);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CInvImg::Free(void)
{
	CGameObject::Free();
}
