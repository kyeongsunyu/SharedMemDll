#pragma once
#include <stdio.h>
#include <vcclr.h>
#include <Windows.h>
#include "MemComm.h"
#include <msclr/marshal.h>
#include <msclr/marshal_cppstd.h>
#include <msclr/marshal_windows.h>
#include <msclr/marshal_atl.h>

using namespace msclr::interop;

using namespace System;

namespace SharedMemDll 
{
	//public ref struct MotorStatus {
	//	int		MotorNum;
	//	bool	Limit;
	//	bool	Org;
	//	bool	Home;
	//	bool	ServoOn;
	//	bool	Driving;
	//	bool	Alarm;
	//	int		CurrentIndex;
	//	int		NextIndex;
	//	double	CurrentPosition;
	//	int		NextPosition;
	//	unsigned char status;
	//	unsigned short int uHomeStep;
	//	int dwServoAlarmCode;
	//	//char strServoAlarmName[1025];
	//	array<char>^ strServoAlarmName;
	//	double dServoLoadRatio;
	//};
	
	public ref struct MotorStatus {
		int		MotorNum;
		bool	Org;
		bool	CW;
		bool	CCW;
		bool	HOME;
		bool	Busy;
		int		CurrentIndex;
		int		NextIndex;
		double	CurrentPosition;
		double	NextPosition;
		bool	ALM;
		bool	SVON;

		unsigned short int uHomeStep;
		int dwServoAlarmCode;
		String^ strServoAlarmName;
		double dServoLoadRatio;
	};

	public ref struct MotorConfig
	{
		unsigned int uAxisNo;
		unsigned int uRate;
		unsigned int uAccel;
		unsigned int uMaxVel;
		unsigned int uJogVel;
		unsigned int uHomeVel;
		unsigned int uMotorType;
		unsigned int uCardType;
		unsigned int uCardIndex;
		unsigned int uUseSkip;
		unsigned int uHomeLevel;
		unsigned int uLimitLevel;
		unsigned int uServoOnLevel;
		unsigned int uAlarmLevel;
		unsigned int uInpUse;
		unsigned int uHomeMethod;
		unsigned int uMtrDir;
		unsigned int uEncDir;
		unsigned int uMaxRec;
		unsigned int uEncoderType;
	};

	public ref struct MotorData
	{
		int	   uAxisNo;
		array<double>^ fPos = gcnew array<double>(100);
		array<double>^ fVel = gcnew array<double>(100);
		array<double>^ uPos = gcnew array<double>(100);
		array<double>^ uVel = gcnew array<double>(100);
		int	   HomeSpeed;
		int	   ZPhaseSpeed;
	};

	public ref struct RecipeData
	{ 
		double Unit_X_Size;
		double Unit_Y_Size;
		double Unit_X_Count;
		double Unit_Y_Count;
		double Unit_X_Pitch;
		double Unit_Y_Pitch;
		double Tray_X_Count;
		double Tray_Y_Count;
		double Vision_Snap_X_Count;
		double Vision_Snap_Y_Count;
		double Picker_Angle;
		double Picker_Place_Vac_Off_Offset;

		bool   Front_Picker_1_Skip;
		bool   Front_Picker_2_Skip;
		bool   Front_Picker_3_Skip;
		bool   Front_Picker_4_Skip;
		bool   Front_Picker_5_Skip;
		bool   Front_Picker_6_Skip;
		bool   Front_Picker_7_Skip;
		bool   Front_Picker_8_Skip;
		bool   Rear_Picker_1_Skip;
		bool   Rear_Picker_2_Skip;
		bool   Rear_Picker_3_Skip;
		bool   Rear_Picker_4_Skip;
		bool   Rear_Picker_5_Skip;
		bool   Rear_Picker_6_Skip;
		bool   Rear_Picker_7_Skip;
		bool   Rear_Picker_8_Skip;
		bool   Scrap_Skip;
		bool   Sponge_Clean_Skip;

		int Saw_Picker_Sponge_Clean;
		int Water_Jet_Water_Clean;
		int Water_Jet_Air_Clean;
		int Saw_Picker_Flip_Y1_Air_Clean;
		int Saw_Picker_Flip_Y2_Air_Clean;
		int Flip_Y1_Air_Clean;
		int Flip_Y2_Air_Clean;
		int Pallet_Y1_Air_Clean;
		int Pallet_Y2_Air_Clean;
		int Vision_Snap_X;
		int Vision_Snap_Y;

		double Front_Picker_Air_Blow_Time;
		double Rear_Picker_Air_Blow_Time;
		double Front_Picker_Vac_On_Time;
		double Rear_Picker_Vac_On_Time;

		double Load_Picker_Air_Blow_Time;
		double Load_Picker_Vac_On_Time;
		double Saw_Picker_Air_Blow_Time;
		double Saw_Picker_Vac_On_Time;

		double Pallet1_Receive_Vac_Value;
		double Pallet2_Receive_Vac_Value;
		double Pallet_First_Sort_Vac_Value;
		double Pallet_Middle_Sort_Vac_Value;
		double Pallet_Last_Sort_Vac_Value;
		double Pallet_First_Sort_PKG_Rate;
		double Pallet_Last_Sort_PKG_Rate;

		/* Original Code
		array<double>^ dData = gcnew array<double>(200);
		*/

		/*
		array<bool>^   bData = gcnew array<bool>(100);
		array<int>^    iData = gcnew array<int>(100);
		array<double>^ dData = gcnew array<double>(100);
		array<double>^ dDeviceData = gcnew array<double>(200);
		*/
	};

	public ref struct USER_INFO
	{
		String^ strUserName;
	};

	public ref struct LOT_INFO
	{
		String^ strLotID;
		int nLotCount;
	};

	public ref struct DEVICE_INFO
	{
		int nDeviceNumber;
		String^ strDeviceName;
	};

	public ref struct LAMP_BUZZER
	{
		array<unsigned int, 2>^ uLampBuzzer = gcnew array<unsigned int, 2>(1000, 10);
	};

	public ref struct SYSTEM_DATA
	{
		array<double>^ dData = gcnew array<double>(100);
	};
	
	public ref struct MachineStatus 
	{
		int nDeviceNo;
		String^ strDeviceName;

		int UPH;
		int TPH;

		int PanelInCount;
		int UnitInCnt;
		int UnitOutCnt;
		int UnitGoodCnt;
		int UnitNGCnt;
		int UnitReworkCnt;
		int FlipY1AirCleanCnt;
		int FlipY2AirCleanCnt;
		int PalletY1AirCleanCnt;
		int PalletY2AirCleanCnt;
		int SawPkFlipY1AirClenaCnt;
		int SawPkFlipY2AirClenaCnt;
		int SpongeCleanCnt;
		int WaterJetWaterCleanCnt;
		int WaterJetAirCleanCnt;
		double AirPressure1;
		double AirPressure2;
		double AirPressure3;

		array<int>^ HomeState = gcnew array<int>(60);

		//array<array<bool>^>^ bInputState = gcnew array<array< bool >^>(20);
		//array<array<bool>^>^ bOutputState = gcnew array<array< bool >^>(20);
		
		array<int, 2>^ bInputState = gcnew array<int, 2>(20, 16);
		array<int, 2>^ bOutputState = gcnew array<int, 2>(20, 16);

		bool bVisionConnected;
	};

	public ref struct TENKEY_JOG
	{
		int uAxisNo;
		bool bTenKeyJog;
	};

	// Line scan trigger. Mirrors _scantriggerrecipe / _scantriggerdisplay in
	// SharedMemBase.h; keep the field order in step with those.
	public ref struct SCANTRIGGER_RECIPE
	{
		unsigned int uAxisNo;
		double dTrigStart;      // mm, absolute
		double dTrigEnd;        // mm, absolute
		double dPitch;          // mm
		double dLineRate;       // Hz

		// Reserved for a later approach profile.
		double dAccel;
		double dDecel;
		int    nDirection;
	};

	// All computed by SEQ. nValidateCode is 0 when the recipe can be run; any
	// other value is a refusal reason from the SCANTRIGGER_VALIDATE list.
	public ref struct SCANTRIGGER_DISPLAY
	{
		double dSpeed;          // mm/s
		double dLineRate;       // Hz
		int    nLineCount;
		double dScanTime;       // s
		double dMotionStart;    // mm
		double dMotionEnd;      // mm

		double dPitchCounts;
		bool   bPitchIsInteger;
		int    nValidateCode;
		int    nState;
		int    nTriggerCount;   // -1 when the counter cannot be read back
	};

	public ref struct SET_3POINT
	{
		int pktype;
		int target;
		int pointno;

		double offsetX;
		double offsetY;
		double offsetT;
	};

	public ref struct FLIP1_VISION_RESULT
	{
		//array<array<int>^>^ Result = gcnew array<array< int >^>(50);	// 가변길이
		array<int, 2>^ Result = gcnew array<int, 2>(50, 50);
	};
	public ref struct FLIP2_VISION_RESULT
	{
		//array<array<int>^>^ Result = gcnew array<array< int >^>(50);	// 가변길이
		array<int, 2>^ Result = gcnew array<int, 2>(50, 50);
	};
	public ref struct PALLET1_VISION_RESULT
	{
		//array<array<int>^>^ Result = gcnew array<array< int >^>(50);	// 가변길이
		array<int, 2>^ Result = gcnew array<int, 2>(50, 50);
	};
	public ref struct PALLET2_VISION_RESULT
	{
		//array<array<int>^>^ Result = gcnew array<array< int >^>(50);	// 가변길이
		array<int, 2>^ Result = gcnew array<int, 2>(50, 50);
	};

	public ref struct FRONT_PK_VISION_RESULT
	{
		array<int>^ Result = gcnew array<int>(10);
	};
	public ref struct REAR_PK_VISION_RESULT
	{
		array<int>^ Result = gcnew array<int>(10);
	};

	public ref struct GOOD_TRAY1_VISION_RESULT
	{
		//array<array<int>^>^ Result = gcnew array<array< int >^>(50);	// 가변길이
		array<int, 2>^ Result = gcnew array<int, 2>(50, 50);
	};
	public ref struct GOOD_TRAY2_VISION_RESULT
	{
		//array<array<int>^>^ Result = gcnew array<array< int >^>(50);	// 가변길이
		array<int, 2>^ Result = gcnew array<int, 2>(50, 50);
	};
	public ref struct NG_TRAY_VISION_RESULT
	{
		//array<array<int>^>^ Result = gcnew array<array< int >^>(50);	// 가변길이
		array<int, 2>^ Result = gcnew array<int, 2>(50, 50);
	};
	public ref struct REW_TRAY_VISION_RESULT
	{
		//array<array<int>^>^ Result = gcnew array<array< int >^>(50);	// 가변길이
		array<int, 2>^ Result = gcnew array<int, 2>(50, 50);
	};

	public ref struct FLIP1_MAP
	{
		//array<array<int>^>^ Map = gcnew array<array< int >^>(50);	// 가변길이
		array<int, 2>^ Map = gcnew array<int, 2>(50, 50);
	};
	public ref struct FLIP2_MAP
	{
		//array<array<int>^>^ Map = gcnew array<array< int >^>(50);	// 가변길이
		array<int, 2>^ Map = gcnew array<int, 2>(50, 50);
	};
	public ref struct PALLET1_MAP
	{
		//array<array<int>^>^ Map = gcnew array<array< int >^>(50);	// 가변길이
		array<int, 2>^ Map = gcnew array<int, 2>(50, 50);
	};
	public ref struct PALLET2_MAP
	{
		//array<array<int>^>^ Map = gcnew array<array< int >^>(50);	// 가변길이
		array<int, 2>^ Map = gcnew array<int, 2>(50, 50);
	};
	public ref struct GOOD_TRAY1_MAP
	{
		//array<array<int>^>^ Map = gcnew array<array< int >^>(50);	// 가변길이
		array<int, 2>^ Map = gcnew array<int, 2>(50, 50);
	};
	public ref struct GOOD_TRAY2_MAP
	{
		//array<array<int>^>^ Map = gcnew array<array< int >^>(50);	// 가변길이
		array<int, 2>^ Map = gcnew array<int, 2>(50, 50);
	};
	public ref struct REWORK_TRAY_MAP
	{
		//array<array<int>^>^ Map = gcnew array<array< int >^>(50);	// 가변길이
		array<int, 2>^ Map = gcnew array<int, 2>(50, 50);
	};
	public ref struct NG_TRAY_MAP
	{
		//array<array<int>^>^ Map = gcnew array<array< int >^>(50);	// 가변길이
		array<int, 2>^ Map = gcnew array<int, 2>(50, 50);
	};

	//Picker Calibration 
	public ref struct PK_CENTER_MOVE
	{
		int PkType;
		int PkNo;
		array<double>^ X_Pos = gcnew array<double>(10);
		array<double>^ Y_Pos = gcnew array<double>(10);
	};

	public ref struct PK_CENTER_OFFSET
	{   
		array<double>^ X_Offset = gcnew array<double>(10);
		array<double>^ Y_Offset = gcnew array<double>(10);
	};


	public ref class CSharedMemory
	{
	private:
		static CMemComm* memcomm;
	public:
		MotorStatus^ RMTStatus;

		MachineStatus^ RMachineStatus;

		MotorConfig^ RMTConfig;
		MotorConfig^ WMTConfig;

		array<MotorData ^>^ RMotorData;
		MotorData^ WMotorData;

		RecipeData^ RRecipeData;
		RecipeData^ WRecipeData;

		USER_INFO^ RUserInfo;
		USER_INFO^ WUserInfo;

		LOT_INFO^ WLotInfo;
		LOT_INFO^ RLotInfo;

		DEVICE_INFO^ RDeviceInfo;
		DEVICE_INFO^ WDeviceInfo;

		LAMP_BUZZER^ WLampBuzzer;

		SYSTEM_DATA^ WSystemData;
		
		TENKEY_JOG^ WTenkeyJog;

		SET_3POINT^ WSet3Point;
		SET_3POINT^ RSet3Point;

		FLIP1_VISION_RESULT^ RFlip1VisionResult;
		FLIP1_VISION_RESULT^ WFlip1VisionResult;

		FLIP2_VISION_RESULT^ RFlip2VisionResult;
		FLIP2_VISION_RESULT^ WFlip2VisionResult;

		PALLET1_VISION_RESULT^ RPallet1VisionResult;
		PALLET1_VISION_RESULT^ WPallet1VisionResult;

		PALLET2_VISION_RESULT^ RPallet2VisionResult;
		PALLET2_VISION_RESULT^ WPallet2VisionResult;

		FRONT_PK_VISION_RESULT^ RFrontPkVisionResult;
		FRONT_PK_VISION_RESULT^ WFrontPkVisionResult;

		REAR_PK_VISION_RESULT^ RRearPkVisionResult;
		REAR_PK_VISION_RESULT^ WRearPkVisionResult;

		GOOD_TRAY1_VISION_RESULT^ RGoodTray1VisionResult;
		GOOD_TRAY1_VISION_RESULT^ WGoodTray1VisionResult;

		GOOD_TRAY2_VISION_RESULT^ RGoodTray2VisionResult;
		GOOD_TRAY2_VISION_RESULT^ WGoodTray2VisionResult;

		NG_TRAY_VISION_RESULT^ RNGTrayVisionResult;
		NG_TRAY_VISION_RESULT^ WNGTrayVisionResult;

		REW_TRAY_VISION_RESULT^ RRewTrayVisionResult;
		REW_TRAY_VISION_RESULT^ WRewTrayVisionResult;

		FLIP1_MAP^			RFlip1Map;
		FLIP1_MAP^			WFlip1Map;

		FLIP2_MAP^			RFlip2Map;
		FLIP2_MAP^			WFlip2Map;

		PALLET1_MAP^		RPallet1Map;
		PALLET1_MAP^		WPallet1Map;

		PALLET2_MAP^		RPallet2Map;
		PALLET2_MAP^		WPallet2Map;

		GOOD_TRAY1_MAP^		RGoodTray1Map;
		GOOD_TRAY1_MAP^		WGoodTray1Map;
		
		GOOD_TRAY2_MAP^		RGoodTray2Map;
		GOOD_TRAY2_MAP^		WGoodTray2Map;
		
		NG_TRAY_MAP^		RNGTrayMap;
		NG_TRAY_MAP^		WNGTrayMap;
		
		REWORK_TRAY_MAP^	RRewTrayMap;
		REWORK_TRAY_MAP^	WRewTrayMap;

		//Picker Calibation
		PK_CENTER_MOVE^		R_PkCenMove;
		PK_CENTER_MOVE^     W_PkCenMove;

		PK_CENTER_OFFSET^   R_FPk_CenOffset;
		PK_CENTER_OFFSET^   W_FPk_CenOffset;
		PK_CENTER_OFFSET^	R_RPk_CenOffset;
		PK_CENTER_OFFSET^	W_RPk_CenOffset;

		SCANTRIGGER_RECIPE^  WScanTriggerRecipe;
		SCANTRIGGER_DISPLAY^ RScanTriggerDisplay;


	public:
		CSharedMemory()
		{
			memcomm = new CMemComm();

			RMTStatus = gcnew MotorStatus();

			RMotorData = gcnew array<MotorData^>(60);
			for (int index = 0; index < RMotorData->Length; index += 1)
			{
				RMotorData[index] = gcnew MotorData;
			}
			for (int index = 0; index < RMotorData->Length; index += 1)
			{
				RMotorData[index]->fPos = gcnew array<double>(100);
				RMotorData[index]->fVel = gcnew array<double>(100);
				RMotorData[index]->uPos = gcnew array<double>(100);
				RMotorData[index]->uVel = gcnew array<double>(100);
			}
			WMotorData = gcnew MotorData();
			WMotorData->fPos = gcnew array<double>(100);
			WMotorData->fVel = gcnew array<double>(100);
			WMotorData->uPos = gcnew array<double>(100);
			WMotorData->uVel = gcnew array<double>(100);

			RMTConfig = gcnew MotorConfig();
			WMTConfig = gcnew MotorConfig();

			RRecipeData = gcnew RecipeData();
			WRecipeData = gcnew RecipeData();

			WSystemData = gcnew SYSTEM_DATA();

			RUserInfo = gcnew USER_INFO();
			WUserInfo = gcnew USER_INFO();

			WLotInfo = gcnew LOT_INFO();
			RLotInfo = gcnew LOT_INFO();

			WDeviceInfo = gcnew DEVICE_INFO();
			RDeviceInfo = gcnew DEVICE_INFO();

			WLampBuzzer = gcnew LAMP_BUZZER();

			RMachineStatus = gcnew MachineStatus();
			//for (int i = 0; i < 20; i++) {
			//	RMachineStatus->bInputState[i] = gcnew array< bool >(16);
			//	RMachineStatus->bOutputState[i] = gcnew array< bool >(16);
			//}

			WSet3Point = gcnew SET_3POINT();
			RSet3Point = gcnew SET_3POINT();

			WTenkeyJog = gcnew TENKEY_JOG();

			WScanTriggerRecipe  = gcnew SCANTRIGGER_RECIPE();
			RScanTriggerDisplay = gcnew SCANTRIGGER_DISPLAY();

			RFlip1VisionResult = gcnew FLIP1_VISION_RESULT();
			WFlip1VisionResult = gcnew FLIP1_VISION_RESULT();
			
			RFlip2VisionResult = gcnew FLIP2_VISION_RESULT();
			WFlip2VisionResult = gcnew FLIP2_VISION_RESULT();

			RPallet1VisionResult = gcnew PALLET1_VISION_RESULT();
			WPallet1VisionResult = gcnew PALLET1_VISION_RESULT();

			RPallet2VisionResult = gcnew PALLET2_VISION_RESULT();
			WPallet2VisionResult = gcnew PALLET2_VISION_RESULT();

			RFrontPkVisionResult = gcnew FRONT_PK_VISION_RESULT();
			WFrontPkVisionResult = gcnew FRONT_PK_VISION_RESULT();

			RRearPkVisionResult = gcnew REAR_PK_VISION_RESULT();
			WRearPkVisionResult = gcnew REAR_PK_VISION_RESULT();

			RGoodTray1VisionResult = gcnew GOOD_TRAY1_VISION_RESULT();
			WGoodTray1VisionResult = gcnew GOOD_TRAY1_VISION_RESULT();

			RGoodTray2VisionResult = gcnew GOOD_TRAY2_VISION_RESULT();
			WGoodTray2VisionResult = gcnew GOOD_TRAY2_VISION_RESULT();

			RNGTrayVisionResult = gcnew NG_TRAY_VISION_RESULT();
			WNGTrayVisionResult = gcnew NG_TRAY_VISION_RESULT();

			RRewTrayVisionResult = gcnew REW_TRAY_VISION_RESULT();
			WRewTrayVisionResult = gcnew REW_TRAY_VISION_RESULT();
						

			RFlip1Map = gcnew FLIP1_MAP();
			WFlip1Map = gcnew FLIP1_MAP();

			RFlip2Map = gcnew FLIP2_MAP();
			WFlip2Map = gcnew FLIP2_MAP();

			RPallet1Map = gcnew PALLET1_MAP();
			WPallet1Map = gcnew PALLET1_MAP();

			RPallet2Map = gcnew PALLET2_MAP();
			WPallet2Map = gcnew PALLET2_MAP();

			RGoodTray1Map = gcnew GOOD_TRAY1_MAP();
			WGoodTray1Map = gcnew GOOD_TRAY1_MAP();

			RGoodTray2Map = gcnew GOOD_TRAY2_MAP();
			WGoodTray2Map = gcnew GOOD_TRAY2_MAP();

			RNGTrayMap = gcnew NG_TRAY_MAP();
			WNGTrayMap = gcnew NG_TRAY_MAP();

			RRewTrayMap = gcnew REWORK_TRAY_MAP();
			WRewTrayMap = gcnew REWORK_TRAY_MAP();

			R_PkCenMove = gcnew PK_CENTER_MOVE();
			W_PkCenMove = gcnew PK_CENTER_MOVE();

			R_FPk_CenOffset = gcnew PK_CENTER_OFFSET();
			W_FPk_CenOffset = gcnew PK_CENTER_OFFSET();
			R_RPk_CenOffset = gcnew PK_CENTER_OFFSET();
			W_RPk_CenOffset = gcnew PK_CENTER_OFFSET();



			//for (int i = 0; i < 50; i++) {
				//RFlip1VisionResult->Result[i] = gcnew array< int >(50);
				//WFlip1VisionResult->Result[i] = gcnew array< int >(50);

				//RFlip2VisionResult->Result[i] = gcnew array< int >(50);
				//WFlip2VisionResult->Result[i] = gcnew array< int >(50);

				//RPallet1VisionResult->Result[i] = gcnew array< int >(50);
				//WPallet1VisionResult->Result[i] = gcnew array< int >(50);

				//RPallet2VisionResult->Result[i] = gcnew array< int >(50);
				//WPallet2VisionResult->Result[i] = gcnew array< int >(50);

				//RGoodTray1VisionResult->Result[i] = gcnew array< int >(50);
				//WGoodTray1VisionResult->Result[i] = gcnew array< int >(50);

				//RGoodTray2VisionResult->Result[i] = gcnew array< int >(50);
				//WGoodTray2VisionResult->Result[i] = gcnew array< int >(50);

				//RNGTrayVisionResult->Result[i] = gcnew array< int >(50);
				//WNGTrayVisionResult->Result[i] = gcnew array< int >(50);

				//RRewTrayVisionResult->Result[i] = gcnew array< int >(50);
				//WRewTrayVisionResult->Result[i] = gcnew array< int >(50);

				//RFlip1Map->Map[i] = gcnew array< int >(50);
				//WFlip1Map->Map[i] = gcnew array< int >(50);

				//RFlip2Map->Map[i] = gcnew array< int >(50);
				//WFlip2Map->Map[i] = gcnew array< int >(50);

				//RPallet1Map->Map[i] = gcnew array< int >(50);
				//WPallet1Map->Map[i] = gcnew array< int >(50);

				//RPallet2Map->Map[i] = gcnew array< int >(50);
				//WPallet2Map->Map[i] = gcnew array< int >(50);

				//RGoodTray1Map->Map[i] = gcnew array< int >(50);
				//WGoodTray1Map->Map[i] = gcnew array< int >(50);

				//RGoodTray2Map->Map[i] = gcnew array< int >(50);
				//WGoodTray2Map->Map[i] = gcnew array< int >(50);

				//RNGTrayMap->Map[i] = gcnew array< int >(50);
				//WNGTrayMap->Map[i] = gcnew array< int >(50);

				//RRewTrayMap->Map[i] = gcnew array< int >(50);
				//WRewTrayMap->Map[i] = gcnew array< int >(50);
			//}		
		}

		
		~CSharedMemory()
		{
		}

		!CSharedMemory()
		{
		}

		//--
		bool IsGetDM();
		unsigned int GetDM(int iStart);
		bool GetDM(int iStart, unsigned int% pData);
		bool SetDM(int iStart, unsigned int Data);
		
		bool GetDM(int iStart, int iCount, array<unsigned long>^ pData);
		bool SetDM(int iStart, int iCount, array<unsigned long>^ pData);

		bool IsGetBit();
		unsigned short int GetBit(int iStart);
		bool SetBit(int iStart, int Data);

		bool IsGetIO();
		unsigned short int GetIO(int iChannel);
		bool SetOutput(int iChannel, WORD Data);

		bool SetServoOnOff(int iAxisNo);
		bool SetServoHome(int iAxisNo);
		bool SetServoAlarmClear(int iAxisNo);

		bool SetMotorVelocityMove(int iAxis, int iDirection, unsigned int uSpeed);
		bool SetMotorRelativeMove(int iAxis, int iPos, unsigned int uSpeed);
		bool SetMotorAbsoluteMove(int iAxis, int iIndex);
		bool SetMotorStop(int iAxis);

		bool GetMotorStatus(int iAxiNo);

		bool GetMachineStatus();
		//bool GetMotorData(int iAxis,
		//	array<double>^ puPos, array<double>^ puVel,
		//	array<double>^ pdPos, array<double>^ pdVel);
		//bool SetMotorData(int iAxis,
		//	array<double>^ puPos, array<double>^ puVel,
		//	array<double>^ pdPos, array<double>^ pdVel);
		bool GetMotorData(int iAxisNo);
		bool SetMotorData();

		bool GetMotorConfig(int iAxis);
		bool SetMotorConfig();

		bool GetRecipe();
		bool SetRecipe();

		bool SetSystemData();

		bool GetUserInfo();
		bool SetUserInfo();

		bool GetLotInfo();
		bool SetLotInfo();

		bool GetDeviceInfo();
		bool SetDeviceInfo();

		bool SetLampBuzzer();
		
		bool SetLoadCountClear();
		bool SetUnLoadCountClear();

		bool SetEStop();

		bool SetTenKey(unsigned int iTenKey);

		bool SetTenKeyJog(int iAxisNo, bool bSet);

		bool SetExitProgram();
		
		bool SetBuzzerOff();

		bool Set3Point(int pktype, int target, int pointno);
		bool Get3PointOffset();

		bool GetFlip1VisionResult();
		bool SetFlip1VisionResult();
		
		bool GetFlip2VisionResult();
		bool SetFlip2VisionResult();

		bool GetPallet1VisionResult();
		bool SetPallet1VisionResult();

		bool GetPallet2VisionResult();
		bool SetPallet2VisionResult();

		bool GetFrontPickerVisionResult();
		bool SetFrontPickerVisionResult();

		bool GetRearPickerVisionResult();
		bool SetRearPickerVisionResult();

		bool GetGoodTray1VisionResult();

		bool GetGoodTray2VisionResult();
		
		bool GetReworkTrayVisionResult();

		bool GetNGTrayVisionResult();

		bool GetFlip1Map();
		bool SetFlip1Map();

		bool GetFlip2Map();
		bool SetFlip2Map();

		bool GetPallet1Map();
		bool SetPallet1Map();

		bool GetPallet2Map();
		bool SetPallet2Map();

		bool GetGoodTray1Map();
		bool SetGoodTray1Map();

		bool GetGoodTray2Map();
		bool SetGoodTray2Map();

		bool GetReworkTrayMap();
		bool SetReworkTrayMap();

		bool GetNGTrayMap();
		bool SetNGTrayMap();

		//Picker Calibration
		bool SetPkCenterMove();
		bool SetPkCenterTrig();
		bool GetFrontPkCenOffset();
		bool GetRearPkCenOffset();
		bool SetPkAutoCal();

		// Line scan trigger. SetScanTriggerRecipe() sends WScanTriggerRecipe and
		// GetScanTriggerDisplay() fills RScanTriggerDisplay; call the second after
		// the first to see how SEQ judged the values.
		bool SetScanTriggerRecipe();
		bool GetScanTriggerDisplay();
		bool SetScanTriggerStart();
		bool SetScanTriggerStop();
		// Commissioning: makes SEQ drive the trigger output pin directly so it
		// can be probed. Moves nothing and needs no recipe.
		bool SetScanTriggerTest();
	};
}
