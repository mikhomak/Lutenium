#include "Monster/EnemyMonsterPawn.h"
#include "Monster/MonsterLeg.h"
#include "Monster/MonsterLegComponent.h"
#include "Monster/Weapons/MonsterWeapon.h"
#include "Monster/Weapons/PipeMW.h"
#include "Monster/Weapons/SirenMW.h"
#include "Monster/Weapons/FanMW.h"
#include "Monster/Weapons/TrafficLightMW.h"
#include "Monster/Weapons/PowerSystemMW.h"
#include "Player/PlayerPawn.h"
#include "Monster/MonsterAIController.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Math/UnrealMathUtility.h"

AEnemyMonsterPawn::AEnemyMonsterPawn()
{
	/* setting the tick values. The tick group is PostPhysics */
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = ETickingGroup::TG_PostPhysics;

	/* Setting the attachment rules */
	const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(
		EAttachmentRule::KeepRelative, false);

	/* Setting the root component as a sphere */
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->SetSimulatePhysics(true);
	SphereComponent->SetEnableGravity(false);
	RootComponent = SphereComponent;

	/* Setting the mesh */
	MonsterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	MonsterMesh->AttachToComponent(SphereComponent, AttachmentTransformRules);
	FlashParameterName = "Flashing Hit Alpha Lerp";


	/* Setting Health settings */
	Health = 500.f;
	DirectDamageReduction = 0.5f;
	bHandleDeathInCpp = true;
	InvincibilityTime = 0.2f;
	bShouldFlash = true;

	/* Setting up body variables */
	BodySocketName = "BodySocket";
	BodyUpMovementSpeed = 1000.f;
	BodyForwardRaycastLength = 50000.f;
	bCheckBodyAltitude = true;

	/* Setting the movement  */
	fDistanceBetweenBodyMeshAndActorToStartMoving = 50000.f;
	fDistanceBetweenBodyMeshAndActorToStartMoving = 5000.f;
	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Pawn Floating Movement"));

	/* Creates sphere for the beam defense */
	BeamDefenseSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Beam Defense Sphere"));
	BeamDefenseSphere->AttachToComponent(SphereComponent, AttachmentTransformRules);

	/* Sockets */
	FirstHightLevelSocketName = "FirstLevelSocket";
	SecondHightLevelSocketName = "SecondLevelSocket";
	FourthHightLevelSocketName = "FourthLevelSocket";

	/* Initialize legs */

	/**
	 * LEFT    3-----------------------------0    RIGHT
	 *         |                             |
	 *         |                             |
	 *         |                             |
	 *         4-----------Monster-----------1
	 *         |                             |
	 *         |                             |
	 *         |                             |
	 *         5-----------------------------2
	 */
	/** RIGHT LEGS */
	RightFrontLeg = CreateDefaultSubobject<UMonsterLegComponent>(TEXT("Right Front Leg"));
	RightFrontLeg->AttachToComponent(SphereComponent, AttachmentTransformRules);
	RightFrontLeg->SetMesh(MonsterMesh);
	RightFrontLeg->SecondJointSocketName = "RightFrontJoint_2_Socket";
	RightFrontLeg->ThirdJointSocketName = "RightFrontSecond_3_JointSocket";
	InitLeg(RightFrontLeg, 0);

	RightMiddleLeg = CreateDefaultSubobject<UMonsterLegComponent>(TEXT("Right Middle Leg"));
	RightMiddleLeg->AttachToComponent(SphereComponent, AttachmentTransformRules);
	RightMiddleLeg->SetMesh(MonsterMesh);
	RightMiddleLeg->SecondJointSocketName = "RightMiddleJoint_2_Socket";
	RightMiddleLeg->ThirdJointSocketName = "RightMiddleSecond_3_JointSocket";
	InitLeg(RightMiddleLeg, 1);

	RightBackLeg = CreateDefaultSubobject<UMonsterLegComponent>(TEXT("Right Back Leg"));
	RightBackLeg->AttachToComponent(SphereComponent, AttachmentTransformRules);
	RightBackLeg->SetMesh(MonsterMesh);
	RightBackLeg->SecondJointSocketName = "RightBackJoint_2_Socket";
	RightBackLeg->ThirdJointSocketName = "RightBackSecond_3_JointSocket";
	InitLeg(RightBackLeg, 2);

	/** LEFT LEGS */
	LeftFrontLeg = CreateDefaultSubobject<UMonsterLegComponent>(TEXT("Left Front Leg"));
	LeftFrontLeg->AttachToComponent(SphereComponent, AttachmentTransformRules);
	LeftFrontLeg->SetMesh(MonsterMesh);
	LeftFrontLeg->SecondJointSocketName = "LeftFrontJoint_2_Socket";
	LeftFrontLeg->ThirdJointSocketName = "LeftFrontSecond_3_JointSocket";
	InitLeg(LeftFrontLeg, 3);

	LeftMiddleLeg = CreateDefaultSubobject<UMonsterLegComponent>(TEXT("Left Middle Leg"));
	LeftMiddleLeg->AttachToComponent(SphereComponent, AttachmentTransformRules);
	LeftMiddleLeg->SetMesh(MonsterMesh);
	LeftMiddleLeg->SecondJointSocketName = "LeftMiddleJoint_2_Socket";
	LeftMiddleLeg->ThirdJointSocketName = "LeftMiddleSecond_3_JointSocket";
	InitLeg(LeftMiddleLeg, 4);

	LeftBackLeg = CreateDefaultSubobject<UMonsterLegComponent>(TEXT("Left Back Leg"));
	LeftBackLeg->AttachToComponent(SphereComponent, AttachmentTransformRules);
	LeftBackLeg->SetMesh(MonsterMesh);
	LeftBackLeg->SecondJointSocketName = "LeftBackJoint_2_Socket";
	LeftBackLeg->ThirdJointSocketName = "LeftBackSecond_3_JointSocket";
	InitLeg(LeftBackLeg, 5);

	ToggleWhatLegsShouldMove(true);
	bIsCurrentLegsOdd = true;
	LegDistanceBetweenLegsToMove = 5000.f;
	LegRaycastDownLength = 200000.f;
	LegLerpValue = 1.f;
	LegStepTime = 15.f;
	LegAddedHightStep = 20000.f;

	/* Setting weapons sockets */
	PipeSocketName = "PipeSocket";
	FanSocketName = "FanSocket";
	SirenSocketName = "SirenSocket";
	TrafficLightSocketName = "TrafficLightSocket";
	PowerSystemSocketName = "PowerSystemSocket";

	/* Weapons */
	CurrentWeaponLevel = 0;
	StepScreamWeaponLevelNeeded = 1;
	/* Don't forget to set the AI Controller in Blueprint! */
}

void AEnemyMonsterPawn::BeginPlay()
{
	Super::BeginPlay();
    // Material
    UMaterialInterface* Material = MonsterMesh->GetMaterial(0);

    if(Material)
    {
        MainMaterialInstance = MonsterMesh->CreateDynamicMaterialInstance(0, Material);
    }

	SpawnWeapons();
	/** Setting MonsterAI, in case it wasn't set */
	if(MonsterAI == nullptr)
	{
		AMonsterAIController* AIContr = Cast<AMonsterAIController>(GetController());
		if(AIContr)
		{
			MonsterAI = AIContr;
		}
	}
}

void AEnemyMonsterPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}


void AEnemyMonsterPawn::SpawnWeapons()
{

	/* Spawn Params */
	FActorSpawnParameters SpawnParams;

	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;


	/* PIPE */
	if(PipeClass)
	{
		APipeMW* Pipe = GetWorld()->SpawnActor<APipeMW>(PipeClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if(Pipe)
		{
			Pipe->AttachToComponent(MonsterMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false), PipeSocketName);
			Pipe->MonsterPawn = this;
			Pipe->MonsterMesh = MonsterMesh;
			WeaponMap.Add(EMonsterWeaponType::Pipe, Pipe);
		}
	}

	/* FAN */
	if(FanClass)
	{
		AFanMW* Fan = GetWorld()->SpawnActor<AFanMW>(FanClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if(Fan)
		{
			Fan->AttachToComponent(MonsterMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false), FanSocketName);
			Fan->MonsterPawn = this;
			Fan->MonsterMesh = MonsterMesh;
			WeaponMap.Add(EMonsterWeaponType::Fan, Fan);
		}
	}

	/* SIREN */
	if(SirenClass)
	{
		ASirenMW* Siren = GetWorld()->SpawnActor<ASirenMW>(SirenClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if(Siren)
		{
			Siren->AttachToComponent(MonsterMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false), SirenSocketName);
			Siren->MonsterPawn = this;
			Siren->MonsterMesh = MonsterMesh;
			WeaponMap.Add(EMonsterWeaponType::Siren, Siren);
		}
	}

	/* TRAFFIC LIGHT */
	if(TrafficLightClass)
	{
		ATrafficLightMW* TrafficLight = GetWorld()->SpawnActor<ATrafficLightMW>(TrafficLightClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if(TrafficLight)
		{
			TrafficLight->AttachToComponent(MonsterMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false), TrafficLightSocketName);
			TrafficLight->MonsterPawn = this;
			TrafficLight->MonsterMesh = MonsterMesh;
			WeaponMap.Add(EMonsterWeaponType::TrafficLight, TrafficLight);
		}
	}

	/* POWER SYSTEM */
	if(PowerSystemClass)
	{
		APowerSystemMW* PowerSystem = GetWorld()->SpawnActor<APowerSystemMW>(PowerSystemClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if(PowerSystem)
		{
			PowerSystem->AttachToComponent(MonsterMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false), PowerSystemSocketName);
			PowerSystem->MonsterPawn = this;
			PowerSystem->MonsterMesh = MonsterMesh;
			PowerSystem->InitializeTowerFencse(); // do it after setting up monster pawn and mesh
			WeaponMap.Add(EMonsterWeaponType::PowerSystem, PowerSystem);
		}
	}

}



/* Setting actor eyes view point so AI CONTROLLER will put perception on the body  */
void AEnemyMonsterPawn::GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const
{
	Location = MonsterMesh->GetSocketLocation(BodySocketName);
	Rotation = GetActorRotation();
}

void AEnemyMonsterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HandleActorAndMeshStopAndStartMoving();
	// movement
	if(MonsterAI && MonsterAI->bCanMove && !bIsActorWaitingForBody)
	{
		PawnMovement->AddInputVector(DirectionToMove, true);
	}
	

	if(bCheckBodyAltitude)
	{
		CheckBodyAltitudeDependingOnLegs();
		if(bBodyMoving)
		{
			FVector NewActorLocation = GetActorLocation();
			NewActorLocation.Z += BodyUpMovementSpeed;
			SetActorLocation(NewActorLocation);
		}
	}
}


void AEnemyMonsterPawn::HandleActorAndMeshStopAndStartMoving()
{
	FVector SubVector = GetActorLocation() - GetCurrentBodyPosition();
	SubVector.Z = 0; // we don't care about Z value
	float fDistnace = FMath::Abs(SubVector.Size());
	if(!bIsActorWaitingForBody && fDistnace > fDistanceBetweenBodyMeshAndActorToStop)
	{
		bIsActorWaitingForBody = true;
	}
	else if(bIsActorWaitingForBody && fDistnace < fDistanceBetweenBodyMeshAndActorToStartMoving)
	{
		bIsActorWaitingForBody = false;
	}
}

FVector AEnemyMonsterPawn::GetCurrentBodyPosition()
{
	return MonsterMesh->GetSocketLocation(BodySocketName);
}


/*
 * Get leg location to use in animation blueprint
 */
FVector AEnemyMonsterPawn::GetLegLocation(int32 LegIndex) const
{
	return Legs.IsValidIndex(LegIndex) && Legs[LegIndex] != nullptr ? Legs[LegIndex]->GetCurrentPosition() : GetActorLocation();
}

void AEnemyMonsterPawn::LegHasMovedEventCaller(const int32 LegIndex, const FVector& Position)
{
	// If the weapon level is high enough, spawns the scream on the position of the stop
	if(CurrentWeaponLevel >= StepScreamWeaponLevelNeeded)
	{
		SpawnStepScream(Position);
	}
	ToggleWhatLegsShouldMove(LegIndex % 2 != 0);
	LegHasMoved(LegIndex, Position);
}


void AEnemyMonsterPawn::SpawnStepScream(FVector Position)
{
	if(UWorld* World = GetWorld())
	{
	    FActorSpawnParameters SpawnParams;
	    SpawnParams.Owner = this;
	    SpawnParams.Instigator = this;
	    
	    AScream* Scream = World->SpawnActor<AScream>(StepScreamClass, Position, GetActorRotation(),
	                                                    SpawnParams);
	    if(Scream)
	    {
            Scream->SetUseSecondMesh(true);
	        Scream->SetDragOrImpulse(false);
	        Scream->bIsEmpScream = false;
	    }
	}
}



/* Checks if there is something between body and the first joint of each leg */
/* If so, start timeline to move the body up*/
void AEnemyMonsterPawn::CheckBodyAltitudeDependingOnLegs()
{
	const FVector BodySocketLocation = MonsterMesh->GetSocketLocation(BodySocketName);
	for (const auto SocketName : TopSocketLocationNames)
	{
		const FVector SocketLocation = MonsterMesh->GetSocketLocation(SocketName);
		FHitResult HitResult;

		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			BodySocketLocation,
			SocketLocation,
			ECollisionChannel::ECC_WorldStatic);

		if (HitResult.bBlockingHit && !bBodyMoving)
		{
			bBodyMoving = true;
			return;
		}
	}

	if(!bBodyMoving)
	{
		FHitResult HitResult;
		const FVector ForwardBodyDirection = BodySocketLocation + DirectionToMove * BodyForwardRaycastLength; 
		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			BodySocketLocation,
			ForwardBodyDirection,
			ECollisionChannel::ECC_WorldStatic);
		if (HitResult.bBlockingHit && !bBodyMoving)
		{
			bBodyMoving = true;
			return;
		}
	}

	bBodyMoving = false;
}

void AEnemyMonsterPawn::ToggleWhatLegsShouldMove(const bool Odd)
{

	bIsCurrentLegsOdd = Odd == bIsCurrentLegsOdd ? !bIsCurrentLegsOdd : bIsCurrentLegsOdd;

	for(int32 Index = 0; Index < Legs.Num(); Index++)
	{
		if(Legs[Index] != nullptr)
		{
			if(Index % 2 == 0)
			{
				Legs[Index]->SetCanMove(!bIsCurrentLegsOdd);
			}
			else
			{
				Legs[Index]->SetCanMove(bIsCurrentLegsOdd);
			}
		}
	}
}

/** Removes a weapon when it got destroyed */
void AEnemyMonsterPawn::LooseWeapon(EMonsterWeaponType WeaponType)
{
	WeaponMap.Remove(WeaponType);
	for (const TPair<EMonsterWeaponType, AMonsterWeapon*>& Pair : WeaponMap)
	{
		if(Pair.Value)
		{
			Pair.Value->UpgradeWeapon();
		}
	}
	CurrentWeaponLevel = CurrentWeaponLevel + 1;
}


float AEnemyMonsterPawn::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	if(!CanBeDamaged())
	{
		return 0.f;
	}

	Damage *= DirectDamageReduction;
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	TakeNonDirectDamage(Damage, false);
	return Damage;
}


void AEnemyMonsterPawn::TakeNonDirectDamage(float Damage, bool bDirectDamage)
{
	if(!CanBeDamaged())
	{
		return;
	}

	// Making monster invincible for sometime
	// We cannot use it in TakeDamage() because we call this method from the weapon when it gets damaged
    SetCanBeDamaged(false);
	Health -= Damage;
	if(Health < 0)
	{
		Die();
		return;
	}

	// Damage material flash
	TakeDamageEvent(Damage, bDirectDamage);
	if(bShouldFlash && MainMaterialInstance != nullptr)
    {
        MainMaterialInstance->SetScalarParameterValue(FlashParameterName, 1.f);
    }
	// Reseting bCanBeDamaged
	FTimerHandle InvincibilityTimer;
    GetWorldTimerManager().SetTimer(InvincibilityTimer, this, &AEnemyMonsterPawn::InvincibilityEnd, InvincibilityTime, false);
}

void AEnemyMonsterPawn::Die()
{
	OnDieEvent();
	if(bHandleDeathInCpp)
	{
		Destroy();
	}
}

/** Player has entered the radius of beam defense */
/** Telling that to MonsterAIController and we will do something with it in behaviour tree*/
void AEnemyMonsterPawn::PlayerHasEnteredBeamDefense(AActor* OverlapActor)
{
	APlayerPawn* Player = Cast<APlayerPawn>(OverlapActor);
	if(Player && MonsterAI)
	{
		MonsterAI->SetIsPlayerInRadiusOfBeamDefense(true);
	}
}

/** Player has exit beam defense radius */
/** Notiyfing monster ai controller */
void AEnemyMonsterPawn::PlayerHasExitdBeamDefense(AActor* OverlapExitActor)
{
	APlayerPawn* Player = Cast<APlayerPawn>(OverlapExitActor);
	if(Player && MonsterAI)
	{
		MonsterAI->SetIsPlayerInRadiusOfBeamDefense(false);
	}
}

/** Returns the world location of the hight level socket of the mesh*/
/** Location should be 1 to 4*/
/** Returns ActorLocation if the Location is not correct */
FVector AEnemyMonsterPawn::GetHightLevelSocketLocation(const int32 Location)
{
	switch(Location)
	{
		case 1:
			return MonsterMesh->GetSocketLocation(FirstHightLevelSocketName);
		case 2:
			return MonsterMesh->GetSocketLocation(SecondHightLevelSocketName);
		case 3:
			return MonsterMesh->GetSocketLocation(ThirdHightLevelSocketName);
		case 4:
			return MonsterMesh->GetSocketLocation(FourthHightLevelSocketName);
	}
	return GetActorLocation();
}


void AEnemyMonsterPawn::InitLeg(UMonsterLegComponent* Leg, int32 LegIndex)
{
	/**
	 * LEFT    3-----------------------------0    RIGHT
	 *         |                             |
	 *         |                             |
	 *         |                             |
	 *         4-----------Monster-----------1
	 *         |                             |
	 *         |                             |
	 *         |                             |
	 *         5-----------------------------2
	 */
	Leg->SetEnemyMonsterPawn(this);
	Leg->SetLegIndex(LegIndex);
	Legs.Insert(Leg, LegIndex);
}


void AEnemyMonsterPawn::SetDirectionToMove(FVector& PositionWS)
{
	DirectionToMove = (PositionWS - GetActorLocation());
	DirectionToMove.Normalize();
}

