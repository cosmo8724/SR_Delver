#include "stdafx.h"
#include "..\Header\ImGuiMgr.h"
#include "../Default/ImGui/imgui.h"
#include "../Default/ImGui/imgui_impl_dx9.h"
#include "../Default/ImGui/imgui_impl_win32.h"
#include "../Default/ImGui/ImGuizmo.h"
#include "../Default/ImGui/ImGuiFileDialog.h"

#include "Terrain.h"
#include "Block.h"

IMPLEMENT_SINGLETON(CImGuiMgr)

CImGuiMgr::CImGuiMgr()
{
}

CImGuiMgr::~CImGuiMgr()
{
	Free();
}

void CImGuiMgr::DefaultSetting_ImGui(LPDIRECT3DDEVICE9 pGraphicDev)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();

	ImGuiStyle&	style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.f;
		style.Colors[ImGuiCol_WindowBg].w = 1.f;
	}

	SetupDevice(pGraphicDev);

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX9_Init(m_pGraphicDev);
}

void CImGuiMgr::Start_ImGui(const _float & fTimeDelta)
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	m_fTimeDelta = fTimeDelta;
}

void CImGuiMgr::End_ImGui()
{
	ImGui::EndFrame();
}

void CImGuiMgr::Render_ImGui()
{
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void CImGuiMgr::SetupDevice(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();

	m_pMapTool = new CMapTool(m_pGraphicDev);
	m_pParticleTool = new CParticleTool(m_pGraphicDev);
}

HRESULT CImGuiMgr::ImGui_Map_Tool(LPDIRECT3DDEVICE9 pGraphicDev, const _float & fTimeDelta)
{
#pragma region Terrain Tool
	ImGui::Begin("Terrain Settings");
	static _bool	bWireFrame = false;

	if (ImGui::CollapsingHeader("Tile Count", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::SliderInt("Width", &m_iWidth, 0, 200);
		ImGui::SliderInt("Depth", &m_iDepth, 0, 200);
		ImGui::SliderInt("Interval", &m_iInterval, 0, 10);
		ImGui::NewLine();
		if (ImGui::Button("Create"))
		{
			CLayer* pLayer = Engine::Get_Layer(L"Layer_Environment");
			pLayer->Delete_GameObject(L"Terrain");

			Engine::Delete_Proto(L"Proto_TerrainTexCom");
			FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexCom", CTerrainTex::Create(pGraphicDev, m_iWidth, m_iDepth, m_iInterval)), E_FAIL);

			CGameObject* pGameObject = nullptr;

			// Terrain
			pGameObject = CTerrain::Create(pGraphicDev);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);

			if (bWireFrame)
				bWireFrame = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear"))
		{
			CLayer* pLayer = Engine::Get_Layer(L"Layer_Environment");
			pLayer->Delete_GameObject(L"Terrain");
		}
	}

	// 터레인 생성 후 이벤트
	CTerrain* pGameObject = dynamic_cast<CTerrain*>(Engine::Get_GameObject(L"Layer_Environment", L"Terrain"));
	if (pGameObject)
	{
		ImGui::NewLine();
		if (ImGui::CollapsingHeader("Options", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::Checkbox("WireFrame", &bWireFrame))
				pGameObject->Set_WireFrame(bWireFrame);
		}

		ImGui::NewLine();
		if (ImGui::CollapsingHeader("Tile Texture", ImGuiTreeNodeFlags_DefaultOpen))
		{
			CTexture* pTextureCom = dynamic_cast<CTexture*>(pGameObject->Get_Component(L"Proto_TerrainTexture", ID_STATIC));

			vector<IDirect3DBaseTexture9*> vecTexture = pTextureCom->Get_Texture();

			for (_uint i = 0; i < 18; ++i)
			{
				if (ImGui::ImageButton((void*)vecTexture[i], ImVec2(32.f, 32.f)))
				{
					//pTextureCom->Set_Texture(i);
					pGameObject->Set_TextureIndex(i);
				}
				if (i == 0 || (i + 1) % 6)
					ImGui::SameLine();
			}
		}
	}

	ImGui::End();
#pragma endregion Terrain Tool

#pragma region Map Tool
	ImGui::Begin("Map Tool");

	if (!pGameObject)
		ImGui::Text("Create Terrain First.");
	else
	{
		static _int	iSelect = 0;
		static BLOCKTYPE	eType = BLOCKTYPE_END;

		if (ImGui::CollapsingHeader("Set Block Texture", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::BulletText("Cave");
			CTexture*	pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Cave_CubeExampleImage"));

			for (size_t i = 0; i < pTexture->Get_Texture().size(); ++i)
			{
				if (ImGui::ImageButton((void*)pTexture->Get_Texture()[i], ImVec2(50.f, 50.f)))
				{
					iSelect = i;
					eType = BLOCK_CAVE;
				}
				if (i == 0 || (i + 1) % 6)
					ImGui::SameLine();
			}
			Safe_Release(pTexture);

			ImGui::NewLine();
			ImGui::BulletText("Cold");
			pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Cold_CubeExampleImage"));

			for (size_t i = 0; i < pTexture->Get_Texture().size(); ++i)
			{
				if (ImGui::ImageButton((void*)pTexture->Get_Texture()[i], ImVec2(50.f, 50.f)))
				{
					iSelect = i;
					eType = BLOCK_COLD;
				}
				if (i == 0 || (i + 1) % 6)
					ImGui::SameLine();
			}
			Safe_Release(pTexture);

			ImGui::NewLine();
			ImGui::BulletText("Dungeon");
			pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Dungeon_CubeExampleImage"));

			for (size_t i = 0; i < pTexture->Get_Texture().size(); ++i)
			{
				if (ImGui::ImageButton((void*)pTexture->Get_Texture()[i], ImVec2(50.f, 50.f)))
				{
					iSelect = i;
					eType = BLOCK_DUNGEON;
				}
				if (i == 0 || (i + 1) % 6)
					ImGui::SameLine();
			}
			Safe_Release(pTexture);

			ImGui::NewLine();
			ImGui::BulletText("Room");
			pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Room_CubeExampleImage"));

			for (size_t i = 0; i < pTexture->Get_Texture().size(); ++i)
			{
				if (ImGui::ImageButton((void*)pTexture->Get_Texture()[i], ImVec2(50.f, 50.f)))
				{
					iSelect = i;
					eType = BLOCK_ROOM;
				}
				if (i == 0 || (i + 1) % 6)
					ImGui::SameLine();
			}
			Safe_Release(pTexture);

			ImGui::NewLine();
			ImGui::BulletText("Sewer");
			pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Sewer_CubeExampleImage"));

			for (size_t i = 0; i < pTexture->Get_Texture().size(); ++i)
			{
				if (ImGui::ImageButton((void*)pTexture->Get_Texture()[i], ImVec2(50.f, 50.f)))
				{
					iSelect = i;
					eType = BLOCK_SEWER	;
				}
				if (i == 0 || (i + 1) % 6)
					ImGui::SameLine();
			}
			Safe_Release(pTexture);

			ImGui::NewLine();
			ImGui::BulletText("Temple");
			pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Temple_CubeExampleImage"));

			for (size_t i = 0; i < pTexture->Get_Texture().size(); ++i)
			{
				if (ImGui::ImageButton((void*)pTexture->Get_Texture()[i], ImVec2(50.f, 50.f)))
				{
					iSelect = i;
					eType = BLOCK_TEMPLE;
				}
				if (i == 0 || (i + 1) % 6)
					ImGui::SameLine();
			}
			Safe_Release(pTexture);
		}

		ImGui::NewLine();
		if (ImGui::CollapsingHeader("Place Blocks", ImGuiTreeNodeFlags_DefaultOpen))
		{
			static wstring wstrObjName = L"";
			static _int iObjTexture = 0;
			static _vec3	vObjPos = { 0.f, 0.f, 0.f };
			static _vec3 vObjScale = { 0.f, 0.f, 0.f };
			static	_bool	bNowCreating = false;
			CGameObject* pTempBlock = nullptr;
			CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

			if (ImGui::Button("Create") && !bNowCreating)
			{
				bNowCreating = true;

				pTempBlock = CBlock::Create(pGraphicDev);
				NULL_CHECK_RETURN(pTempBlock, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Block", pTempBlock), E_FAIL);
			}

			if (bNowCreating)
			{
				ImGui::SameLine();
				ImGui::BulletText("LB to put Block & RB to cancel.");

				pTempBlock = Engine::Get_GameObject(L"Layer_GameLogic", L"Block");
				dynamic_cast<CBlock*>(pTempBlock)->SetBlockType(eType);
				dynamic_cast<CBlock*>(pTempBlock)->SetTextureIndex(iSelect);

				if (pTempBlock && Mouse_Down(DIM_LB))//(Get_DIMouseState(DIM_LB) & 0x80))
				{
					//m_iBlockCnt = 0;
					TCHAR	*	szObjTag = new TCHAR[MAX_PATH];
					wsprintf(szObjTag, L"Block_%d", m_iBlockCnt);
					m_vecObjTags.push_back(szObjTag);

					//	CGameObject
					CGameObject* pCloneBlock = (CBlock::Create(*dynamic_cast<CBlock*>(pTempBlock)));//new CBlock(*dynamic_cast<CBlock*>(pTempBlock));
					dynamic_cast<CBlock*>(pCloneBlock)->SetBlock();
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_vecObjTags.back(), pCloneBlock), E_FAIL);
					++m_iBlockCnt;
				}
				if (Get_DIMouseState(DIM_RB))
				{
					pLayer->Delete_GameObject(L"Block");
					bNowCreating = false;
				}
				if (Get_DIKeyState(DIK_LCONTROL) && Key_Down(DIK_Z))
				{
					if (m_iBlockCnt > 0)
						--m_iBlockCnt;
					TCHAR		szObjTag[256];
					wsprintf(szObjTag, L"Block_%d", m_iBlockCnt);
					pLayer->Delete_GameObject(szObjTag);
				}
			}
			static _bool	bNowSelecting = false;
			if (ImGui::Button("Select Cube"))
			{
				bNowSelecting = true;

				if (bNowCreating)
				{
					pLayer->Delete_GameObject(L"Block");
					bNowCreating = false;
				}
			}

			if (bNowSelecting)
			{
				ImGui::SameLine();
				ImGui::BulletText("LB to put Block & RB to cancel.");

				if (Get_DIMouseState(DIM_LB))
				{
					CBlock*	pSelectBlock = nullptr;
					CBlock*	pSelectedBlock = nullptr;
					_float		fDistance = 0.f;
					_float		fLastDistance = 0.f;

					CLayer*	pLayer = Engine::Get_Layer(L"Layer_GameLogic");
					map<const _tchar*, CGameObject*> mapGameObject = *(pLayer->Get_mapGameObject());

					for (auto ObjIter = mapGameObject.begin(); ObjIter != mapGameObject.end(); ++ObjIter)
					{
						pSelectBlock = dynamic_cast<CBlock*>(ObjIter->second);

						if (pSelectBlock)
						{
							if (pSelectBlock->m_pCalculatorCom->CheckIntersectCube(g_hWnd, pSelectBlock->m_pBufferCom, pSelectBlock->m_pTransCom, &fDistance))
							{
								_float fTemp = fDistance;
								if (pSelectedBlock == nullptr)
								{
									fLastDistance = fTemp;
									pSelectedBlock = pSelectBlock;
									if (wstrObjName != L"")
									{
										CBlock*	pBlock = dynamic_cast<CBlock*>(pLayer->Get_GameObject(wstrObjName.c_str()));
										pBlock->SetChanging();
									}
									wstrObjName = ObjIter->first;
								}
								else
								{
									if (fTemp < fLastDistance)
									{
										fLastDistance = fTemp;
										pSelectedBlock = pSelectBlock;
									}
									else
										continue;
								}
							}
						}
					}
					if (pSelectedBlock)
					{
						pSelectedBlock->SetChanging();
						iObjTexture = pSelectedBlock->GetTextureIndex();
						vObjPos = pSelectedBlock->m_pTransCom->Get_Pos();
						vObjScale = pSelectedBlock->m_pTransCom->Get_Scale();
						pSelectedBlock = nullptr;
					}
				}

				if (Get_DIMouseState(DIM_RB))
				{
					bNowSelecting = false;
					if (wstrObjName != L"")
					{
						CBlock*	pBlock = dynamic_cast<CBlock*>(pLayer->Get_GameObject(wstrObjName.c_str()));
						pBlock->SetChanging();
					}
					wstrObjName = L"";
					iObjTexture = 0;
					vObjPos = { 0.f, 0.f, 0.f };
					vObjScale = { 0.f, 0.f, 0.f };
				}
			}

			if (ImGui::Button("Save"))
			{
				bNowCreating = false;

				auto iter = find_if(pLayer->Get_mapGameObject()->begin(), pLayer->Get_mapGameObject()->end(), CTag_Finder(L"Block"));

				if (iter != pLayer->Get_mapGameObject()->end())
					pLayer->Delete_GameObject(L"Block");

				ImGuiFileDialog::Instance()->OpenDialog("ChooseSaveFolder", "Choose Folder", nullptr, ".", 1, nullptr, ImGuiFileDialogFlags_Modal);
			}

			if (ImGuiFileDialog::Instance()->Display("ChooseSaveFolder"))
			{
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					string	strPath = ImGuiFileDialog::Instance()->GetCurrentPath();
					strPath += "\\Map.dat";
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

					for (auto iter = pLayer->Get_mapGameObject()->begin(); iter != pLayer->Get_mapGameObject()->end(); ++iter)
					{
						CBlock*	pBlock = dynamic_cast<CBlock*>(iter->second);
						dwStrByte = sizeof(TCHAR) * (lstrlen(iter->first) + 1);
						if (pBlock)
						{
							WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
							WriteFile(hFile, iter->first, dwStrByte, &dwByte, nullptr);

							WriteFile(hFile, &pBlock->m_pTransCom->m_vInfo[INFO_RIGHT], sizeof(_vec3), &dwByte, nullptr);
							WriteFile(hFile, &pBlock->m_pTransCom->m_vInfo[INFO_UP], sizeof(_vec3), &dwByte, nullptr);
							WriteFile(hFile, &pBlock->m_pTransCom->m_vInfo[INFO_LOOK], sizeof(_vec3), &dwByte, nullptr);
							WriteFile(hFile, &pBlock->m_pTransCom->m_vInfo[INFO_POS], sizeof(_vec3), &dwByte, nullptr);
							WriteFile(hFile, &pBlock->m_pTransCom->m_vAngle, sizeof(_vec3), &dwByte, nullptr);
							WriteFile(hFile, &pBlock->m_pTransCom->m_vScale, sizeof(_vec3), &dwByte, nullptr);
							WriteFile(hFile, &pBlock->m_pTransCom->m_matWorld, sizeof(_matrix), &dwByte, nullptr);
							//WriteFile(hFile, pBlock->m_pTextureCom, sizeof(CTexture), &dwByte, nullptr);
							WriteFile(hFile, &pBlock->m_iTexture, sizeof(_int), &dwByte, nullptr);
							WriteFile(hFile, &pBlock->m_fScale, sizeof(_float), &dwByte, nullptr);
							//WriteFile(hFile, &pBlock->m_bClone, sizeof(_bool), &dwByte, nullptr);
							WriteFile(hFile, &pBlock->m_bSet, sizeof(_bool), &dwByte, nullptr);
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

			ImGui::SameLine();
			if (ImGui::Button("Load"))
			{
				bNowCreating = false;

				auto iter = find_if(pLayer->Get_mapGameObject()->begin(), pLayer->Get_mapGameObject()->end(), CTag_Finder(L"Block"));

				if (iter != pLayer->Get_mapGameObject()->end())
					pLayer->Delete_GameObject(L"Block");

				ImGuiFileDialog::Instance()->OpenDialog("ChooseLoadMap", "Choose Folder", ".dat", ".", 1, nullptr, ImGuiFileDialogFlags_Modal);
			}

			if (ImGuiFileDialog::Instance()->Display("ChooseLoadMap"))
			{
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					string	strPath = ImGuiFileDialog::Instance()->GetFilePathName();
					const char* pPath = strPath.c_str();
					int iLength = strlen(pPath) + 1;
					TCHAR* wpPath = new TCHAR[iLength];
					size_t	Temp;
					mbstowcs_s(&Temp, wpPath, iLength, pPath, iLength);

					HANDLE	hFile = CreateFile(wpPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

					if (INVALID_HANDLE_VALUE == hFile)
						return E_FAIL;

					m_iBlockCnt = 0;
					DWORD	dwByte = 0;
					DWORD	dwStrByte = 0;
					CBlock*	pBlock = CBlock::Create(pGraphicDev);
					CGameObject*	pTemp = nullptr;

					while (true)
					{
						if (pBlock)
						{
							ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

							TCHAR*	pName = new TCHAR[dwStrByte];
							ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

							ReadFile(hFile, &pBlock->m_pTransCom->m_vInfo[INFO_RIGHT], sizeof(_vec3), &dwByte, nullptr);
							ReadFile(hFile, &pBlock->m_pTransCom->m_vInfo[INFO_UP], sizeof(_vec3), &dwByte, nullptr);
							ReadFile(hFile, &pBlock->m_pTransCom->m_vInfo[INFO_LOOK], sizeof(_vec3), &dwByte, nullptr);
							ReadFile(hFile, &pBlock->m_pTransCom->m_vInfo[INFO_POS], sizeof(_vec3), &dwByte, nullptr);
							ReadFile(hFile, &pBlock->m_pTransCom->m_vAngle, sizeof(_vec3), &dwByte, nullptr);
							ReadFile(hFile, &pBlock->m_pTransCom->m_vScale, sizeof(_vec3), &dwByte, nullptr);
							ReadFile(hFile, &pBlock->m_pTransCom->m_matWorld, sizeof(_matrix), &dwByte, nullptr);
							//ReadFile(hFile, pBlock->m_pTextureCom, sizeof(CTexture), &dwByte, nullptr);
							ReadFile(hFile, &pBlock->m_iTexture, sizeof(_int), &dwByte, nullptr);
							ReadFile(hFile, &pBlock->m_fScale, sizeof(_float), &dwByte, nullptr);
							//ReadFile(hFile, &pBlock->m_bClone, sizeof(_bool), &dwByte, nullptr);
							ReadFile(hFile, &pBlock->m_bSet, sizeof(_bool), &dwByte, nullptr);

							if (0 == dwByte)
							{
								Safe_Release(pBlock);
								Safe_Delete_Array(wpPath);
								Safe_Delete_Array(pName);
								break;
							}
							m_vecObjTags.push_back(pName);
							pTemp = CBlock::Create(*pBlock);
							pLayer->Add_GameObject(m_vecObjTags.back(), pTemp);
							m_iBlockCnt++;
						}
					}
					CloseHandle(hFile);

					ImGuiFileDialog::Instance()->Close();
				}
				if (!ImGuiFileDialog::Instance()->IsOk())
				{
					ImGuiFileDialog::Instance()->Close();
				}
			}

			ImGui::NewLine();
			ImGui::Text("Current Block Info");
			ImGui::BulletText("Name : %ws", wstrObjName.c_str());
			ImGui::BulletText("Texture : %d", iObjTexture);
			ImGui::BulletText("Position : (%f, %f, %f)", vObjPos.x, vObjPos.y, vObjPos.z);
			ImGui::BulletText("Scale : (%f, %f, %f)", vObjScale.x, vObjScale.y, vObjScale.z);
			ImGui::BulletText("Change Texture");
			ImGui::SameLine();
			if (ImGui::Button("<") && wstrObjName != L"")
			{
				CBlock* pBlock = dynamic_cast<CBlock*>(pLayer->Get_GameObject(wstrObjName.c_str()));
				if (iObjTexture > 0)
				{
					iObjTexture--;
					pBlock->SetTextureIndex(iObjTexture);
				}
			}
			ImGui::SameLine();
			if (ImGui::Button(">") && wstrObjName != L"")
			{
				CBlock* pBlock = dynamic_cast<CBlock*>(pLayer->Get_GameObject(wstrObjName.c_str()));
				if (iObjTexture < 7)
				{
					iObjTexture++;
					pBlock->SetTextureIndex(iObjTexture);
				}
			}
			if (ImGui::Button("Delete This Block"))
			{
				pLayer->Delete_GameObject(wstrObjName.c_str());
				wstrObjName = L"";
				iObjTexture = 0;
				vObjPos = { 0.f, 0.f, 0.f };
				vObjScale = { 0.f, 0.f, 0.f };
				m_iBlockCnt--;
			}
		}
	}

	ImGui::End();
#pragma endregion Map Tool
	return S_OK;
}

void CImGuiMgr::MapTool()
{
	m_pMapTool->MapTool_Window(m_fTimeDelta);
}

void CImGuiMgr::ParticleTool()
{
	m_pParticleTool->ParticleTool_Window(m_fTimeDelta);
}

void CImGuiMgr::Free()
{
	for (size_t i = 0; i < m_vecObjTags.size(); i++)
		Safe_Delete_Array(m_vecObjTags[i]);
	m_vecObjTags.clear();

	Safe_Delete(m_pMapTool);
	Safe_Delete(m_pParticleTool);

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Safe_Release(m_pGraphicDev);
}