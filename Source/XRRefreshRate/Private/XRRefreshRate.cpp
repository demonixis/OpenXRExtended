// Copyright Epic Games, Inc. All Rights Reserved.

#include "XRRefreshRate.h"

#define LOCTEXT_NAMESPACE "FXRRefreshRateModule"

void FXRRefreshRateModule::StartupModule()
{
	Extension = MakeUnique<FXRRefreshRateExtension>();
	Extension->RegisterOpenXRExtensionModularFeature();
}

void FXRRefreshRateModule::ShutdownModule()
{
	if (Extension)
	{
		Extension->UnregisterOpenXRExtensionModularFeature();
		Extension.Reset();
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FXRRefreshRateModule, XRRefreshRate)