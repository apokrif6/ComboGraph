#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * ComboGraph module
 */
class FComboGraphRuntimeModule final : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static FComboGraphRuntimeModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FComboGraphRuntimeModule>("ComboGraphRuntime");
	}

	static bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ComboGraphRuntime");
	}
};
