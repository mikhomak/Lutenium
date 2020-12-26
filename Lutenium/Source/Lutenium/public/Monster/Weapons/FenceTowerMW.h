
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
    UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category="Power System")
    class APowerSystemMW* PowerSystem;

    // ------------------------------------------------------------------
	// Indexes
	// ------------------------------------------------------------------

    /** Indexes are used in Power System to controll the logic of each tower (towers don't take decisions individually) */

    /**
     * Index of the hight level on the monster leg (See sockets of the mesh)
     * The defensive beam should only be between the towers on the same level(debatable but for now that)
    */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Index")
    int32 HightIndex;

    /**
     * Index of the position on the level on each leg(1 to 4)
     * Makes easier to raycast from tower to tower (we only have to use 2 towers on the same hight level to coverage all the defensive)
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Index")
    int32 PositionIndex;

    // ------------------------------------------------------------------
	// Beam
	// ------------------------------------------------------------------

    /** Variable to check if we should raycast the player in each tick */
    UPROPERTY(BlueprintReadWrite, Category="Beam")
    bool bActiveBeam;

    /**
     * Handles checks whenever the player is in the beam.
     * On player entering the beam we should activate the EMP movement effect
     * On player exitin we should deactivate the EMP effect
     * This variable helps us keeping track of the player position inside the beam
     */
    UPROPERTY(BlueprintReadWrite, Category="Beam")
    bool bIsPlayerInBeam;

    /**
     * Indicates whether this tower should create beam to its neighbors(raycasting for the player and creating vfx)
     * Should be true if the position index is 0 or 3
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Beam")
    bool bParentFenceTower;

    /**
     * Left Neighbor of the tower
     * Used for the vfx in BP(setting target beam each tick)
     * @warning NULL for even position indexes!!!
     */
    UPROPERTY(BlueprintReadWrite, Category="Beam")
    class AFenceTowerMW* LeftNeighborFenceTower;

    /**
     * Right Neighbor of the tower
     * Used for the vfx in BP(setting target beam each tick)
     * @warning NULL for even position indexes!!!
     */
    UPROPERTY(BlueprintReadWrite, Category="Beam")
    class AFenceTowerMW* RightNeighborFenceTower;

    /** The radius of the beam and the raycast */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Beam")
    float BeamRadius;

    /** The time of emp applied to the player in case he doesn't escape with the speed */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Beam")
    float BeamEmpTime;

    /** The force that applied to the emp to the player rotation */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Beam")
    float BeamEmpForce;

    /**
     * Activates defensive beam
     * If the beam is active (bActiveBeam == true), on each tick raycasting from tower to tower for the player
     * If the player has been found in the raycast, activates EMP movement effect on him
     * Activates event for doing some cool stuff in BP (VFX SFX ya know the drill)
     * @param bActive - activate or deactivate beam
     * @param bLeft - Indicates left or right Neighbor
     */
    FORCEINLINE UFUNCTION(Category="Beam")
    void SetActiveBeam(bool bActive, bool bLeft)
    {
        /* Invoking events */
        if(bActive)
        {
            OnActivateBeam(bLeft);
        }
        else
        {
            bIsPlayerInBeam = false; // obligatory disabling player inside the beam variable
            OnDeactivateBeam(bLeft);
        }

        if(bActiveBeam != bActive)
        {
            bActiveBeam = bActive;
        }
     }

    /**
     * Event to activate beam VFX, SFX in BP
     * @param TargetLocation - location of the target for the beam (to set VFX beam target variable)
     * @param bLeft - Indicates left or right Neighbor
     */
    UFUNCTION(BlueprintImplementableEvent, Category="Beam")
    void OnActivateBeam(const bool bLeft);

    /**
     * Event to deactivate beam VFX, SFX in BP
     * @param TargetLocation - location of the target for the beam (to set VFX beam target variable)
     * @param bLeft - Indicates left or right Neighbor
     */
    UFUNCTION(BlueprintImplementableEvent, Category="Beam")
    void OnDeactivateBeam(const bool bLeft);

    // ------------------------------------------------------------------
	// Overrides of AMonsterWeaapon
	// ------------------------------------------------------------------

    /** Overriding Dying function to unassigned the tower from the power system, not the monster! (Monster itslef doesn't control each tower individually)*/
    virtual void Die() override;

    /** Shoots projectiles */
    virtual void ExecuteAttack() override;

};