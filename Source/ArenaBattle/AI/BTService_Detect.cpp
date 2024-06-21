// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"
#include "ABAI.h"
#include "AIController.h"
#include "Interface/ABCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Physics/ABCollision.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect"); // Node 이름 설정
	Interval = 1.0f; // 1초 단위로 수행하도록 간격 조정
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); // AI로부터 Pawn의 정보 가져옴
	if (nullptr == ControllingPawn)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation(); // Pawn의 위치 
	UWorld* World = ControllingPawn->GetWorld(); // Pawn이 속한 월드
	if (nullptr == World)
	{
		return;
	}

	IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn); // AI Interface 형변환
	if (nullptr == AIPawn)
	{
		return;
	}

	float DetectRadius = AIPawn->GetAIDetectRange(); // 감지할 영역의 값을 가져옴

	TArray<FOverlapResult> OverlapResults; // 오버랩 함수를 사용해 체크 진행
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel( // 플레이어가 다수 있다는 가정하에 멀티 함수 사용 -> 결과물이 배열로 나옴
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_ABACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult) // 감지가 되었을 때
	{
		for (auto const& OverlapResult : OverlapResults) // 감지된 모든 Pawn에 대해 검사 수행
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController()) // 플레이어 캐릭터인 경우
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn); // 타겟 값을 저장
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f); // 녹색으로 해당 영역 표시

				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f); 
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);
				// 플레이어의 위치를 선, 점으로 표현
				return;
			}
		}
	}

	// Pawn이 감지했으나, 플레이어를 못찾거나 찾지 못했을 때 똑같이 설정해주되 null 값 설정
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}