// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class COMBATFRAMEWORK_API ArrayUtils
{
public:
	template <typename T>
	static T GetRandomElement(const TArray<T>& Array)
	{
		if (Array.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Array is empty! Returning default value."));
			return T(); // Return default value for the type
		}

		int32 RandomIndex = FMath::RandRange(0, Array.Num() - 1);
		return Array[RandomIndex];
	}
};
