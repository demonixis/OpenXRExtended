#include "XRRefreshRateBPLibrary.h"
#include "XRRefreshRate.h"
#include "Modules/ModuleManager.h"
#include "XRRefreshRateExtension.h"

static FXRRefreshRateExtension* GetExtension()
{
	return FXRRefreshRateModule::Get().GetExtension();
}

bool UXRRefreshRateBPLibrary::XR_GetSupportedRefreshRates(TArray<float>& OutRates)
{
	OutRates.Reset();
	if (const FXRRefreshRateExtension* Ext = GetExtension())
	{
		return Ext->GetSupportedRefreshRates(OutRates);
	}
	return false;
}

bool UXRRefreshRateBPLibrary::XR_GetCurrentRefreshRate(float& OutHz)
{
	OutHz = 0.f;
	if (const FXRRefreshRateExtension* Ext = GetExtension())
	{
		return Ext->GetCurrentRefreshRate(OutHz);
	}
	return false;
}

bool UXRRefreshRateBPLibrary::XR_SetRefreshRate(const int32 Hz)
{
	if (const FXRRefreshRateExtension* Ext = GetExtension())
	{
		return Ext->SetRefreshRate(Hz);
	}
	return false;
}
