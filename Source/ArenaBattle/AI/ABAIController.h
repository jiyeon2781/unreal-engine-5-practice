// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ABAIController.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABAIController : public AAIController
{
	GENERATED_BODY()
	
	
public:
	AABAIController();

	void RunAI(); // AI 구동
	void StopAI(); // AI 중지

protected:
	virtual void OnPossess(APawn* InPawn) override;
	// 어떤 컨트롤러가 폰에 빙의해서 조종할 때 발생되는 이벤트 함수

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};
