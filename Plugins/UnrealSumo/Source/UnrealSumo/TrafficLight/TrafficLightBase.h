// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrafficSignBase.h"
#include "TrafficLightState.h"
#include "TrafficLightBase.generated.h"

class USumoGameInstance;

UCLASS()
class UNREALSUMO_API ATrafficLightBase : public ATrafficSignBase
{
	GENERATED_BODY()


public:
    ATrafficLightBase(const FObjectInitializer &ObjectInitializer);


    /**
     * Override default BeginPlay function. Called when the game starts or when objects are spawned
     */
    virtual void BeginPlay() override;

    void OnConstruction(const FTransform &Transform);

    virtual void Tick(float DeltaSeconds) override;

    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    void SwitchTrafficLightState();

    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    void SetTrafficLightState(ETrafficLightState State);

    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    ETrafficLightState GetTrafficLightState() const
    {
        return State;
    }

    // Getter
    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    float GetGreenTime() const;

    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    float GetYellowTime() const;

    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    float GetRedTime() const;

    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    float GetElapsedTime() const;

    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    bool GetTimeIsFrozen() const;


    // Setter
    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    void SetGreenTime(float InGreenTime);

    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    void SetYellowTime(float InYellowTime);

    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    void SetRedTime(float InRedTime);

    // used from replayer
    void SetElapsedTime(float InElapsedTime);

    UFUNCTION(Category = "Traffic Light", BlueprintCallable)
    void SetTimeIsFrozen(bool InTimeIsFrozen);


    // Shared custom GameInstance class. Variables in SumoGameInstance are modified in SumoGameMode.
    USumoGameInstance* SumoGameInstance;

protected:
    // virtual void OnConstruction(const FTransform &Transform) override;
    UFUNCTION(Category = "Traffic Light", BlueprintImplementableEvent)
    void OnTrafficLightStateChanged(ETrafficLightState TrafficLightState);


private:
    std::vector<std::pair<char,double>> FirstTrafficLightLogic;


    UPROPERTY(Category = "Traffic Light", EditAnywhere)
    ETrafficLightState State = ETrafficLightState::Red;


    UPROPERTY(Category = "Traffic Light", EditAnywhere)
    float GreenTime = 10.0f;

    UPROPERTY(Category = "Traffic Light", EditAnywhere)
    float YellowTime = 2.0f;


    UPROPERTY(Category = "Traffic Light", EditAnywhere)
    float RedTime = 7.0f;

    UPROPERTY(Category = "Traffic Light", VisibleAnywhere)
    float ElapsedTime = 0.0f;

    UPROPERTY(Category = "Traffic Light", EditAnywhere)
    bool TimeIsFrozen = false;

    UPROPERTY(Category = "Traffic Light", EditAnywhere)
    double GreenTick = 0;

    UPROPERTY(Category = "Traffic Light", EditAnywhere)
    double YellowTick = 0;


    UPROPERTY(Category = "Traffic Light", EditAnywhere)
    double RedTick = 0;

    char ExtractLightState(std::string TL_State, int TL_Group);

    void TickByMachineTime(float DeltaSeconds);

    void TickByCount();

    UPROPERTY(Category = "Traffic Light", VisibleAnywhere)
    double ElapsedTick = 0;
};
