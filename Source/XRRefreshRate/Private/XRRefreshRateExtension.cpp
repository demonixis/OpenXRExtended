#include "XRRefreshRateExtension.h"
#include "Modules/ModuleManager.h"
#include "OpenXRCore.h"
#include "Misc/ScopeLock.h"

static FXRRefreshRateExtension* GRefreshRateExt = nullptr;

FXRRefreshRateExtension::FXRRefreshRateExtension()
{
}

void FXRRefreshRateExtension::RegisterOpenXRExtensionModularFeature()
{
    check(GRefreshRateExt == nullptr);
    GRefreshRateExt = this;
    IModularFeatures::Get().RegisterModularFeature(GetModularFeatureName(), this);
}

void FXRRefreshRateExtension::UnregisterOpenXRExtensionModularFeature()
{
    if (GRefreshRateExt == this)
    {
        IModularFeatures::Get().UnregisterModularFeature(GetModularFeatureName(), this);
        GRefreshRateExt = nullptr;
    }
}

bool FXRRefreshRateExtension::GetOptionalExtensions(TArray<const ANSICHAR*>& OutExtensions)
{
    OutExtensions.Add("XR_FB_display_refresh_rate");
    return true;
}

void FXRRefreshRateExtension::PostCreateInstance(XrInstance InInstance)
{
    Instance = InInstance;
    LoadFunctionPointers();
}

void FXRRefreshRateExtension::PostCreateSession(XrSession InSession)
{
    Session = InSession;
    bReady = (pfnEnumerateRates && pfnGetRate && pfnRequestRate);
}

void FXRRefreshRateExtension::OnDestroySession(XrSession InSession)
{
    if (Session == InSession)
    {
        Session = XR_NULL_HANDLE;
        bReady = false;
    }
}

void FXRRefreshRateExtension::LoadFunctionPointers()
{
    if (Instance == XR_NULL_HANDLE) return;

    auto Load = [&](const char* Name, PFN_xrVoidFunction* OutFn) -> bool
    {
        const XrResult R = xrGetInstanceProcAddr(Instance, Name, OutFn);
        return (R == XR_SUCCESS && *OutFn != nullptr);
    };

    bExtPresent  = Load("xrEnumerateDisplayRefreshRatesFB", reinterpret_cast<PFN_xrVoidFunction*>(&pfnEnumerateRates));
    bExtPresent &= Load("xrGetDisplayRefreshRateFB",        reinterpret_cast<PFN_xrVoidFunction*>(&pfnGetRate));
    bExtPresent &= Load("xrRequestDisplayRefreshRateFB",    reinterpret_cast<PFN_xrVoidFunction*>(&pfnRequestRate));

    if (!bExtPresent)
    {
        UE_LOG(LogTemp, Warning, TEXT("[XRRefreshRate] XR_FB_display_refresh_rate functions not available (runtime may not support it)."));
        pfnEnumerateRates = nullptr;
        pfnGetRate = nullptr;
        pfnRequestRate = nullptr;
    }
}

bool FXRRefreshRateExtension::GetSupportedRefreshRates(TArray<float>& OutRates) const
{
    OutRates.Reset();
    if (!IsReady() || !pfnEnumerateRates) return false;

    uint32 Count = 0;
    XrResult R = pfnEnumerateRates(Session, 0, &Count, nullptr);
    if (R != XR_SUCCESS || Count == 0) return false;

    OutRates.SetNumUninitialized(Count);
    R = pfnEnumerateRates(Session, Count, &Count, OutRates.GetData());
    if (R != XR_SUCCESS)
    {
        OutRates.Reset();
        return false;
    }
    return true;
}

bool FXRRefreshRateExtension::GetCurrentRefreshRate(float& OutHz) const
{
    OutHz = 0.f;
    if (!IsReady() || !pfnGetRate) return false;

    XrResult R = pfnGetRate(Session, &OutHz);
    return (R == XR_SUCCESS);
}

bool FXRRefreshRateExtension::SetRefreshRate(int32 Hz) const
{
    if (!IsReady() || !pfnRequestRate) return false;

    TArray<float> Rates;
    if (GetSupportedRefreshRates(Rates))
    {
        const float Desired = static_cast<float>(Hz);
        if (!Rates.ContainsByPredicate([&](float V){ return FMath::IsNearlyEqual(V, Desired, 0.01f); }))
        {
            UE_LOG(LogTemp, Warning, TEXT("[XRRefreshRate] %d Hz not supported by runtime."), Hz);
            return false;
        }
    }
    const XrResult R = pfnRequestRate(Session, static_cast<float>(Hz));
    const bool bOk = (R == XR_SUCCESS);
    if (!bOk)
    {
        UE_LOG(LogTemp, Warning, TEXT("[XRRefreshRate] xrRequestDisplayRefreshRateFB(%d) failed: %d"), Hz, (int32)R);
    }
    return bOk;
}
