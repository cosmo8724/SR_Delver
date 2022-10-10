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

		const char* items[] = { "Stone", "Grass", "Tree", "Jar", "Jam"};
		static int item_current = 0;

		ImGui::Combo("Item Type", &item_current, items, IM_ARRAYSIZE(items), 4);
		ImGui::Text("Current Object Type : %s", items[item_current]);

		switch (item_current)
		{
		case ECO_STONE:
			break;

		case ECO_GRASS:
			break;

		case ECO_TREE:
			break;

		case ECO_JAR:
			break;

		case ECO_JAM:
			break;
		}
	}
	else
		ImGui::Text("Create Terrain First.");

	ImGui::End();

	return S_OK;
}
