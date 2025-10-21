// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "XRRefreshRateExtension.h"
#include "Modules/ModuleManager.h"

class FXRRefreshRateModule : public IModuleInterface
{
	TUniquePtr<FXRRefreshRateExtension> Extension;
	
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static inline FXRRefreshRateModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FXRRefreshRateModule>("XRRefreshRate");
	}
	
	FXRRefreshRateExtension* GetExtension() const { return Extension.Get(); }
};
