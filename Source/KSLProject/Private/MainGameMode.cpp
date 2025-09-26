// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"



// 업데이트 결과 판정
void AMainGameMode::UpdateJudgeResult(FString InLabel, bool bInIsCorrect)
{
	LastJudge.label      = InLabel;
	LastJudge.bIsCorrect = bInIsCorrect;

	UE_LOG(LogTemp, Log, TEXT("[Judge] Updated: Label=%s, Correct=%s"),
		*InLabel,
		bInIsCorrect ? TEXT("True") : TEXT("False"));
}



// 툭정 문제 및 시퀀스 판정 조회
bool AMainGameMode::GetIsCorrectFor(FString InLabel,
	bool& bOutIsCorrect) const
{
	if (LastJudge.label == InLabel)
	{
		bOutIsCorrect = LastJudge.bIsCorrect;
		return true;
	}
	bOutIsCorrect = false;
	return false;
}
