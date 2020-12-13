
#pragma once

#include "CoreMinimal.h"
#include "Monster/Weapons/MonsterWeapon.h"
#include "Monster/Weapons/WeaponsUtils/FenceTower2DArray.h"
#include "GameFramework/Actor.h"
#include "PowerSystemMW.generated.h"


UCLASS()
class LUTENIUM_API APowerSystemMW : public AMonsterWeapon
{
	GENERATED_BODY()
public:

    APowerSystemMW();

	virtual void BeginPlay() override;

    // ------------------------------------------------------------------
	// Overrides of AMonsterWeaapon
	// ------------------------------------------------------------------

    /** Shoots projectiles from towers */
    virtual void ExecuteAttack() override;

    // ------------------------------------------------------------------
	// Fence Towers
	// ------------------------------------------------------------------

    /** 2D array of all the fence towers of the monster */
    /** First index - hight index */
    /** Second index - Position index */
    /** Indexes should coolorate with the names of the sockets of the monster mesh */
    /** Using that array we can execute the behaviour we want of the whole system (Power System itself doesn't have direct actions)*/
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Fence Towers")
    TArray<FFenceTower2DArray> FenceTowers;

    /** Activated beam defense on all the hight levels */
    /** Beam defense if a beam between two towers on the same levels and if they are neighbours by position index (beam can't be created from 1st to 3rd tower fence based on position index) */
    /** If player overlaps with the beam, it will active EMP movement effect */
    UFUNCTION(BlueprintCallable, Category="Fence Towers")
    void ActivateBeamDefense();

    /** Deactivates the beam defense */
    UFUNCTION(BlueprintCallable, Category="Fence Towers")
    void DeactivateBeamDefense();

protected:

    // ------------------------------------------------------------------
	// Fence Towers
	// ------------------------------------------------------------------

    /** Format of the string to construct the name to find a socket for fence tower depending on the hight and position indexes*/
    /** {} - special symbol to indicate where the index should be */
    /** IMPORTANT!!! First {} is for HIGHT and the second one is for POSITION */
    /** IMPORTANT!!! First index starts with 0!!!! */
    /** By default the format is FenceTowerHight{}Position{} */
    /** Example - The third Fence Tower of the second hight level should be attached to FenceTowerHight1Position2 socket of the mesh*/
    /** Don't forget to create the sockets on the mesh AND NAME THEM WITH THE SAME FORMAT */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Fence Towers Initialization")
    FString TowerFenceSocketFormat;

    /** Helper value to initialize the array of fence towers*/
    /** Represents the maximum amount of hight levles */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Fence Towers Initialization")
    int32 FenceTowersHightLevelsAmount;

    /** Helper value to initialize the array of fence towers*/
    /** Represents the maximum amount of positions */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Fence Towers Initialization")
    int32 FenceTowersPositionsAmount;

    /** Class of FenceTower that will be spawned */
    /** Should be BP child class of AFenceTowerMW */
    UPROPERTY(EditDefaultsOnly, Category = "Fence Towers Initialization")
    TSubclassOf<class AFenceTowerMW> FenceTowerClass;

    /** Creates socket name based on the TowerFenceSocketFormat and the indexes */
    UFUNCTION(BlueprintCallable)
    FName ConstructSocketName(int32 HightIndex, int32 PositionIndex);

};