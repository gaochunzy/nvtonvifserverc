#include <cmd_type.h>
#include "appTools.h"
#include "onvifHandle.h"
#include "commIPC.h"

typedef int (*Push_CommIPC_Command_Func)(const hmap_t, const void_ptr);
typedef int (*Parse_CommIPC_Command_Func)(const hmap_t, const void_ptr);

int sendCommIPCFunc(const int type, const void_ptr info,
		const Push_CommIPC_Command_Func pushFunc,
		const Parse_CommIPC_Command_Func parseFunc) {
	if (NULL == info) {
		return RET_CODE_ERROR_NULL_OBJECT;
	}
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
	char macaddr[INFO_LENGTH] = {0};
	int result = getStrValueFromList(outList, e_net_macaddr, macaddr);
	if (strlen(macaddr) < 1) {
		return RET_CODE_ERROR_INVALID_VALUE;
	}
	info->size = 1;
	strcpy(info->netCardInfos[0].mac, macaddr);
	getStrValueFromList(outList, e_net_cardname, info->netCardInfos[0].name);
	getStrValueFromList(outList, e_net_ip, info->netCardInfos[0].ip);
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
	int result = getStrValueFromList(outList, e_sys_hardwareId, info->hardwareId);
	getStrValueFromList(outList, e_sys_manufacturer, info->manufacturer);
	getStrValueFromList(outList, e_sys_Model, info->model);
	getStrValueFromList(outList, e_sys_hdversion, info->firmwareVersion);
	return RET_CODE_SUCCESS;
}

int getDeviceInfo(OnvifDeviceInfo* info) {
	return sendCommIPCFunc(T_Get, info, getDeviceInfo_PushCmd, getDeviceInfo_ParseCmd);
}