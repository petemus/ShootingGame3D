#pragma once

UENUM(BlueprintType)
enum class EOpenDir : uint8
{
    EOD_NONE = 0,
    EOD_LEFT = (1 << 0), // 0001
    EOD_RIGHT = (1 << 1), // 0010
    EOD_UP = (1 << 2), // 0100
    EOD_DOWN = (1 << 3), // 1000
};