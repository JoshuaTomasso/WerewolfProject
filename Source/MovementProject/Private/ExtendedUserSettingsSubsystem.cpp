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

int32 UExtendedUserSettingsSubsystem::GetActiveDisplayIndex() const
{
	if (!GEngine || !GEngine->GameViewport)
	{
		return 0;
	}

	const TSharedPtr<SWindow> GameWindow = GEngine->GameViewport->GetWindow();
	if (!GameWindow.IsValid())
	{
		return 0;
	}

	const FVector2D WindowCenter = GameWindow->GetPositionInScreen() + GameWindow->GetSizeInScreen() * 0.5f;

	FDisplayMetrics DisplayMetrics;
	FDisplayMetrics::RebuildDisplayMetrics(DisplayMetrics);

	for (int32 i = 0; i < DisplayMetrics.MonitorInfo.Num(); ++i)
	{
		const FPlatformRect& Rect = DisplayMetrics.MonitorInfo[i].DisplayRect;
		if (WindowCenter.X >= Rect.Left && WindowCenter.X < Rect.Right &&
			WindowCenter.Y >= Rect.Top && WindowCenter.Y < Rect.Bottom)
		{
			return i;
		}
	}

	return 0;
}
