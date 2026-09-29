#pragma once

#include "SharedMemBase.h"
#define MAX_DM      1000
#define MAX_AXIS	60

class CMemComm : SHARED_MEMORY_BASE
{
private:
	HANDLE hMutex;
	bool bBusy, bReturn;
	TMemCommand TxBuffer, RxBuffer;

private:
	void InitMemvar();

public:
	unsigned int DM[MAX_DM], WDM[MAX_DM];
	unsigned short int IO[100];
	unsigned short int BIT[100];

	_device				RDEV, WDEV;
	//--------------------------------------//
	_mtdata         	MTRD[MAX_AXIS], MTWD;
	_mtstatus       	MTRS[MAX_AXIS];
	_mtcfg          	MTCONFIG[MAX_AXIS], MTWDCFG;
	_lampbuzzer     	LAMP;
	_errorcode      	ERRLIST;
	_machinestatus  	RMACHINESTATUS, WMACHINESTATUS;
	_userinfo			RUSERINFO, WUSERINFO;
	_lotinfo        	RLOTINFO, WLOTINFO;
	_deviceinfo			RDEVICEINFO, WDEVICEINFO;
	_systemdata        	RSystemData, WSystemData;
	_tenkeyjog     		WTENKEYJOG;
	_set3point			RSet3Point, WSet3Point;
	_flip1visionresult	RFlip1VisionResult, WFlip1VisionResult;
	_flip2visionresult	RFlip2VisionResult, WFlip2VisionResult;
	_pallet1visionresult	RPallet1VisionResult, WPallet1VisionResult;
	_pallet2visionresult	RPallet2VisionResult, WPallet2VisionResult;
	_frontpkvisionresult	RFrontPkVisionResult, WFrontPkVisionResult;
	_rearpkvisionresult		RRearPkVisionResult, WRearPkVisionResult;
	_goodtray1visionresult	RGoodTray1VisionResult, WGoodTray1VisionResult;
	_goodtray2visionresult	RGoodTray2VisionResult, WGoodTray2VisionResult;
	_reworktrayvisionresult RReworkTrayVisionResult, WReworkTrayVisionResult;
	_ngtrayresult			RNGTrayVisionResult, WNGTrayVisionResult;
	_scantriggerrecipe		WScanTriggerRecipe;
	_scantriggerdisplay		RScanTriggerDisplay;
	_flip1map			RFlip1Map, WFlip1Map;
	_flip2map			RFlip2Map, WFlip2Map;
	_pallet1map			RPallet1Map, WPallet1Map;
	_pallet2map			RPallet2Map, WPallet2Map;
	_frontpkmap			RFrontPkMap, WFrontPkMap;
	_rearpkmap			RRearPkMap, WRearPkMap;
	_goodtray1map		RGoodTray1Map, WGoodTray1Map;
	_goodtray2map		RGoodTray2Map, WGoodTray2Map;
	_reworktraymap		RReworkTrayMap, WReworkTrayMap;
	_ngtraymap			RNGTrayMap, WNGTrayMap;
	//------Picker Calibration
	_pkcentermove			R_PKCenMove, W_PKCenMove;
	_frontpkcenteroffset    R_FPKCenOffset, W_FPKCenOffset;
	_rearpkcenteroffset     R_RPKCenOffset, W_RPKCenOffset;
public:
	CMemComm() //-- CONSTRUCTOR
	{
		InitMemvar();
	}

	~CMemComm() //-- DESTRUCTOR
	{
		if (nullptr != hMutex)
		{
			CloseHandle(hMutex);
			hMutex = nullptr;
		}
	}

	bool MemPort(TMemCommand membuffer);

	void Decode();

	bool ReadDM();
	bool WriteArrayDM(int iStart, int iCount);
	bool WriteDM(int iStart, int Data);

	bool ReadBit();
	bool WriteBit(int iStart, int Data);

	bool ReadIo();
	bool WriteIO(int iStart, WORD Data);
		
	bool WriteServoOnOff(int iAxis);
	bool WriteServoHome(int iAxis);
	bool WriteServoAlarmClear(int iAxis);
	bool WriteMotorVelocityMove(int iAxis, int iDirection, unsigned int uSpeed);
	bool WriteMotorRelativeMove(int iAxis, int iPos, unsigned int uSpeed);
	bool WriteMotorAbsoluteMove(int iAxis, int iIndex);
	bool WriteMotorStop(int iAxis);

	bool ReadMotorStatus(int iAxis);
	
	bool ReadMotorData(int iAxis);
	bool WriteMotorData(); 
	
	bool ReadMotorConfig(int iAxis);
	bool WriteMotorConfig();

	bool ReadDevice();
	bool WriteDevice();

	bool WriteTenkey(unsigned short int iTenkey);
	bool WriteLampBuzzer();

	bool ReadErrorList();

	bool WriteExitProgram();

	bool ReadMachineStatus();
	bool WriteMachineStatus();

	bool ReadUserInfo();
	bool WriteUserInfo();

	bool ReadLotInfo();
	bool WriteLotInfo();

	bool ReadDeviceInfo();
	bool WriteDeviceInfo();

	bool WriteSystemData();

	bool WriteTenKeyJog();

	bool WriteLoadCntClear();
	bool WriteUnLoadCntClear();

	bool WriteESTOP();
	
	bool WriteBuzzerOff();

	bool ReadFlip1VisionResult();
	bool WriteFlip1VisionResult();

	bool ReadFlip2VisionResult();
	bool WriteFlip2VisionResult();

	bool ReadPallet1VisionResult();
	bool WritePallet1VisionResult();

	bool ReadPallet2VisionResult();
	bool WritePallet2VisionResult();

	bool ReadFrontPickerVisionResult();
	bool WriteFrontPkVisionResult();
	
	bool ReadRearPickerVisionResult();
	bool WriteRearPickerVisionResult();

	bool ReadGoodTray1VisionResult();
	bool ReadGoodTray2VisionResult();
	bool ReadReworkTrayVisionResult();
	bool ReadNGTrayVisionResult();
	
	bool WriteSet3Point(int pktype, int target, int pointno);
	bool ReadSet3PointOffset();

	bool ReadFlip1Map();
	bool WriteFlip1Map();

	bool ReadFlip2Map();
	bool WriteFlip2Map();

	bool ReadPallet1Map();
	bool WritePallet1Map();

	bool ReadPallet2Map();
	bool WritePallet2Map();

	bool ReadFrontPickerMap();
	bool ReadRearPickerMap();

	bool ReadGoodTray1Map();
	bool WriteGoodTray1Map();

	bool ReadGoodTray2Map();
	bool WriteGoodTray2Map();

	bool ReadReworkTrayMap();
	bool WriteRewTrayMap();

	bool ReadNGTrayMap();
	bool WriteNGTrayMap();

	//Picker Calibration
	bool WritePKCenMove();
	bool WriteCenTrig();
	bool WritePKAutoCal();
	bool ReadFPKOffset();
	bool ReadRPKOffset();

	// Line scan trigger
	bool WriteScanTriggerRecipe();
	bool ReadScanTriggerDisplay();
	bool WriteScanTriggerStart();
	bool WriteScanTriggerStop();
	bool WriteScanTriggerTest();
};

