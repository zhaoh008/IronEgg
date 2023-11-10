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
	FMatrix Matrix_Z;
	float Angele=PI/3;
	Matrix_Z.SetColumn(0,FVector(FMath::Cos(Angele),-FMath::Sin(Angele),0));
	Matrix_Z.SetColumn(1,FVector(FMath::Sin(Angele),FMath::Cos(Angele),0));
	Matrix_Z.SetColumn(2,FVector(0,0,1));
	Matrix_Z.SetColumn(3,FVector(0,0,0));
	Matrix_Z.M[3][0]=0;
	Matrix_Z.M[3][1]=0; 
	Matrix_Z.M[3][2]=0;
	Matrix_Z.M[3][3]=1.0f;
	//打印矩阵第4行数据
	Matrix_Z.DebugPrint();
	FVector Po= Matrix_Z.TransformPosition(FVector (1,1,1));
	//
	UE_LOG(LogTemp,Log,TEXT("z 60 %s"),*Po.ToString());
	
	FTransform tr{
		FRotator{ 0.f, 60.0f, 0.0f },    // Rotation around each axis in degrees (y, x ,z)
		FVector{ 0.0f, 0.0f, 0.0f },        // Translation
		FVector{ 1.0f, 1.0f, 1.0f }         // Scale
	};
	FMatrix mt=tr.ToMatrixWithScale();
	mt.DebugPrint();
	Po= tr.TransformPosition(FVector (1,1,1));
	//
	UE_LOG(LogTemp,Log,TEXT("z 60 %s"),*Po.ToString());

	
	FMatrix Matrix_X;
	
	Matrix_X.SetColumn(0,FVector(1,0,0));
	Matrix_X.SetColumn(1,FVector(0,FMath::Cos(Angele),-FMath::Sin(Angele)));
	Matrix_X.SetColumn(2,FVector(0,FMath::Sin(Angele),FMath::Cos(Angele)));
	Matrix_X.SetColumn(3,FVector(0,0,0));
	
	Matrix_X.M[3][0]=0;
	Matrix_X.M[3][1]=0; 
	Matrix_X.M[3][2]=0;
	Matrix_X.M[3][3]=1.0f;


	//打印矩阵第4行数据
	Matrix_X.DebugPrint();
	 Po= Matrix_X.TransformPosition(FVector (1,1,1));
	//
	UE_LOG(LogTemp,Log,TEXT("y 60 %s"),*Po.ToString());
	
	FTransform tr_x{
		FRotator{ 0.f, 0.0f, -60.0f },    // Rotation around each axis in degrees (y, x ,z)
		FVector{ 0.0f, 0.0f, 0.0f },        // Translation
		FVector{ 1.0f, 1.0f, 1.0f }         // Scale
	};
	 mt=tr_x.ToMatrixWithScale();
	mt.DebugPrint();
	Po= tr_x.TransformPosition(FVector (1,1,1));
	UE_LOG(LogTemp,Log,TEXT("y 60 %s"),*Po.ToString());
	
	FMatrix Matrix_Y;
	Matrix_Y.SetColumn(0,FVector(FMath::Cos(Angele),0,FMath::Sin(Angele)));
	Matrix_Y.SetColumn(1,FVector(0,1,0));
	Matrix_Y.SetColumn(2,FVector(-FMath::Sin(Angele),0,FMath::Cos(Angele)));
	Matrix_Y.SetColumn(3,FVector(0,0,0));
	Matrix_Y.M[3][0]=0;
	Matrix_Y.M[3][1]=0; 
	Matrix_Y.M[3][2]=0;
	Matrix_Y.M[3][3]=1.0f;

	
	//打印矩阵第4行数据
	Matrix_Y.DebugPrint();
	Po= Matrix_Y.TransformPosition(FVector (1,1,1));
	//
	UE_LOG(LogTemp,Log,TEXT("x 60 %s"),*Po.ToString());
	
	FTransform tr_y{
		FRotator{ -60.f, 0.0f, 0.0f },    // Rotation around each axis in degrees (y, x ,z)
		FVector{ 0.0f, 0.0f, 0.0f },        // Translation
		FVector{ 1.0f, 1.0f, 1.0f }         // Scale
	};
	mt=tr_y.ToMatrixWithScale();
	mt.DebugPrint();
	Po= tr_y.TransformPosition(FVector (1,1,1));
	//
	UE_LOG(LogTemp,Log,TEXT("x 60 %s"),*Po.ToString());
	

}

