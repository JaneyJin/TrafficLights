// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrafficSignBase.h"
#include "TrafficLightState.h"
#include "Engine/StaticMesh.h"  // TODO: DELETE
#include "Engine/Texture2D.h"   // TODO: DELETE
#include "AmericanLight.generated.h"

USTRUCT()
struct FTrafficLightPieces
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    UStaticMesh* Mesh;

    UPROPERTY(EditAnywhere)
    FTransform RelativeTransform;

    UPROPERTY(EditAnywhere)
    ETrafficLightState LightColor;

    UPROPERTY(EditAnywhere)
    bool Masked;

    UPROPERTY(EditAnywhere)
    UTexture2D* Mask;

    UPROPERTY(EditAnywhere)
    float Rotation;

    FTrafficLightPieces(){
        // static ConstructorHelpers::FObjectFinder<UStaticMesh> LightMesh(TEXT("StaticMesh'/UnrealSumo/TrafficLightSign/Static/SM_TLights_Leds.SM_TLights_Leds'"));
        // Mesh = LightMesh.Object;
        Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/UnrealSumo/TrafficLightSign/Static/SM_TLights_Leds.SM_TLights_Leds'"));
        LightColor = ETrafficLightState::Red;
        Masked = false;
        Mask = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/UnrealSumo/TrafficLightSign/Static/T_TrafficLightSign_op.T_TrafficLightSign_op'"));
        Rotation = 0.f;
        //UE_LOG(LogTemp, Warning, TEXT("%s, %s, %s"), *Mesh->GetName(), *UEnum::GetValueAsString(LightColor), *Mask->GetName())
    }

};

USTRUCT()
struct FTrafficLightStruct
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    TArray<FTrafficLightPieces> Lights;

    UPROPERTY(EditAnywhere)
    FString Description;

    UPROPERTY(EditAnywhere)
    FTransform Position;

    FTrafficLightStruct(){
        FTrafficLightPieces TLElement;
        TLElement.LightColor = ETrafficLightState::Green;
        Lights.Add(TLElement);
        TLElement.LightColor = ETrafficLightState::Yellow;
        TLElement.RelativeTransform.SetLocation(FVector(0,0,28));
        Lights.Add(TLElement);
        TLElement.LightColor = ETrafficLightState::Red;
        TLElement.RelativeTransform.SetLocation(FVector(0,0,56));
        Lights.Add(TLElement);
    }
};

UCLASS()
class UNREALSUMO_API AAmericanLight : public ATrafficSignBase
{
	GENERATED_BODY()


public:
    AAmericanLight(const FObjectInitializer &ObjectInitializer);

    UPROPERTY(VisibleAnywhere)
    TArray<FTrafficLightStruct> Heads;

};
