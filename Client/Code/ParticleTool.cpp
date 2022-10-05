#include "stdafx.h"
#include "..\Header\ParticleTool.h"
#include "../Default/ImGui/ImGuiFileDialog.h"
#include "Export_Function.h"

#define		AND			ImGui::SameLine()
#define		WIDTH(num)	PushItemWidth(num)
#define		ENTER		ImGui::NewLine()

USING(ImGui)

CParticleTool::CParticleTool(LPDIRECT3DDEVICE9 pDevice)
{
	m_pGraphicDev = pDevice;
	m_pGraphicDev->AddRef();

	ZeroMemory(&m_tInfo, sizeof(PINFO));
}

CParticleTool::~CParticleTool()
{
	Safe_Release(m_pGraphicDev);
}

void CParticleTool::ParticleTool_Window(const _float& fTimeDelta)
{
	Begin("Particle Tool");

	Text("Type");
	RadioButton("Firework", &m_iType, 0);	AND;
	RadioButton("Snow", &m_iType, 1);		AND;
	RadioButton("Laser", &m_iType, 2);		AND;
	RadioButton("Spot", &m_iType, 3);		AND;
	RadioButton("Fountain", &m_iType, 4);	AND;
	if (Button("Set"))
		m_pParticle->Set_Type((PTYPE)m_iType); ENTER;

	if (Button("Play"))
	{
		m_pParticle->Set_Play(true);
	} AND;
	if (Button("Stop"))
	{
		m_pParticle->Set_Play(false);
	} ENTER;

	const char* items[] = { "Texture1", "Texture2", "Texture3", "Texture4", "Texture5" };

	static int item_current = 0;
	if (ListBox("Texture", &item_current, items, IM_ARRAYSIZE(items), 4))
		m_pParticle->Set_Particle_Texture(item_current);
	ENTER;

	if (SliderInt("  Max Count", &m_tInfo.iMaxParticles, 0, 10000, "%d", 0))
	{
		if (m_tInfo.iMaxParticles != 0)
			m_pParticle->Set_ParticleCnt(m_tInfo.iMaxParticles);
	}

	if (SliderFloat("  Max Size", &m_tInfo.fSize, 0.f, 3.f, "%.2f", 0))
		m_pParticle->Set_PointSize(m_tInfo.fSize);
	ENTER;
	

	//WIDTH(200);
	static _float	fVel;
	if (SliderFloat("  Velocity", &fVel, -20.f, 20.f, "%.1f", 0))
		m_pParticle->Set_Velocity(fVel);
	ENTER;


	if (SliderFloat("  lifeTime", &m_tAttribute.fLifeTime, 0.f, 20.f, "%.1f", 0))
		m_pParticle->Set_LifeTime(m_tAttribute.fLifeTime);
	ENTER;

	static float col[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	if (ColorEdit4("Color", m_tAttribute.tColor))
		m_pParticle->Set_Color(m_tAttribute.tColor);

	static _bool randColor;
	if (Checkbox("IsColorRandom", &randColor))
		m_pParticle->Is_ColorRand(randColor);
	ENTER;

	static _float frameSpeed;
	if (SliderFloat("  FrameSpeed", &frameSpeed, 0.f, 3.f, "%.2f", 0))
		m_pParticle->Set_FrameSpeed(frameSpeed);

	static _bool reFrame;
	if (Checkbox("IsFrameRepeat", &reFrame))
		m_pParticle->Is_FrameRepeat(reFrame);

	ENTER;


	m_pParticle->Play_Particle();
	Button("Save"); AND; Button("Load");


	End();
}