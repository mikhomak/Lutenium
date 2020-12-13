#pragma once

#include "FenceTower2DArray.generated.h"

/** Used for 2D array of FenceTowers in PowerSystemMW */
USTRUCT(Blueprintable)
struct FFenceTower2DArray
{
    GENERATED_BODY()
public:

    TArray<class AFenceTowerMW*> Array;

    class AFenceTowerMW* operator[] (int32 i) {
        return Array[i];
    }

    void Add(class AFenceTowerMW* FenceTower) {
        Array.Add(FenceTower);
    }

    int32 Num() {
        return Array.Num();
    }

    FFenceTower2DArray()
    {
        Array.Empty();
    }
};

/** CREDITS - https://answers.unrealengine.com/questions/551199/dynamic-2d-array-using-tarray.html */