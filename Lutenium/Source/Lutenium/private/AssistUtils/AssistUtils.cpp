#include "../../public/AssistUtils/AssistUtils.h"
#include "../../public/PlaneMovementComponent.h"
#include "Math/Vector.h"

bool FAssistUtils::bHasApplied = false;

void FAssistUtils::ApplyTakeOffAcceleration(UPlaneMovementComponent* PlaneMovementComponent, const FVector Velocity,
                                           const float MinTakeOffVelocity, float AddedAcceleration)
{
   if(!FAssistUtils::bHasApplied && Velocity.Size() < MinTakeOffVelocity)
   {      
       PlaneMovementComponent->AddAcceleration(AddedAcceleration);
       bHasApplied = true;
   }
   else if(Velocity.Size() > MinTakeOffVelocity){
        FAssistUtils::bHasApplied = false;
   }
}

