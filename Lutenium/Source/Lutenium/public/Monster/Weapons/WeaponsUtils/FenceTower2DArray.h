#pragma once

#include "FenceTower2DArray.generated.h"

/** Used for 2D array of FenceTowers in PowerSystemMW */
USTRUCT(Blueprintable)
struct FFenceTower2DArray
{
    GENERATED_BODY()
public:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Array)
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

    bool IsValidIndex(int32 Index)
    {
        return Array.IsValidIndex(Index);
    }

    void RemoveAt(int32 Index, int32 Count, bool bAllowShrinking)
    {
        Array.RemoveAt(Index, Count, bAllowShrinking);
    }

    FFenceTower2DArray()
    {
        Array.Empty();
    }

    void Insert(class AFenceTowerMW* FenceTower, int Index)
    {
        Array.Insert(FenceTower, Index);
    }
};

/** CREDITS - https://answers.unrealengine.com/questions/551199/dynamic-2d-array-using-tarray.html */