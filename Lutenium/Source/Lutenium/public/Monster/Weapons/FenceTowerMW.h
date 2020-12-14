
#pragma once

#include "CoreMinimal.h"
#include "Monster/Weapons/MonsterWeapon.h"
#include "GameFramework/Actor.h"
#include "FenceTowerMW.generated.h"


UCLASS()
class LUTENIUM_API AFenceTowerMW : public AMonsterWeapon
{
	GENERATED_BODY()
public:

    AFenceTowerMW();

    virtual void Tick(float DeltaTime) override;

    // ------------------------------------------------------------------
	// General
	// ------------------------------------------------------------------

    /** Reference to the main power system weapon */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Power System")
    class APowerSystemMW* PowerSystem;

    // ------------------------------------------------------------------
	// Indexes
	// ------------------------------------------------------------------

    /** Indexes are used in Power System to controll the logic of each tower (towers don't take decisions individually) */

    /** Index of the hight level on the monster leg (See sockets of the mesh) */
    /** The defensive beam should only be between the towers on the same level(debatable but for now that) */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Index")
    int32 HightIndex;

    /** Index of the position on the level on each leg(1 to 4) */
    /** Makes easier to raycast from tower to tower (we only have to use 2 towers on the same hight level to coverage all the defensive) */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Index")
    int32 PositionIndex;

    // ------------------------------------------------------------------
	// Beam
	// ------------------------------------------------------------------

    /** Variable to check if we should raycast the player in each tick */
    UPROPERTY(BlueprintReadWrite, Category="Beam")
    bool bActiveBeam;

    /** Handles checks whenever the player is in the beam. */
    /** On player entering the beam we should activate the EMP movement effect */
    /** On player exitin we should deactivate the EMP effect */
    /** This variable helps us keeping track of the player position inside the beam */
    UPROPERTY(BlueprintReadWrite, Category="Beam")
    bool bIsPlayerInBeam;

    /** Target location where the beam should end */
    UPROPERTY(BlueprintReadWrite, Category="Beam")
    FVector BeamTargetLocation;

    /** The radius of the beam and the raycast */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Beam")
    float BeamRadius;

    /** The time of emp applied to the player in case he doesn't escape with the speed */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Beam")
    float BeamEmpTime;

    /** The force that applied to the emp to the player rotation */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Beam")
    float BeamEmpForce;

    /** Activates defensive beam */
    /** TargetLocation is the location where we should raycast for the player (should be neighbour tower) */
    /** If the beam is active (bActiveBeam == true), on each tick raycasting from tower to tower for the player */
    /** If the player has been found in the raycast, activates EMP movement effect on him */
    /** Activates event for doing some cool stuff in BP (VFX SFX ya know the drill)*/
    FORCEINLINE UFUNCTION(Category="Beam")
    void SetActiveBeam(bool bActive, FVector TargetLocation)
    {
        if(bActiveBeam != bActive)
        {
            if(bActive)
            {
                OnActivateBeam(TargetLocation);
            }
            else
            {
                bIsPlayerInBeam = false; // obligatory disabling player inside the beam variable
                OnDeactivateBeam(TargetLocation);
            }
            bActiveBeam = bActive;
            BeamTargetLocation = TargetLocation;
        }
     }

    /** Event to activate beam VFX, SFX in BP */
    UFUNCTION(BlueprintImplementableEvent, Category="Beam")
    void OnActivateBeam(FVector& TargetLocation);

    /** Event to deactivate beam VFX, SFX in BP */
    UFUNCTION(BlueprintImplementableEvent, Category="Beam")
    void OnDeactivateBeam(FVector& TargetLocation);

    // ------------------------------------------------------------------
	// Overrides of AMonsterWeaapon
	// ------------------------------------------------------------------

    /** Overriding Dying function to unassigned the tower from the power system, not the monster! (Monster itslef doesn't control each tower individually)*/
    virtual void Die() override;

    /** Shoots projectiles */
    virtual void ExecuteAttack() override;

};