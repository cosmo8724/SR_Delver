#include "stdafx.h"
#include "..\Header\InfoTool.h"

#include "../Default/ImGui/ImGuiFileDialog.h"
#include "Export_Function.h"

#include "GreenSlime.h"

#define		AND			ImGui::SameLine()
#define		WIDTH(num)	PushItemWidth(num)k
#define		ENTER		ImGui::NewLine()

USING(ImGui)

CInfoTool::CInfoTool(LPDIRECT3DDEVICE9 pDevice)
{
	m_pGraphicDev = pDevice;
	m_pGraphicDev->AddRef();

}

CInfoTool::~CInfoTool()
{
	Safe_Release(m_pGraphicDev);
}

HRESULT CInfoTool::InfoTool_Window(const _float & fTimeDelta)
{
	Begin("Info Tool");

	static _float iPosX = 0;
	InputFloat("PosX", &iPosX);

	static _float iPosY = 0;
	InputFloat("PosY", &iPosY);

	static _float iPosZ = 0;
	InputFloat("PosZ", &iPosZ);


	if (ImGui::Button("GreenSlime Create"))
	{
		CLayer* pLayer = Engine::Get_Layer(L"Layer_Tool_GameLogic");

		CGameObject* pGameObject = nullptr;

		// GreenSlime
		//pGameObject = CGreenSlime::Create(m_pGraphicDev, iPosX, iPosY, iPosZ);
		//NULL_CHECK_RETURN(pGameObject, E_FAIL);
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GreenSlime", pGameObject), E_FAIL);
	}



	//static char szName[256];
	//if (InputText("Name", szName, 256))
	//	m_tCInfo.wszName = (_tchar)szName;

	//static _int iHp = 0;
	//if (InputInt("Hp", &iHp))
	//	m_tCInfo.iHp = iHp;

	//static _int iAttack = 0;
	//if (InputInt("Attack", &iAttack))
	//	m_tCInfo.iAttack = iAttack;


	//static _int iHp = 0;
	//if(SliderInt("Hp", &iHp, 0, 100))
	//	m_tCInfo.iHp = iHp;

	//static _int iAttack = 0;
	//if (SliderInt("Attack", &iAttack, 0, 100))
	//	m_tCInfo.iAttack = iAttack;







	End();
	return S_OK;
}