// Fill out your copyright notice in the Description page of Project Settings.


#include "ProblemUI.h"
#include "Components/Image.h"
#include "Components/Overlay.h"

// display 단어 바꿔주기
void UProblemUI::SetSignWord(const FText& InText)
{
	
	if (txt_signword)
	{
		txt_signword->SetText(InText);
	}
}





