#include "../Default/stdafx.h"
#include "../Default/ImGui/ImGuizmo.h"
#include "../Default/ImGui/ImGuiFileDialog.h"
#include "ObjectTool.h"
#include "Export_Function.h"
#include "Terrain.h"
#include "Jar.h"
#include "DynamicCamera.h"
#include "Stone.h"
#include "Grass.h"
#include "Tree.h"
#include "BonFire.h"
#include "Jam.h"
#include "ShortTorch.h"
#include "LongTorch.h"
#include "EcoMush.h"
#include "EcoWeb.h"
#include "Statue.h"
#include "RockFall.h"
#include "TreasureBox.h"

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
	//CTerrain* pGameObject = dynamic_cast<CTerrain*>(Engine::Get_GameObject(L"Layer_Tool_Environment", L"Terrain"));
	CLayer*	pLayer = Engine::Get_Layer(L"Layer_Tool_GameLogic");
	static CEcoObject* pCurObject = nullptr;
	static	_bool	bNowCrafting = false;
	//if (pGameObject)
	//{
		const char* items[] = { "Stone", "Grass", "Tree", "Jar", "Bonfire", "Jam", "Long Torch", "Short Torch", "MushRoom", "Web", "Statue", "RockFall", "Door", "TreasureBox"};
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

				pEcoObject = CStone::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pEcoObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pEcoObject), E_FAIL);

				m_iStoneCnt++;
				break;

			case ECO_GRASS:
				wsprintf(szObjTag, L"Grass_%d", m_iGrassCnt);
				m_vecObjTags.push_back(szObjTag);

				pEcoObject = CGrass::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pEcoObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pEcoObject), E_FAIL);

				m_iGrassCnt++;
				break;

			case ECO_TREE:
				wsprintf(szObjTag, L"Tree_%d", m_iTreeCnt);
				m_vecObjTags.push_back(szObjTag);

				pEcoObject = CTree::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pEcoObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pEcoObject), E_FAIL);

				m_iTreeCnt++;
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
			case ECO_BONFIRE:
			{
				wsprintf(szObjTag, L"BonFire_%d", m_iBonFireCnt);
				m_vecObjTags.push_back(szObjTag);

				pEcoObject = CBonFire::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pEcoObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pEcoObject), E_FAIL);

				m_iBonFireCnt++;
				break;
			}
			case ECO_JAM:
			{
				wsprintf(szObjTag, L"Jam_%d", m_iJamCnt);
				m_vecObjTags.push_back(szObjTag);

				pEcoObject = CJam::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pEcoObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pEcoObject), E_FAIL);

				m_iJamCnt++;
				break;
			}
			case ECO_TORCH2:
			{
				wsprintf(szObjTag, L"ShortTorch_%d", m_iShortTorchCnt);
				m_vecObjTags.push_back(szObjTag);

				pEcoObject = CShortTorch::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pEcoObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pEcoObject), E_FAIL);

				m_iShortTorchCnt++;
				break;
			}
			case ECO_TORCH1:
			{
				wsprintf(szObjTag, L"LongTorch_%d", m_iLongTorchCnt);
				m_vecObjTags.push_back(szObjTag);

				pEcoObject = CLongTorch::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pEcoObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pEcoObject), E_FAIL);

				m_iLongTorchCnt++;
				break;
			}
			case ECO_MUSHROOM:
			{
				wsprintf(szObjTag, L"Mushroom_%d", m_iMushroomCnt);
				m_vecObjTags.push_back(szObjTag);

				pEcoObject = CEcoMush::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pEcoObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pEcoObject), E_FAIL);

				m_iMushroomCnt++;
				break;
			}
			case ECO_WEB:
			{
				wsprintf(szObjTag, L"Web_%d", m_iWebCnt);
				m_vecObjTags.push_back(szObjTag);

				pEcoObject = CEcoWeb::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pEcoObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pEcoObject), E_FAIL);

				m_iWebCnt++;
				break;
			}
			case ECO_STATUE:
			{
				wsprintf(szObjTag, L"Statue_%d", m_iStatueCnt);
				m_vecObjTags.push_back(szObjTag);

				pEcoObject = CStatue::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pEcoObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pEcoObject), E_FAIL);

				m_iStatueCnt++;
				break;
			}
			case ECO_ROCKFALL:
			{
				wsprintf(szObjTag, L"RockFall_%d", m_iRockFallCnt);
				m_vecObjTags.push_back(szObjTag);

				pEcoObject = CRockFall::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pEcoObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pEcoObject), E_FAIL);

				m_iRockFallCnt++;
				break;
			}
			case ECO_DOOR:
			{
				break;
			}
			case ECO_TREASUREBOX:
			{
				wsprintf(szObjTag, L"TreasureBox_%d", m_iTreasureBoxCnt);
				m_vecObjTags.push_back(szObjTag);

				pEcoObject = CTreasureBox::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pEcoObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pEcoObject), E_FAIL);

				m_iTreasureBoxCnt++;
				break;
			}

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
			ImGuizmo::SetRect(float(lt.x), float(lt.y), io.DisplaySize.x, io.DisplaySize.y);

			ImGuizmo::Manipulate(matView, matProj, CurGuizmoType, ImGuizmo::WORLD, matWorld);
			ImGuizmo::DecomposeMatrixToComponents(matWorld, vPos, vAngle, vScale);
			memcpy(&pTransCom->m_vInfo[INFO_POS], vPos, sizeof(vPos));
			memcpy(&pTransCom->m_vAngle, vAngle * D3DX_PI / 180.f, sizeof(vAngle));
			memcpy(&pTransCom->m_vScale, vScale, sizeof(vScale));

			ImGui::BulletText("Current Texture : %d (0 ~ %d)", pCurObject->Get_CurrentTexture(), pCurObject->Get_TextureCom()->Get_FrameEnd());
			ImGui::SameLine();
			if (ImGui::Button("<"))
				pCurObject->MinusTexture();
			ImGui::SameLine();
			if (ImGui::Button(">"))
				pCurObject->PlusTexture();

			if (ImGui::Button("Apply") || Mouse_Down(DIM_RB))
			{
				bNowCrafting = false;
				iSelectObject = -1;
			}
		}

		if (ImGui::Button("Save"))
		{
			if (bNowCrafting)
			{
				bNowCrafting = false;
				iSelectObject = -1;
			}
			ImGuiFileDialog::Instance()->OpenDialog("SaveEcoObject", "Choose Folder", nullptr, ".", 1, nullptr, ImGuiFileDialogFlags_Modal);
		}
		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			if (bNowCrafting)
			{
				bNowCrafting = false;
				iSelectObject = -1;
			}
			ImGuiFileDialog::Instance()->OpenDialog("LoadEcoObject", "Choose Folder", ".dat", ".", 1, nullptr, ImGuiFileDialogFlags_Modal);
		}

		if (ImGuiFileDialog::Instance()->Display("SaveEcoObject"))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				string	strPath = ImGuiFileDialog::Instance()->GetCurrentPath();
				strPath += "\\EcoObject.dat";
				const char* pPath = strPath.c_str();
				int iLength = strlen(pPath) + 1;
				TCHAR* wpPath = new TCHAR[iLength];
				size_t	Temp;
				mbstowcs_s(&Temp, wpPath, iLength, pPath, iLength);

				HANDLE	hFile = CreateFile(wpPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

				if (INVALID_HANDLE_VALUE == hFile)
					return E_FAIL;

				DWORD	dwByte = 0;
				DWORD	dwStrByte = 0;

				auto		mapGameObject = pLayer->Get_mapGameObject();
				for (auto iter = mapGameObject->begin(); iter != mapGameObject->end(); iter++)
				{
					CEcoObject*	pEcoObject = dynamic_cast<CEcoObject*>(iter->second);
					dwStrByte = sizeof(TCHAR) * (lstrlen(iter->first) + 1);
					if (pEcoObject)
					{
						CTransform*	pTransCom = dynamic_cast<CTransform*>(pEcoObject->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
						
						WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
						WriteFile(hFile, iter->first, dwStrByte, &dwByte, nullptr);
						WriteFile(hFile, &pEcoObject->m_eType, sizeof(ECOOBJTYPE), &dwByte, nullptr);
						WriteFile(hFile, &pTransCom->m_vInfo[INFO_RIGHT], sizeof(_vec3), &dwByte, nullptr);
						WriteFile(hFile, &pTransCom->m_vInfo[INFO_UP], sizeof(_vec3), &dwByte, nullptr);
						WriteFile(hFile, &pTransCom->m_vInfo[INFO_LOOK], sizeof(_vec3), &dwByte, nullptr);
						WriteFile(hFile, &pTransCom->m_vInfo[INFO_POS], sizeof(_vec3), &dwByte, nullptr);
						WriteFile(hFile, &pTransCom->m_vAngle, sizeof(_vec3), &dwByte, nullptr);
						WriteFile(hFile, &pTransCom->m_vScale, sizeof(_vec3), &dwByte, nullptr);
						WriteFile(hFile, &pTransCom->m_matWorld, sizeof(_matrix), &dwByte, nullptr);
						WriteFile(hFile, &pEcoObject->Get_CurrentTexture(), sizeof(_int), &dwByte, nullptr);
					}
				}
				CloseHandle(hFile);

				Safe_Delete_Array(wpPath);
				ImGuiFileDialog::Instance()->Close();
			}
			if (!ImGuiFileDialog::Instance()->IsOk())
			{
				ImGuiFileDialog::Instance()->Close();
			}
		}

		if (ImGuiFileDialog::Instance()->Display("LoadEcoObject"))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				if (!m_vecObjTags.empty())
				{
					for (size_t i = 0; i < m_vecObjTags.size(); ++i)
					{
						pLayer->Delete_GameObject(m_vecObjTags[i]);
						Safe_Delete_Array(m_vecObjTags[i]);
					}
					m_vecObjTags.clear();
				}
				m_iStoneCnt = 0;
				m_iGrassCnt = 0;
				m_iTreeCnt = 0;
				m_iJarCnt = 0;
				m_iBonFireCnt = 0;
				m_iJamCnt = 0;
				m_iShortTorchCnt = 0;
				m_iLongTorchCnt = 0;
				m_iMushroomCnt = 0;
				m_iWebCnt = 0;
				m_iStatueCnt = 0;
				m_iRockFallCnt = 0;
				m_iLadderCnt = 0;
				m_iTreasureBoxCnt = 0;

				string	strPath = ImGuiFileDialog::Instance()->GetFilePathName();
				const char* pPath = strPath.c_str();
				int iLength = strlen(pPath) + 1;
				TCHAR* wpPath = new TCHAR[iLength];
				size_t	Temp;
				mbstowcs_s(&Temp, wpPath, iLength, pPath, iLength);

				HANDLE	hFile = CreateFile(wpPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

				if (INVALID_HANDLE_VALUE == hFile)
					return E_FAIL;

				DWORD	dwByte = 0;
				DWORD	dwStrByte = 0;
				CEcoObject* pEcoObject = CEcoObject::Create(m_pGraphicDev);
				CEcoObject* pCloneObject = nullptr;
				CTransform*	pTransCom = dynamic_cast<CTransform*>(pEcoObject->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));

				while (true)
				{
					ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

					TCHAR*	pName = new TCHAR[dwStrByte];
					ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

					ReadFile(hFile, &pEcoObject->m_eType, sizeof(ECOOBJTYPE), &dwByte, nullptr);
					ReadFile(hFile, &pTransCom->m_vInfo[INFO_RIGHT], sizeof(_vec3), &dwByte, nullptr);
					ReadFile(hFile, &pTransCom->m_vInfo[INFO_UP], sizeof(_vec3), &dwByte, nullptr);
					ReadFile(hFile, &pTransCom->m_vInfo[INFO_LOOK], sizeof(_vec3), &dwByte, nullptr);
					ReadFile(hFile, &pTransCom->m_vInfo[INFO_POS], sizeof(_vec3), &dwByte, nullptr);
					ReadFile(hFile, &pTransCom->m_vAngle, sizeof(_vec3), &dwByte, nullptr);
					ReadFile(hFile, &pTransCom->m_vScale, sizeof(_vec3), &dwByte, nullptr);
					ReadFile(hFile, &pTransCom->m_matWorld, sizeof(_matrix), &dwByte, nullptr);
					_int	iTexture = 0;
					ReadFile(hFile, &iTexture, sizeof(_int), &dwByte, nullptr);
					pEcoObject->Set_CurrentTexture(iTexture);

					if (0 == dwByte)
					{
						Safe_Release(pEcoObject);
						Safe_Delete_Array(wpPath);
						Safe_Delete_Array(pName);
						break;
					}

					m_vecObjTags.push_back(pName);
					switch (pEcoObject->m_eType)
					{
					case ECO_STONE:
						pCloneObject = CStone::Create(pEcoObject);
						m_iStoneCnt++;
						break;

					case ECO_GRASS:
						pCloneObject = CGrass::Create(pEcoObject);
						m_iGrassCnt++;
						break;

					case ECO_TREE:
						pCloneObject = CTree::Create(pEcoObject);
						m_iTreeCnt++;
						break;

					case ECO_JAR:
						pCloneObject = CJar::Create(pEcoObject);
						m_iJarCnt++;
						break;

					case ECO_BONFIRE:
						pCloneObject = CBonFire::Create(pEcoObject);
						m_iBonFireCnt++;
						break;

					case ECO_JAM:
						pCloneObject = CJam::Create(pEcoObject);
						m_iJamCnt++;
						break;

					case ECO_TORCH2:
						pCloneObject = CShortTorch::Create(pEcoObject);
						m_iShortTorchCnt++;
						break;

					case ECO_TORCH1:
						pCloneObject = CLongTorch::Create(pEcoObject);
						m_iLongTorchCnt++;
						break;

					case ECO_MUSHROOM:
						pCloneObject = CEcoMush::Create(pEcoObject);
						m_iMushroomCnt++;
						break;

					case ECO_WEB:
						pCloneObject = CEcoWeb::Create(pEcoObject);
						m_iWebCnt++;
						break;

					case ECO_STATUE:
						pCloneObject = CStatue::Create(pEcoObject);
						m_iStatueCnt++;
						break;

					case ECO_ROCKFALL:
						pCloneObject = CRockFall::Create(pEcoObject);
						m_iRockFallCnt++;
						break;

					case ECO_TREASUREBOX:
						pCloneObject = CTreasureBox::Create(pEcoObject);
						m_iTreasureBoxCnt++;
						break;
					}
					pLayer->Add_GameObject(m_vecObjTags.back(), pCloneObject);
				}
				CloseHandle(hFile);
				ImGuiFileDialog::Instance()->Close();
			}
			if (!ImGuiFileDialog::Instance()->IsOk())
			{
				ImGuiFileDialog::Instance()->Close();
			}
		}

		for (_int i = 0; i < iObjectCnt; ++i)
			Safe_Delete_Array(szObjects[i]);
		Safe_Delete_Array(szObjects);
	//}
	//else
	//	ImGui::Text("Create Terrain First.");

	ImGui::End();

	return S_OK;
}
