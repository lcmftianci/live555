#include "YHMediaClient.h"
#include "WinCriticalSection.h"
#include "MetMediaSink.h"
								 //#include "YHWMPlayDemoDlg.h"
char const* clientProtocolName = "RTSP";
CYHMediaClient* CYHMediaClient::pYHMediaClient = NULL;
WinCriticalSection g_cs;

CYHMediaClient* CYHMediaClient::GetInstance()
{
	Mutex mutex(g_cs);
	if (CYHMediaClient::pYHMediaClient == NULL)
	{
		pYHMediaClient = new CYHMediaClient;
	}
	return pYHMediaClient;
}

CYHMediaClient::CYHMediaClient(void)
{

}

CYHMediaClient::~CYHMediaClient(void)
{
	m_listRTSPClient.empty();
}

BOOL CYHMediaClient::CreateRTPClient(LONG lID, const char *chServerURL)
{
	g_cs.Enter();
	TaskScheduler *pTaskScheduler = BasicTaskScheduler::createNew();
	UsageEnvironment *pEnv = BasicUsageEnvironment::createNew(*pTaskScheduler);
	RTSPClient* pRTSPClient = (RTSPClient*)RTSPClient::createNew(*pEnv, chServerURL, 0, NULL, 0);
	if (NULL == pRTSPClient)
	{
		*pEnv << "Failed to create" << clientProtocolName << " client: " << pEnv->getResultMsg() << "\n";
		return FALSE;
	}
	m_mapClientID.insert(std::make_pair(lID, pRTSPClient));
	YHRTSPClient *stucClient = new YHRTSPClient(pRTSPClient);
	stucClient->pTaskScheduler = pTaskScheduler;
	stucClient->pEnv = pEnv;
	CYHMediaClient::GetInstance()->m_listRTSPClient.push_back(stucClient);
	GetOptions(pRTSPClient, ContinueAfterOptions);
	g_cs.Leave();
	stucClient->pEnv->taskScheduler().doEventLoop(&stucClient->m_cEventLoop);
	if (pTaskScheduler)
	{
		delete pTaskScheduler;
		pTaskScheduler = NULL;
	}
	if (stucClient)
	{
		delete stucClient;
		stucClient = NULL;
	}
	return TRUE;
}

void CYHMediaClient::StopStreaming(int nClientID)
{
	Mutex mutex(g_cs);
	std::map<int, RTSPClient*>::iterator iter = m_mapClientID.find(nClientID);
	if (iter != m_mapClientID.end())
	{
		RTSPClient* pRTSPClient = iter->second;
		Shutdown(pRTSPClient);
		EraseYHRTSPClient(pRTSPClient);
		m_mapClientID.erase(iter);
	}
}

void CYHMediaClient::Shutdown(RTSPClient *pClient)
{
	YHRTSPClient* pYHRTSPClient = GetYHRTSPClient(pClient);
	if (pYHRTSPClient != NULL)
	{
		if (pYHRTSPClient->pMediaSession != NULL)
		{
			TearDownSession(pClient, pYHRTSPClient->pMediaSession, ContinueAfterTearDown);
		}
		else
		{
			ContinueAfterTearDown(NULL, 0, NULL);
		}
		CYHMediaClient::GetInstance()->CloseMediaSinks(pClient);
		if (pYHRTSPClient->pMediaSession)
		{
			Medium::close(pYHRTSPClient->pMediaSession);
			pYHRTSPClient->pMediaSession = NULL;
		}
		// Finally, shut down our client:
		if (pClient)
		{
			Medium::close(pClient);
			pClient = NULL;
		}
		pYHRTSPClient->m_cEventLoop = 1;

	}

}

void CYHMediaClient::GetOptions(RTSPClient* pRTSPClient, RTSPClient::responseHandler* afterFunc)
{
	Mutex mutex(g_cs);
	if (pRTSPClient)
	{
		YHRTSPClient *pYHClient = CYHMediaClient::GetInstance()->GetYHRTSPClient(pRTSPClient);
		pRTSPClient->sendOptionsCommand(afterFunc, pYHClient->pAuthenticator);
	}
}

void CYHMediaClient::GetSDPDescription(RTSPClient* pRTSPClient, RTSPClient::responseHandler* afterFunc)
{
	Mutex mutex(g_cs);
	if (pRTSPClient)
	{
		YHRTSPClient *pYHClient = CYHMediaClient::GetInstance()->GetYHRTSPClient(pRTSPClient);
		pRTSPClient->sendDescribeCommand(afterFunc, pYHClient->pAuthenticator);
	}
}

void CYHMediaClient::SetupSubsession(RTSPClient* pRTSPClient, MediaSubsession* subsession, Boolean streamUsingTCP, RTSPClient::responseHandler* afterFunc)
{
	Mutex mutex(g_cs);
	Boolean forceMulticastOnUnspecified = False;
	if (pRTSPClient)
	{
		YHRTSPClient *pYHClient = CYHMediaClient::GetInstance()->GetYHRTSPClient(pRTSPClient);
		pRTSPClient->sendSetupCommand(*subsession, afterFunc, False, streamUsingTCP, forceMulticastOnUnspecified, pYHClient->pAuthenticator);
	}
}

void CYHMediaClient::StartPlayingSession(RTSPClient* pRTSPClient, MediaSession* session, double start, double end, float scale, RTSPClient::responseHandler* afterFunc)
{
	//Mutex mutex(g_cs);
	if (pRTSPClient)
	{
		YHRTSPClient *pYHClient = CYHMediaClient::GetInstance()->GetYHRTSPClient(pRTSPClient);
		pRTSPClient->sendPlayCommand(*session, afterFunc, start, end, scale, pYHClient->pAuthenticator);
	}
}

void CYHMediaClient::TearDownSession(RTSPClient* pRTSPClient, MediaSession* session, RTSPClient::responseHandler* afterFunc)
{
	Mutex mutex(g_cs);
	if (pRTSPClient)
	{
		YHRTSPClient *pYHClient = CYHMediaClient::GetInstance()->GetYHRTSPClient(pRTSPClient);
		pRTSPClient->sendTeardownCommand(*session, afterFunc, pYHClient->pAuthenticator);
	}
}

void CYHMediaClient::ContinueAfterOptions(RTSPClient* pClient, int resultCode, char* resultString)
{
	Mutex mutex(g_cs);
	if (CYHMediaClient::GetInstance() != NULL)
	{
		if (resultCode != 0)
		{
			*(CYHMediaClient::GetInstance()->GetEnvironment(pClient)) << clientProtocolName << " \"OPTIONS\" request failed: " << resultString << "\n";
			return;
		}
		else
		{
			*(CYHMediaClient::GetInstance()->GetEnvironment(pClient)) << clientProtocolName << " \"OPTIONS\" request returned: " << resultString << "\n";
		}
		delete[] resultString;

		CYHMediaClient::GetInstance()->GetSDPDescription(pClient, ContinueAfterDescribe);
	}
	return;
}

void CYHMediaClient::ContinueAfterDescribe(RTSPClient* pClient, int resultCode, char* resultString)
{
	Mutex mutex(g_cs);
	if (CYHMediaClient::GetInstance() != NULL)
	{
		if (resultCode != 0)
		{
			CYHMediaClient::GetInstance()->Shutdown();
			return;
		}

		char* sdpDescription = resultString;
		// Create a media session object from this SDP description:
		MediaSession *pMediaSession = MediaSession::createNew(*(CYHMediaClient::GetInstance()->GetEnvironment(pClient)), sdpDescription);
		delete[] sdpDescription;

		if (pMediaSession == NULL)
		{
			*(CYHMediaClient::GetInstance()->GetEnvironment(pClient)) << "Failed to create a MediaSession object from the SDP description: " << CYHMediaClient::GetInstance()->GetEnvironment(pClient)->getResultMsg() << "\n";
			CYHMediaClient::GetInstance()->Shutdown(pClient);
			return;
		}
		else if (!pMediaSession->hasSubsessions())
		{
			*(CYHMediaClient::GetInstance()->GetEnvironment(pClient)) << "This session has no media subsessions (i.e., \"m=\" lines)\n";
			CYHMediaClient::GetInstance()->Shutdown(pClient);
			return;
		}

		// Then, setup the "RTPSource"s for the session:
		MediaSubsessionIterator iter(*pMediaSession);

		MediaSubsession *subsession;
		YHRTSPClient *pYHClient = CYHMediaClient::GetInstance()->GetYHRTSPClient(pClient);
		pYHClient->bMadeProgress = False;
		pYHClient->pMediaSession = pMediaSession;

		while ((subsession = iter.next()) != NULL)
		{
			if (!subsession->initiate())
			{
				*(CYHMediaClient::GetInstance()->GetEnvironment(pClient)) << "Unable to create receiver for \"" << subsession->mediumName()
					<< "/" << subsession->codecName()
					<< "\" subsession: " << CYHMediaClient::GetInstance()->GetEnvironment(pClient)->getResultMsg() << "\n";
			}
			else
			{
				*(CYHMediaClient::GetInstance()->GetEnvironment(pClient)) << "Created receiver for \"" << subsession->mediumName()
					<< "/" << subsession->codecName()
					<< "\" subsession (client ports " << subsession->clientPortNum()
					<< "-" << subsession->clientPortNum() + 1 << ")\n";

				pYHClient->bMadeProgress = True;
				if (subsession->rtpSource() != NULL)
				{
					// Because we're saving the incoming data, rather than playing
					// it in real time, allow an especially large time threshold
					// (1 second) for reordering misordered incoming packets:
					unsigned const thresh = 500000; // 0.5 second
					subsession->rtpSource()->setPacketReorderingThresholdTime(thresh);

					// Set the RTP source's OS socket buffer size as appropriate - either if we were explicitly asked (using -B),
					// or if the desired FileSink buffer size happens to be larger than the current OS socket buffer size.
					// (The latter case is a heuristic, on the assumption that if the user asked for a large FileSink buffer size,
					// then the input data rate may be large enough to justify increasing the OS socket buffer size also.)
					int socketNum = subsession->rtpSource()->RTPgs()->socketNum();
					unsigned curBufferSize = getReceiveBufferSize(*CYHMediaClient::GetInstance()->GetEnvironment(pClient), socketNum);
					if (pYHClient->socketInputBufferSize > 0 || pYHClient->fileSinkBufferSize > curBufferSize)
					{
						unsigned newBufferSize = pYHClient->socketInputBufferSize > 0 ? pYHClient->socketInputBufferSize : pYHClient->fileSinkBufferSize;
						newBufferSize = setReceiveBufferTo(*CYHMediaClient::GetInstance()->GetEnvironment(pClient), socketNum, newBufferSize);
					}
				}
			}
		}
		if (!pYHClient->bMadeProgress)
		{
			CYHMediaClient::GetInstance()->Shutdown(pClient);
			return;
		}

		// Perform additional 'setup' on each subsession, before playing them:
		CYHMediaClient::GetInstance()->SetupStreams(pClient);
	}
}

void CYHMediaClient::SetupStreams(RTSPClient* pRTSPClient)
{
	Mutex mutex(g_cs);
	YHRTSPClient *struClient = GetYHRTSPClient(pRTSPClient);
	if (struClient->iter == NULL)
		struClient->iter = new MediaSubsessionIterator(*(struClient->pMediaSession));
	MediaSubsession *subsession = NULL;
	while ((subsession = struClient->iter->next()) != NULL)
	{
		// We have another subsession left to set up:
		if (subsession->clientPortNum() == 0)
			continue; // port # was not set
		if (pRTSPClient != NULL)
		{
			SetupSubsession(pRTSPClient, subsession, /*struClient->bStreamUsingTCP*/True, ContinueAfterSetup);
		}
		return;
	}

	// We're done setting up subsessions.
	if (!struClient->bMadeProgress)
	{
		Shutdown(pRTSPClient);
		return;
	}

	// Create and start "FileSink"s for each subsession:
	struClient->bMadeProgress = False;
	MediaSubsessionIterator iter(*(struClient->pMediaSession));
	while ((subsession = iter.next()) != NULL)
	{
		if (subsession->readSource() == NULL)
			continue; // was not initiated

					  // Create an output file for each desired stream:
		CMetMediaSink* pMediaSink;
		unsigned int requestedBufferSize = 524288;
		pMediaSink = CMetMediaSink::createNew(*struClient->pEnv, struClient->socketInputBufferSize);
		pMediaSink->SetMediaSession(struClient->pMediaSession);
		pMediaSink->SetPlayHandle(GetRTSPClientID(pRTSPClient));
		subsession->sink = pMediaSink;
		if (subsession->sink == NULL)
		{
			*struClient->pEnv << "Failed to create MediaSink for \"" << "MediaClient" << "\": " << struClient->pEnv->getResultMsg() << "\n";
		}
		else
		{
			subsession->sink->startPlaying(*(subsession->readSource()), SubsessionAfterPlaying, subsession);

			// Also set a handler to be called if a RTCP "BYE" arrives
			// for this subsession:
			if (subsession->rtcpInstance() != NULL)
			{
				subsession->rtcpInstance()->setByeHandler(SubsessionByeHandler, subsession);
			}
			struClient->bMadeProgress = True;
		}
	}
	if (!struClient->bMadeProgress)
	{
		Shutdown(pRTSPClient);
		return;
	}
	double duration = 0;
	YHRTSPClient* pYHRTSPClient = GetYHRTSPClient(pRTSPClient);
	duration = pYHRTSPClient->pMediaSession->playEndTime();
	if (duration < 0)
		duration = 0.0;

	double dwEnd = duration;
	StartPlayingSession(pRTSPClient, pYHRTSPClient->pMediaSession, 0, dwEnd, 1.0, ContinueAfterPlay);
}

void CYHMediaClient::ContinueAfterSetup(RTSPClient* pClient, int resultCode, char* resultString)
{
	Mutex mutex(g_cs);
	if (pClient == NULL)
	{
		return;
	}

	if (CYHMediaClient::GetInstance() != NULL)
	{
		if (resultCode == 0)
		{
			YHRTSPClient *pYHClient = CYHMediaClient::GetInstance()->GetYHRTSPClient(pClient);
			pYHClient->bMadeProgress = True;
		}
		// Set up the next subsession, if any:
		CYHMediaClient::GetInstance()->SetupStreams(pClient);
	}
}

void CYHMediaClient::ContinueAfterPlay(RTSPClient* pClient, int resultCode, char* resultString)
{
	Mutex mutex(g_cs);
	if (CYHMediaClient::GetInstance() != NULL)
	{
		if (resultCode != 0)
		{
			*(CYHMediaClient::GetInstance()->GetEnvironment(pClient)) << "Failed to start playing session: " << resultString << "\n";
			CYHMediaClient::GetInstance()->Shutdown(pClient);
			return;
		}
		else
		{
			*(CYHMediaClient::GetInstance()->GetEnvironment(pClient)) << "Started playing session\n";
		}
		char const* actionString = "Receiving streamed data...";
		*(CYHMediaClient::GetInstance()->GetEnvironment(pClient)) << actionString << "...\n";
	}
}

void CYHMediaClient::SubsessionAfterPlaying(void* clientData)
{
	Mutex mutex(g_cs);
	MediaSubsession* subsession = (MediaSubsession*)clientData;
	Medium::close(subsession->sink);
	subsession->sink = NULL;
}

void CYHMediaClient::SubsessionByeHandler(void* clientData)
{
	Mutex mutex(g_cs);
	MediaSubsession* subsession = (MediaSubsession*)clientData;
	if (subsession != NULL)
	{
		// Act now as if the subsession had closed:
		SubsessionAfterPlaying(subsession);
	}
}

void CYHMediaClient::ContinueAfterTearDown(RTSPClient* pClient, int resultCode, char* resultString)
{
	/*
	Mutex mutex(g_cs);
	// Now that we've stopped any more incoming data from arriving, close our output files:
	if (CYHMediaClient::GetInstance()!= NULL)
	{
	CYHMediaClient::GetInstance()->CloseMediaSinks(pClient);
	YHRTSPClient *struClient = CYHMediaClient::GetInstance()->GetYHRTSPClient(pClient);
	if (struClient->pMediaSession)
	{
	Medium::close(struClient->pMediaSession);
	}
	// Finally, shut down our client:
	if (struClient->pAuthenticator)
	{
	delete struClient->pAuthenticator;
	}
	Medium::close(pClient);
	struClient->m_cEventLoop = 1;
	}*/

}

void CYHMediaClient::CloseMediaSinks(RTSPClient *pClient)
{
	YHRTSPClient* pYHRTSPClient = GetYHRTSPClient(pClient);
	if (pYHRTSPClient->pMediaSession == NULL)
		return;
	MediaSubsessionIterator iter(*pYHRTSPClient->pMediaSession);
	MediaSubsession* subsession;
	while ((subsession = iter.next()) != NULL)
	{
		Medium::close(subsession->sink);
		subsession->sink = NULL;
	}
}

YHRTSPClient* CYHMediaClient::GetYHRTSPClient(RTSPClient *pClient)
{
	if (m_listRTSPClient.size() == 0)
	{
		return NULL;
	}
	std::list<YHRTSPClient*>::iterator iter;
	for (iter = m_listRTSPClient.begin(); iter != m_listRTSPClient.end(); ++iter)
	{
		if (pClient == (*iter)->pClient)
		{
			return *iter;
		}
		else
			continue;
	}
	return NULL;
}

void CYHMediaClient::EraseYHRTSPClient(RTSPClient *pClient)
{
	if (m_listRTSPClient.size() == 0)
	{
		return;
	}
	std::list<YHRTSPClient*>::iterator iter;
	for (iter = m_listRTSPClient.begin(); iter != m_listRTSPClient.end(); ++iter)
	{
		if (pClient == (*iter)->pClient)
		{
			break;
		}
		else
			continue;
	}
	m_listRTSPClient.erase(iter);
}
LONG CYHMediaClient::GetRTSPClientID(RTSPClient* pClient)
{
	std::map<int, RTSPClient*>::iterator iter = m_mapClientID.begin();
	int nClientID = -1;
	for (; iter != m_mapClientID.end(); ++iter)
	{
		if (pClient == iter->second)
		{
			nClientID = iter->first;
		}
	}
	return nClientID;
}

void CYHMediaClient::SetFileSinkAndSocket(YHRTSPClient *pYHClient, unsigned fileSinkBufferSize, unsigned socketInputBufferSize)
{
	if (NULL == pYHClient)
	{
		return;
	}
	pYHClient->fileSinkBufferSize = fileSinkBufferSize;
	pYHClient->socketInputBufferSize = socketInputBufferSize;

	return;
}
