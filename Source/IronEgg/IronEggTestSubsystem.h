// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "IronEggTestSubsystem.generated.h"

/**
 *
 */
UCLASS()
class IRONEGG_API UIronEggTestSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:



	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;


	//测试线程函数
	UFUNCTION(BlueprintCallable, Category = "JustForFun")
		void TestThreadFunc();
};


//线程类A
class FMyThreadA : public FRunnable
{



public:

	//构造
	FMyThreadA(FEvent* InEvent) : MyEvent(InEvent) {}
	//析构
	~FMyThreadA() {

	}

	bool Init() override;


	uint32 Run() override;


	void Stop() override;


	void Exit() override;

private:

	FEvent* MyEvent;

};
//线程类B
class FMyThreadB : public FRunnable
{
public:
	bool Init() override;


	uint32 Run() override;


	void Stop() override;


	void Exit() override;

private:
	FEvent* MyEvent;

};
