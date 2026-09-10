// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeOptionRotator.h"


void UCodeOptionRotator::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	MyText->SetText(OR_ChangeableText);
	
	OnRotatedWithDirection.AddDynamic(this, &UCodeOptionRotator::HandleRotated);
	
}

void UCodeOptionRotator::HandleRotated(int32 Value, ERotatorDirection RotatorDir)
{
	if (ImgPips)
	{
		if (UMaterialInstanceDynamic* DynMat = ImgPips->GetDynamicMaterial())
		{
			DynMat->SetScalarParameterValue(TEXT("ActivePipIndex"), static_cast<float>(Value));
		}
	}
	
}

void UCodeOptionRotator::MarkAsCustom()
{
	MyText->SetText(FText::FromString(TEXT("Custom")));
	SetActivePipIndex(-1);
}

void UCodeOptionRotator::SetupPips(int32 Count)
{
	if (ImgPips)
	{
		if (UMaterialInstanceDynamic* DynMat = ImgPips->GetDynamicMaterial())
		{
			DynMat->SetScalarParameterValue(TEXT("PipCount"), static_cast<float>(Count));
		}
	}
}

void UCodeOptionRotator::SetActivePipIndex(int32 Index)
{
	if (ImgPips)
	{
		if (UMaterialInstanceDynamic* DynMat = ImgPips->GetDynamicMaterial())
		{
			DynMat->SetScalarParameterValue(TEXT("ActivePipIndex"), static_cast<float>(Index));
		}
	}
}
