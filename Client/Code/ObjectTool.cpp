#include "../Default/stdafx.h"
#include "../Default/ImGui/ImGuizmo.h"
#include "ObjectTool.h"
#include "Export_Function.h"
#include "Terrain.h"
#include "Jar.h"
#include "DynamicCamera.h"

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
	ImGuizmo::BeginFrame();

	static ImGuizmo::OPERATION CurGuizmoType(ImGuizmo::TRANSLATE);
	CTerrain* pGameObject = dynamic_cast<CTerrain*>(Engine::Get_GameObject(L"Layer_Tool_Environment", L"Terrain"));
	CLayer*	pLayer = Engine::Get_Layer(L"Layer_Tool_GameLogic");
	static CEcoObject* pCurObject = nullptr;
	static	_bool	bNowCrafting = false;
	if (pGameObject)
	{
		ImGui::Text("This is ObjectTool.");
		ImGui::Text("Add Tool Logic Here...");

		const char* items[] = { "Stone", "Grass", "Tree", "Jar", "Jam"};
		static _int item_current = 0;

		ImGui::Combo("Item Type", &item_current, items, IM_ARRAYSIZE(items), 4);
		ImGui::Text("Current Object Type : %s", items[item_current]);
		if (ImGui::Button("Create") && !bNowCrafting)
		{
			bNowCrafting = true;
			CEcoObject* pEcoObject = nullptr;
			TCHAR	*	szObjTag = new TCHAR[MAX_PATH];

			switch (item_current)
			{
			case ECO_STONE:
				wsprintf(szObjTag, L"Stone_%d", m_iStoneCnt);
				m_vecObjTags.push_back(szObjTag);
				break;

			case ECO_GRASS:
				wsprintf(szObjTag, L"Grass_%d", m_iGrassCnt);
				m_vecObjTags.push_back(szObjTag);
				break;

			case ECO_TREE:
				wsprintf(szObjTag, L"Tree_%d", m_iTreeCnt);
				m_vecObjTags.push_back(szObjTag);
				break;

			case ECO_JAR:
			{
				wsprintf(szObjTag, L"Jar_%d", m_iJarCnt);
				m_vecObjTags.push_back(szObjTag);

				pEcoObject = CJar::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pEcoObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pEcoObject), E_FAIL);

				m_iJarCnt++;
				break;
			}
			case ECO_JAM:
				wsprintf(szObjTag, L"Jam_%d", m_iJamCnt);
				m_vecObjTags.push_back(szObjTag);
				break;

			case ECO_END:
				break;
			}

			if (pEcoObject)
				pCurObject = dynamic_cast<CEcoObject*>(pLayer->Get_GameObject(m_vecObjTags.back()));
		}
		
		static _int iSelectObject = -1;
		_int	 iObjectCnt = m_vecObjTags.size();
		char** szObjects = new char*[iObjectCnt];
		for (_int i = 0; i < iObjectCnt; ++i)
		{
			const TCHAR*	wszObjTag = m_vecObjTags[i];
			size_t	iLength = lstrlen(wszObjTag) + 1;
			szObjects[i] = new char[iLength];
			size_t Temp;
			wcstombs_s(&Temp, szObjects[i], iLength, wszObjTag, iLength);
		}

		ImGui::ListBox("Object List", &iSelectObject, szObjects, iObjectCnt);
		if (iSelectObject != -1)
		{
			bNowCrafting = true;
			pCurObject = dynamic_cast<CEcoObject*>(pLayer->Get_GameObject(m_vecObjTags[iSelectObject]));
		}

		if (bNowCrafting && pCurObject)
		{
			CTransform*	pTransCom = dynamic_cast<CTransform*>(pCurObject->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
			_matrix	matWorld;
			pTransCom->Get_WorldMatrix(&matWorld);
			_vec3		vPos, vScale, vAngle;
			ImGuizmo::DecomposeMatrixToComponents(matWorld, vPos, vAngle, vScale);
			vAngle = { D3DXToDegree(vAngle.x), D3DXToDegree(vAngle.y) , D3DXToDegree(vAngle.z) };
			vAngle /= 60.000f;

			ImGui::Text("ImGuizmo Type");
			if (ImGui::RadioButton("Translate", CurGuizmoType == ImGuizmo::TRANSLATE))
				CurGuizmoType = ImGuizmo::TRANSLATE;
			ImGui::SameLine();
			if (ImGui::RadioButton("Scale", CurGuizmoType == ImGuizmo::SCALE))
				CurGuizmoType = ImGuizmo::SCALE;
			ImGui::SameLine();
			if (ImGui::RadioButton("Rotate", CurGuizmoType == ImGuizmo::ROTATE))
				CurGuizmoType = ImGuizmo::ROTATE;

			CDynamicCamera* pCamera = dynamic_cast<CDynamicCamera*>(Engine::Get_GameObject(L"Layer_Tool_Environment", L"DynamicCamera"));
			_matrix	matView, matProj;
			pCamera->Get_ViewMatrix(matView);
			pCamera->Get_ProjectMatrix(matProj);
			
			ImGui::InputFloat3("Position", &vPos.x);
			ImGui::InputFloat3("Scale", &vScale.x);
			ImGui::InputFloat3("Angle", &vAngle.x);

			vAngle *= 60.000f;
			vAngle = { D3DXToRadian(vAngle.x), D3DXToRadian(vAngle.y), D3DXToRadian(vAngle.z) };
			ImGuizmo::RecomposeMatrixFromComponents(vPos, vAngle, vScale, matWorld);

			ImGuiIO& io = ImGui::GetIO();
			RECT rt;
			GetClientRect(g_hWnd, &rt);
			POINT lt{ rt.left, rt.top };
			ClientToScreen(g_hWnd, &lt);
			ImGuizmo::SetRect(lt.x, lt.y, io.DisplaySize.x, io.DisplaySize.y);

			ImGuizmo::Manipulate(matView, matProj, CurGuizmoType, ImGuizmo::WORLD, matWorld);
			ImGuizmo::DecomposeMatrixToComponents(matWorld, vPos, vAngle, vScale);
			memcpy(&pTransCom->m_vInfo[INFO_POS], vPos, sizeof(vPos));
			memcpy(&pTransCom->m_vAngle, vAngle * D3DX_PI / 180.f, sizeof(vAngle));
			memcpy(&pTransCom->m_vScale, vScale, sizeof(vScale));

			if (ImGui::Button("Apply"))
			{
				bNowCrafting = false;
				iSelectObject = -1;
			}
		}

		for (_int i = 0; i < iObjectCnt; ++i)
			Safe_Delete_Array(szObjects[i]);
		Safe_Delete_Array(szObjects);
	}
	else
		ImGui::Text("Create Terrain First.");

	ImGui::End();

	return S_OK;
}
