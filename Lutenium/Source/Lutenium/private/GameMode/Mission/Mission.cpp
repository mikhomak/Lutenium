#include "GameMode/Mission/Mission.h"


AMission::AMission()
{
	bCheckMissionInfoClass = true;
}



void AMission::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	MissionTime = MissionTime + DeltaSeconds;
	if(bHasTimer && bHasStarted && !bHasFinished && MissionTime > MissionTimer)
	{
		TimerHasExpired();
	}
}


bool AMission::StartMission(UMissionInfo* Info)
{
	if(!CheckIfMissionInfoClassIsCorrect(Info))
	{
		return false;
	}
	bool Result = false;
	StartMissionEvent(Info, Result);
	if(Result)
	{
		bHasStarted = true;
	}
	return Result;
}

bool AMission::UpdateMission(UMissionInfo* Info)
{
	if(!CheckIfMissionInfoClassIsCorrect(Info))
	{
		return false;
	}
	bool Result = false;
	UpdateMissionEvent(Info, Result);
	return Result;
}

bool AMission::FinishMission(UMissionInfo* Info)
{
	if(!CheckIfMissionInfoClassIsCorrect(Info))
	{
		return false;
	}
	bool Result = false;
	FinishMissionEvent(Info, Result);
	if(Result)
	{
		bHasFinished = false;
	}
	return Result;
}

bool AMission::CheckIfMissionInfoClassIsCorrect(UMissionInfo* Info)
{
	return !bCheckMissionInfoClass || Info== nullptr || Info->GetClass()->IsChildOf(MissionInfoClass.Get());
}