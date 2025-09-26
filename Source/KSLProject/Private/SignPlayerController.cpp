// Fill out your copyright notice in the Description page of Project Settings.

#include "SignPlayerController.h"
#include "Blueprint/UserWidget.h"


void ASignPlayerController::BeginPlay()
{
	Super::BeginPlay();

	MainChar = Cast<AMainCharacter>(GetPawn());
	ShowWidgetForState(GamePlayState::MainMenu);
}




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
	// 토픽에 따라 첫번째 문제 선택
	if (topicId == 0) // 숫자
	{
		startquestionid = TEXT("0");
	}
	else if (topicId ==1) // 의사소통
	{
		startquestionid = TEXT("11");
	}
	else // 랜덤 문제 시작
	{
		startquestionid = FString::FromInt(FMath::RandRange(0, 24));
	};

	SetGameState(GamePlayState::PlayingOneSequence);
}


// 첫번째 시퀀스 모션 노출
void ASignPlayerController::PlayOneSeqMotion()
{
	// 여기 수정해야 함.
	label = startquestionid;
	
	if (MainChar &&
		MainChar->PlaySignMontageByKey(startquestionid, 1))
	{
		SetGameState(GamePlayState::WaitingJudge);
	}

	ShowWidgetForState(GamePlayState::PlayingOneSequence);
}

// 두번째 시퀀스 모션 노출 (두번째 시퀀스 있는 경우만)
void ASignPlayerController::PlayTwoSeqMotion()
{
	const FString Label = TEXT("10_01");

	if (MainChar &&
		MainChar->PlaySignMontageByKey(Label, 2))
	{
		SetGameState(GamePlayState::WaitingJudge);
	}
}

// 판정이 완료되면
void ASignPlayerController::OnJudgeDone(bool bCorrect)
{
	if (!bCorrect)
	{
		SetGameState(GamePlayState::WaitingJudge);
		return;
	}

	SetGameState(GamePlayState::Feedback);
}



// 상태에 맞는 위젯 노출
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
