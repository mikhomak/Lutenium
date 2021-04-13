#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Mission.generated.h"

/**
 * Mission class
 * Each Mission has EMissionInfo with it, info class contains all the data elements
 * This class contains functionallity to deal with EMissionInfo
 * Life cycles:
 *		1) Start() - When the mission starts
 *		2) Update() - Update the mission with wharever data you want
 *		3) Finish() - Finishes the mission
 * Override those methods in BP
 * In those methods we pass EMissionInfo however you should handle what to do with it in BP
 * Each mission has to have its subclass of EMissionInfo so you can add wharever data you want for this particular mission
 * @warning this class is the foundation for the missions, it doesn't do anything by itself, you have to override life cycles in BP!!!
 */
UCLASS()
class AMission : public AActor
{
	GENERATED_BODY()

public:
	AMission();

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mission Information")
	FString MissionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mission Information")
	FString MissionDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mission Information")
	bool bHasTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mission Information")
	float MissionTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mission Information")
	float MissionTimer;


	/**
	 * Override this method for starting the mission in BP
	 * If this event is not being invoked, check bCheckMissionInfoClass
	 */
	UFUNCTION(BlueprintCallable, Category="Mission life cycle")
	bool StartMission(class UPrimaryDataAsset* Info);

	/**
	 * Override this method for updating the mission in BP
	 * If this event is not being invoked, check bCheckMissionInfoClass
	 */
	UFUNCTION(BlueprintCallable, Category="Mission life cycle")
	bool UpdateMission(class UPrimaryDataAsset* Info);

	/**
	 * Override this method for finishing the mission in BP
	 * If this event is not being invoked, check bCheckMissionInfoClass
	 */
	UFUNCTION(BlueprintCallable, Category="Mission life cycle")
	bool FinishMission(class UPrimaryDataAsset* Info);




	/**
	 * Override this method for starting the mission in BP
	 * If this event is not being invoked, check bCheckMissionInfoClass
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="Mission life cycle")
	void StartMissionEvent(class UPrimaryDataAsset* Info, bool& bSuccess);

	/**
	 * Override this method for updating the mission in BP
	 * If this event is not being invoked, check bCheckMissionInfoClass
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="Mission life cycle")
	void UpdateMissionEvent(class UPrimaryDataAsset* Info, bool& bSuccess);

	/**
	 * Override this method for finishing the mission in BP
	 * If this event is not being invoked, check bCheckMissionInfoClass
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="Mission life cycle")
	void FinishMissionEvent(class UPrimaryDataAsset* Info, bool& bSuccess);


	UFUNCTION(BlueprintImplementableEvent, Category="Mission Information|Timer")
	void TimerHasExpired();

};
