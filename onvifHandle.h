#ifndef ONVIFHANDLE_H_
#define ONVIFHANDLE_H_

#include "appCommon.h"
#include "stdsoap2.h"

#define GSOAP_RET_CODE_NOT_IMPLEMENT -1

#define ONVIF_SCOPE_NAME "onvif://www.onvif.org/type/NetworkVideoTransmitter"
#define DEFAULT_HARDWARE_ID "3707D96D27A4"
#define DEFAULT_URN_HARDWARE_ID_PREFIX "urn:uuid:D149F919-4013-437E-B480-"
#define DEVICE_TYPE "tdn:NetworkVideoTransmitter"
#define VIDEO_SOURCE_TOKEN "CaoYonghuaSource_token"
#define ALL 1
#define HOST_NAME "NVTCYH"
#define ONVIF_VERSION_MAJOR 2
#define ONVIF_VERSION_MINOR 20

#ifdef __cplusplus
extern "C" {
#endif

typedef struct St_OnvifRunParam {
	bool runFlag; // 启动onvif标志
	int servicePort; // 对应onvif服务端口
	char ip[IPV4_LEN];
	char hardwareId[SMALL_INFO_LENGTH];
	char urnHardwareId[INFO_LENGTH];
} OnvifRunParam;

typedef struct St_OnvifDeviceInfo {
	char hardwareId[SMALL_INFO_LENGTH];
	char firmwareVersion[SMALL_INFO_LENGTH];
	char manufacturer[SMALL_INFO_LENGTH];
	char model[SMALL_INFO_LENGTH];
	char serialNumber[INFO_LENGTH];
} OnvifDeviceInfo;

typedef struct St_OnvifNTPInfo {
	int enable;
	char address[INFO_LENGTH];
} OnvifNTPInfo;

typedef struct St_OnvifNetCardInfo {
	int size;
	NetCardInfo netCardInfos[5];
} OnvifNetCardInfo;

typedef struct St_OnvifSystemDateTime {
	bool ntpSet;
	time_t localTime;
	int timeZone;
} OnvifSystemDateTime;

typedef struct St_OnvifVideoChannelInfo {
	int channelNo; // channel No, start from 0
	int stream_enable; ///< 0:disable, 1:enable
	int enc_type; ///< 0:ENC_TYPE_H264, 1:ENC_TYPE_MPEG, 2:ENC_TYPE_MJPEG
	int frame_rate; ///< frame rate per second
	int bit_rate; ///< Bitrate per second
	int width; ///< width
	int height; ///< hieght
	int bright;
	int saturation;
	int contrast;
	int sharpness;
	int wbMode; // whitebalance mode 0 is auto, other
	int wbCrGain;
	int wbCbGain;
	int backLightCompMode; // BacklightCompensation mode
	int backLightCompLevel; // BacklightCompensation level
	int wideDynamicMode; // wide Dynamic range Mode
	int wideDynamicLevel; // wide Dynamic range Mode
	int govLength; //
	int videoEncodeProfile;
	int quality;
	int rtspPort;
	char videoAddr[LARGE_INFO_LENGTH];
} OnvifVideoChannelInfo;

typedef struct St_OnvifPTZConfiguration {
	int defaultTimeout;  // continous move default time out
} OnvifPTZConfiguration;

typedef struct St_OnvifPTZContinousMoveInfo {
	bool setTimeOut;	// set timeout flag
	int timeOut;	// timeout value
	bool setZoom;	// set zoom flag
	float zoom;		// zoom value
	bool setPt;		// set pan /tilt
	float x;		// pan value
	float y;		// tilt value
} OnvifPTZContinousMoveInfo;

typedef struct St_OnvifPTZStopInfo {
	int stopPt;		// stop pan tilt
	int stopZoom;	// stop zoom
} OnvifPTZStopInfo;

extern OnvifRunParam onvifRunParam;
void* my_soap_malloc(struct soap* soap, size_t n);
int startOnvifApp();
void stopOnvifApp();

int getOnvifSoapActionNotSupportCode(struct soap *soap, const char *faultInfo,
		const char* faultDetail);
int getOnvifSoapActionNotSupportSubCode1(struct soap *soap, const char* subCode1, const char *faultInfo,
		const char* faultDetail);
int getOnvifSoapActionFailedCode(struct soap *soap, const char *faultInfo,
		const char* faultDetail);
int getOnvifSoapSendInvalidArgFailedCode(struct soap *soap,
		const char *faultInfo, const char* faultDetail);
int setNTPInfo(OnvifNTPInfo* info);
int getNTPInfo(OnvifNTPInfo* info);

int getDeviceInfo(OnvifDeviceInfo* info);
int getNetCardInfo(OnvifNetCardInfo* info);
int deviceReboot();
int setSystemFactoryDefault();
int getVideoCount(int* count);
int getDeviceTime(OnvifSystemDateTime* info);
int setDeviceTime(OnvifSystemDateTime* info);
int getVideoChannelInfo(OnvifVideoChannelInfo* info);
int getVideoChannelStreamInfo(OnvifVideoChannelInfo* info);
int setPTZStopInfo(OnvifPTZStopInfo* info);
int setPTZContinousMoveInfo(OnvifPTZContinousMoveInfo* info);
char* getPTZConfigurationToken(struct soap* soap, int index);

int getIndexFromTokenName(const char* tokenName, const char* prefix);
char* getIndexTokeName(struct soap *soap, const char* prefix, const int index);

SOAP_FMAC1 int SOAP_FMAC2 getOnvifSoapSenderSubCode2Fault(struct soap *soap,
		const char *faultsubcodeQName, const char *faultsubcodeQName1, const char *faultstring,
		const char *faultdetailXML);
struct tt__PTZConfiguration* getPTZConfiguration(struct soap* soap);
#ifdef __cplusplus
}
#endif
#endif /* ONVIFHANDLE_H_ */
