#pragma once

#include "Camera/CameraComponent.h" // UCameraComponent
#include "EgoVehicle.h"             // AEgoVehicle
#include "Engine/Scene.h"           // FPostProcessSettings
#include "GameFramework/Pawn.h"     // CreatePlayerInputComponent
#include "Kismet/KismetMathLibrary.h" // Kismet Math Library for Weighted Moving Average function
#include "Kismet/KismetStringLibrary.h"
#include <deque>
///////////// : Extra Libraries : /////////////////////

#ifndef _WIN32
// can only use LogitechWheel plugin on Windows! :(
#undef USE_LOGITECH_PLUGIN
#define USE_LOGITECH_PLUGIN false
#endif

#if USE_LOGITECH_PLUGIN
#include "LogitechSteeringWheelLib.h" // LogitechWheel plugin for hardware integration & force feedback
#endif

#if USE_ARDUINO_PLUGIN
#include "SerialCom.h"
#endif

#include "DReyeVRPawn.generated.h"

UCLASS()
class ADReyeVRPawn : public APawn
{
    GENERATED_BODY()

  public:
    ADReyeVRPawn(const FObjectInitializer &ObjectInitializer);

    virtual void SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) override;
    virtual void Tick(float DeltaSeconds) override;

    void BeginPlayer(APlayerController *PlayerIn);
    void BeginEgoVehicle(AEgoVehicle *Vehicle, UWorld *World);

    void SetEgoVehicle(AEgoVehicle *Vehicle)
    {
        EgoVehicle = Vehicle;
    }

    APlayerController *GetPlayer()
    {
        return Player;
    }

    UCameraComponent *GetCamera()
    {
        return FirstPersonCam;
    }

    const UCameraComponent *GetCamera() const
    {
        return FirstPersonCam;
    }

    bool GetIsLogiConnected() const
    {
        return bIsLogiConnected;
    }

  protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void BeginDestroy() override;
    void ReadConfigVariables();

    class UWorld *World = nullptr;
    class AEgoVehicle *EgoVehicle = nullptr;

  private:
    ////////////////:CAMERA:////////////////
    UPROPERTY(Category = Camera, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent *FirstPersonCam;
    void ConstructCamera();
    float FieldOfView = 90.f; // in degrees
    void NextShader();
    void PrevShader();
    size_t CurrentShaderIdx = 0; // 0th shader is rgb (camera)

    void TickSpectatorScreen(float DeltaSeconds); // to render the spectator screen (VR) or flat-screen hud (non-VR)
    void DrawSpectatorScreen();
    void DrawFlatHUD(float DeltaSeconds);

    ////////////////:STEAMVR:////////////////
    void InitSteamVR();         // Initialize the Head Mounted Display
    void InitSpectator();       // Initialize the VR spectator
    void TickSteamVR();         // Ensure SteamVR is active on every tick
    void InitReticleTexture();  // initializes the spectator-reticle texture
    UTexture2D *ReticleTexture; // UE4 texture for eye reticle
    float HUDScaleVR;           // How much to scale the HUD in VR

    ////////////////:FLATHUD:////////////////
    // (Flat) HUD (NOTE: ONLY FOR NON VR)
    void InitFlatHUD();
    UPROPERTY(Category = HUD, EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    class ADReyeVRHUD *FlatHUD;
    FVector2D ReticlePos;                // 2D reticle position from eye gaze
    int ReticleSize = 100;               // diameter of reticle (line thickness is 10% of this)
    bool bDrawFlatHud = true;            // whether to draw the flat hud at all (default true, but false in VR)
    bool bDrawFPSCounter = true;         // draw FPS counter in top left corner
    bool bDrawGaze = false;              // whether or not to draw a line for gaze-ray on HUD
    bool bDrawSpectatorReticle = false;   // Reticle used in the VR-spectator mode
    bool bDrawFlatReticle = false;        // Reticle used in the flat mode (uses HUD) (ONLY in non-vr mode)
    bool bEnableSpectatorScreen = false; // don't spent time rendering the spectator screen
    bool bRectangularReticle = false;    // draw the reticle texture on the HUD & Spectator (NOT RECOMMENDED)

    ////////////////:INPUTS:////////////////
    void MouseLookUp(const float mY_Input);
    void MouseTurn(const float mX_Input);
    bool InvertMouseY;
    float ScaleMouseY;
    float ScaleMouseX;

    // keyboard mechanisms to access Axis vehicle control (steering, throttle, brake)
    void SetBrakeKbd(const float in);
    void SetSteeringKbd(const float in);
    void SetThrottleKbd(const float in);
    // most of the time, the participant will use the logi for inputs, but if needed the experimenter
    // can use the keyboard to reposition/takeover without input conflict
    bool bOverrideInputsWithKbd = true; // keyboard > logi priority for inputs

    void SetupEgoVehicleInputComponent(UInputComponent *PlayerInputComponent, AEgoVehicle *EV);
    UInputComponent *InputComponent = nullptr;
    APlayerController *Player = nullptr;

    ////////////////////////////////////////// : Arduino Control : /////////////////////////////////////////////
#if USE_ARDUINO_PLUGIN
    USerial* Serial; // Object of USerial class
    bool bOpened; // Flag for Port Connected or Not
    void connectSerial(); // Connect and Initialize the Port ad given Port name and Baud rate
    void disconnectSerial(); // disconnects the Serial Port and Clean up the Port
    std::tuple<float, float, float> extractValues(FString val); // returns the extracted values from Arduino in form of Tuple
    void updateSerial(); 
#endif
    void TickSerial(); // Ticking the Arduino controller
         
    ///////////////////////////////////////////// : Utilities: /////////////////////////////////////////////
    void Print_String(FString stringData); // Prints string values on the game window
    // float normalizeInRange(float X, float min, float max, float rangeX, float rangeY); // Return the normalized values between given range

    ////////////////:LOGI:////////////////
    void InitLogiWheel();
    void TickLogiWheel();
    void DestroyLogiWheel(bool DestroyModule);
    bool bLogLogitechWheel = false;
    int WheelDeviceIdx = 0; // usually leaving as 0 is fine, only use 1 if 0 is taken
#if USE_LOGITECH_PLUGIN
    struct DIJOYSTATE2 *Old = nullptr; // global "old" struct for the last state
    void LogLogitechPluginStruct(const struct DIJOYSTATE2 *Now);
    void LogitechWheelUpdate();      // for logitech wheel integration
    void ApplyForceFeedback() const; // for logitech wheel integration
    float WheelRotationLast, AccelerationPedalLast, BrakePedalLast;
#endif
    bool bIsLogiConnected = false; // check if Logi device is connected (on BeginPlay)
    bool bIsHMDConnected = false;  // checks for HMD connection on BeginPlay
    // default logi plugin behaviour is to set things to 0.5 for some reason
    // "Pedals will output a value of 0.5 until the wheel/pedals receive any kind of input."
    // https://github.com/HARPLab/LogitechWheelPlugin
    bool bPedalsDefaulting = true;
};
