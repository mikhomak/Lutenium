// Fill out your copyright notice in the Description page of Project Settings.


#include "../../../public/Monster/Weapons/TrafficLightMW.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

ATrafficLightMW::ATrafficLightMW() : AMonsterWeapon()
{
    Health = 100.f;
}

void ATrafficLightMW::DoRedLight()
{
    
}


void ATrafficLightMW::DoYellowLight()
{
    
}

void ATrafficLightMW::DoGreenLight()
{
    
}

void ATrafficLightMW::RedTriggerOverlap(
    class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{

}

void ATrafficLightMW::RedTriggerOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                            class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ATrafficLightMW::YellowTriggerOverlap(
    class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{

}

void ATrafficLightMW::YellowTriggerOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                            class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ATrafficLightMW::GreenTriggerOverlap(
    class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{

}

void ATrafficLightMW::GreenTriggerOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                            class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}