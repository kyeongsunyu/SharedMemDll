#include "pch.h"
#include "MemComm.h"

void CMemComm::InitMemvar()
{
	if (!InitObject(RUN_MMI, 100)) {
		exit(0);
	}
	 
	ZeroMemory(&TxBuffer, sizeof(TMemCommand));
	ZeroMemory(&RxBuffer, sizeof(TMemCommand));

	hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"/MUTEX/MMICOMM");
	if (nullptr == hMutex) {
		hMutex = CreateMutex(nullptr, FALSE, L"/MUTEX/MMICOMM");
	}
	 
	ZeroMemory(DM, sizeof(DM));
	ZeroMemory(IO, sizeof(IO));
	ZeroMemory(BIT, sizeof(BIT));
	/*ZeroMemory(MTRD, sizeof(MTRD));
	ZeroMemory(&MTWD, sizeof(MTWD));
	ZeroMemory(MTRS, sizeof(MTRS));
	ZeroMemory(&RDEV, sizeof(RDEV));
	ZeroMemory(&WDEV, sizeof(WDEV));
	ZeroMemory(&LAMP, sizeof(LAMP));*/

	bBusy = false;
}

bool CMemComm::MemPort(TMemCommand membuffer)
{
	bReturn = false;

	//-- hMutex는 GUI내의 여러 쓰레드들에서 동시접근을 방지하기 위함임.
	//-- WaitForSingleObject(hMutex, 500)를 진입하면 hMutex는 자동을 잠김 상태가 되면
	//-- 이미 진입한 명령이 완전이 끝나서 ReleaseMutex(hMutex);를 호출하기 전까지는 다른 명령이 동시에 진입하지 못함.
	if (WaitForSingleObject(hMutex, 1000) != WAIT_OBJECT_0)
		return bReturn;

	TxBuffer = membuffer;

	TxBuffer.Result = 0;
	RxBuffer.Result = 0;

	FlushInOutBuffer();
	//-- GUI에서 Send()하기전에 hEventRx 이벤트를 클리어 해 놓고, SEQ에서 명령을 받고 응답을 완료 할 때 이벤트를 셋을 한다.
	//-- 만약 제어가 명령을 받지 못하는 상황(SEQ가 꺼져 있거나 기타의 이유로 응답의 지연이 발생)이면 최대 500msec까지 제어를 기다린다.
	//-- RTX(RTOS)는 SEQ가 워낙에 빠르게 동작하므로 문제가 되지 않지만 WINDOWS CONSOL모드에서는 SEQ루프가 윈도우 상에서 지연이 발생되기 때문에 
	//-- 명령을 보내고 바로 받으면 수신을 못하는 경우가 발생되는 것을 방지 하기 위함임. 
	ResetEvent(hEventRx);
	if (!Send(&TxBuffer))
	{
		ReleaseMutex(hMutex);
		return bReturn;
	}

	//-- WIN 32에서 필요, RTX(RTOS)상에서는 WaitForSingleObject(hEventRx, 500); 를 삭제해도 무방하다.
	WaitForSingleObject(hEventRx, 500);

	if (Recv(&RxBuffer))
	{
		Decode();
		bReturn = true;
	}
	else
	{
		bReturn = false;
	}
	Sleep(1);
	ReleaseMutex(hMutex);
	return bReturn;
}

void CMemComm::Decode()
{
	switch (RxBuffer.Command)
	{
	case CMD_READ_BITV:
		memcpy(BIT, RxBuffer.Arg.IoBit.uData, sizeof(BIT));
		break;
	case CMD_READ_DM:
		memcpy(DM, RxBuffer.Arg.DataMemory.uData, sizeof(RxBuffer.Arg.DataMemory.uData));
		break;
	case CMD_READ_IO:
		memcpy(IO, RxBuffer.Arg.IoBit.uData, sizeof(IO));
		break;
	case CMD_READ_MOTORSTATUS:
		if ((RxBuffer.Arg.MotorStatus.MotorNum >= 0) && (RxBuffer.Arg.MotorStatus.MotorNum <= MAX_AXIS)) {
			MTRS[RxBuffer.Arg.MotorStatus.MotorNum] = RxBuffer.Arg.MotorStatus;
		}
		break;
	case CMD_READ_MOTORDATA:
		if ((RxBuffer.Arg.MotorData.uAxisNo >= 0) && (RxBuffer.Arg.MotorData.uAxisNo <= MAX_AXIS)) {
			MTRD[RxBuffer.Arg.MotorData.uAxisNo] = RxBuffer.Arg.MotorData;
		}
		break;
	case CMD_READ_MOTORCONFIG:
		MTCONFIG[RxBuffer.Arg.MotorCfg.uAxisNo] = RxBuffer.Arg.MotorCfg;
		break;
	case CMD_READ_ERRORCODE:
		ERRLIST = RxBuffer.Arg.ErrorCode;
		break;
	case CMD_READ_STATUS:
		RMACHINESTATUS = RxBuffer.Arg.MachineStatus;
		break;
	case CMD_READ_LOTINFO:
		RLOTINFO = RxBuffer.Arg.LotInfo;
		break;
	case CMD_READ_DEVICE_INFO:
		RDEVICEINFO = RxBuffer.Arg.DeviceInfo;
		break;
	case CMD_READ_DEVICEDATA:
		RDEV = RxBuffer.Arg.Device;
		break;
	case CMD_READ_3POINT_OFFSET:
		RSet3Point = RxBuffer.Arg.Set3Point;
		break;
	case CMD_READ_FLIP1_VISION_RESULT:
		RFlip1VisionResult = RxBuffer.Arg.Flip1VisionResult;
		break;
	case CMD_READ_FLIP2_VISION_RESULT:
		RFlip2VisionResult = RxBuffer.Arg.Flip2VisionResult;
		break;
	case CMD_READ_PALLET1_VISION_RESULT:
		RPallet1VisionResult = RxBuffer.Arg.Pallet1VisionResult;
		break;
	case CMD_READ_PALLET2_VISION_RESULT:
		RPallet2VisionResult = RxBuffer.Arg.Pallet2VisionResult;
		break;
	case CMD_READ_FRONT_PICKER_RESULT:
		RFrontPkVisionResult = RxBuffer.Arg.FrontPkVisionResult;
		break;
	case CMD_READ_REAR_PICKER_RESULT:
		RRearPkVisionResult = RxBuffer.Arg.RearPkVisionResult;
		break;
	case CMD_READ_GOOD_TRAY1_VISION_RESULT:
		RGoodTray1VisionResult = RxBuffer.Arg.GoodTray1VisionResult;
		break;
	case CMD_READ_GOOD_TRAY2_VISION_RESULT:
		RGoodTray2VisionResult = RxBuffer.Arg.GoodTray2VisionResult;
		break;
	case CMD_READ_REWORK_TRAY_VISION_RESULT:
		RReworkTrayVisionResult = RxBuffer.Arg.ReworkTrayVisionResult;
		break;
	case CMD_READ_NG_TRAY_VISION_RESULT:
		RNGTrayVisionResult = RxBuffer.Arg.NGTrayVisionResult;
		break;
	case CMD_READ_FLIP1_MAP:
		RFlip1Map = RxBuffer.Arg.Flip1Map;
		break;
	case CMD_READ_FLIP2_MAP:
		RFlip2Map = RxBuffer.Arg.Flip2Map;
		break;
	case CMD_READ_PALLET1_MAP:
		RPallet1Map = RxBuffer.Arg.Pallet1Map;
		break;
	case CMD_READ_PALLET2_MAP:
		RPallet2Map = RxBuffer.Arg.Pallet2Map;
		break;
	case CMD_READ_FRONT_PICKER_MAP:
		RFrontPkMap = RxBuffer.Arg.FrontPkMap;
		break;
	case CMD_READ_REAR_PICKER_MAP:
		RRearPkMap = RxBuffer.Arg.RearPkMap;
		break;
	case CMD_READ_GOOD_TRAY1_MAP:
		RGoodTray1Map = RxBuffer.Arg.GoodTray1Map;
		break;
	case CMD_READ_GOOD_TRAY2_MAP:
		RGoodTray2Map = RxBuffer.Arg.GoodTray2Map;
		break;
	case CMD_READ_REWORK_TRAY_MAP:
		RReworkTrayMap = RxBuffer.Arg.ReworkTrayMap;
		break;
	case CMD_READ_NG_TRAY_MAP:
		RNGTrayMap = RxBuffer.Arg.NGTrayMap;
		break;
	case CMD_READ_FRONT_PK_CENTER_OFFSET:
		R_FPKCenOffset = RxBuffer.Arg.FrontPkCenterOffset;
		break;
	case CMD_READ_REAR_PK_CENTER_OFFSET:
		R_RPKCenOffset = RxBuffer.Arg.RearPkCenterOffset;
		break;
	}
}

bool CMemComm::ReadDM()
{
	TMemCommand membuffer;
	membuffer.Command = CMD_READ_DM;
	membuffer.Arg.DataMemory.uStart = 0;
	membuffer.Arg.DataMemory.uCount = MAX_DM;

	bReturn = MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WriteArrayDM(int iStart, int iCount)
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_DM;
	membuffer.Arg.DataMemory.uStart = iStart;
	membuffer.Arg.DataMemory.uCount = iCount;

	//for (int idx = 0; idx < iCount; idx++)
	//{
	//	membuffer.Arg.DataMemory.uData[iStart + idx] = WDM[iStart + idx];
	//}

	memcpy(&membuffer.Arg.DataMemory.uData[iStart], &WDM[iStart], sizeof(unsigned int) * iCount);
	bReturn = MemPort(membuffer);

	return bReturn;
}

//---------------------------------------------------------------------------
bool CMemComm::WriteDM(int iStart, int Data)
{
	TMemCommand membuffer;
	membuffer.Command = CMD_WRITE_DM;
	membuffer.Arg.DataMemory.uStart = iStart;
	membuffer.Arg.DataMemory.uCount = 1;
	membuffer.Arg.DataMemory.uData[iStart] = Data;

	bReturn = MemPort(membuffer);

	return bReturn;
}

bool CMemComm::ReadBit()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_BITV;
	membuffer.Arg.IoBit.uStart = 0;
	membuffer.Arg.IoBit.uCount = 20;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WriteBit(int iStart, int Data)
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_BITV;
	membuffer.Arg.IoBit.uStart = iStart;
	membuffer.Arg.IoBit.uCount = 1;
	membuffer.Arg.IoBit.uData[iStart] = Data;

	MemPort(membuffer);
	return false;
}

bool CMemComm::ReadIo()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_IO;
	membuffer.Arg.IoBit.uStart = 0;
	membuffer.Arg.IoBit.uCount = 10;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WriteIO(int iStart, WORD Data)
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_IO;
	membuffer.Arg.IoBit.uStart = iStart;
	membuffer.Arg.IoBit.uCount = 1;
	membuffer.Arg.IoBit.uData[iStart] = Data;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WriteServoOnOff(int iAxis)
{
	TMemCommand membuffer;

	membuffer.Command = CMD_SERVO_ONOFF;
	membuffer.Arg.MotorMove.uAxisNo = iAxis;
	//membuffer.Arg.MotorMove.bOnOff = bOnOff;

	MemPort(membuffer);

	return bReturn;
}
bool CMemComm::WriteServoHome(int iAxis)
{
	TMemCommand membuffer;

	membuffer.Command = CMD_SERVO_HOME;
	membuffer.Arg.MotorMove.uAxisNo = iAxis;

	MemPort(membuffer);

	return bReturn;
}
bool CMemComm::WriteServoAlarmClear(int iAxis)
{
	TMemCommand membuffer;

	membuffer.Command = CMD_SERVO_ALARM_CLEAR;
	membuffer.Arg.MotorMove.uAxisNo = iAxis;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WriteMotorVelocityMove(int iAxis, int iDirection, unsigned int uSpeed)
{
	TMemCommand membuffer;

	membuffer.Command = CMD_MOTORSCONTINUE;
	membuffer.Arg.MotorMove.uAxisNo = iAxis;
	membuffer.Arg.MotorMove.iDir = iDirection;
	membuffer.Arg.MotorMove.uVel = uSpeed;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WriteMotorRelativeMove(int iAxis, int iPos, unsigned int uSpeed)
{
	TMemCommand membuffer;

	membuffer.Command = CMD_MOTORSRELATIVE;
	membuffer.Arg.MotorMove.uAxisNo = iAxis;
	membuffer.Arg.MotorMove.iPos = iPos;
	membuffer.Arg.MotorMove.uVel = uSpeed;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WriteMotorAbsoluteMove(int iAxis, int iIndex)
{
	TMemCommand membuffer;

	membuffer.Command = CMD_MOTORINDEXMOVE;
	membuffer.Arg.MotorMove.uAxisNo = iAxis;
	membuffer.Arg.MotorMove.uIdx = iIndex;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WriteMotorStop(int iAxis)
{
	TMemCommand membuffer;

	membuffer.Command = CMD_MOTORSTOP;
	membuffer.Arg.MotorMove.uAxisNo = iAxis;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::ReadMotorStatus(int iAxis)
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_MOTORSTATUS;
	membuffer.Arg.MotorStatus.MotorNum = iAxis;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::ReadMotorData(int iAxis)
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_MOTORDATA;
	membuffer.Arg.MotorData.uAxisNo = iAxis;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WriteMotorData()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_MOTORDATA;
	memcpy(&membuffer.Arg.MotorData, &MTWD, sizeof(MTWD));

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::ReadMotorConfig(int iAxis)
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_MOTORCONFIG;
	membuffer.Arg.MotorCfg.uAxisNo = iAxis;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WriteMotorConfig()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_MOTORCONFIG;
	memcpy(&membuffer.Arg.MotorCfg, &MTWDCFG, sizeof(MTWDCFG));

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::ReadDevice()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_DEVICEDATA;
	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WriteDevice()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_DEVICEDATA;
	membuffer.Arg.Device = WDEV;
	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WriteTenkey(unsigned short int iTenkey)
{
	TMemCommand membuffer;

	membuffer.Command = CMD_TENKEY;
	membuffer.Arg.Manual.TenKeyNumber = iTenkey;
	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WriteLampBuzzer()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_LAMPBUZZER;
	membuffer.Arg.LampBuzzer = LAMP;
	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::ReadErrorList()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_ERRORCODE;
	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WriteExitProgram()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_PROGRAMEXIT;
	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::ReadMachineStatus()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_STATUS;
	MemPort(membuffer);
	return bReturn;
}

bool CMemComm::WriteMachineStatus()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_STATUS;
	membuffer.Arg.MachineStatus = WMACHINESTATUS;
	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::ReadUserInfo()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_USER_INFO;
	MemPort(membuffer);
	return bReturn;
}
bool CMemComm::WriteUserInfo()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_USER_INFO;
	membuffer.Arg.UserInfo = WUSERINFO;
	MemPort(membuffer);

	return bReturn;
}
bool CMemComm::ReadLotInfo()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_LOTINFO;
	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WriteLotInfo()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_LOTINFO;
	membuffer.Arg.LotInfo = WLOTINFO;
	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::ReadDeviceInfo()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_DEVICE_INFO;
	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WriteDeviceInfo()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_DEVICE_INFO;
	membuffer.Arg.DeviceInfo = WDEVICEINFO;
	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WriteSystemData()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_SYSTEM_DATA;
	membuffer.Arg.SystemData = WSystemData;
	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WriteTenKeyJog()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_TENKEYJOG;

	membuffer.Arg.TenKeyJog = WTENKEYJOG;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WriteLoadCntClear()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_LOAD_CNT_CLEAR;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WriteUnLoadCntClear()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_UNLOAD_CNT_CLEAR;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WriteESTOP()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_EMERGENCY_STOP;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WriteBuzzerOff()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_BUZZER_OFF;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WriteSet3Point(int pktype, int target, int pointno)
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_3POINT_SETTING;

	membuffer.Arg.Set3Point.pktype = pktype;
	membuffer.Arg.Set3Point.target = target;
	membuffer.Arg.Set3Point.pointno = pointno;

	MemPort(membuffer);

	return bReturn;
}
bool CMemComm::ReadSet3PointOffset()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_3POINT_OFFSET;
	MemPort(membuffer);
	return bReturn;
}
bool CMemComm::ReadFlip1VisionResult()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_FLIP1_VISION_RESULT;
	MemPort(membuffer);
	return bReturn;
}
bool CMemComm::WriteFlip1VisionResult()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_FLIP1_VISION_RESULT;

	membuffer.Arg.Flip1VisionResult = WFlip1VisionResult;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::ReadFlip2VisionResult()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_FLIP2_VISION_RESULT;
	MemPort(membuffer);
	return bReturn;
}
bool CMemComm::WriteFlip2VisionResult()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_FLIP2_VISION_RESULT;

	membuffer.Arg.Flip2VisionResult = WFlip2VisionResult;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::ReadPallet1VisionResult()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_PALLET1_VISION_RESULT;
	MemPort(membuffer);
	return bReturn;
}
bool CMemComm::WritePallet1VisionResult()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_PALLET1_VISION_RESULT;

	membuffer.Arg.Pallet1VisionResult = WPallet1VisionResult;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::ReadPallet2VisionResult()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_PALLET2_VISION_RESULT;
	MemPort(membuffer);
	return bReturn;
}
bool CMemComm::WritePallet2VisionResult()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_PALLET2_VISION_RESULT;

	membuffer.Arg.Pallet2VisionResult = WPallet2VisionResult;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::ReadFrontPickerVisionResult()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_FRONT_PICKER_RESULT;
	MemPort(membuffer);
	return bReturn;
}
bool CMemComm::WriteFrontPkVisionResult()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_FRONT_PICKER_VISION_RESULT;

	membuffer.Arg.FrontPkVisionResult = WFrontPkVisionResult;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::ReadRearPickerVisionResult()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_REAR_PICKER_RESULT;
	MemPort(membuffer);
	return bReturn;
}
bool CMemComm::WriteRearPickerVisionResult()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_REAR_PICKER_VISION_RESULT;

	membuffer.Arg.RearPkVisionResult = WRearPkVisionResult;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::ReadGoodTray1VisionResult()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_GOOD_TRAY1_VISION_RESULT;
	MemPort(membuffer);
	return bReturn;
}

bool CMemComm::ReadGoodTray2VisionResult()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_GOOD_TRAY2_VISION_RESULT;
	MemPort(membuffer);
	return bReturn;
}

bool CMemComm::ReadReworkTrayVisionResult()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_REWORK_TRAY_VISION_RESULT;
	MemPort(membuffer);
	return bReturn;
}
bool CMemComm::ReadNGTrayVisionResult()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_NG_TRAY_VISION_RESULT;
	MemPort(membuffer);
	return bReturn;
}

bool CMemComm::ReadFlip1Map()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_FLIP1_MAP;
	MemPort(membuffer);
	return bReturn;
}
bool CMemComm::WriteFlip1Map()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_FLIP1_MAP;

	membuffer.Arg.Flip1Map = WFlip1Map;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::ReadFlip2Map()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_FLIP2_MAP;
	MemPort(membuffer);
	return bReturn;
}
bool CMemComm::WriteFlip2Map()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_FLIP2_MAP;

	membuffer.Arg.Flip2Map = WFlip2Map;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::ReadPallet1Map()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_PALLET1_MAP;
	MemPort(membuffer);
	return bReturn;
}
bool CMemComm::WritePallet1Map()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_PALLET1_MAP;

	membuffer.Arg.Pallet1Map = WPallet1Map;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::ReadPallet2Map()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_PALLET2_MAP;
	MemPort(membuffer);
	return bReturn;
}
bool CMemComm::WritePallet2Map()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_PALLET2_MAP;

	membuffer.Arg.Pallet2Map = WPallet2Map;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::ReadFrontPickerMap()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_FRONT_PICKER_MAP;
	MemPort(membuffer);
	return bReturn;
}

bool CMemComm::ReadRearPickerMap()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_REAR_PICKER_MAP;
	MemPort(membuffer);
	return bReturn;
}

bool CMemComm::ReadGoodTray1Map()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_GOOD_TRAY1_MAP;
	MemPort(membuffer);
	return bReturn;
}
bool CMemComm::WriteGoodTray1Map()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_GOOD_TRAY1_MAP;

	membuffer.Arg.GoodTray1Map = WGoodTray1Map;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::ReadGoodTray2Map()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_GOOD_TRAY2_MAP;
	MemPort(membuffer);
	return bReturn;
}
bool CMemComm::WriteGoodTray2Map()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_GOOD_TRAY2_MAP;

	membuffer.Arg.GoodTray2Map = WGoodTray2Map;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::ReadReworkTrayMap()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_REWORK_TRAY_MAP;
	MemPort(membuffer);
	return bReturn;
}
bool CMemComm::WriteRewTrayMap()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_REWORK_TRAY_MAP;

	membuffer.Arg.ReworkTrayMap = WReworkTrayMap;

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::ReadNGTrayMap()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_NG_TRAY_MAP;
	MemPort(membuffer);
	return bReturn;
}
bool CMemComm::WriteNGTrayMap()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_NG_TRAY_MAP;

	membuffer.Arg.NGTrayMap = WNGTrayMap;

	MemPort(membuffer);

	return bReturn;
}

//Picker Calibration
bool CMemComm::WritePKCenMove()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_PK_CENTER_MOVE;
	memcpy(&membuffer.Arg.PkCenterMove, &W_PKCenMove, sizeof(W_PKCenMove));

	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WriteCenTrig()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_PK_CENTER_TRIGGER;
	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::WritePKAutoCal()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_WRITE_PK_CENTER_AUTO_CAL;
	memcpy(&membuffer.Arg.PkCenterMove, &W_PKCenMove, sizeof(W_PKCenMove));
	MemPort(membuffer);

	return bReturn;
}

bool CMemComm::ReadFPKOffset()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_FRONT_PK_CENTER_OFFSET;
	MemPort(membuffer);
	return bReturn;
}

bool CMemComm::ReadRPKOffset()
{
	TMemCommand membuffer;

	membuffer.Command = CMD_READ_REAR_PK_CENTER_OFFSET;
	MemPort(membuffer);
	return bReturn;
}

