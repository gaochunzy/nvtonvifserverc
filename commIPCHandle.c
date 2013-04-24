#include <cmd_type.h>
#include "appTools.h"
#include "onvifHandle.h"
#include "commIPC.h"

typedef int (*Push_CommIPC_Command_Func)(const hmap_t, const void_ptr);
typedef int (*Parse_CommIPC_Command_Func)(const hmap_t, const void_ptr);

int sendCommIPCFunc(const int type, const void_ptr info,
		const Push_CommIPC_Command_Func pushFunc,
		const Parse_CommIPC_Command_Func parseFunc) {
	if (NULL == pushFunc) {
		return RET_CODE_ERROR_NULL_OBJECT;
	}
	hmap_t inList = hashmap_create();
	int result = pushFunc(inList, info);
	if (!isRetCodeSuccess(result)) {
		destroyHashMapList(inList);
		return result;
	}
	hmap_t outList = hashmap_create();
	result = sendAndRetList(type, inList, outList);
	if (!isRetCodeSuccess(result)) {
		destroyHashMapList(inList);
		destroyHashMapList(outList);
		return result;
	}
	if (NULL != parseFunc) {
		result = parseFunc(outList, info);
	}
	destroyHashMapList(inList);
	destroyHashMapList(outList);
	return result;
}

int setNTPInfo_PushCmd(const hmap_t inList, const void_ptr info1) {
	OnvifNTPInfo* info = (OnvifNTPInfo*) info1;
	char* address = info->address;
	if (strlen(address) < 1) {
		return RET_CODE_ERROR_INVALID_IP;
	}
	if (strlen(address) > 0) {
		putIntValueInList(inList, e_time_ntpenable, ENABLE_YES);
		putStrValueInList(inList, e_time_ntpserver, address);
	} else
		putIntValueInList(inList, e_time_ntpenable, ENABLE_NO);
	return RET_CODE_SUCCESS;
}

int setNTPInfo(OnvifNTPInfo* info) {
	return sendCommIPCFunc(T_Set, info, setNTPInfo_PushCmd, NULL);
}

int getNTPInfo_PushCmd(const hmap_t inList, const void_ptr info1) {
	OnvifNTPInfo* info = (OnvifNTPInfo*) info1;
	putNullValueInList(inList, e_time_ntpenable);
	putNullValueInList(inList, e_time_ntpserver);
	return RET_CODE_SUCCESS;
}

int getNTPInfo_ParseCmd(const hmap_t outList, const void_ptr info1) {
	OnvifNTPInfo* info = (OnvifNTPInfo*) info1;
	int result = getStrValueFromList(outList, e_time_ntpserver, info->address);
	result = getIntValueFromList(outList, e_time_ntpenable, &info->enable);
	return result;
}


int getNTPInfo(OnvifNTPInfo* info) {
	return sendCommIPCFunc(T_Get, info, getNTPInfo_PushCmd, getNTPInfo_ParseCmd);
}

int getNetCardInfo_PushCmd(const hmap_t inList, const void_ptr info1) {
	OnvifNetCardInfo* info = (OnvifNetCardInfo*) info1;
	putNullValueInList(inList, e_net_cardname);
	putNullValueInList(inList, e_net_macaddr);
	putNullValueInList(inList, e_net_ip);
	putNullValueInList(inList, e_net_netmask);
	putNullValueInList(inList, e_net_gateway);
	return RET_CODE_SUCCESS;
}

int getNetCardInfo_ParseCmd(const hmap_t outList, const void_ptr info1) {
	OnvifNetCardInfo* info = (OnvifNetCardInfo*) info1;
	memset(info, 0, sizeof(OnvifNetCardInfo));
	char macaddr[INFO_LENGTH] = {0};
	int result = getStrValueFromList(outList, e_net_macaddr, macaddr);
	if (strlen(macaddr) < 1) {
		return RET_CODE_ERROR_INVALID_VALUE;
	}
	info->size = 1;
	strcpy(info->netCardInfos[0].mac, macaddr);
	getStrValueFromList(outList, e_net_cardname, info->netCardInfos[0].name);
	getStrValueFromList(outList, e_net_ip, info->netCardInfos[0].ip);
	strcpy(onvifRunParam.ip, info->netCardInfos[0].ip);
	if (strlen(onvifRunParam.ip) < 1) {
		getLocalIp(onvifRunParam.ip);
	}
	getStrValueFromList(outList, e_net_netmask, info->netCardInfos[0].mask);
	getStrValueFromList(outList, e_net_gateway, info->netCardInfos[0].gateway);
	return result;
}

int getNetCardInfo(OnvifNetCardInfo* info) {
	return sendCommIPCFunc(T_Get, info, getNetCardInfo_PushCmd, getNetCardInfo_ParseCmd);
}

int getDeviceInfo_PushCmd(const hmap_t inList, const void_ptr info1) {
	OnvifDeviceInfo* info = (OnvifDeviceInfo*) info1;
	putNullValueInList(inList, e_sys_hardwareId);
	putNullValueInList(inList, e_sys_manufacturer);
	putNullValueInList(inList, e_sys_Model);
	putNullValueInList(inList, e_sys_serialNumber);
	putNullValueInList(inList, e_sys_hdversion);
	return RET_CODE_SUCCESS;
}

int getDeviceInfo_ParseCmd(const hmap_t outList, const void_ptr info1) {
	OnvifDeviceInfo* info = (OnvifDeviceInfo*) info1;
	memset(info, 0, sizeof(OnvifDeviceInfo));
	getStrValueFromList(outList, e_sys_hardwareId, info->hardwareId);
	if (strlen(info->hardwareId) > 0) {
		strcpy(onvifRunParam.hardwareId,  info->hardwareId);
		sprintf(onvifRunParam.urnHardwareId, "%s%s", DEFAULT_URN_HARDWARE_ID_PREFIX, onvifRunParam.urnHardwareId);
	}

	getStrValueFromList(outList, e_sys_manufacturer, info->manufacturer);
	getStrValueFromList(outList, e_sys_Model, info->model);
	getStrValueFromList(outList, e_sys_hdversion, info->firmwareVersion);
	return RET_CODE_SUCCESS;
}

int getDeviceInfo(OnvifDeviceInfo* info) {
	return sendCommIPCFunc(T_Get, info, getDeviceInfo_PushCmd, getDeviceInfo_ParseCmd);
}

int deviceReboot_PushCmd(const hmap_t inList, const void_ptr info1) {
	putIntValueInList(inList, e_reboot, ENABLE_YES);
	return RET_CODE_SUCCESS;
}

int deviceReboot() {
	return sendCommIPCFunc(T_Set, NULL, deviceReboot_PushCmd, NULL);
}

int setSystemFactoryDefault_PushCmd(const hmap_t inList, const void_ptr info1) {
	putIntValueInList(inList, e_reset, ENABLE_YES);
	return RET_CODE_SUCCESS;
}

int setSystemFactoryDefault() {
	return sendCommIPCFunc(T_Set, NULL, setSystemFactoryDefault_PushCmd, NULL);
}


int getVideoCount_PushCmd(const hmap_t inList, const void_ptr info1) {
	putNullValueInList(inList, e_video_chn_num);
	return RET_CODE_SUCCESS;
}

int getVideoCount_ParseCmd(const hmap_t outList, const void_ptr info1) {
	return getIntValueFromList(outList, e_video_chn_num, (int*) info1);
}

int getVideoCount(int* count) {
	return sendCommIPCFunc(T_Get, count, getVideoCount_PushCmd, getVideoCount_ParseCmd);
}

int getDeviceTime_PushCmd(const hmap_t inList, const void_ptr info1) {
	putNullValueInList(inList, e_time_ntpenable);
	putNullValueInList(inList, e_time_systime);
	putNullValueInList(inList, e_time_Zone);
	return RET_CODE_SUCCESS;
}

int getDeviceTime_ParseCmd(const hmap_t outList, const void_ptr info1) {
	OnvifSystemDateTime* info = (OnvifSystemDateTime*)info1;
	int ntp;
	int result = getIntValueFromList(outList, e_time_ntpenable, &ntp);
	if (!isRetCodeSuccess(result))
		return result;
	info->ntpSet = ntp;
	int zone = 0;
	result = getIntValueFromList(outList, e_time_ntpenable, &zone);
	if (!isRetCodeSuccess(result))
		return result;
	if (zone > 24) {
		zone = zone / 60;
	}
	info->timeZone = zone;
	char localTime1[INFO_LENGTH]= {0};
	result = getStrValueFromList(outList, e_time_systime, localTime1);
	if (!isRetCodeSuccess(result))
		return result;
	if (strlen(localTime1) < 1)
		return RET_CODE_ERROR_NULL_VALUE;
	time_t tim;
	result = parseTimeZoneTimeStr(localTime1, zone, zone, &tim);
	if (!isRetCodeSuccess(result))
		return result;
	info->localTime = tim;
	return result;
}

int getDeviceTime(OnvifSystemDateTime* info) {
	return sendCommIPCFunc(T_Get, info, getDeviceTime_PushCmd, getDeviceTime_ParseCmd);
}

int getVideoChannelInfo_PushCmd(const hmap_t inList, const void_ptr info1) {
	OnvifVideoChannelInfo* info = (OnvifVideoChannelInfo*)info1;
	if (NULL == info) {
		return RET_CODE_ERROR_NULL_OBJECT;
	}
	if (info->channelNo < 1) {
		return RET_CODE_ERROR_INVALID_VALUE;
	}
	putIntValueInList(inList, e_Chn, info->channelNo);
	putIntValueInList(inList, e_Sub_Chn, 0);
	putNullValueInList(inList, e_Stream_enable);
	putNullValueInList(inList, e_enc_type);
	putNullValueInList(inList, e_frame_rate);
	putNullValueInList(inList, e_bit_rate);
	putNullValueInList(inList, e_width);
	putNullValueInList(inList, e_height);
	return RET_CODE_SUCCESS;
}

int getVideoChannelInfo_ParseCmd(const hmap_t outList, const void_ptr info1) {
	OnvifVideoChannelInfo* info = (OnvifVideoChannelInfo*)info1;
	int value;
	int result = getIntValueFromList(outList, e_Stream_enable, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->stream_enable = value;
	result = getIntValueFromList(outList, e_enc_type, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->enc_type = value;
	result = getIntValueFromList(outList, e_frame_rate, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->frame_rate = value;
	result = getIntValueFromList(outList, e_bit_rate, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->bit_rate = value;
	result = getIntValueFromList(outList, e_width, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->width = value;
	result = getIntValueFromList(outList, e_height, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->height = value;
	return result;
}

int getVideoChannelInfo(OnvifVideoChannelInfo* info) {
	return sendCommIPCFunc(T_Get, info, getVideoChannelInfo_PushCmd, getVideoChannelInfo_ParseCmd);
}
