// Fill out your copyright notice in the Description page of Project Settings.

#include "SignPlayerController.h"

#include "Blueprint/UserWidget.h"

void ASignPlayerController::SetGameState(GamePlayState NewState)
{
	CurrentState = NewState;

	switch (NewState)
	{
	case GamePlayState::MainMenu:
		break;
	case GamePlayState::TopicSelect:
		break;
	case GamePlayState::LessonIntro:
		BeginLesson();
		break;
	case GamePlayState::PlayingOneSequence:
		PlayOneSeqMotion();
		break;
	case GamePlayState::PlayingTwoSequence:
		PlayTwoSeqMotion();
		break;
	case GamePlayState::WaitingJudge:
		break;
	case GamePlayState::Feedback:
		break;
	case GamePlayState::TopicComplete:
		break;
	case GamePlayState::ResultMenu:
		break;
	}
}

void ASignPlayerController::BeginLesson()
{
}

void ASignPlayerController::PlayOneSeqMotion()
{
	AMainCharacter* ControlledPlayer = Cast<AMainCharacter>(GetPawn());
	if (ControlledPlayer &&
		ControlledPlayer->PlaySignMontageByKey(FName(TEXT("10")), FName(TEXT("00")), 1))
	{
		SetGameState(GamePlayState::WaitingJudge);
	}
}

void ASignPlayerController::PlayTwoSeqMotion()
{
	AMainCharacter* ControlledPlayer = Cast<AMainCharacter>(GetPawn());
	if (ControlledPlayer &&
		ControlledPlayer->PlaySignMontageByKey(FName(TEXT("10")), FName(TEXT("01")), 2))
	{
		SetGameState(GamePlayState::WaitingJudge);
	}
}

void ASignPlayerController::OnJudgeDone(bool bCorrect)
{
	if (!bCorrect)
	{
		SetGameState(GamePlayState::WaitingJudge);
		return;
	}

	SetGameState(GamePlayState::Feedback);
}

void ASignPlayerController::ShowWidgetForState(GamePlayState State)
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	TSubclassOf<UUserWidget> WidgetClass = nullptr;

	switch (State)
	{
	case GamePlayState::MainMenu:
		WidgetClass = MainMenuClass;
		break;
	case GamePlayState::PlayingOneSequence:
		WidgetClass = PlayingClass;
		break;
	case GamePlayState::Feedback:
		WidgetClass = FeedbackClass;
		break;
	case GamePlayState::TopicComplete:
		WidgetClass = TopicCompleteClass;
		break;
	case GamePlayState::ResultMenu:
		WidgetClass = ResultClass;
		break;
	default:
		break;
	}

	if (WidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(this, WidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport(0);
		}
	}
}
