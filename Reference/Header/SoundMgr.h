#pragma once
#include "Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CSoundMgr : public CBase
{
	DECLARE_SINGLETON(CSoundMgr)

private:
	explicit CSoundMgr();
	virtual ~CSoundMgr();

public:
	HRESULT	Ready_Sound();

public:
	void	Set_ChannelVolume(CHANNELID eID, _float fVolume);

public:
	HRESULT	Play_Sound(const _tchar* pSoundKey, CHANNELID eID, _float fVolume);
	HRESULT	PlayBGM(const _tchar* pSoundKey, _float fVolume);
	HRESULT	StopSound(CHANNELID eID);
	HRESULT	StopAllSound();

private:
	HRESULT	LoadSoundFile();

private:
	virtual void	Free() override;

private:
	map<const _tchar*, FMOD_SOUND*>	m_mapSound;
	vector<_tchar*>							m_vecSoundKey;
	FMOD_CHANNEL*						m_pChannelArr[MAXCHANNEL];
	FMOD_SYSTEM*							m_pSystem;
};

END