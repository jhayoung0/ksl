// Fill out your copyright notice in the Description page of Project Settings.


#include "EndGameUI.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UEndGameUI::SetUIText(const FText& InText)
{
	
	if (Txt_changed)
	{
		Txt_changed->SetText(InText);
	}
}


