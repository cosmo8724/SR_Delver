#pragma once
#include "Base.h"
#include "Engine_Include.h"
#include "Export_Function.h"

#include "MapTool.h"
#include "ObjectTool.h"
#include "ParticleTool.h"
#include "InfoTool.h"

class CImGuiMgr : public CBase
{
	DECLARE_SINGLETON(CImGuiMgr)

private:
	explicit CImGuiMgr();
	virtual ~CImGuiMgr();

public:
	void			Set_Particle(CUserParticle* pParticle) { m_pParticleTool->m_pParticle = pParticle; }
	CMapTool*		Get_MapTool() { return m_pMapTool; }
	void			Get_MapWidth(_int* Width, _int* Depth, _int* Interval) { *Width = m_iWidth; *Depth = m_iDepth; *Interval = m_iInterval; }

public:
	void	DefaultSetting_ImGui(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Start_ImGui(const _float & fTimeDelta);
	void	End_ImGui();
	void	Render_ImGui();
	void	SetupDevice(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	HRESULT		ImGui_Map_Tool(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fTimeDelta);
	void		MapTool();
	void		ObjectTool();
	void		ParticleTool();
	void		InfoTool();

public:
	virtual void Free() override;

private:
	// For MapTool
	_int	m_iWidth = 50;
	_int	m_iDepth = 50;
	_int	m_iInterval = 1;
	_int	m_iBlockCnt = 0;

	vector<TCHAR*>	m_vecObjTags;

	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;
	CMapTool*				m_pMapTool = nullptr;
	CObjectTool*				m_pObjectTool = nullptr;
	CParticleTool*			m_pParticleTool = nullptr;
	CInfoTool*				m_pInfoTool = nullptr;

	_float					m_fTimeDelta;
};