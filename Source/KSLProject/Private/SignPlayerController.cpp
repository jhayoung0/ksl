// Fill out your copyright notice in the Description page of Project Settings.

#include "SignPlayerController.h"

#include "MotionRow.h"
#include "ProblemUI.h"
#include "Blueprint/UserWidget.h"


void ASignPlayerController::BeginPlay()
{
	Super::BeginPlay();

	MainChar = Cast<AMainCharacter>(GetPawn());
	SetGameState(GamePlayState::MainMenu);
}

void ASignPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FString EnumName = UEnum::GetValueAsString(CurrentState);
	UE_LOG(LogTemp, Warning, TEXT("CurrentState: %s"), *EnumName);
	
}


void ASignPlayerController::SetGameState(GamePlayState NewState)
{
	PreviousState = CurrentState;
	CurrentState  = NewState;

	switch (CurrentState)
	{
	case GamePlayState::MainMenu:
		break;
	case GamePlayState::TopicSelect:
		break;
	case GamePlayState::LessonIntro:
		BeginLesson(); // 1회 호출
		break;
	case GamePlayState::PlayingOneSequence:
		PlayOneSeqMotion();
		ShowWidgetForState(GamePlayState::PlayingOneSequence);
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


	ShowWidgetForState(CurrentState);
	
}

void ASignPlayerController::BeginLesson()
{
	// 토픽에 따라 첫번째 문제 선택
	if (topicId == 0) // 숫자
	{
		startquestionid = TEXT("0");
	}
	else if (topicId==1) // 의사소통
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
	// 1) Row 조회
	if (!MotionTable) return;
	//const FName RowKey(*startquestionid); // *startquestionid
	const FMotionRow* Row = MotionTable->FindRow<FMotionRow>(TEXT("0"), TEXT("PlayOneSeqMotion"));
	if (Row)
	{
		// 2) UI 단어 갱신
		if (PlayingUI)
		{
			PlayingUI->SetSignWord(FText::FromString(Row->sign_text));
		}
	}

	// 3) 몽타주 재생 → 상태 전이
	if (MainChar)
	{
		MainChar->PlaySignMontageByKey(startquestionid, 1);
		UE_LOG(LogTemp, Warning, TEXT("montage"));
		//SetGameState(GamePlayState::WaitingJudge);
	}

	// 4) judgeresult에서 값 꺼내와서 판정
	// seq가 하나일 경우
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

	PlayingUI = nullptr;

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

			if (State == GamePlayState::PlayingOneSequence)
			{
				PlayingUI = Cast<UProblemUI>(CurrentWidget);
			}
		}
	}
}
