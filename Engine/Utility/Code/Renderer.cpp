#include "..\..\Header\Renderer.h"
#include "Management.h"
#include "Camera.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer()
{
}


CRenderer::~CRenderer()
{
	Free();
}

void CRenderer::Add_RenderGroup(RENDERID eID, CGameObject * pGameObject)
{
	if (eID >= RENDER_END || nullptr == pGameObject)
		return;

	m_RenderGroup[eID].push_back(pGameObject);
	pGameObject->AddRef();
}

void CRenderer::Render_GameObject(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	for (_uint i = 0; i < RENDER_END; ++i)
	{
		Set_RenderState(pGraphicDev, i);

		for (auto& iter : m_RenderGroup[i])
		{
			iter->Render_Obejct();
			Safe_Release(iter);			// ������ �ƴ�, ���۷��� ī��Ʈ ����
		}
		m_RenderGroup[i].clear();
	}
}

void CRenderer::Clear_RenderGroup(void)
{
	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}
}

void CRenderer::Set_RenderState(LPDIRECT3DDEVICE9 & pGraphicDev, _int _i)
{
	if (_i == RENDER_UI)	//��������
	{
		_matrix				m_matProj, m_matView, m_matOrtho;
		D3DXMatrixIdentity(&m_matView);
		//D3DXMatrixIdentity(&m_matWorld);

		//// ������ ��
		//D3DXMatrixScaling(&m_matView, 100.f, 25.f, 1.f);

		//////// ������
		//m_matView._41 = -250.f;
		//m_matView._42 = -250.f;

		D3DXMatrixOrthoLH(&m_matOrtho, WINCX, WINCY, 0.f, 1.f);

		//pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
		pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matOrtho);
	}
	else //��������
	{
		CGameObject* pObj = nullptr;

		pObj = CManagement::GetInstance()->Get_GameObject(L"Layer_Environment", L"StaticCamera");

		if (!pObj)
			pObj = CManagement::GetInstance()->Get_GameObject(L"Layer_Tool_Environment", L"DynamicCamera");

		_matrix matView, matProj;
		D3DXMatrixIdentity(&matProj);
		if (pObj == nullptr)
		{
			pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
		}
		else
		{
			static_cast<CCamera*>(pObj)->Get_ViewMatrix(matView);
			static_cast<CCamera*>(pObj)->Get_ProjectMatrix(matProj);

			//pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
			pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
		}
	}
}

void Engine::CRenderer::Free(void)
{
	Clear_RenderGroup();
}
