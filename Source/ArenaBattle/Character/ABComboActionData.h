// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UABComboActionData();

	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;
	// 몽타주 섹션 이름 지정

	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount; 
	// 콤보 개수

	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate;
	// 프레임 기준 재생 속도 -> 정확한 타이밍에 체크가 일어나도록 설정

	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameCount;
	// 입력이 사전에 입력됐는지 감지하는 프레임들을 지정
};
