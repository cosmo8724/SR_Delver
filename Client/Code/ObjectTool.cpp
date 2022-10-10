#include "../Default/stdafx.h"
#include "ObjectTool.h"
#include "Export_Function.h"
#include "Terrain.h"

CObjectTool::CObjectTool(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CObjectTool::~CObjectTool()
{
	for (size_t i = 0; i < m_vecObjTags.size(); ++i)
		Safe_Delete_Array(m_vecObjTags[i]);
	m_vecObjTags.clear();

	Safe_Release(m_pGraphicDev);
}

HRESULT CObjectTool::ObjectTool_Window(const _float & fTimeDelta)
{
	ImGui::Begin("Object Tool");

	CTerrain* pGameObject = dynamic_cast<CTerrain*>(Engine::Get_GameObject(L"Layer_Tool_Environment", L"Terrain"));
	if (pGameObject)
	{
		ImGui::Text("This is ObjectTool.");
		ImGui::Text("Add Tool Logic Here...");
	}
	else
		ImGui::Text("Create Terrain First.");

	ImGui::End();

	return S_OK;
}
