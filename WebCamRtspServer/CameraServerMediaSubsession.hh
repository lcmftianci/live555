#ifndef _CameraServerMediaSubsession_h
#define _CameraServerMediaSubsession_h

#include "OnDemandServerMediaSubsession.hh"
#include "CameraFramedSource.hh"

class CameraServerMediaSubsession : public OnDemandServerMediaSubsession {
public:
    static CameraServerMediaSubsession* createNew(UsageEnvironment& env) {
        return new CameraServerMediaSubsession(env);
    }

protected:
    CameraServerMediaSubsession(UsageEnvironment& env)
        : OnDemandServerMediaSubsession(env, false) {
            printf("[%s] CameraServerMediaSubsession\n", envir().getResultMsg());
        }

    virtual FramedSource* createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate) {
        printf("[%s] createNewStreamSource\n", envir().getResultMsg());
        estBitrate = 500; // 估计的比特率
        return (FramedSource*)CameraFramedSource::createNew(envir());
    }

    virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource) {
        printf("[%s] createNewRTPSink\n", envir().getResultMsg());
        return H264VideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic);
    }
};

#endif // _CameraServerMediaSubsession_h