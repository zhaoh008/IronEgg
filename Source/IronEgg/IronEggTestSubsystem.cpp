// Fill out your copyright notice in the Description page of Project Settings.


#include "IronEggTestSubsystem.h"

void UIronEggTestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UIronEggTestSubsystem::Deinitialize()
{
	Super::Deinitialize();

}

void UIronEggTestSubsystem::TestThreadFunc()
{

}

bool FMyThreadA::Init()
{
	return true;
}

uint32 FMyThreadA::Run()
{
	return 0;
}

void FMyThreadA::Stop()
{
	//打印停止
	UE_LOG(LogTemp, Warning, TEXT("Thread A is stopping..."));

}

void FMyThreadA::Exit()
{
	//打印退出
	UE_LOG(LogTemp, Warning, TEXT("Thread A is exiting..."));

}

bool FMyThreadB::Init()
{
	return true;
}

uint32 FMyThreadB::Run()
{
	return 0;
}

void FMyThreadB::Stop()
{
	//打印停止
	UE_LOG(LogTemp, Warning, TEXT("Thread A is stopping..."));
}

void FMyThreadB::Exit()
{
	UE_LOG(LogTemp, Warning, TEXT("Thread A is exiting..."));
}
