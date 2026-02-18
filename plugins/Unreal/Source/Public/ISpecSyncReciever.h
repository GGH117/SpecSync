#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISpecSyncReceiver.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USpecSyncReceiver : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for Game Objects to react to SpecSync Optimization Commands.
 */
class SPECSYNC_API ISpecSyncReceiver
{
	GENERATED_BODY()

public:
	/** * Called when the SpecSync Agent decides to change a specific graphics setting.
	 * @param SettingName - The name of the setting (e.g., "ShadowQuality")
	 * @param NewValue    - The new integer value (0-4)
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "SpecSync")
	void OnOptimizationCommandReceived(const FString& SettingName, int32 NewValue);

	/**
	 * Called during thermal throttling events to force a low-power state.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "SpecSync")
	void OnThermalThrottleTriggered();
};