// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"


#define TIMEMANAGEMENT_MODULE_NAME TEXT("TimeManagement")

class ITimeSynchronizationManager;

class TIMEMANAGEMENT_API ITimeManagementModule : public IModuleInterface
{
public:
	
	/**
	 * Singleton-like access to ITimeManagementModule
	 * @return Returns ITimeManagementModule singleton instance, loading the module on demand if needed
	 */
	static inline ITimeManagementModule& Get()
	{
		return FModuleManager::LoadModuleChecked<ITimeManagementModule>(TIMEMANAGEMENT_MODULE_NAME);
	}

	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 * @return True if the module is loaded and ready to use
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded(TIMEMANAGEMENT_MODULE_NAME);
	}

	/** Return the current TimeSynchronizationManager that is responsible to own the current timecode for the Engine */
	virtual ITimeSynchronizationManager* GetTimeSynchronizationManager() = 0;
	virtual ITimeSynchronizationManager* GetTimeSynchronizationManager() const = 0;
	
	/** Set the current TimeSynchronizationManager that is responsible to own the current timecode for the Engine */
	virtual void SetTimeSynchronizationManager(ITimeSynchronizationManager* Manager) = 0;
};
