#ifndef APPTOOLS_H_
#define APPTOOLS_H_

#include <stdio.h>
#include <sys/time.h>
#include "appCommon.h"
#include "strList.h"

#ifdef __cplusplus
	extern "C" {
#endif

	int getLocalIp(char* pIp);
	int getNetCardMac(unsigned char *pMac);
	void stopSoap(struct soap* soap1);
	int getServiceURL(char* value, const char* ip, const int port);
	void getAppointServiceURL(char* value, const char* serviceURL, const char* service);
	bool isValidHandle(int handle);
	bool isRetCodeSuccess(int retCode);
	void getDateTimeStr(char* info, const int len, const time_t dtValue);
	void getCurrentDateTimeStr(char* info, const int len);
	void getCurrentDateTimeMSecStr(char* info, const int len);
	int parseTimeZoneTimeStr(const char* timeInfo, const short srcTimeZone, const short destTimeZone,
			time_t* value);
	int parseTimeStr(const char* timeInfo, time_t* value);
	int convertBCDToDec(const unsigned char *bcd, int length);
	int convertDecToBCD(int value, unsigned char *bcd);
	void convertDecToHexStr(const int src, char* dest);
	int convertHexStrToDec(const char* src, int* dest);
	void myThreadSleep();
	void squeezeChar(char s[], char c);
	void parseListByDiv(const PStrList list, const char* value, const char* div);
	bool charIsPositive(char c);
	bool charIsNegative(char c);
	bool charIsNumber(char c);
	int parsePosixTimeZone(const char* tzone, int* tz);
#ifdef __cplusplus
	}
#endif

#endif /* APPTOOLS_H_ */
