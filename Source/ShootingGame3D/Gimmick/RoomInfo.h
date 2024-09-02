#pragma once

#include "CoreMinimal.h"
#include "RoomEnums.h"
#include "RoomInfo.generated.h"



USTRUCT(BlueprintType)
struct FRoomInfo
{
    GENERATED_BODY()

public:
    int32 RoomNum;
    uint8 OpenDir = static_cast<uint8>(EOpenDir::EOD_NONE);
    bool bIsVisited;
};