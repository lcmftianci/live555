#pragma once

#include "BasicUsageEnvironment\BasicUsageEnvironment.hh"
#include "groupsock\GroupsockHelper.hh"
#include "liveMedia\liveMedia.hh"
#include <iostream>
#include <list>
#include <map>

struct YHRTSPClient
{
	TaskScheduler* pTaskScheduler;
	UsageEnvironment* pEnv;
	RTSPClient* pClient;
	MediaSession *pMediaSession;
	MediaSubsessionIterator *iter;
	Boolean bMadeProgress;
	unsigned fileSinkBufferSize;
	unsigned socketInputBufferSize;
	Boolean  bStreamUsingTCP;
	Authenticator* pAuthenticator;
	char	 m_cEventLoop;
	YHRTSPClient()
	{
		pClient = NULL;
		pMediaSession = NULL;
		iter = NULL;
		bMadeProgress = False;
		fileSinkBufferSize = 100000;
		socketInputBufferSize = 524288;
		bStreamUsingTCP = False;
		pAuthenticator = NULL;
		m_cEventLoop = 0;

	}
	YHRTSPClient(RTSPClient *client)
	{
		pClient = client;
		pMediaSession = NULL;
		iter = NULL;
		bMadeProgress = False;
		fileSinkBufferSize = 100000;
		socketInputBufferSize = 524288;
		bStreamUsingTCP = False;
		pAuthenticator = NULL;
		m_cEventLoop = 0;
	}
};

class CYHMediaClient
{
public:
	static CYHMediaClient* GetInstance();
	CYHMediaClient(void);
	~CYHMediaClient(void);
public:
	BOOL CreateRTPClient(LONG lID, const char *chServerURL);
	//BOOL StartStreaming(char *chWatchVariable = NULL);
	void StopStreaming(int nClientID);

	void GetSDPDescription(RTSPClient* pRTSPClient, RTSPClient::responseHandler* afterFunc);
	void SetupStreams(RTSPClient* pRTSPClient);
	void SetupSubsession(RTSPClient* pRTSPClient, MediaSubsession* subsession, Boolean streamUsingTCP, RTSPClient::responseHandler* afterFunc);
	void StartPlayingSession(RTSPClient* pRTSPClient, MediaSession* session, double start, double end, float scale, RTSPClient::responseHandler* afterFunc);
	void TearDownSession(RTSPClient* pRTSPClient, MediaSession* session, RTSPClient::responseHandler* afterFunc);

	void SetFileSinkAndSocket(YHRTSPClient *pYHClient, unsigned fileSinkBufferSize, unsigned socketInputBufferSize);
	void SetStreamUsingTCP(YHRTSPClient* pYHClient, Boolean bStreamUsingTCP) { pYHClient->bStreamUsingTCP = bStreamUsingTCP; };

	UsageEnvironment *GetEnvironment(RTSPClient* pRTSPClient)
	{
		UsageEnvironment *pEnv = NULL;
		if (NULL == pRTSPClient)
		{
			return pEnv;
		}
		YHRTSPClient *pYHClient = GetYHRTSPClient(pRTSPClient);
		if (NULL != pYHClient)
		{
			pEnv = pYHClient->pEnv;
		}

		return pEnv;
	};

private:
	void GetOptions(RTSPClient* pRTSPClient, RTSPClient::responseHandler* afterFunc);
	void Shutdown(RTSPClient *pClient = NULL);
	YHRTSPClient* GetYHRTSPClient(RTSPClient *pClient);
	void EraseYHRTSPClient(RTSPClient *pClient);
	LONG GetRTSPClientID(RTSPClient* pClient);
	void CloseMediaSinks(RTSPClient *pClient);

	static void ContinueAfterOptions(RTSPClient* pClient, int resultCode, char* resultString);
	static void ContinueAfterDescribe(RTSPClient* pClient, int resultCode, char* resultString);
	static void ContinueAfterSetup(RTSPClient* pClient, int resultCode, char* resultString);
	static void ContinueAfterPlay(RTSPClient* pClient, int resultCode, char* resultString);
	static void SubsessionAfterPlaying(void* clientData);
	static void SubsessionByeHandler(void* clientData);
	static void ContinueAfterTearDown(RTSPClient* pClient, int resultCode, char* resultString);

public:
	std::list<YHRTSPClient*> m_listRTSPClient;
	std::map<int, RTSPClient*> m_mapClientID;
private:
	static CYHMediaClient* pYHMediaClient;
	//TaskScheduler* pTaskScheduler;
	//UsageEnvironment* pEnv;
};