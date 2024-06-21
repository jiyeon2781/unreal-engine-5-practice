// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ABAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UABAnimInstance::UABAnimInstance()
{
	MoveingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
}

void UABAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	// 현재 AnimInstance를 소유하고 있는 액터정보를 얻어올 수 있음
	// -> Actor 타입이므로 캐릭터인지 알 수 없음 
	// 형변환 진행

	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
		// 캐릭터 무브먼트에 대한 값 가져옴
	}
}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		// z값을 제외한 x,y의 크기를 구해서 지면의 속력 저장
		bIsIdle = GroundSpeed < MoveingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
		// 떨어지고 있는지, Velocity의 z값이 Jumping Threshould보다 큰지
	} // 프레임마다 업데이트됨
}
