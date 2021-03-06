
#pragma once

#include "CoreMinimal.h"
#include "Monster/Weapons/MonsterWeapon.h"
#include "Monster/Weapons/WeaponsUtils/FenceTower2DArray.h"
#include "GameFramework/Actor.h"
#include "PowerSystemMW.generated.h"

/**
 * Weapon that has fence towers
 * Doesn't do much on its own
 * Doesn't have a mesh (it does, but it's empty)
 * What does it do:
 *      1) Initialize array of FenceTowers with correct indexes with APowerSystemMW::InitializeTowerFencse() on BeginPlay of the EnemyMonsterPawn.h
 *      2) Attach them to the mesh via TowerFenceSocketFormat
 *      3) On DoAttack() from the weapon, shoots the PowerProjectile from every available tower via AFenceTowerMW::ExecuteAttack()
 *      4) Has a new method APowerSystemMW::ActivateBeamDefense() which activates the beam defense from all available PARENT fence towers(@see FenceTower.h)
 *      5) On APowerSystemMW::DeactivateBeamDefense(), deactivates the beam in every parent tower
 * @see FenceTower.h
 * @see MonsterWeapon.h
 */
UCLASS()
class LUTENIUM_API APowerSystemMW : public AMonsterWeapon
{
	GENERATED_BODY()
public:

    APowerSystemMW();

    // ------------------------------------------------------------------
	// Overrides of AMonsterWeaapon
	// ------------------------------------------------------------------

    /** Shoots projectiles from towers */
    virtual void ExecuteAttack() override;

    /** Before attack event */
    virtual void BeforeAttackEvent_Implementation() override;

    /* Creates beams between different hights  */
    virtual void SpecificUpgrade(int32 Level) override;

    // ------------------------------------------------------------------
	// Fence Towers
	// ------------------------------------------------------------------

    /**
     * Spawn all the tower fences and add them to arrays
     * Usually it would be in BeginPlay, but because this actor is spawned, we have to directly use this function after spawnging it
     */
    UFUNCTION(BlueprintCallable, Category="Fence Towers")
    void InitializeTowerFencse();

    /**
     * 2D array of all the fence towers of the monster
     * First index - hight index
     * Second index - Position index
     * Indexes should coolorate with the names of the sockets of the monster mesh
     * Using that array we can execute the behaviour we want of the whole system (Power System itself doesn't have direct actions)
     */
    UPROPERTY(BlueprintReadOnly, Category="Fence Towers")
    TArray<FFenceTower2DArray> FenceTowers;

    /**
     * Activated beam defense on all the hight levels
     * Beam defense if a beam between two towers on the same levels and if they are neighbours by position index (beam can't be created from 1st to 3rd tower fence based on position index)
     * If player overlaps with the beam, it will active EMP movement effect
     */
    UFUNCTION(BlueprintCallable, Category="Fence Towers")
    void ActivateBeamDefense();

    /** Deactivates the beam defense */
    UFUNCTION(BlueprintCallable, Category="Fence Towers")
    void DeactivateBeamDefense();

protected:

    // ------------------------------------------------------------------
	// Fence Towers
	// ------------------------------------------------------------------

    /**
     * Safe setting active beam for tower fences
     * Handles destroyed(nullptr) towers
     * FenceTowerStart - the tower from which we should raycast
     * FirstTargetFenceTower - first target tower (we only interested in the presense and position of this tower) to racyast
     * SecondTargetFenceTower - first target tower (we only interested in the presense and position of this tower) to racyast
     * @param bActivate - indicates if beam should be active or not
     * @param FenceTowerStart - tower from which we should activate beam(0 and 3)
     * @param bLeft - indicates to which neighbor we should raycast
     */
    UFUNCTION(BlueprintCallable, Category="Fence Towers")
    void SafeActiveBeam(bool bActivate, class AFenceTowerMW* FenceTowerStart, bool bLeft);

    /**
     * Format of the string to construct the name to find a socket for fence tower depending on the hight and position indexes
     * {} - special symbol to indicate where the index should be
     * IMPORTANT!!! First {} is for HIGHT and the second one is for POSITION
     * IMPORTANT!!! First index starts with 0!!!!
     * By default the format is FenceTowerHight{0}Position{1}Socket
     * Example - The third Fence Tower of the second hight level should be attached to FenceTowerHight1Position2 socket of the mesh
     * Don't forget to create the sockets on the mesh AND NAME THEM WITH THE SAME FORMAT
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category= "Fence Towers Initialization")
    FString TowerFenceSocketFormat;

    /**
     * Helper value to initialize the array of fence towers
     * Represents the maximum amount of hight levles
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category= "Fence Towers Initialization")
    int32 FenceTowersHightLevelsAmount;

    /**
     * Helper value to initialize the array of fence towers
     * Represents the maximum amount of positions
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category= "Fence Towers Initialization")
    int32 FenceTowersPositionsAmount;

    /**
     * Class of FenceTower that will be spawned
     * Should be BP child class of AFenceTowerMW
     * The index are:
     * POSITION INDEXES:
     *
     *
     *      LEFT    0---------------------1  RIGHT
     *              |                     |
     *              |                     |
     *              2       MONSTER       3
     *              |                     |
     *              |                     |
     *              4---------------------5
     *
     * 2, 1 and 5 are parent towers that create beans to other towers
     *
     * HIGH INDEXES:
     *
     *                 THOSE ARE THE LEGS OKAY?
     *
     *
     *                  FRONT  MIDDLE  BACK
     *
     *                    /2\    |    /2\
     *                   /   \   |   /   \
     *                  /     \  |  /     \
     *                 1     MONSTER       1
     *                /          |          \
     *               /           |           \
     *              0            |            0
     *---------------------------------------------- FLOOR
     */
    UPROPERTY(EditDefaultsOnly, Category = "Fence Towers Initialization")
    TSubclassOf<class AFenceTowerMW> FenceTowerClass;

    /** Creates socket name based on the TowerFenceSocketFormat and the indexes */
    UFUNCTION(BlueprintCallable)
    FName ConstructSocketName(int32 HightIndex, int32 PositionIndex);

    /**
     * Helper method to add to the parent tower's BeamFenceTowers array two neighbors
     * Doesn't do anything if Parent is not vallid
     * Doesn't add new neighors if they are not valid
     * @param ParentHightIndex - Hight Index of the parent tower to add new neighbors to
     * @param ParentPositionIndex - Position Index of the parent tower to add new neighbors to
     * @param NeighborsHightIndex - Hight Index of the neighbors
     * @param LeftNeighborPositionIndex - Position Index of the left neighbor
     * @param RightNeighborPositionIndex - Position Index of the right neighbor
     */
    void SaveAddNewNeighborsToTheParentTower(int ParentHightIndex,
                                             int ParentPositionIndex,
                                             int NeighborsHightIndex,
                                             int LeftNeighborPositionIndex,
                                             int RightNeighborPositionIndex);
};