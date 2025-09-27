#include "WebSocketActor.h"
#include "MediaPlayer.h"
#include "MediaSource.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Dom/JsonObject.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/World.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "MainCharacter.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Misc/Base64.h"
#include "Modules/ModuleManager.h"
#include "Serialization/JsonSerializer.h"
#include "TimerManager.h"
#include "WebSocketsModule.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/MediaAssets/Public/MediaPlayer.h"
#include "Slate/WidgetRenderer.h"

AWebSocketActor::AWebSocketActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AWebSocketActor::SetFrameSending(bool bShouldSend)
{
	bShouldSendFrames = bShouldSend;

	if (bShouldSendFrames)
	{
		StartSendingFrames();
	}
	else
	{
		StopSendingFrames();
	}
}

void AWebSocketActor::BeginPlay()
{
	Super::BeginPlay();

	if (TargetWidgetClass)
	{
		TargetWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), TargetWidgetClass);
		if (TargetWidgetInstance)
		{
			TargetWidgetInstance->AddToViewport();

			ClippingAreaWidget = TargetWidgetInstance->WidgetTree->FindWidget<UWidget>(TEXT("ClippingAreaWidget"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create widget instance."));
		}
	}

	// 디버깅 로그 추가: ClippingAreaWidget이 제대로 찾아졌는지 확인합니다.
	if (ClippingAreaWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("ClippingAreaWidget was found and assigned successfully!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ClippingAreaWidget could NOT be found! Check UMG widget name and 'Is Variable' checkbox."));
	}

	if (MediaPlayer)
	{
		if (MediaPlayer->OpenUrl(camurl))
		{
			UE_LOG(LogTemp, Log, TEXT("Successfully opened webcam: %s"), *camurl);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to open webcam: %s"), *camurl);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MediaPlayer is not assigned in the editor."));
	}
		
	
	WidgetRenderer = MakeShared<FWidgetRenderer>(true);
	InitializeWebSocket();
}

void AWebSocketActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(FrameSendTimerHandle);
	}

	CloseWebSocket();

	Super::EndPlay(EndPlayReason);
}

void AWebSocketActor::InitializeWebSocket()
{
	if (ServerURL.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("WebSocket server URL is empty."));
		return;
	}

	if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		FModuleManager::LoadModuleChecked<FWebSocketsModule>("WebSockets");
	}

	WebSocket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, Protocol);
	if (!WebSocket.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create WebSocket. URL: %s"), *ServerURL);
		return;
	}

	BindEvents();
	WebSocket->Connect();
}

void AWebSocketActor::BindEvents()
{
	if (!WebSocket.IsValid())
	{
		return;
	}

	WebSocket->OnConnected().AddUObject(this, &AWebSocketActor::OnConnected);
	WebSocket->OnConnectionError().AddUObject(this, &AWebSocketActor::OnConnectionError);
	WebSocket->OnMessage().AddUObject(this, &AWebSocketActor::OnMessageReceived);
	WebSocket->OnClosed().AddUObject(this, &AWebSocketActor::OnClosed);
}

void AWebSocketActor::CloseWebSocket()
{
	if (WebSocket.IsValid())
	{
		WebSocket->Close();
		WebSocket.Reset();
	}
}

void AWebSocketActor::SendTestData()
{
	if (!WebSocket.IsValid() || !WebSocket->IsConnected())
	{
		return;
	}

	// 테스트용 JSON 객체를 만듭니다.
	TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	JsonObject->SetStringField(TEXT("message"), TEXT("Hello from Unreal Engine!"));
	JsonObject->SetNumberField(TEXT("sign_id"), 0);

	// JSON 객체를 문자열로 변환합니다.
	FString JsonString;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	if (!FJsonSerializer::Serialize(JsonObject, Writer))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to serialize test JSON payload."));
		return;
	}

	// 서버로 전송합니다.
	UE_LOG(LogTemp, Log, TEXT("Sending test JSON: %s"), *JsonString);
	WebSocket->Send(JsonString);
}

void AWebSocketActor::OnConnected()
{
	UE_LOG(LogTemp, Log, TEXT("Connected to server."));

	if (bShouldSendFrames)
	{
		StartSendingFrames();
	}
	
	//SendTestData(); 

	//SendFrameData();
	//if (UWorld* World = GetWorld())
	//{
	//	World->GetTimerManager().SetTimer(FrameSendTimerHandle, this, &AWebSocketActor::SendFrameData, 500.f, true);
	//}
}

void AWebSocketActor::OnConnectionError(const FString& Error)
{
	UE_LOG(LogTemp, Error, TEXT("WebSocket connection error: %s"), *Error);
}

void AWebSocketActor::OnMessageReceived(const FString& Message)
{
	UE_LOG(LogTemp, Display, TEXT("Server message: %s"), *Message);

	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Message);

	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		// 서버가 "True"라는 문자열을 보내므로, 문자열로 받은 후 비교하여 bool 값으로 변환합니다.
		FString SuccessString;

		bool bIsNetSuccess = false;
		if (JsonObject->TryGetStringField(TEXT("code"), SuccessString))
		{
			bIsNetSuccess = (SuccessString == TEXT("200"));
		}
		LastServerResponse.Code = bIsNetSuccess;
		if (!LastServerResponse.Code)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to Connect, code : 400")); 
			return;
		}
			
		
		bool bIsSuccess = false;
		if (JsonObject->TryGetStringField(TEXT("is_correct"), SuccessString))
		{
			bIsSuccess = (SuccessString == TEXT("True"));
		}

		// 서버가 "one"이라는 문자열을 보내므로, FString으로 받습니다.
		FString SignId;
		JsonObject->TryGetStringField(TEXT("sign_id"), SignId);

		LastServerResponse.bIsCorrect = bIsSuccess;
		LastServerResponse.SignId = SignId;

		// 로그 출력 형식을 bool(%s)과 FString(%s)에 맞게 수정합니다.
		UE_LOG(LogTemp, Log, TEXT("JSON response - success: %s, sign_id: %s, code: %s"), 
			(bIsSuccess ? TEXT("true") : TEXT("false")), 
			*SignId,
			*SuccessString);

		if (bIsSuccess)
		{
			OnSuccessResponse.Broadcast(LastServerResponse);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to parse JSON response."));
	}
}

void AWebSocketActor::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	UE_LOG(LogTemp, Warning, TEXT("WebSocket closed - code: %d, reason: %s, clean: %s"), StatusCode, *Reason, bWasClean ? TEXT("true") : TEXT("false"));
}

void AWebSocketActor::StartSendingFrames()
{
	if (WebSocket.IsValid() && WebSocket->IsConnected() && !GetWorld()->GetTimerManager().IsTimerActive(FrameSendTimerHandle))
	{
		UE_LOG(LogTemp, Log, TEXT("Starting frame data sending timer..."));
		GetWorld()->GetTimerManager().SetTimer(FrameSendTimerHandle, this, &AWebSocketActor::SendFrameData, 1.f / 6.f, true);
	}
}

void AWebSocketActor::StopSendingFrames()
{
	UE_LOG(LogTemp, Log, TEXT("Stopping frame data sending timer..."));
	GetWorld()->GetTimerManager().ClearTimer(FrameSendTimerHandle);
}

void AWebSocketActor::SendFrameData()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("player"), FoundActors);

	int32 SignValue = -1; // 기본값은 음수로 설정하여, 못 찾았을 경우 전송을 중지하도록 함

	if (FoundActors.Num() > 0)
	{
		// 찾은 액터를 실제 클래스로 형변환(Cast) 시도
		// "AMyPlayerActor"를 실제 사용하는 액터 클래스 이름으로 반드시 변경해야 합니다.
		AMainCharacter* PlayerActor = Cast<AMainCharacter>(FoundActors[0]); 
		if (PlayerActor)
		{
			// 형변환에 성공하면 sign 값을 읽어옵니다.
			//SignValue = PlayerActor->sign;
		}
	}

	// 2. sign 값에 따라 전송 여부 결정
	if (SignValue < 0)
	{
		// sign 값이 음수이면, 전송 상태를 false로 바꾸고 이번 프레임 전송을 중단합니다.
		if (bShouldSendFrames)
		{
			SetFrameSending(false); // 타이머를 멈추고 bool 변수를 업데이트합니다.
		}
		return; // 함수를 즉시 종료하여 아래의 이미지 전송 로직이 실행되지 않도록 합니다.
	}
	else
	{
		// sign 값이 양수인데 전송이 꺼져있었다면 다시 켭니다.
		if (!bShouldSendFrames)
		{
			SetFrameSending(true);
		}
	}
	
	if (!WebSocket.IsValid())
	{
		return;
	}

	if (!TargetWidgetInstance || !ClippingAreaWidget)
	{
		UE_LOG(LogTemp, Verbose, TEXT("No widget instance available for capture."));
		return;
	}

	if (!CaptureWidgetToRenderTarget())
	{
		return;
	}

	FTextureRenderTargetResource* RenderTargetResource = RenderTarget ? RenderTarget->GameThread_GetRenderTargetResource() : nullptr;
	if (!RenderTargetResource)
	{
		UE_LOG(LogTemp, Warning, TEXT("Render target resource unavailable."));
		return;
	}

	TArray<FColor> PixelData;
	if (!RenderTargetResource->ReadPixels(PixelData))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to read pixels from render target."));
		return;
	}

	if (PixelData.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No pixel data captured."));
		return;
	}

	const int32 Width = RenderTarget->SizeX;
	const int32 Height = RenderTarget->SizeY;

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(TEXT("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
	if (!ImageWrapper.IsValid() || !ImageWrapper->SetRaw(PixelData.GetData(), PixelData.Num() * sizeof(FColor), Width, Height, ERGBFormat::BGRA, 8))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to encode captured widget as PNG."));
		return;
	}

	const TArray64<uint8>& CompressedData64 = ImageWrapper->GetCompressed(100);
	TArray<uint8> PngBytes;
	PngBytes.Append(CompressedData64.GetData(), CompressedData64.Num());

	FString Base64Image = FBase64::Encode(PngBytes);

	TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();

	//JsonObject->SetNumberField(TEXT("sign_id"), SignValue);
	JsonObject->SetNumberField(TEXT("sign_id"), 0);
	JsonObject->SetStringField(TEXT("images"), Base64Image);

	FString JsonString;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	if (!FJsonSerializer::Serialize(JsonObject, Writer))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to serialize JSON payload."));
		return;
	}

	WebSocket->Send(JsonString);
}

bool AWebSocketActor::CaptureWidgetToRenderTarget()
{
	if (!ClippingAreaWidget)
	{
		UE_LOG(LogTemp, Verbose, TEXT("ClippingAreaWidget is not valid."));
		return false;
	}

	if (!RenderTarget)
	{
		RenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(this, CaptureWidth, CaptureHeight, RTF_RGBA8);
	}

	if (!RenderTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create render target for capture."));
		return false;
	}

	if (WidgetRenderer.IsValid())
	{
		// 전체 위젯이 아닌, ClippingAreaWidget만 렌더 타겟에 그립니다.
		WidgetRenderer->DrawWidget(RenderTarget, ClippingAreaWidget->TakeWidget(), FVector2D(CaptureWidth, CaptureHeight), 0.f);
	}

	return true;
}




