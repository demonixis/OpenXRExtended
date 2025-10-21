// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OpenXRExtendedLibrary.generated.h"

UENUM(BlueprintType)
enum class EXREnvironmentBlendMode : uint8
{
	None UMETA(DisplayName="(None/Auto)", Hidden),
	Opaque = 1 UMETA(DisplayName="Opaque"),
	Additive = 2 UMETA(DisplayName="Additive"),
	AlphaBlend = 3 UMETA(DisplayName="Alpha Blend / Passthrough")
};

UENUM(BlueprintType)
enum class EXRFoveationLevel : uint8
{
	Off = 0 UMETA(DisplayName="Off"),
	Low = 1 UMETA(DisplayName="Low"),
	Medium = 2 UMETA(DisplayName="Medium"),
	High = 3 UMETA(DisplayName="High")
};

UCLASS()
class OPENXREXTENDED_API UOpenXRExtendedLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="XR|OpenXR")
	static void SetSpaceWarpEnabled(bool bEnable);
	UFUNCTION(BlueprintCallable, Category="XR|OpenXR")
	static void SetPassthroughEnabled(bool bEnable, bool bAlsoSetStartupCVars = true);
	UFUNCTION(BlueprintCallable, Category="XR|OpenXR")
	static void SetVRSFoveation(EXRFoveationLevel Level, bool bGazeTracked = false, bool bPreviewMask = false);
	UFUNCTION(BlueprintCallable, Category="XR|OpenXR")
	static void SetFBFoveation(EXRFoveationLevel Level, bool bDynamic = false, float VerticalOffsetDeg = 0.f);
	UFUNCTION(BlueprintCallable, Category="XR|OpenXR")
	static void SetXRRenderTargetScalePercent(int32 Percent);
	UFUNCTION(BlueprintCallable, Category="XR|OpenXR")
	static void SetXRDynamicResolutionEnabled(bool bEnable, float Min, float Max, float RefreshRateHz);
	UFUNCTION(BlueprintCallable, Category="XR|OpenXR")
	static void SetXRDynamicResolutionExtEnabled(bool bEnable, int32 MinPercent, int32 MaxPercent,
	                                          const float FrameTimeBudgetMs, const bool bAutoMode);
	UFUNCTION(BlueprintCallable, Category="XR|OpenXR")
	static void SetStereoLayerEmulationEnabled(bool bEnable);
	UFUNCTION(BlueprintCallable, Category="XR|OpenXR")
	static void SetStereoLayerLegacyFacingEnabled(bool bEnable);
	UFUNCTION(BlueprintCallable, Category="XR|OpenXR")
	static void SetStereoLayerEmulatedDepthEnabled(bool bEnable);
	UFUNCTION(BlueprintCallable, Category="XR|OpenXR")
	static void SetEnvironmentBlendMode(EXREnvironmentBlendMode Mode);
	UFUNCTION(BlueprintCallable, Category="XR|OpenXR")
	static void SetAlphaInvertPassEnabled(bool bEnable);
	UFUNCTION(BlueprintCallable, Category="XR|OpenXR")
	static bool SetHMDRefreshRateHz(float Hz);

private:
	static bool CVarExists(const TCHAR* Name);
	static void SetCVarBool(const TCHAR* Name, bool bValue);
	static void SetCVarInt(const TCHAR* Name, int32 Value);
	static void SetCVarFloat(const TCHAR* Name, float Value);
	static bool IsMobile();
};
