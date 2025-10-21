// OpenXRExtendedLibrary.cpp
#include "OpenXRExtendedLibrary.h"
#include "XRRefreshRateBPLibrary.h"
#include "HAL/IConsoleManager.h"

static IConsoleVariable* FindCVar(const TCHAR* Name)
{
	return IConsoleManager::Get().FindConsoleVariable(Name);
}

bool UOpenXRExtendedLibrary::CVarExists(const TCHAR* Name)
{
	return FindCVar(Name) != nullptr;
}

void UOpenXRExtendedLibrary::SetCVarBool(const TCHAR* Name, const bool bValue)
{
	if (IConsoleVariable* C = FindCVar(Name)) { C->Set(bValue ? 1 : 0, ECVF_SetByCode); }
	else { UE_LOG(LogTemp, Warning, TEXT("[XRQuick] CVar not found: %s"), Name); }
}

void UOpenXRExtendedLibrary::SetCVarInt(const TCHAR* Name, const int32 Value)
{
	if (IConsoleVariable* C = FindCVar(Name)) { C->Set(Value, ECVF_SetByCode); }
	else { UE_LOG(LogTemp, Warning, TEXT("[XRQuick] CVar not found: %s"), Name); }
}

void UOpenXRExtendedLibrary::SetCVarFloat(const TCHAR* Name, const float Value)
{
	if (IConsoleVariable* C = FindCVar(Name)) { C->Set(Value, ECVF_SetByCode); }
	else { UE_LOG(LogTemp, Warning, TEXT("[XRQuick] CVar not found: %s"), Name); }
}

bool UOpenXRExtendedLibrary::IsMobile()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	return true;
#else
	return false;
#endif
}

void UOpenXRExtendedLibrary::SetSpaceWarpEnabled(const bool bEnable)
{
	if (bEnable)
	{
		SetCVarBool(TEXT("r.Velocity.DirectlyRenderOpenXRMotionVectors"), true);
	}
	SetCVarBool(TEXT("xr.OpenXRFrameSynthesis"), bEnable);
}

void UOpenXRExtendedLibrary::SetPassthroughEnabled(const bool bEnable, const bool bAlsoSetStartupCVars)
{
	// 3 = AlphaBlend, 1 = Opaque
	SetCVarInt(TEXT("xr.OpenXREnvironmentBlendMode"),
	           bEnable
		           ? static_cast<int32>(EXREnvironmentBlendMode::AlphaBlend)
		           : static_cast<int32>(EXREnvironmentBlendMode::Opaque));

	if (bAlsoSetStartupCVars)
	{
		if (IsMobile())
		{
			SetCVarBool(TEXT("r.Mobile.PropagateAlpha"), bEnable);
		}
		else
		{
			if (CVarExists(TEXT("r.PostProcessing.PropagateAlpha")))
				SetCVarBool(TEXT("r.PostProcessing.PropagateAlpha"), bEnable);
			if (CVarExists(TEXT("r.PostProcess.PropagateAlpha")))
				SetCVarBool(TEXT("r.PostProcess.PropagateAlpha"), bEnable);
		}
		SetCVarBool(TEXT("xr.OpenXRInvertAlpha"), bEnable);
	}
}

void UOpenXRExtendedLibrary::SetVRSFoveation(EXRFoveationLevel Level, const bool bGazeTracked, const bool bPreviewMask)
{
	SetCVarInt(TEXT("xr.VRS.FoveationLevel"), static_cast<int32>(Level));
	SetCVarBool(TEXT("xr.VRS.GazeTrackedFoveation"), bGazeTracked);
	SetCVarBool(TEXT("xr.VRS.FoveationPreview"), bPreviewMask);
}

void UOpenXRExtendedLibrary::SetFBFoveation(EXRFoveationLevel Level, const bool bDynamic, const float VerticalOffsetDeg)
{
	SetCVarInt(TEXT("xr.OpenXRFBFoveationLevel"), static_cast<int32>(Level));
	SetCVarBool(TEXT("xr.OpenXRFBFoveationDynamic"), bDynamic);
	SetCVarFloat(TEXT("xr.OpenXRFBFoveationVerticalOffset"), VerticalOffsetDeg);
}

void UOpenXRExtendedLibrary::SetXRRenderTargetScalePercent(int32 Percent)
{
	Percent = FMath::Clamp(Percent, 50, 200);
	if (CVarExists(TEXT("xr.SecondaryScreenPercentage.HMDRenderTarget")))
	{
		SetCVarInt(TEXT("xr.SecondaryScreenPercentage.HMDRenderTarget"), Percent);
	}
	else
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("[XRQuick] xr.SecondaryScreenPercentage.HMDRenderTarget not found (check UE version)."));
	}
}

void UOpenXRExtendedLibrary::SetXRDynamicResolutionEnabled(const bool bEnable, const float Min, const float Max,
                                                           const float RefreshRateHz)
{
	SetXRDynamicResolutionExtEnabled(bEnable, Min * 100, Max * 100, 1000 / RefreshRateHz, true);
}

void UOpenXRExtendedLibrary::SetXRDynamicResolutionExtEnabled(const bool bEnable, int32 MinPercent, int32 MaxPercent,
                                                              const float FrameTimeBudgetMs, const bool bAutoMode)
{
	MinPercent = FMath::Clamp(MinPercent, 10, 200);
	MaxPercent = FMath::Clamp(MaxPercent, MinPercent, 300);

	if (CVarExists(TEXT("r.DynamicRes.MinScreenPercentage")))
	{
		SetCVarInt(TEXT("r.DynamicRes.MinScreenPercentage"), MinPercent);
	}

	if (CVarExists(TEXT("r.DynamicRes.MaxScreenPercentage")))
	{
		SetCVarInt(TEXT("r.DynamicRes.MaxScreenPercentage"), MaxPercent);
	}

	if (CVarExists(TEXT("r.DynamicRes.FrameTimeBudget")))
	{
		SetCVarFloat(TEXT("r.DynamicRes.FrameTimeBudget"), FrameTimeBudgetMs);
	}

	// 2 = Auto, 1 = Manual, 0 = Off
	if (CVarExists(TEXT("r.DynamicRes.OperationMode")))
	{
		if (!bEnable) SetCVarInt(TEXT("r.DynamicRes.OperationMode"), 0);
		else SetCVarInt(TEXT("r.DynamicRes.OperationMode"), bAutoMode ? 2 : 1);
	}

	// Starting 5.7, without Post Processing ;)
	if (CVarExists(TEXT("xr.MobileLDRDynamicResolution")))
	{
		SetCVarBool(TEXT("xr.MobileLDRDynamicResolution"), bEnable);
	}

	if (bEnable && CVarExists(TEXT("xr.SecondaryScreenPercentage.HMDRenderTarget")))
	{
		SetCVarInt(TEXT("xr.SecondaryScreenPercentage.HMDRenderTarget"), FMath::Clamp(MaxPercent, 50, 200));
	}
}

void UOpenXRExtendedLibrary::SetStereoLayerEmulationEnabled(const bool bEnable)
{
	SetCVarBool(TEXT("xr.OpenXRForceStereoLayerEmulation"), bEnable);
}

void UOpenXRExtendedLibrary::SetStereoLayerLegacyFacingEnabled(const bool bEnable)
{
	if (CVarExists(TEXT("xr.StereoLayers.UseLegacyFacing")))
	{
		SetCVarBool(TEXT("xr.StereoLayers.UseLegacyFacing"), bEnable);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[XRQuick] xr.StereoLayers.UseLegacyFacing not found (ok if not supported)."));
	}
}

void UOpenXRExtendedLibrary::SetStereoLayerEmulatedDepthEnabled(const bool bEnable)
{
	if (CVarExists(TEXT("xr.StereoLayers.RenderEmulatedLayerDepth")))
	{
		SetCVarBool(TEXT("xr.StereoLayers.RenderEmulatedLayerDepth"), bEnable);
	}
	else
	{
		UE_LOG(LogTemp, Log,
		       TEXT("[XRQuick] xr.StereoLayers.RenderEmulatedLayerDepth not found (ok if not supported)."));
	}
}

void UOpenXRExtendedLibrary::SetEnvironmentBlendMode(EXREnvironmentBlendMode Mode)
{
	if (Mode == EXREnvironmentBlendMode::None) return;
	SetCVarInt(TEXT("xr.OpenXREnvironmentBlendMode"), static_cast<int32>(Mode));
}

void UOpenXRExtendedLibrary::SetAlphaInvertPassEnabled(const bool bEnable)
{
	if (CVarExists(TEXT("OpenXR.AlphaInvertPass")))
	{
		SetCVarBool(TEXT("OpenXR.AlphaInvertPass"), bEnable);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[XRQuick] OpenXR.AlphaInvertPass not found (branch-dependent)."));
	}
}

bool UOpenXRExtendedLibrary::SetHMDRefreshRateHz(const float Hz)
{
	const bool bResult = UXRRefreshRateBPLibrary::XR_SetRefreshRate(Hz);

	if (!bResult)
	{
		UE_LOG(LogTemp, Warning, TEXT( "[UOpenXRExtendedLibrary] Wasn't able to change the refresh rate." ));
	}

	return bResult;
}
