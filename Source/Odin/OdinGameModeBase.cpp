#include "OdinGameModeBase.h"
#include "UObject/ConstructorHelpers.h"
#include "LogMacros.h"

namespace AOdinGameModeBase_Consts
{
	const FName	GameLiftSDKName = FName("GameLiftServerSDK");
}

AOdinGameModeBase::AOdinGameModeBase()
{
}

void AOdinGameModeBase::BeginPlay()
{
	Super::BeginPlay();
#if WITH_GAMELIFT
	//InitGameLift();
#endif
}

void AOdinGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	//AOD_BasePlayerState* PlayerState = NewPlayer ? NewPlayer->GetPlayerState<AOD_BasePlayerState>() : nullptr;
	//if (PlayerState)
	//{
	//	PlayerState->Initialize();
	//}
	Super::PostLogin(NewPlayer);
}

void AOdinGameModeBase::InitGameLift()
{
	UE_LOG(LogTemp, Log, TEXT("Initializing the GameLift Server"));

	//Getting the module first.
	FGameLiftServerSDKModule* GameLiftSDKModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(AOdinGameModeBase_Consts::GameLiftSDKName);
	if (!GameLiftSDKModule)
		return;

	//Define the server parameters for a GameLift Anywhere fleet. These are not needed for a GameLift managed EC2 fleet.
    FServerParameters ServerParameters;

    //AuthToken returned from the "aws gamelift get-compute-auth-token" API. Note this will expire and require a new call to the API after 15 minutes.
    if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), ServerParameters.m_authToken))
    {
        UE_LOG(LogTemp, Log, TEXT("AUTH_TOKEN: %s"), *ServerParameters.m_authToken)
    }

    //The Host/compute-name of the GameLift Anywhere instance.
    if (FParse::Value(FCommandLine::Get(), TEXT("-hostid="), ServerParameters.m_hostId))
    {
        UE_LOG(LogTemp, Log, TEXT("HOST_ID: %s"), *ServerParameters.m_hostId)
    }

    //The Anywhere Fleet ID.
    if (FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), ServerParameters.m_fleetId))
    {
        UE_LOG(LogTemp, Log, TEXT("FLEET_ID: %s"), *ServerParameters.m_fleetId)
    }

    //The WebSocket URL (GameLiftServiceSdkEndpoint).
    if (FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), ServerParameters.m_webSocketUrl))
    {
        UE_LOG(LogTemp, Log, TEXT("WEBSOCKET_URL: %s"), *ServerParameters.m_webSocketUrl)
    }

    //The PID of the running process
    ServerParameters.m_processId = FString::Printf(TEXT("%d"), GetCurrentProcessId());
    UE_LOG(LogTemp, Log, TEXT("PID: %s"), *ServerParameters.m_processId);

    //InitSDK establishes a local connection with GameLift's agent to enable further communication.
    //Use InitSDK(serverParameters) for a GameLift Anywhere fleet. 
    //Use InitSDK() for a GameLift managed EC2 fleet.
    GameLiftSDKModule->InitSDK(ServerParameters);

    //Implement callback function onStartGameSession
    //GameLift sends a game session activation request to the game server
    //and passes a game session object with game properties and other settings.
    //Here is where a game server takes action based on the game session object.
    //When the game server is ready to receive incoming player connections, 
    //it invokes the server SDK call ActivateGameSession().
    auto onGameSession = [=](Aws::GameLift::Server::Model::GameSession gameSession)
    {
        const FString GameSessionId = FString(gameSession.GetGameSessionId());
        UE_LOG(LogTemp, Log, TEXT("GameSession Initializing: %s"), *GameSessionId);
        GameLiftSDKModule->ActivateGameSession();
    };

    Params.OnStartGameSession.BindLambda(onGameSession);

    //Implement callback function OnProcessTerminate
    //GameLift invokes this callback before shutting down the instance hosting this game server.
    //It gives the game server a chance to save its state, communicate with services, etc., 
    //and initiate shut down. When the game server is ready to shut down, it invokes the 
    //server SDK call ProcessEnding() to tell GameLift it is shutting down.
    auto onProcessTerminate = [=]() 
    {
        UE_LOG(LogTemp, Log, TEXT("Game Server Process is terminating"));
        GameLiftSDKModule->ProcessEnding();
    };

    Params.OnTerminate.BindLambda(onProcessTerminate);

    //Implement callback function OnHealthCheck
    //GameLift invokes this callback approximately every 60 seconds.
    //A game server might want to check the health of dependencies, etc.
    //Then it returns health status true if healthy, false otherwise.
    //The game server must respond within 60 seconds, or GameLift records 'false'.
    //In this example, the game server always reports healthy.
    auto onHealthCheck = []() 
    {
        UE_LOG(LogTemp, Log, TEXT("Performing Health Check"));
        return true;
    };

    Params.OnHealthCheck.BindLambda(onHealthCheck);

    //The game server gets ready to report that it is ready to host game sessions
    //and that it will listen on port 7777 for incoming player connections.
    Params.port = 7777;

    //Here, the game server tells GameLift where to find game session log files.
    //At the end of a game session, GameLift uploads everything in the specified 
    //location and stores it in the cloud for access later.
    TArray<FString> logfiles;
    logfiles.Add(TEXT("GameLift426Test/Saved/Logs/GameLift426Test.log"));
    Params.logParameters = logfiles;

    //The game server calls ProcessReady() to tell GameLift it's ready to host game sessions.
    UE_LOG(LogTemp, Log, TEXT("Calling Process Ready"));
    GameLiftSDKModule->ProcessReady(Params);

	
}
