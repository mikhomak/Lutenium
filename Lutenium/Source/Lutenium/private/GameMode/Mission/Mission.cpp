#include "GameMode/Mission/Mission.h"


AMission::AMission()
{

}



void AMission::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	MissionTime = MissionTime + DeltaSeconds;
	if(bHasTimer && MissionTime > MissionTimer)
	{
		TimerHasExpired();
	}
}


bool AMission::StartMission(UPrimaryDataAsset* Info)
{
	bool Result = false;
	StartMissionEvent(Info, Result);
	return Result;
}

bool AMission::UpdateMission(UPrimaryDataAsset* Info)
{
	bool Result = false;
	UpdateMissionEvent(Info, Result);
	return Result;
}

bool AMission::FinishMission(UPrimaryDataAsset* Info)
{
	bool Result = false;
	FinishMissionEvent(Info, Result);
	return Result;
}