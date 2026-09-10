// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeKeyButton.h"

void UCodeKeyButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	UpdateKey(DefaultKey);
	
}

void UCodeKeyButton::UpdateKey(const FKey& Key)
{
	DefaultKey = Key;
	
	T_KeyName->SetText(DefaultKey.GetDisplayName());
}
