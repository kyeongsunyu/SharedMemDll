#include "pch.h"

#include "SharedMemDll.h"

using namespace SharedMemDll;
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Diagnostics;

//////////////////////////////////////////////////////////////////////////
bool CSharedMemory::IsGetDM()
{
	if (memcomm == nullptr) return false;
	
	return memcomm->ReadDM();
}
unsigned int CSharedMemory::GetDM(int iStart)
{
	if (memcomm == nullptr) return false;

	return memcomm->DM[iStart];
}
bool CSharedMemory::GetDM(int iStart, unsigned int% pData)
{
	if (memcomm == nullptr) return false;
	
	pData = memcomm->DM[iStart];
	return true;
}
bool CSharedMemory::SetDM(int iStart, unsigned int Data)
{
	if (memcomm == nullptr) return false;
	//-- MAX_DM을 넘어서면 실패를 리턴한다.
	if (iStart >= (MAX_DM - 1)) return false;
	return memcomm->WriteDM(iStart, Data);
}

bool CSharedMemory::GetDM(int iStart, int iCount, array<unsigned long>^ pData)
{
	if (memcomm == nullptr) return false;
	//-- MAX_DM을 넘어서면 실패를 리턴한다.
	if ((iCount + iStart) > MAX_DM) return false;

	for (int nIdx = 0; nIdx < iCount; nIdx++)
	{
		pData[nIdx] = memcomm->DM[iStart + nIdx];
	}

	//memcpy(&pData, &memcomm->DM[iStart], sizeof(unsigned long) * iCount);
	return true;
}

bool CSharedMemory::SetDM(int iStart, int iCount, array<unsigned long>^ pData)
{
	if (memcomm == nullptr) return false;
	//-- MAX_DM을 넘어서면 실패를 리턴한다. Max 100개 까지 한껀번에 보낼수 있다.
	if ((iCount + iStart) > MAX_DM) return false;

	for (int nIdx = 0; nIdx < iCount; nIdx++)
	{
		memcomm->WDM[iStart + nIdx] = pData[nIdx];
	}

	//memcpy(&memcomm->WDM[iStart], &pData, sizeof(unsigned long) * iCount);
	return memcomm->WriteArrayDM(iStart, iCount);
}
//////////////////////////////////////////////////////////////////////////
bool CSharedMemory::IsGetBit()
{
	if (memcomm == nullptr) return false;
	
	return memcomm->ReadBit();
}

unsigned short int CSharedMemory::GetBit(int iStart)
{
	if (memcomm == nullptr) return false;

	return memcomm->BIT[iStart];
}

bool CSharedMemory::SetBit(int iStart,int Data)
{
	if (memcomm == nullptr) return false;

	return memcomm->WriteBit(iStart, Data);
}
//////////////////////////////////////////////////////////////////////////
bool CSharedMemory::IsGetIO()
{
	if (memcomm == nullptr) return false;

	return memcomm->ReadIo();
}

unsigned short int CSharedMemory::GetIO(int iChannel)
{
	if (memcomm == nullptr) return false;

	if (memcomm == nullptr) return false;
	return memcomm->IO[iChannel];
}

bool CSharedMemory::SetOutput(int iChannel, WORD data)
{
	if (memcomm == nullptr) return false;

	return memcomm->WriteIO(iChannel, data);
}
//////////////////////////////////////////////////////////////////////////
bool CSharedMemory::SetServoOnOff(int iAxisNo)
{
	if (memcomm == nullptr) return false;

	return memcomm->WriteServoOnOff(iAxisNo);
}

bool CSharedMemory::SetServoHome(int iAxisNo)
{
	if (memcomm == nullptr) return false;

	return memcomm->WriteServoHome(iAxisNo);
}
bool CSharedMemory::SetServoAlarmClear(int iAxisNo)
{
	if (memcomm == nullptr) return false;

	return memcomm->WriteServoAlarmClear(iAxisNo);
}

bool CSharedMemory::SetMotorVelocityMove(int iAxis, int iDirection, unsigned int uSpeed)
{
	if (memcomm == nullptr) return false;

	return memcomm->WriteMotorVelocityMove(iAxis, iDirection, uSpeed);
}

bool CSharedMemory::SetMotorRelativeMove(int iAxis, int iPos, unsigned int uSpeed)
{
	if (memcomm == nullptr) return false;

	return memcomm->WriteMotorRelativeMove(iAxis, iPos, uSpeed);
}

bool CSharedMemory::SetMotorAbsoluteMove(int iAxis, int iIndex)
{
	if (memcomm == nullptr) return false;

	return memcomm->WriteMotorAbsoluteMove(iAxis, iIndex);
}

bool CSharedMemory::SetMotorStop(int iAxis)
{
	if (memcomm == nullptr) return false;

	return memcomm->WriteMotorStop(iAxis);
}

bool CSharedMemory::GetMotorStatus(int iAxiNo)//,[out] MotorStatus% ms)
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadMotorStatus(iAxiNo))
	{
		RMTStatus->MotorNum = memcomm->MTRS[iAxiNo].MotorNum;
		
		RMTStatus->CCW = memcomm->MTRS[iAxiNo].CCW;
		RMTStatus->CW = memcomm->MTRS[iAxiNo].CW;
		RMTStatus->Org = memcomm->MTRS[iAxiNo].Org;
		RMTStatus->HOME = memcomm->MTRS[iAxiNo].HOME;
		RMTStatus->SVON = memcomm->MTRS[iAxiNo].SVON;
		RMTStatus->Busy = memcomm->MTRS[iAxiNo].Busy;
		RMTStatus->ALM = memcomm->MTRS[iAxiNo].ALM;
		RMTStatus->CurrentIndex = memcomm->MTRS[iAxiNo].CurrentIndex;
		RMTStatus->NextIndex = memcomm->MTRS[iAxiNo].NextIndex;
		RMTStatus->CurrentPosition = memcomm->MTRS[iAxiNo].CurrentPosition;
		RMTStatus->NextPosition = memcomm->MTRS[iAxiNo].NextPosition;

		RMTStatus->uHomeStep = memcomm->MTRS[iAxiNo].uHomeStep;
		RMTStatus->dwServoAlarmCode = memcomm->MTRS[iAxiNo].dwServoAlarmCode;
		RMTStatus->strServoAlarmName = gcnew System::String(memcomm->MTRS[iAxiNo].strServoAlarmName, 0, 1025);
		RMTStatus->dServoLoadRatio = memcomm->MTRS[iAxiNo].dServoLoadRatio;
		return true;
	}
	return false;
}

bool CSharedMemory::GetMachineStatus()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadMachineStatus())
	{
		RMachineStatus->nDeviceNo = memcomm->RMACHINESTATUS.nDeviceNo;
		RMachineStatus->strDeviceName = gcnew System::String(memcomm->RMACHINESTATUS.strDeviceName, 0, 64);

		RMachineStatus->UPH = memcomm->RMACHINESTATUS.UPH;
		RMachineStatus->TPH = memcomm->RMACHINESTATUS.TPH;
		RMachineStatus->PanelInCount = memcomm->RMACHINESTATUS.PanelInCnt;
		RMachineStatus->UnitInCnt = memcomm->RMACHINESTATUS.UnitInCnt;
		RMachineStatus->UnitOutCnt = memcomm->RMACHINESTATUS.UnitOutCnt;
		RMachineStatus->UnitGoodCnt = memcomm->RMACHINESTATUS.UnitGoodCnt;
		RMachineStatus->UnitReworkCnt = memcomm->RMACHINESTATUS.UnitReworkCnt;
		RMachineStatus->UnitNGCnt = memcomm->RMACHINESTATUS.UnitNGCnt;

		RMachineStatus->FlipY1AirCleanCnt = memcomm->RMACHINESTATUS.FlipY1AirCleanCnt;
		RMachineStatus->FlipY2AirCleanCnt = memcomm->RMACHINESTATUS.FlipY2AirCleanCnt;
		RMachineStatus->PalletY1AirCleanCnt = memcomm->RMACHINESTATUS.PalletY1AirCleanCnt;
		RMachineStatus->PalletY2AirCleanCnt = memcomm->RMACHINESTATUS.PalletY2AirCleanCnt;
		RMachineStatus->SawPkFlipY1AirClenaCnt = memcomm->RMACHINESTATUS.SawPkFlipY1AirClenaCnt;
		RMachineStatus->SawPkFlipY2AirClenaCnt = memcomm->RMACHINESTATUS.SawPkFlipY2AirClenaCnt;
		RMachineStatus->SpongeCleanCnt = memcomm->RMACHINESTATUS.SpongeCleanCnt;
		RMachineStatus->WaterJetWaterCleanCnt = memcomm->RMACHINESTATUS.WaterJetWaterCleanCnt;
		RMachineStatus->WaterJetAirCleanCnt = memcomm->RMACHINESTATUS.WaterJetAirCleanCnt;
		RMachineStatus->AirPressure1 = memcomm->RMACHINESTATUS.AirPressure1;
		RMachineStatus->AirPressure2 = memcomm->RMACHINESTATUS.AirPressure2;
		RMachineStatus->AirPressure3 = memcomm->RMACHINESTATUS.AirPressure3;

		pin_ptr<int> pHomeState = &RMachineStatus->HomeState[0];
		int* npHomeState = pHomeState;
		memcpy(npHomeState, memcomm->RMACHINESTATUS.HomeState, sizeof(int) * 60);

		pin_ptr<int> pInputState = &RMachineStatus->bInputState[0, 0];
		int* npInputState = pInputState;
		memcpy(npInputState, memcomm->RMACHINESTATUS.bInputState, sizeof(bool) * 20 * 16);

		pin_ptr<int> pOutputState = &RMachineStatus->bOutputState[0, 0];
		int* npOutputState = pOutputState;
		memcpy(npOutputState, memcomm->RMACHINESTATUS.bOutputState, sizeof(bool) * 20 * 16);

		//for (int i = 0; i < 60; i++)
		//{
		//	RMachineStatus->HomeState[i] = memcomm->RMACHINESTATUS.HomeState[i];
		//}
		//for (int i = 0; i < 20; i++)
		//{
		//	for (int j = 0; j < 16; j++) 
		//	{
		//		RMachineStatus->bInputState[i][j] = memcomm->RMACHINESTATUS.bInputState[i][j];
		//		RMachineStatus->bOutputState[i][j] = memcomm->RMACHINESTATUS.bOutputState[i][j];
		//	}
		//}

		RMachineStatus->bVisionConnected = memcomm->RMACHINESTATUS.bVisionConnected;
		return true;
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
//bool CSharedMemory::GetMotorData(
//	int iAxis, 
//	array<double>^ puPos, array<double>^ puVel,
//	array<double>^ pdPos, array<double>^ pdVel)
//{
//	if (memcomm == nullptr) return false;
//
//	bool bRet = memcomm->ReadMotorData(iAxis);
//
//	memcpy(&puPos, &memcomm->MTRD[iAxis].uPos, sizeof(int) * 100);
//	memcpy(&puVel, &memcomm->MTRD[iAxis].uVel, sizeof(int) * 100);
//	memcpy(&pdPos, &memcomm->MTRD[iAxis].fPos, sizeof(double) * 100);
//	memcpy(&pdVel, &memcomm->MTRD[iAxis].fVel, sizeof(double) * 100);
//	
//	return bRet;
//}
//
//bool CSharedMemory::SetMotorData(int iAxis,
//	array<double>^ puPos, array<double>^ puVel,
//	array<double>^ pdPos, array<double>^ pdVel)
//{
//	if (memcomm == nullptr) return false;
//
//	memcomm->MTWD.uAxisNo = iAxis;
//	memcpy(&memcomm->MTWD.uPos, &puPos, sizeof(int) * 100);
//	memcpy(&memcomm->MTWD.uVel, &puVel, sizeof(int) * 100);
//	memcpy(&memcomm->MTWD.fPos, &pdPos, sizeof(double) * 100);
//	memcpy(&memcomm->MTWD.fVel, &pdVel, sizeof(double) * 100);
//
//	return memcomm->WriteMotorData();
//}
bool CSharedMemory::GetMotorData(int iAxisNo)
{
	if (memcomm == nullptr) return false;
	
	if (memcomm->ReadMotorData(iAxisNo))
	{
		//RMotorData[iAxisNo]->uAxisNo = memcomm->MTRD[iAxisNo].uAxisNo;
		//for (int i = 0; i < 100; i++) {
		//	RMotorData[iAxisNo]->fPos[i] = memcomm->MTRD[iAxisNo].fPos[i];
		//	RMotorData[iAxisNo]->fVel[i] = memcomm->MTRD[iAxisNo].fVel[i];
		//	RMotorData[iAxisNo]->uPos[i] = memcomm->MTRD[iAxisNo].uPos[i];
		//	RMotorData[iAxisNo]->uVel[i] = memcomm->MTRD[iAxisNo].uVel[i];
		//}
		
		RMotorData[iAxisNo]->uAxisNo = memcomm->MTRD[iAxisNo].uAxisNo;

		pin_ptr<double> pin_pfPos = &RMotorData[iAxisNo]->fPos[0];
		double* pfPos = pin_pfPos;
		memcpy(pfPos, memcomm->MTRD[iAxisNo].fPos, sizeof(double) * 100);

		pin_ptr<double> pin_pfVel = &RMotorData[iAxisNo]->fVel[0];
		double* pfVel = pin_pfVel;
		memcpy(pfVel, memcomm->MTRD[iAxisNo].fVel, sizeof(double) * 100);

		pin_ptr<double> pin_puPos = &RMotorData[iAxisNo]->uPos[0];
		double* puPos = pin_puPos;
		memcpy(puPos, memcomm->MTRD[iAxisNo].uPos, sizeof(double) * 100);

		pin_ptr<double> pin_puVel = &RMotorData[iAxisNo]->uVel[0];
		double* puVel = pin_puVel;
		memcpy(puVel, memcomm->MTRD[iAxisNo].uVel, sizeof(double) * 100);

		return true;
	}
	return false;
}
bool CSharedMemory::SetMotorData()
{
	if (memcomm == nullptr) return false;

	//memcomm->MTWD.uAxisNo = WMotorData->uAxisNo;
	//for (int i = 0; i < 100; i++) {
	//	memcomm->MTWD.fPos[i] = WMotorData->fPos[i];
	//	memcomm->MTWD.fVel[i] = WMotorData->fVel[i];
	//	memcomm->MTWD.uPos[i] = WMotorData->uPos[i];
	//	memcomm->MTWD.uVel[i] = WMotorData->uVel[i];
	//}

	memcomm->MTWD.uAxisNo = WMotorData->uAxisNo;

	pin_ptr<double> pin_pfPos = &WMotorData->fPos[0];
	double* pfPos = pin_pfPos;
	memcpy(memcomm->MTWD.fPos, pfPos, sizeof(double) * 100);

	pin_ptr<double> pin_pfVel = &WMotorData->fVel[0];
	double* pfVel = pin_pfVel;
	memcpy(memcomm->MTWD.fVel, pfVel, sizeof(double) * 100);

	pin_ptr<double> pin_puPos = &WMotorData->uPos[0];
	double* puPos = pin_puPos;
	memcpy(memcomm->MTWD.uPos, puPos, sizeof(double) * 100);

	pin_ptr<double> pin_puVel = &WMotorData->uVel[0];
	double* puVel = pin_puVel;
	memcpy(memcomm->MTWD.uVel, puVel, sizeof(double) * 100);

	return memcomm->WriteMotorData();
}
//////////////////////////////////////////////////////////////////////////
bool CSharedMemory::GetMotorConfig(int iAxis)
{
	if (memcomm == nullptr) return false;

	bool bRet = memcomm->ReadMotorConfig(iAxis);
	
	RMTConfig->uAxisNo		= memcomm->MTCONFIG[iAxis].uAxisNo;
	RMTConfig->uRate		= memcomm->MTCONFIG[iAxis].uRate;
	RMTConfig->uAccel		= memcomm->MTCONFIG[iAxis].uAccel;
	RMTConfig->uMaxVel		= memcomm->MTCONFIG[iAxis].uMaxVel;
	RMTConfig->uJogVel		= memcomm->MTCONFIG[iAxis].uJogVel;
	RMTConfig->uHomeVel		= memcomm->MTCONFIG[iAxis].uHomeVel;
	RMTConfig->uMotorType	= memcomm->MTCONFIG[iAxis].uMotorType;
	RMTConfig->uCardType	= memcomm->MTCONFIG[iAxis].uCardType;
	RMTConfig->uCardIndex	= memcomm->MTCONFIG[iAxis].uCardIndex;
	RMTConfig->uUseSkip		= memcomm->MTCONFIG[iAxis].uUseSkip;
	RMTConfig->uHomeLevel	= memcomm->MTCONFIG[iAxis].uHomeLevel;
	RMTConfig->uLimitLevel	= memcomm->MTCONFIG[iAxis].uLimitLevel;
	RMTConfig->uServoOnLevel= memcomm->MTCONFIG[iAxis].uServoOnLevel;
	RMTConfig->uAlarmLevel	= memcomm->MTCONFIG[iAxis].uAlarmLevel;
	RMTConfig->uInpUse		= memcomm->MTCONFIG[iAxis].uInpUse;
	RMTConfig->uHomeMethod	= memcomm->MTCONFIG[iAxis].uHomeMethod;
	RMTConfig->uMtrDir		= memcomm->MTCONFIG[iAxis].uMtrDir;
	RMTConfig->uEncDir		= memcomm->MTCONFIG[iAxis].uEncDir;
	RMTConfig->uMaxRec		= memcomm->MTCONFIG[iAxis].uMaxRec;
	RMTConfig->uEncoderType = memcomm->MTCONFIG[iAxis].uEncoderType;

	return bRet;
}

bool CSharedMemory::SetMotorConfig()
{
	if (memcomm == nullptr) return false;

	memcomm->MTWDCFG.uAxisNo		= WMTConfig->uAxisNo ;
	memcomm->MTWDCFG.uRate			= WMTConfig->uRate ;
	memcomm->MTWDCFG.uAccel			= WMTConfig->uAccel ;
	memcomm->MTWDCFG.uMaxVel		= WMTConfig->uMaxVel ;
	memcomm->MTWDCFG.uJogVel		= WMTConfig->uJogVel ;
	memcomm->MTWDCFG.uHomeVel		= WMTConfig->uHomeVel ;
	memcomm->MTWDCFG.uMotorType		= WMTConfig->uMotorType ;
	memcomm->MTWDCFG.uCardType		= WMTConfig->uCardType ;
	memcomm->MTWDCFG.uCardIndex		= WMTConfig->uCardIndex ;
	memcomm->MTWDCFG.uUseSkip		= WMTConfig->uUseSkip ;
	memcomm->MTWDCFG.uHomeLevel		= WMTConfig->uHomeLevel ;
	memcomm->MTWDCFG.uLimitLevel	= WMTConfig->uLimitLevel ;
	memcomm->MTWDCFG.uServoOnLevel	= WMTConfig->uServoOnLevel ;
	memcomm->MTWDCFG.uAlarmLevel	= WMTConfig->uAlarmLevel ;
	memcomm->MTWDCFG.uInpUse		= WMTConfig->uInpUse ;
	memcomm->MTWDCFG.uHomeMethod	= WMTConfig->uHomeMethod ;
	memcomm->MTWDCFG.uMtrDir		= WMTConfig->uMtrDir ;
	memcomm->MTWDCFG.uEncDir		= WMTConfig->uEncDir ;
	memcomm->MTWDCFG.uMaxRec		= WMTConfig->uMaxRec ;
	memcomm->MTWDCFG.uEncoderType	= WMTConfig->uEncoderType ;

	return memcomm->WriteMotorConfig();
}

//////////////////////////////////////////////////////////////////////////
bool CSharedMemory::GetRecipe()
{
	if (memcomm == nullptr) return false;

	if(memcomm->ReadDevice())
	{
		/*
		for (int i = 0; i < 200; i++) {
			RRecipeData->dDeviceData[i] = memcomm->RDEV.dDeviceData[i];
		}

		for (int i = 0; i < 100; i++) {
			RRecipeData->dData[i] = memcomm->RDEV.dData[i];
			RRecipeData->iData[i] = memcomm->RDEV.iData[i];
			RRecipeData->bData[i] = memcomm->RDEV.bData[i];
		}
		*/


		/* Original Code
		for (int i = 0; i < 200; i++) {
			RRecipeData->dData[i] = memcomm->RDEV.dData[i];
		}
		*/


		//pin_ptr<double> p1 = &RRecipeData->dData[0];
		//double* p2 = p1;
		//memcpy(p2, memcomm->RDEV.dData, sizeof(double) * 100);

		return true;
	}
	return false;
}
bool CSharedMemory::SetRecipe()
{
	if (memcomm == nullptr) return false;


	memcomm->WDEV.Unit_X_Size = WRecipeData->Unit_X_Size;
	memcomm->WDEV.Unit_Y_Size = WRecipeData->Unit_Y_Size;
	memcomm->WDEV.Unit_X_Count = WRecipeData->Unit_X_Count;
	memcomm->WDEV.Unit_Y_Count = WRecipeData->Unit_Y_Count;
	memcomm->WDEV.Unit_X_Pitch = WRecipeData->Unit_X_Pitch;
	memcomm->WDEV.Unit_Y_Pitch = WRecipeData->Unit_Y_Pitch;
	memcomm->WDEV.Tray_X_Count = WRecipeData->Tray_X_Count;
	memcomm->WDEV.Tray_Y_Count = WRecipeData->Tray_Y_Count;
	memcomm->WDEV.Vision_Snap_X_Count = WRecipeData->Vision_Snap_X_Count;
	memcomm->WDEV.Vision_Snap_Y_Count = WRecipeData->Vision_Snap_Y_Count;
	memcomm->WDEV.Picker_Angle = WRecipeData->Picker_Angle;
	memcomm->WDEV.Picker_Place_Vac_Off_Offset = WRecipeData->Picker_Place_Vac_Off_Offset;

	memcomm->WDEV.Front_Picker_1_Skip = WRecipeData->Front_Picker_1_Skip;
	memcomm->WDEV.Front_Picker_2_Skip = WRecipeData->Front_Picker_2_Skip;
	memcomm->WDEV.Front_Picker_3_Skip = WRecipeData->Front_Picker_3_Skip;
	memcomm->WDEV.Front_Picker_4_Skip = WRecipeData->Front_Picker_4_Skip;
	memcomm->WDEV.Front_Picker_5_Skip = WRecipeData->Front_Picker_5_Skip;
	memcomm->WDEV.Front_Picker_6_Skip = WRecipeData->Front_Picker_6_Skip;
	memcomm->WDEV.Front_Picker_7_Skip = WRecipeData->Front_Picker_7_Skip;
	memcomm->WDEV.Front_Picker_8_Skip = WRecipeData->Front_Picker_8_Skip;
	memcomm->WDEV.Rear_Picker_1_Skip = WRecipeData->Rear_Picker_1_Skip;
	memcomm->WDEV.Rear_Picker_2_Skip = WRecipeData->Rear_Picker_2_Skip;
	memcomm->WDEV.Rear_Picker_3_Skip = WRecipeData->Rear_Picker_3_Skip;
	memcomm->WDEV.Rear_Picker_4_Skip = WRecipeData->Rear_Picker_4_Skip;
	memcomm->WDEV.Rear_Picker_5_Skip = WRecipeData->Rear_Picker_5_Skip;
	memcomm->WDEV.Rear_Picker_6_Skip = WRecipeData->Rear_Picker_6_Skip;
	memcomm->WDEV.Rear_Picker_7_Skip = WRecipeData->Rear_Picker_7_Skip;
	memcomm->WDEV.Rear_Picker_8_Skip = WRecipeData->Rear_Picker_8_Skip;
	memcomm->WDEV.Scrap_Skip = WRecipeData->Scrap_Skip;
	memcomm->WDEV.Sponge_Clean_Skip = WRecipeData->Sponge_Clean_Skip;

	memcomm->WDEV.Saw_Picker_Sponge_Clean = WRecipeData->Saw_Picker_Sponge_Clean;
	memcomm->WDEV.Water_Jet_Water_Clean = WRecipeData->Water_Jet_Water_Clean;
	memcomm->WDEV.Water_Jet_Air_Clean = WRecipeData->Water_Jet_Air_Clean;
	memcomm->WDEV.Saw_Picker_Flip_Y1_Air_Clean = WRecipeData->Saw_Picker_Flip_Y1_Air_Clean;
	memcomm->WDEV.Saw_Picker_Flip_Y2_Air_Clean = WRecipeData->Saw_Picker_Flip_Y2_Air_Clean;
	memcomm->WDEV.Flip_Y1_Air_Clean = WRecipeData->Flip_Y1_Air_Clean;
	memcomm->WDEV.Flip_Y2_Air_Clean = WRecipeData->Flip_Y2_Air_Clean;
	memcomm->WDEV.Pallet_Y1_Air_Clean = WRecipeData->Pallet_Y1_Air_Clean;
	memcomm->WDEV.Pallet_Y2_Air_Clean = WRecipeData->Pallet_Y2_Air_Clean;

	memcomm->WDEV.Front_Picker_Air_Blow_Time = WRecipeData->Front_Picker_Air_Blow_Time;
	memcomm->WDEV.Rear_Picker_Air_Blow_Time = WRecipeData->Rear_Picker_Air_Blow_Time;
	//-----
	memcomm->WDEV.Front_Picker_Vac_On_Time = WRecipeData->Front_Picker_Vac_On_Time;
	memcomm->WDEV.Rear_Picker_Vac_On_Time  = WRecipeData->Rear_Picker_Vac_On_Time;

	memcomm->WDEV.Load_Picker_Air_Blow_Time = WRecipeData->Load_Picker_Air_Blow_Time;
	memcomm->WDEV.Load_Picker_Vac_On_Time =WRecipeData->Load_Picker_Vac_On_Time;
	memcomm->WDEV.Saw_Picker_Air_Blow_Time = WRecipeData->Saw_Picker_Air_Blow_Time;
	memcomm->WDEV.Saw_Picker_Vac_On_Time = WRecipeData->Saw_Picker_Vac_On_Time;

	memcomm->WDEV.Pallet1_Receive_Vac_Value = WRecipeData->Pallet2_Receive_Vac_Value;
	memcomm->WDEV.Pallet2_Receive_Vac_Value = WRecipeData->Pallet2_Receive_Vac_Value;
	memcomm->WDEV.Pallet_First_Sort_Vac_Value = WRecipeData->Pallet_First_Sort_Vac_Value;
	memcomm->WDEV.Pallet_Middle_Sort_Vac_Value = WRecipeData->Pallet_Middle_Sort_Vac_Value;
	memcomm->WDEV.Pallet_Last_Sort_Vac_Value = WRecipeData->Pallet_Last_Sort_Vac_Value;
	memcomm->WDEV.Pallet_First_Sort_PKG_Rate = WRecipeData->Pallet_First_Sort_PKG_Rate;
	memcomm->WDEV.Pallet_Last_Sort_PKG_Rate = WRecipeData->Pallet_Last_Sort_PKG_Rate;

	/*
	for (int i = 0; i < 100; i++)
	{
		memcomm->WDEV.dData[i] = WRecipeData->dData[i];
	}
	*/

	/* Original Code
	pin_ptr<double> pin_p1 = &WRecipeData->dData[0];
	double* p1 = pin_p1;
	memcpy(memcomm->WDEV.dData, p1, sizeof(double) * 200);
	return memcomm->WriteDevice();
	*/
/*
	pin_ptr<double> pin_p1 = &WRecipeData->dData[0];
	double* p1 = pin_p1;
	pin_ptr<int> pin_p2 = &WRecipeData->iData[0];
	int* p2 = pin_p2;
	pin_ptr<bool> pin_p3 = &WRecipeData->bData[0];
	bool* p3 = pin_p3;
	pin_ptr<double> pin_p4 = &WRecipeData->dDeviceData[0];
	double* p4 = pin_p4;

	memcpy(memcomm->WDEV.dData, p1, sizeof(double) * 100);
	memcpy(memcomm->WDEV.iData, p2, sizeof(int) * 100);
	memcpy(memcomm->WDEV.bData, p3, sizeof(bool) * 100);
	memcpy(memcomm->WDEV.dDeviceData, p4, sizeof(double) * 200);
*/
	return memcomm->WriteDevice();
}
//////////////////////////////////////////////////////////////////////////
bool CSharedMemory::SetSystemData()
{
	if (memcomm == nullptr) return false;

	pin_ptr<double> pin_p1 = &WSystemData->dData[0];
	double* p1 = pin_p1;
	memcpy(memcomm->WSystemData.dData, p1, sizeof(double) * 100);
	return memcomm->WriteSystemData();
}
//////////////////////////////////////////////////////////////////////////
bool CSharedMemory::SetLampBuzzer()
{
	if (memcomm == nullptr) return false;

	pin_ptr<unsigned int> pin_p1 = &WLampBuzzer->uLampBuzzer[0,0];
	unsigned int* p1 = pin_p1;

	memcpy(memcomm->LAMP.uLampBuzzer, p1, sizeof(unsigned int) * 1000*10);
	return memcomm->WriteLampBuzzer();
}
//////////////////////////////////////////////////////////////////////////
bool CSharedMemory::SetLoadCountClear()
{
	if (memcomm == nullptr) return false;

	return memcomm->WriteLoadCntClear();
}
bool CSharedMemory::SetUnLoadCountClear()
{
	if (memcomm == nullptr) return false;

	return memcomm->WriteUnLoadCntClear();
}

bool CSharedMemory::SetEStop()
{
	if (memcomm == nullptr) return false;

	return memcomm->WriteESTOP();
}

bool CSharedMemory::SetTenKey(unsigned int iTenKey)
{
	if (memcomm == nullptr) return false;

	return memcomm->WriteTenkey(iTenKey);
}

bool CSharedMemory::SetTenKeyJog(int iAxisNo, bool bSet)
{
	if (memcomm == nullptr) return false;

	memcomm->WTENKEYJOG.uAxisNo = iAxisNo;
	memcomm->WTENKEYJOG.bTenKeyJog = bSet;

	return memcomm->WriteTenKeyJog();
}

//////////////////////////////////////////////////////////////////////////
bool CSharedMemory::GetUserInfo()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadUserInfo()) {
		RUserInfo->strUserName = gcnew System::String(memcomm->RUSERINFO.strUserName, 0, 64);
		return true;
	}
	return false;
}
bool CSharedMemory::SetUserInfo()
{
	if (memcomm == nullptr) return false;

	IntPtr ptrToNativeString = Marshal::StringToHGlobalAnsi(WUserInfo->strUserName);
	char* charPtrA = (char*)ptrToNativeString.ToPointer();
	strcpy_s(memcomm->WUSERINFO.strUserName, charPtrA);

	return memcomm->WriteUserInfo();
}
//////////////////////////////////////////////////////////////////////////

bool CSharedMemory::GetLotInfo()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadLotInfo())
	{
		RLotInfo->strLotID = gcnew System::String(memcomm->RLOTINFO.strLotID, 0, 128);
		RLotInfo->nLotCount = memcomm->RLOTINFO.nLotCount;

		return true;
	}
	return false;
}

bool CSharedMemory::SetLotInfo()
{
	if (memcomm == nullptr) return false;

	IntPtr ptrToNativeString = Marshal::StringToHGlobalAnsi(WLotInfo->strLotID);
	char* charPtrA = (char*)ptrToNativeString.ToPointer();
	strcpy_s(memcomm->WLOTINFO.strLotID, charPtrA);

	memcomm->WLOTINFO.nLotCount = WLotInfo->nLotCount;

	return memcomm->WriteLotInfo();
}
//////////////////////////////////////////////////////////////////////////
bool CSharedMemory::GetDeviceInfo()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadDeviceInfo()) {
		RDeviceInfo->nDeviceNumber = memcomm->RDEVICEINFO.nDeviceNumber;
		RDeviceInfo->strDeviceName = gcnew System::String(memcomm->RDEVICEINFO.strDeviceName, 0, 64);
		return true;
	}
	return false;
}
bool CSharedMemory::SetDeviceInfo()
{
	if (memcomm == nullptr) return false;

	memcomm->WDEVICEINFO.nDeviceNumber = WDeviceInfo->nDeviceNumber;

	IntPtr ptrToNativeString = Marshal::StringToHGlobalAnsi(WDeviceInfo->strDeviceName);
	char* charPtrA = (char*)ptrToNativeString.ToPointer();
	strcpy_s(memcomm->WDEVICEINFO.strDeviceName, charPtrA);

	return memcomm->WriteDeviceInfo();
}

bool CSharedMemory::SetExitProgram()
{
	if (memcomm == nullptr) return false;

	return memcomm->WriteExitProgram();
}

bool CSharedMemory::SetBuzzerOff()
{
	if (memcomm == nullptr) return false;

	return memcomm->WriteBuzzerOff();
}

bool CSharedMemory::Set3Point(int pktype, int target, int pointno)
{
	if (memcomm == nullptr) return false;

	return memcomm->WriteSet3Point(pktype, target, pointno);
}
bool CSharedMemory::Get3PointOffset()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadSet3PointOffset()) {
		RSet3Point->target = memcomm->RSet3Point.target;
		RSet3Point->pktype = memcomm->RSet3Point.pktype;
		RSet3Point->pointno = memcomm->RSet3Point.pointno;
		RSet3Point->offsetX = memcomm->RSet3Point.offsetX;
		RSet3Point->offsetY = memcomm->RSet3Point.offsetY;
		RSet3Point->offsetT = memcomm->RSet3Point.offsetT;
		return true;
	}
	return false;
}

bool CSharedMemory::GetFlip1VisionResult()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadFlip1VisionResult()) {
		pin_ptr<int> p1 = &RFlip1VisionResult->Result[0, 0];
		int* np = p1;
		memcpy(np, memcomm->RFlip1VisionResult.Result, sizeof(int) * 50 * 50);
		return true;
	}
	return false;
}
bool CSharedMemory::SetFlip1VisionResult()
{
	if (memcomm == nullptr) return false;

	pin_ptr<int> p1 = &RFlip1VisionResult->Result[0, 0];
	int* np = p1;
	memcpy(memcomm->RFlip1VisionResult.Result, np, sizeof(int) * 50 * 50);
	return memcomm->WriteFlip1VisionResult();
}
bool CSharedMemory::GetFlip2VisionResult()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadFlip2VisionResult()) {
		pin_ptr<int> p1 = &RFlip2VisionResult->Result[0, 0];
		int* np = p1;
		memcpy(np, memcomm->RFlip2VisionResult.Result, sizeof(int) * 50 * 50);
		return true;
	}
	return false;
}
bool CSharedMemory::SetFlip2VisionResult()
{
	if (memcomm == nullptr) return false;

	pin_ptr<int> p1 = &RFlip2VisionResult->Result[0, 0];
	int* np = p1;
	memcpy(memcomm->RFlip2VisionResult.Result, np, sizeof(int) * 50 * 50);
	return memcomm->WriteFlip2VisionResult();
}
bool CSharedMemory::GetPallet1VisionResult()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadPallet1VisionResult()) {
		pin_ptr<int> p1 = &RPallet1VisionResult->Result[0, 0];
		int* np = p1;
		memcpy(np, memcomm->RPallet1VisionResult.Result, sizeof(int) * 50 * 50);
		return true;
	}
	return false;
}
bool CSharedMemory::SetPallet1VisionResult()
{
	if (memcomm == nullptr) return false;

	pin_ptr<int> p1 = &WPallet1VisionResult->Result[0, 0];
	int* np = p1;
	memcpy(memcomm->WPallet1VisionResult.Result, np, sizeof(int) * 50 * 50);
	return memcomm->WritePallet1VisionResult();
}
bool CSharedMemory::GetPallet2VisionResult()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadPallet2VisionResult()) {
		pin_ptr<int> p1 = &RPallet2VisionResult->Result[0, 0];
		int* np = p1;
		memcpy(np, memcomm->RPallet2VisionResult.Result, sizeof(int) * 50 * 50);
		return true;
	}
	return false;
}
bool CSharedMemory::SetPallet2VisionResult()
{
	if (memcomm == nullptr) return false;

	pin_ptr<int> p1 = &WPallet2VisionResult->Result[0, 0];
	int* np = p1;
	memcpy(memcomm->WPallet2VisionResult.Result, np, sizeof(int) * 50 * 50);
	return memcomm->WritePallet2VisionResult();
}
bool CSharedMemory::GetFrontPickerVisionResult()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadFrontPickerVisionResult()) {
		pin_ptr<int> p1 = &RFrontPkVisionResult->Result[0];
		int* np = p1;
		memcpy(np, memcomm->RFrontPkVisionResult.Result, sizeof(int) * 10);
		return true;
	}
	return false;
}
bool CSharedMemory::SetFrontPickerVisionResult()
{
	if (memcomm == nullptr) return false;

	pin_ptr<int> p1 = &WFrontPkVisionResult->Result[0];
	int* np = p1;
	memcpy(memcomm->WFrontPkVisionResult.Result, np, sizeof(int) * 10);
	return memcomm->WriteFrontPkVisionResult();
}
bool CSharedMemory::GetRearPickerVisionResult()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadRearPickerVisionResult()) {
		pin_ptr<int> p1 = &RRearPkVisionResult->Result[0];
		int* np = p1;
		memcpy(np, memcomm->RRearPkVisionResult.Result, sizeof(int) * 10);
		return true;
	}
	return false;
}
bool CSharedMemory::SetRearPickerVisionResult()
{
	if (memcomm == nullptr) return false;

	pin_ptr<int> p1 = &WRearPkVisionResult->Result[0];
	int* np = p1;
	memcpy(memcomm->WRearPkVisionResult.Result, np, sizeof(int) * 10);
	return memcomm->WriteRearPickerVisionResult();
}
bool CSharedMemory::GetGoodTray1VisionResult()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadGoodTray1VisionResult()) {
		pin_ptr<int> p1 = &RGoodTray1VisionResult->Result[0, 0];
		int* np = p1;
		memcpy(np, memcomm->RGoodTray1VisionResult.Result, sizeof(int) * 50 * 50);
		return true;
	}
	return false;
}
bool CSharedMemory::GetGoodTray2VisionResult()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadGoodTray2VisionResult()) {
		pin_ptr<int> p1 = &RGoodTray2VisionResult->Result[0, 0];
		int* np = p1;
		memcpy(np, memcomm->RGoodTray2VisionResult.Result, sizeof(int) * 50 * 50);
		return true;
	}
	return false;
}
bool CSharedMemory::GetReworkTrayVisionResult()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadReworkTrayVisionResult()) {
		pin_ptr<int> p1 = &RRewTrayVisionResult->Result[0, 0];
		int* np = p1;
		memcpy(np, memcomm->RReworkTrayVisionResult.Result, sizeof(int) * 50 * 50);
		return true;
	}
	return false;
}
bool CSharedMemory::GetNGTrayVisionResult()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadNGTrayVisionResult()) {
		pin_ptr<int> p1 = &RNGTrayVisionResult->Result[0, 0];
		int* np = p1;
		memcpy(np, memcomm->RNGTrayVisionResult.Result, sizeof(int) * 50 * 50);
		return true;
	}
	return false;
}

bool CSharedMemory::GetFlip1Map()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadFlip1Map()) {
		pin_ptr<int> p1 = &RFlip1Map->Map[0,0];
		int* np = p1;
		memcpy(np, memcomm->RFlip1Map.Map, sizeof(int) * 50 * 50);
		return true;
	}
	return false;
}
bool CSharedMemory::SetFlip1Map()
{
	if (memcomm == nullptr) return false;

	pin_ptr<int> p1 = &WFlip1Map->Map[0, 0];
	int* np = p1;
	memcpy(memcomm->WFlip1Map.Map, np, sizeof(int) * 50 * 50);
	return memcomm->WriteFlip1Map();
}

bool CSharedMemory::GetFlip2Map()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadFlip2Map()) {
		pin_ptr<int> p1 = &RFlip2Map->Map[0, 0];
		int* np = p1;
		memcpy(np, memcomm->RFlip2Map.Map, sizeof(int) * 50 * 50);
		return true;
	}
	return false;
}
bool CSharedMemory::SetFlip2Map()
{
	if (memcomm == nullptr) return false;

	pin_ptr<int> p1 = &WFlip2Map->Map[0, 0];
	int* np = p1;
	memcpy(memcomm->WFlip2Map.Map, np, sizeof(int) * 50 * 50);
	return memcomm->WriteFlip2Map();
}

bool CSharedMemory::GetPallet1Map()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadPallet1Map()) {
		pin_ptr<int> p1 = &RPallet1Map->Map[0, 0];
		int* np = p1;
		memcpy(np, memcomm->RPallet1Map.Map, sizeof(int) * 50 * 50);
		return true;
	}
	return false;
}
bool CSharedMemory::SetPallet1Map()
{
	if (memcomm == nullptr) return false;

	pin_ptr<int> p1 = &WPallet1Map->Map[0, 0];
	int* np = p1;
	memcpy(memcomm->WPallet1Map.Map, np, sizeof(int) * 50 * 50);
	return memcomm->WritePallet1Map();
}

bool CSharedMemory::GetPallet2Map()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadPallet2Map()) {
		pin_ptr<int> p1 = &RPallet2Map->Map[0, 0];
		int* np = p1;
		memcpy(np, memcomm->RPallet2Map.Map, sizeof(int) * 50 * 50);
		return true;
	}
	return false;
}
bool CSharedMemory::SetPallet2Map()
{
	if (memcomm == nullptr) return false;

	pin_ptr<int> p1 = &WPallet2Map->Map[0, 0];
	int* np = p1;
	memcpy(memcomm->WPallet2Map.Map, np, sizeof(int) * 50 * 50);
	return memcomm->WritePallet2Map();
}

bool CSharedMemory::GetGoodTray1Map()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadGoodTray1Map()) {
		pin_ptr<int> p1 = &RGoodTray1Map->Map[0, 0];
		int* np = p1;
		memcpy(np, memcomm->RGoodTray1Map.Map, sizeof(int) * 50 * 50);
		return true;
	}
	return false;
}

bool CSharedMemory::SetGoodTray1Map()
{
	if (memcomm == nullptr) return false;

	pin_ptr<int> p1 = &WGoodTray1Map->Map[0, 0];
	int* np = p1;
	memcpy(memcomm->WGoodTray1Map.Map, np, sizeof(int) * 50 * 50);
	return memcomm->WriteGoodTray1Map();
}

bool CSharedMemory::GetGoodTray2Map()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadGoodTray2Map()) {
		pin_ptr<int> p1 = &RGoodTray2Map->Map[0, 0];
		int* np = p1;
		memcpy(np, memcomm->RGoodTray2Map.Map, sizeof(int) * 50 * 50);
		return true;
	}
	return false;
}

bool CSharedMemory::SetGoodTray2Map()
{
	if (memcomm == nullptr) return false;

	pin_ptr<int> p1 = &WGoodTray2Map->Map[0, 0];
	int* np = p1;
	memcpy(memcomm->WGoodTray2Map.Map, np, sizeof(int) * 50 * 50);
	return memcomm->WriteGoodTray2Map();
}

bool CSharedMemory::GetReworkTrayMap()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadReworkTrayMap()) {
		pin_ptr<int> p1 = &RRewTrayMap->Map[0, 0];
		int* np = p1;
		memcpy(np, memcomm->RReworkTrayMap.Map, sizeof(int) * 50 * 50);
		return true;
	}
	return false;
}
bool CSharedMemory::SetReworkTrayMap()
{
	if (memcomm == nullptr) return false;

	pin_ptr<int> p1 = &WRewTrayMap->Map[0, 0];
	int* np = p1;
	memcpy(memcomm->WReworkTrayMap.Map, np, sizeof(int) * 50 * 50);
	return memcomm->WriteRewTrayMap();
}

bool CSharedMemory::GetNGTrayMap()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadNGTrayMap()) {
		pin_ptr<int> p1 = &RNGTrayMap->Map[0, 0];
		int* np = p1;
		memcpy(np, memcomm->RNGTrayMap.Map, sizeof(int) * 50 * 50);
		return true;
	}
	return false;
}
bool CSharedMemory::SetNGTrayMap()
{
	if (memcomm == nullptr) return false;

	pin_ptr<int> p1 = &WNGTrayMap->Map[0, 0];
	int* np = p1;
	memcpy(memcomm->WNGTrayMap.Map, np, sizeof(int) * 50 * 50);
	return memcomm->WriteNGTrayMap();
}

//Picker Calibration
bool CSharedMemory::SetPkCenterMove()
{
	if (memcomm == nullptr) return false;
     memcomm->W_PKCenMove.PkType = W_PkCenMove->PkType;
	 memcomm->W_PKCenMove.PkNo   = W_PkCenMove->PkNo;

	 pin_ptr<double> p1 = &W_PkCenMove->X_Pos[0];
	 double* np1 = p1;
	 memcpy(memcomm->W_PKCenMove.posx, np1, sizeof(double) * 10);

	 pin_ptr<double> p2 = &W_PkCenMove->Y_Pos[0];
	 double* np2 = p2;
	 memcpy(memcomm->W_PKCenMove.posy, np2, sizeof(double) * 10);

	 return  memcomm->WritePKCenMove();
}

bool CSharedMemory::SetPkCenterTrig()
{
	if (memcomm == nullptr) return false;
	return  memcomm->WriteCenTrig();
}

bool CSharedMemory::GetFrontPkCenOffset()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadFPKOffset()) {
		pin_ptr<double> p1 = &R_FPk_CenOffset->X_Offset[0];
		double* np1 = p1;
		memcpy(np1, memcomm->R_FPKCenOffset.XOffset, sizeof(double) * 10);

		pin_ptr<double> p2 = &R_FPk_CenOffset->Y_Offset[0];
		double* np2 = p2;
		memcpy(np2, memcomm->R_FPKCenOffset.YOffset, sizeof(double) * 10);

		return true;
	}

	return false;
}

bool CSharedMemory::GetRearPkCenOffset()
{
	if (memcomm == nullptr) return false;

	if (memcomm->ReadRPKOffset()) {
		pin_ptr<double> p1 = &R_RPk_CenOffset->X_Offset[0];
		double* np1 = p1;
		memcpy(np1, memcomm->R_RPKCenOffset.XOffset, sizeof(double) * 10);

		pin_ptr<double> p2 = &R_RPk_CenOffset->Y_Offset[0];
		double* np2 = p2;
		memcpy(np2, memcomm->R_RPKCenOffset.YOffset, sizeof(double) * 10);

		return true;
	}

	return false;
}

bool CSharedMemory::SetPkAutoCal()
{
	if (memcomm == nullptr) return false;
	memcomm->W_PKCenMove.PkType = W_PkCenMove->PkType;
	memcomm->W_PKCenMove.PkNo = -1;  

	pin_ptr<double> p1 = &W_PkCenMove->X_Pos[0];
	double* np1 = p1;
	memcpy(memcomm->W_PKCenMove.posx, np1, sizeof(double) * 10);

	pin_ptr<double> p2 = &W_PkCenMove->Y_Pos[0];
	double* np2 = p2;
	memcpy(memcomm->W_PKCenMove.posy, np2, sizeof(double) * 10);

	return  memcomm->WritePKAutoCal();
}

//---------------------------------------------------------------//
// Line scan trigger
bool CSharedMemory::SetScanTriggerRecipe()
{
	if (memcomm == nullptr) return false;

	memcomm->WScanTriggerRecipe.uAxisNo    = WScanTriggerRecipe->uAxisNo;
	memcomm->WScanTriggerRecipe.dTrigStart = WScanTriggerRecipe->dTrigStart;
	memcomm->WScanTriggerRecipe.dTrigEnd   = WScanTriggerRecipe->dTrigEnd;
	memcomm->WScanTriggerRecipe.dPitch     = WScanTriggerRecipe->dPitch;
	memcomm->WScanTriggerRecipe.dLineRate  = WScanTriggerRecipe->dLineRate;
	memcomm->WScanTriggerRecipe.dAccel     = WScanTriggerRecipe->dAccel;
	memcomm->WScanTriggerRecipe.dDecel     = WScanTriggerRecipe->dDecel;
	memcomm->WScanTriggerRecipe.nDirection = WScanTriggerRecipe->nDirection;
	memset(memcomm->WScanTriggerRecipe.uReserved, 0,
		   sizeof(memcomm->WScanTriggerRecipe.uReserved));

	return memcomm->WriteScanTriggerRecipe();
}

bool CSharedMemory::GetScanTriggerDisplay()
{
	if (memcomm == nullptr) return false;

	bool bRet = memcomm->ReadScanTriggerDisplay();

	RScanTriggerDisplay->dSpeed          = memcomm->RScanTriggerDisplay.dSpeed;
	RScanTriggerDisplay->dLineRate       = memcomm->RScanTriggerDisplay.dLineRate;
	RScanTriggerDisplay->nLineCount      = memcomm->RScanTriggerDisplay.nLineCount;
	RScanTriggerDisplay->dScanTime       = memcomm->RScanTriggerDisplay.dScanTime;
	RScanTriggerDisplay->dMotionStart    = memcomm->RScanTriggerDisplay.dMotionStart;
	RScanTriggerDisplay->dMotionEnd      = memcomm->RScanTriggerDisplay.dMotionEnd;
	RScanTriggerDisplay->dPitchCounts    = memcomm->RScanTriggerDisplay.dPitchCounts;
	RScanTriggerDisplay->bPitchIsInteger = memcomm->RScanTriggerDisplay.bPitchIsInteger;
	RScanTriggerDisplay->nValidateCode   = memcomm->RScanTriggerDisplay.nValidateCode;
	RScanTriggerDisplay->nState          = memcomm->RScanTriggerDisplay.nState;
	RScanTriggerDisplay->nTriggerCount   = memcomm->RScanTriggerDisplay.nTriggerCount;

	return bRet;
}

bool CSharedMemory::SetScanTriggerStart()
{
	if (memcomm == nullptr) return false;
	return memcomm->WriteScanTriggerStart();
}

bool CSharedMemory::SetScanTriggerStop()
{
	if (memcomm == nullptr) return false;
	return memcomm->WriteScanTriggerStop();
}

bool CSharedMemory::SetScanTriggerTest()
{
	if (memcomm == nullptr) return false;
	return memcomm->WriteScanTriggerTest();
}
