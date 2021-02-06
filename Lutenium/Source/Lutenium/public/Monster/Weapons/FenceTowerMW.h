
#pragma once

#include "CoreMinimal.h"
#include "Monster/Weapons/MonsterWeapon.h"
#include "GameFramework/Actor.h"
#include "FenceTowerMW.generated.h"

/**
 * Not a weapon by itself, it is the part of PowerSystem
 * Does two things:
 *      1) Create defense beam to other towers if it's a parent tower
 *      2) Shoots the projectile that follows the player
 * Tower is situated on the mesh, and it has indexes to determine on wich socket
 * Parent towers are the ones who create beam to other towers(we don't need to create a beam from each tower to cover every beam)
 * Parent towers are 1, 2 and 5
 * Other towers don't create the beam, they are used as a location for parents' beams
 * If one of the tower gets destroyed, do not create beam to that tower (if it was a parent tower, then it just stops creating towers)
 * On the upgrade two things happen:
 *      1) Beams now go also between high indexes
 *      2) Shoots two projectiles from each tower with a slight delay
 * @see PowerSystemMW.h
 * @see PowerProjectile.h
 */
UCLASS()
class LUTENIUM_API AFenceTowerMW : public AMonsterWeapon
{
	GENERATED_BODY()
public:

    AFenceTowerMW();

    virtual void Tick(float DeltaTime) override;

    // -----------------------------------------------------------------------------------------------------------
	// General
	// -----------------------------------------------------------------------------------------------------------

    /** Reference to the main power system weapon */
    UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category="Power System")
    class APowerSystemMW* PowerSystem;

    // -----------------------------------------------------------------------------------------------------------
	// Indexes
    // -----------------------------------------------------------------------------------------------------------


    /** Indexes are used in Power System to controll the logic of each tower (towers don't take decisions individually) */

    /**
     * Index of the hight level on the monster leg (See sockets of the mesh)
     * The defensive beam should only be between the towers on the same level
     *
     *                 THOSE ARE THE LEGS OKAY?
     *
     *
     *                  FRONT  MIDDLE  BACK
     *
     *                    /1\    |    /1\
     *                   /   \   |   /   \
     *                  /     \  |  /     \
     *                 /     MONSTER       \
     *                /          |          \
     *               /           |           \
     *              0            |            0
     *---------------------------------------------- FLOOR
    */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Index")
    int32 HightIndex;

    /**
     * Index of the position on the level on each leg(0 to 5)
     * Makes easier to raycast from tower to tower (we only have to use 2 towers on the same hight level to coverage all the defensive)
      *
     *      LEFT    0---------------------1  RIGHT
     *              |                     |
     *              |                     |
     *              2       MONSTER       3
     *              |                     |
     *              |                     |
     *              4---------------------5
     *
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Index")
    int32 PositionIndex;

    /** Indactes whenever this tower is active or going to be destroyed */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Index")
    bool bIsActiveTower;

    // -----------------------------------------------------------------------------------------------------------
	// Beam
    // -----------------------------------------------------------------------------------------------------------

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
     * Should be true if the position index is 1, 2 or 5
     * Parents towers are the ones that create beam to another towers
     *
     *      LEFT    0---------------------1  RIGHT
     *              |                     |
     *              |                     |
     *              2       MONSTER       3
     *              |                     |
     *              |                     |
     *              4---------------------5
     *
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Beam")
    bool bParentFenceTower;


    /**
     * Towers to create beams to
     * Valid only for the parent towers
     * At the begging only two adjustent towers are added
     * On the upgrade also adds towers from another hight index
     */
    UPROPERTY(BlueprintReadWrite, Category="Beam")
    TArray<class AFenceTowerMW*> BeamFenceTowers;

    /** The radius of the beam and the raycast */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Beam")
    float BeamRadius;

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
        /* Activates only for parent towers */
        if(!bParentFenceTower)
        {
            return;
        }
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

    /**
     * Emp Movement Effect of the player
     * Being set only for the parent fence tower
     * Being set in tick if this vairable is null
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Beam")
    class UEmpMovementEffect* PlayerEmpMovementEffect;

    /**
     * Handles players if they have overllaped with the beam defense
     * Getting called in Tick()
     * @param Player - shouldn't be nullptr
     */
    UFUNCTION(BlueprintCallable, Category = "Beam")
    void HandlePlayerBeam(class APlayerPawn* Player);

    /**
     * Handles missile if it has overllaped with the beam defense
     * Getting called in Tick()
     * @param Missile - shouldn't be nullptr
     */
    UFUNCTION(BlueprintCallable, Category = "Beam")
    void HandleMissileBeam(class AMissile* Missile);

    /**
     * Event that gets fired when player has entered the active beam
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Beam")
    void PlayerHasEnteredTheBeamEven(const class AActor* Actor);

    // -----------------------------------------------------------------------------------------------------------
	// Projectile
    // -----------------------------------------------------------------------------------------------------------

    /**
     * Projectile class to spawn
     * Should be SubClass because we need to put a BP class of APowerProjecile
     */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class APowerProjectile> PowerProjectileClass;

	/** Arrow component to determine the spawn location and the forward vector for spaned projectiles */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Projectile")
    class UArrowComponent* ArrowProjectile;
    // -----------------------------------------------------------------------------------------------------------
	// Overrides of AMonsterWeaapon
    // -----------------------------------------------------------------------------------------------------------

    /** Overriding Dying function to unassigned the tower from the power system, not the monster! (Monster itslef doesn't control each tower individually)*/
    virtual void Die() override;

    /** Shoots projectiles */
    virtual void ExecuteAttack() override;

    /** Before attack event */
    virtual void BeforeAttackEvent_Implementation() override;

};