#pragma once
#include "CoreMinimal.h"
#include "IOpenXRExtensionPlugin.h"
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>

class XRREFRESHRATE_API FXRRefreshRateExtension : public IOpenXRExtensionPlugin
{
	XrInstance Instance = XR_NULL_HANDLE;
	XrSession Session = XR_NULL_HANDLE;
	bool bReady = false;
	bool bExtPresent = false;
	PFN_xrEnumerateDisplayRefreshRatesFB pfnEnumerateRates = nullptr;
	PFN_xrGetDisplayRefreshRateFB pfnGetRate = nullptr;
	PFN_xrRequestDisplayRefreshRateFB pfnRequestRate = nullptr;

public:
	FXRRefreshRateExtension();

	virtual ~FXRRefreshRateExtension()
	{
	}

	void RegisterOpenXRExtensionModularFeature();
	void UnregisterOpenXRExtensionModularFeature();

	virtual bool GetOptionalExtensions(TArray<const ANSICHAR*>& OutExtensions) override;
	virtual void PostCreateInstance(XrInstance InInstance) override;
	virtual void PostCreateSession(XrSession InSession) override;
	virtual void OnDestroySession(XrSession InSession) override;

	bool IsReady() const { return bReady && Session != XR_NULL_HANDLE; }
	bool GetSupportedRefreshRates(TArray<float>& OutRates) const;
	bool GetCurrentRefreshRate(float& OutHz) const;
	bool SetRefreshRate(int32 Hz) const;

private:
	void LoadFunctionPointers();
};
