#include "stdafx.h"
#include "../Default/ImGui/ImGuizmo.h"
#include "../Default/ImGui/ImGuiFileDialog.h"
#include "Export_Function.h"
#include "InfoTool.h"

#include "Monster.h"
#include "GreenSlime.h"
#include "PinkSlime.h"
#include "Fist.h"
#include "BlueBat.h"
#include "BrownBat.h"
#include "Stick.h"
#include "Leaf.h"
#include "SkeletonGhost.h"
#include "GreenSpider.h"
#include "Mimic.h"
#include "SongBoss.h"
#include "DynamicCamera.h"

#define		AND			ImGui::SameLine()
#define		WIDTH(num)	PushItemWidth(num)k
#define		ENTER		ImGui::NewLine()

USING(ImGui)

CInfoTool::CInfoTool(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CInfoTool::~CInfoTool()
{
	for (size_t i = 0; i < m_vecObjTags.size(); ++i)
		Safe_Delete_Array(m_vecObjTags[i]);
	m_vecObjTags.clear();

	Safe_Release(m_pGraphicDev);
}

HRESULT CInfoTool::InfoTool_Window(const _float & fTimeDelta)
{
	ImGui::Begin("Monster Tool");
	ImGuizmo::BeginFrame();

	static ImGuizmo::OPERATION CurGuizmoType(ImGuizmo::TRANSLATE);
	//CTerrain* pGameObject = dynamic_cast<CTerrain*>(Engine::Get_GameObject(L"Layer_Tool_Environment", L"Terrain"));
	CLayer*	pLayer = Engine::Get_Layer(L"Layer_Tool_GameLogic");
	static Engine::CMonster* pCurObject = nullptr;
	static	_bool		bNowCrafting = false;

	const char* Monsters[] = { "Green Slime", "Pink Slime", "Fist", "Blue Bat", "Brown Bat", "Stick", "Leaf", "Skeleton Ghost", "Green Spider", "Mimic", "Octopus", "Song Boss" };
	static _int iCurrent_Monster = 0;

	ImGui::Combo("Monster Type", &iCurrent_Monster, Monsters, IM_ARRAYSIZE(Monsters), 4);
	ImGui::Text("Current Monster Type : %s", Monsters[iCurrent_Monster]);
	if (ImGui::Button("Create") && !bNowCrafting)
	{
		bNowCrafting = true;
		Engine::CMonster* pMonster = nullptr;
		TCHAR	*	szObjTag = new TCHAR[MAX_PATH];

		switch (iCurrent_Monster)
		{
		case MOB_GREENSLIME:
			wsprintf(szObjTag, L"GreenSlime_%d", m_iGreenSlimeCnt);
			m_vecObjTags.push_back(szObjTag);

			pMonster = CGreenSlime::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pMonster, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pMonster), E_FAIL);

			m_iGreenSlimeCnt++;
			break;

		case MOB_PINKSLIME:
			wsprintf(szObjTag, L"PinkSlime_%d", m_iPinkSlimeCnt);
			m_vecObjTags.push_back(szObjTag);

			pMonster = CPinkSlime::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pMonster, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pMonster), E_FAIL);

			m_iPinkSlimeCnt++;
			break;

		case MOB_FIST:
			wsprintf(szObjTag, L"Fist_%d", m_iFistCnt);
			m_vecObjTags.push_back(szObjTag);

			pMonster = CFist::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pMonster, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pMonster), E_FAIL);

			m_iFistCnt++;
			break;

		case MOB_BLUEBAT:
		{
			wsprintf(szObjTag, L"BlueBat_%d", m_iBlueBatCnt);
			m_vecObjTags.push_back(szObjTag);

			pMonster = CBlueBat::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pMonster, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pMonster), E_FAIL);

			m_iBlueBatCnt++;
			break;
		}
		case MOB_BROWNBAT:
		{
			wsprintf(szObjTag, L"BrownBat_%d", m_iBrownBatCnt);
			m_vecObjTags.push_back(szObjTag);

			pMonster = CBrownBat::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pMonster, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pMonster), E_FAIL);

			m_iBrownBatCnt++;
			break;
		}
		case MOB_STICK:
		{
			wsprintf(szObjTag, L"Stick_%d", m_iStickCnt);
			m_vecObjTags.push_back(szObjTag);

			pMonster = CStick::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pMonster, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pMonster), E_FAIL);

			m_iStickCnt++;
			break;
		}
		case MOB_LEAF:
		{
			wsprintf(szObjTag, L"Leaf_%d", m_iLeafCnt);
			m_vecObjTags.push_back(szObjTag);

			pMonster = CLeaf::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pMonster, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pMonster), E_FAIL);

			m_iLeafCnt++;
			break;
		}
		case MOB_SKELETONGHOST:
		{
			wsprintf(szObjTag, L"SkeletonGhost_%d", m_iSkeletonGhostCnt);
			m_vecObjTags.push_back(szObjTag);

			pMonster = CSkeletonGhost::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pMonster, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pMonster), E_FAIL);

			m_iSkeletonGhostCnt++;
			break;
		}
		case MOB_GREENSPIDER:
		{
			wsprintf(szObjTag, L"GreenSpider_%d", m_iGreenSpiderCnt);
			m_vecObjTags.push_back(szObjTag);

			pMonster = CGreenSpider::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pMonster, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pMonster), E_FAIL);

			m_iGreenSpiderCnt++;
			break;
		}
		case MOB_MIMIC:
		{
			wsprintf(szObjTag, L"Mimic_%d", m_iMimicCnt);
			m_vecObjTags.push_back(szObjTag);

			pMonster = CMimic::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pMonster, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pMonster), E_FAIL);

			m_iMimicCnt++;
			break;
		}
		case MOB_OCTOPUS:
		{
			/*wsprintf(szObjTag, L"Statue_%d", m_iStatueCnt);
			m_vecObjTags.push_back(szObjTag);

			pMonster = CStatue::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pMonster, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pMonster), E_FAIL);

			m_iStatueCnt++;*/
			break;
		}
		case MOB_SONGBOSS:
		{
			wsprintf(szObjTag, L"SongBoss_%d", m_iSongBossCnt);
			m_vecObjTags.push_back(szObjTag);

			pMonster = CSongBoss::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pMonster, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pMonster), E_FAIL);

			m_iSongBossCnt++;
			break;
		}

		case ECO_END:
			break;
		}

		if (pMonster)
			pCurObject = dynamic_cast<Engine::CMonster*>(pLayer->Get_GameObject(m_vecObjTags.back()));
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
		pCurObject = dynamic_cast<Engine::CMonster*>(pLayer->Get_GameObject(m_vecObjTags[iSelectObject]));
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
		ImGuiFileDialog::Instance()->OpenDialog("SaveMonster", "Choose Folder", nullptr, ".", 1, nullptr, ImGuiFileDialogFlags_Modal);
	}
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		if (bNowCrafting)
		{
			bNowCrafting = false;
			iSelectObject = -1;
		}
		ImGuiFileDialog::Instance()->OpenDialog("LoadMonster", "Choose Folder", ".dat", ".", 1, nullptr, ImGuiFileDialogFlags_Modal);
	}

	if (ImGuiFileDialog::Instance()->Display("SaveMonster"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			string	strPath = ImGuiFileDialog::Instance()->GetCurrentPath();
			strPath += "\\Monsters.dat";
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
				Engine::CMonster*	pMonster = dynamic_cast<Engine::CMonster*>(iter->second);
				dwStrByte = sizeof(TCHAR) * (lstrlen(iter->first) + 1);
				if (pMonster)
				{
					CTransform*	pTransCom = dynamic_cast<CTransform*>(pMonster->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));

					WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
					WriteFile(hFile, iter->first, dwStrByte, &dwByte, nullptr);
					WriteFile(hFile, &pTransCom->m_vInfo[INFO_RIGHT], sizeof(_vec3), &dwByte, nullptr);
					WriteFile(hFile, &pTransCom->m_vInfo[INFO_UP], sizeof(_vec3), &dwByte, nullptr);
					WriteFile(hFile, &pTransCom->m_vInfo[INFO_LOOK], sizeof(_vec3), &dwByte, nullptr);
					WriteFile(hFile, &pTransCom->m_vInfo[INFO_POS], sizeof(_vec3), &dwByte, nullptr);
					WriteFile(hFile, &pTransCom->m_vAngle, sizeof(_vec3), &dwByte, nullptr);
					WriteFile(hFile, &pTransCom->m_vScale, sizeof(_vec3), &dwByte, nullptr);
					WriteFile(hFile, &pTransCom->m_matWorld, sizeof(_matrix), &dwByte, nullptr);
					WriteFile(hFile, &pMonster->m_eType, sizeof(MONSTERTYPE), &dwByte, nullptr);
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

	if (ImGuiFileDialog::Instance()->Display("LoadMonster"))
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
			m_iGreenSlimeCnt = 0;
			m_iPinkSlimeCnt = 0;
			m_iFistCnt = 0;
			m_iBlueBatCnt = 0;
			m_iBrownBatCnt = 0;
			m_iStickCnt = 0;
			m_iLeafCnt = 0;
			m_iSkeletonGhostCnt = 0;
			m_iGreenSpiderCnt = 0;
			m_iMimicCnt = 0;
			m_iOctopusCnt = 0;
			m_iSongBossCnt = 0;

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
			Engine::CMonster* pMonster = Engine::CMonster::Create(m_pGraphicDev);
			Engine::CMonster* pCloneObject = nullptr;
			CTransform*	pTransCom = dynamic_cast<CTransform*>(pMonster->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));

			while (true)
			{
				ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

				TCHAR*	pName = new TCHAR[dwStrByte];
				ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

				ReadFile(hFile, &pTransCom->m_vInfo[INFO_RIGHT], sizeof(_vec3), &dwByte, nullptr);
				ReadFile(hFile, &pTransCom->m_vInfo[INFO_UP], sizeof(_vec3), &dwByte, nullptr);
				ReadFile(hFile, &pTransCom->m_vInfo[INFO_LOOK], sizeof(_vec3), &dwByte, nullptr);
				ReadFile(hFile, &pTransCom->m_vInfo[INFO_POS], sizeof(_vec3), &dwByte, nullptr);
				ReadFile(hFile, &pTransCom->m_vAngle, sizeof(_vec3), &dwByte, nullptr);
				ReadFile(hFile, &pTransCom->m_vScale, sizeof(_vec3), &dwByte, nullptr);
				ReadFile(hFile, &pTransCom->m_matWorld, sizeof(_matrix), &dwByte, nullptr);
				ReadFile(hFile, &pMonster->m_eType, sizeof(MONSTERTYPE), &dwByte, nullptr);

				if (0 == dwByte)
				{
					Safe_Release(pMonster);
					Safe_Delete_Array(wpPath);
					Safe_Delete_Array(pName);
					break;
				}

				m_vecObjTags.push_back(pName);
				switch (pMonster->m_eType)
				{
				case MOB_GREENSLIME:
					pCloneObject = CGreenSlime::Create(pMonster);
					m_iGreenSlimeCnt++;
					break;

				case MOB_PINKSLIME:
					pCloneObject = CPinkSlime::Create(pMonster);
					m_iPinkSlimeCnt++;
					break;

				case MOB_FIST:
					pCloneObject = CFist::Create(pMonster);
					 m_iFistCnt++;
					break;

				case MOB_BLUEBAT:
					pCloneObject = CBlueBat::Create(pMonster);
					m_iBlueBatCnt++;
					break;

				case MOB_BROWNBAT:
					pCloneObject = CBrownBat::Create(pMonster);
					m_iBrownBatCnt++;
					break;

				case MOB_STICK:
					pCloneObject = CStick::Create(pMonster);
					m_iStickCnt++;
					break;

				case MOB_LEAF:
					pCloneObject = CLeaf::Create(pMonster);
					m_iLeafCnt++;
					break;

				case MOB_SKELETONGHOST:
					pCloneObject = CSkeletonGhost::Create(pMonster);
					m_iSkeletonGhostCnt++;
					break;

				case MOB_GREENSPIDER:
					pCloneObject = CGreenSpider::Create(pMonster);
					m_iGreenSpiderCnt++;
					break;

				case MOB_MIMIC:
					pCloneObject = CMimic::Create(pMonster);
					m_iMimicCnt++;
					break;

				case MOB_OCTOPUS:
					/*pCloneObject = CStatue::Create(pMonster);
					m_iOctopusCnt++;*/
					break;

				case MOB_SONGBOSS:
					pCloneObject = CSongBoss::Create(pMonster);
					m_iSongBossCnt++;
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