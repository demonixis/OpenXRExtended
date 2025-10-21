#pragma once
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XRRefreshRateBPLibrary.generated.h"

UCLASS()
class XRREFRESHRATE_API UXRRefreshRateBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="XR|RefreshRate")
	static bool XR_GetSupportedRefreshRates(TArray<float>& OutRates);
	UFUNCTION(BlueprintCallable, Category="XR|RefreshRate")
	static bool XR_GetCurrentRefreshRate(float& OutHz);
	UFUNCTION(BlueprintCallable, Category="XR|RefreshRate")
	static bool XR_SetRefreshRate(int32 Hz);
};
