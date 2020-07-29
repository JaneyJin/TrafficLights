// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrafficLights/TrafficSignBase.h"
#include "TrafficLights/TrafficLightState.h"
#include "TrafficLightBase.generated.h"

/**
 * 
 */
UCLASS()
class TRAFFICLIGHT_API ATrafficLightBase : public ATrafficSignBase
{
	GENERATED_BODY()

public:
    ATrafficLightBase(const FObjectInitializer &ObjectInitializer);

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

protected:
    // virtual void OnConstruction(const FTransform &Transform) override;
    UFUNCTION(Category = "Traffic Light", BlueprintImplementableEvent)
    void OnTrafficLightStateChanged(ETrafficLightState TrafficLightState);


private:
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

};
