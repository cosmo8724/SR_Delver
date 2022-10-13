#include "..\..\Header\SoundMgr.h"
#include "Export_Function.h"

IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr()
{
}


CSoundMgr::~CSoundMgr()
{
	Free();
}

HRESULT CSoundMgr::Ready_Sound()
{
	FAILED_CHECK_RETURN(FMOD_System_Create(&m_pSystem), E_FAIL);
	
	FAILED_CHECK_RETURN(FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL), E_FAIL);

	FAILED_CHECK_RETURN(LoadSoundFile(), E_FAIL);

	return S_OK;
}

void CSoundMgr::Set_ChannelVolume(CHANNELID eID, _float fVolume)
{
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);
	FMOD_System_Update(m_pSystem);
}

HRESULT CSoundMgr::Play_Sound(const _tchar * pSoundKey, CHANNELID eID, _float fVolume)
{
	map<const _tchar*, FMOD_SOUND*>::iterator	iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	if (iter == m_mapSound.end())
		return E_FAIL;

	FMOD_BOOL	bPlay = FALSE;

	if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay))
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[eID]);

	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);

	return S_OK;
}

HRESULT CSoundMgr::PlayBGM(const _tchar * pSoundKey, _float fVolume)
{
	map<const _tchar*, FMOD_SOUND*>::iterator	iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	if (iter == m_mapSound.end())
		return E_FAIL;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[SOUND_BGM]);

	FMOD_Channel_SetMode(m_pChannelArr[SOUND_BGM], FMOD_LOOP_NORMAL);

	FMOD_Channel_SetVolume(m_pChannelArr[SOUND_BGM], fVolume);

	FMOD_System_Update(m_pSystem);

	return S_OK;
}

HRESULT CSoundMgr::StopSound(CHANNELID eID)
{
	FAILED_CHECK_RETURN(FMOD_Channel_Stop(m_pChannelArr[eID]), E_FAIL);

	return S_OK;
}

HRESULT CSoundMgr::StopAllSound()
{
	for (_int i = 0; i < MAXCHANNEL; ++i)
		FAILED_CHECK_RETURN(FMOD_Channel_Stop(m_pChannelArr[i]), E_FAIL);

	return S_OK;
}

HRESULT CSoundMgr::LoadSoundFile()
{
	_finddata_t	fd;

	_long	Handle = _findfirst("../Bin/Resource/Sound/*.*", &fd);

	if (Handle == -1)
		return E_FAIL;

	_int	iResult = 0;
	char	szCurPaht[MAX_PATH] = "../Bin/Resource/Sound/";
	char	szFullPath[MAX_PATH] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPaht);
		strcat_s(szFullPath, fd.name);

		FMOD_SOUND* pSound = nullptr;
		FMOD_RESULT eResult = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, nullptr, &pSound);

		if (eResult == FMOD_OK)
		{
			_int	iLength = strlen(fd.name) + 1;

			TCHAR*	pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);

			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			m_vecSoundKey.push_back(pSoundKey);
			m_mapSound.emplace(m_vecSoundKey.back(), pSound);
		}

		iResult = _findnext(Handle, &fd);
	}
	FMOD_System_Update(m_pSystem);

	_findclose(Handle);

	return S_OK;
}

void CSoundMgr::Free()
{
	for (auto& MyPair : m_mapSound)
		FMOD_Sound_Release(MyPair.second);

	for (size_t i = 0; i < m_vecSoundKey.size(); ++i)
		Safe_Delete_Array(m_vecSoundKey[i]);

	m_mapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}
