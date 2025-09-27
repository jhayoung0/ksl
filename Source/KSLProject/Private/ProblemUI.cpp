// Fill out your copyright notice in the Description page of Project Settings.


#include "ProblemUI.h"

// display 단어 바꿔주기
void UProblemUI::SetSignWord(const FText& InText)
{
	UE_LOG(LogTemp, Warning, TEXT("SetSignWord called. txt_signword: %s"), *GetNameSafe(txt_signword));
	
	if (txt_signword)
	{
		txt_signword->SetText(InText);
	}
}
