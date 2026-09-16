// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtendedUserSettingsSubsystem.h"
#include "GenericPlatform/GenericApplication.h"
#include "Widgets/SWindow.h"

TArray<FDisplayInfo> UExtendedUserSettingsSubsystem::GetAllDisplayInfo() const
{
	TArray<FDisplayInfo> MonitorInfoArray;
	
	FDisplayMetrics DisplayMetrics;
	FDisplayMetrics::RebuildDisplayMetrics(DisplayMetrics);
	
	for (const FMonitorInfo& MonitorInfo : DisplayMetrics.MonitorInfo)
	{
		MonitorInfoArray.Add(FDisplayInfo(
			MonitorInfo.Name,
			MonitorInfo.ID,
			MonitorInfo.NativeWidth,
			MonitorInfo.NativeHeight,
			MonitorInfo.MaxResolution,
			MonitorInfo.bIsPrimary,
			MonitorInfo.DPI
		));
	}
	
	return MonitorInfoArray;
}

void UExtendedUserSettingsSubsystem::SetActiveDisplayByID(const FString& DisplayID)
{
	FDisplayMetrics DisplayMetrics;
	FDisplayMetrics::RebuildDisplayMetrics(DisplayMetrics);
	for (const FMonitorInfo& MonitorInfo : DisplayMetrics.MonitorInfo)
	{
		if (MonitorInfo.ID == DisplayID)
		{
			FVector2D NewWindowPosition(MonitorInfo.WorkArea.Left, MonitorInfo.WorkArea.Top);
			if (GEngine && GEngine->GameViewport)
			{
				TSharedPtr<SWindow> GWindow = GEngine->GameViewport->GetWindow();
				
				if (GWindow.IsValid())
				{
					GWindow->MoveWindowTo(NewWindowPosition);
				}
			}
			
			return;
		}
		
	}
}
