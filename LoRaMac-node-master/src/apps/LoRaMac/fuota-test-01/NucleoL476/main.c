#include <stdio.h>
#include "../firmwareVersion.h"
#include "../../common/githubVersion.h"
#include "utilities.h"
#include "board.h"
#include "gpio.h"
#include "uart.h"
#include "RegionCommon.h"

#include "cli.h"
#include "Commissioning.h"
#include "LmHandler.h"
#include "LmhpCompliance.h"
#include "LmhpClockSync.h"
#include "LmhpRemoteMcastSetup.h"
#include "LmHandlerMsgDisplay.h"
#include "time.h"
#include "delay.h"
#include "gpio-board.h"
#include <stdlib.h>

#define Random                              // algorithm Select  Random or CSMA or Qlearning
#define PAYLOAD         30                 // payload size 30, 50, 100
#define NODE_NUM        2                   // node number 2, 4, 6, 8

// common parameters
#if NODE_NUM == 2
    #define BACKOFF_MAX    4
#elif NODE_NUM == 4
    #define BACKOFF_MAX    8
#elif NODE_NUM == 6
    #define BACKOFF_MAX    12
#elif NODE_NUM == 8
    #define BACKOFF_MAX    16
#endif


int DR = 5;                             // default SF7
int CAD = 3;                            // default SF9
static const uint8_t DRs[2]  = { 5, 3};      // SF7, SF9
static u_int32_t ACK_CNT = 0;               // Total Received ACK
int backOFF = 0;

#ifdef Random
    #define maxAction  BACKOFF_MAX * 2
    int action = maxAction;
    int flag = 0;
    u_int32_t backOFFTime = 0;
    static uint8_t RandomProcess( void );
    int state = 0;

#elif defined CSMA
    uint8_t MaxChanges = 0;
    static uint8_t CSMAProcess( void );

#elif defined Qlearning
    #define maxAction  BACKOFF_MAX * 2
    int action = maxAction;
    static uint8_t envState = 0;
    uint32_t NVisit[3] = {1};
    float PrevQ = 0;
    uint8_t sfcnt = 12;
    
    #if NODE_NUM == 2
        uint32_t NSelect[3][maxAction] = {{1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1}};
        static float Q_table[3][maxAction] = {{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};
    #elif NODE_NUM == 4
        uint32_t NSelect[3][maxAction] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};
        static float Q_table[3][maxAction] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
    #elif NODE_NUM == 6
        uint32_t NSelect[3][maxAction] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};
    static float Q_table[3][maxAction] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
    #elif NODE_NUM == 8
        uint32_t NSelect[3][maxAction] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};
        static float Q_table[3][maxAction] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
    #endif
    
    static void ActionSelect();
    static void UpdateQTable(float reward);
    static uint8_t QLearningProcess( void );
    static float rewardArr[2] = {3.2, 1};

#endif
// ~ common parameters

/*!
 * LoRaWAN default end-device class
 */
#define LORAWAN_DEFAULT_CLASS                       CLASS_A

/*!
 * Defines the application data transmission duty cycle. 40s, value in [ms].
 */
#define APP_TX_DUTYCYCLE                            40000

/*!
 * Defines a random delay for application data transmission duty cycle. 5s,
 * value in [ms].
 */
#define APP_TX_DUTYCYCLE_RND                        1000

/*!
 * LoRaWAN Adaptive Data Rate
 *
 * \remark Please note that when ADR is enabled the end-device should be static
 */
#define LORAWAN_ADR_STATE                           LORAMAC_HANDLER_ADR_OFF

/*!
 * Default datarate
 *
 * \remark Please note that LORAWAN_DEFAULT_DATARATE is used only when ADR is disabled 
 */
#define LORAWAN_DEFAULT_DATARATE                    DR_3
/*!
 * LoRaWAN confirmed messages
 */
#define LORAWAN_DEFAULT_CONFIRMED_MSG_STATE         LORAMAC_HANDLER_CONFIRMED_MSG

/*!
 * User application data buffer size
 */
#define LORAWAN_APP_DATA_BUFFER_MAX_SIZE            242

/*!
 * LoRaWAN ETSI duty cycle control enable/disable
 *
 * \remark Please note that ETSI mandates duty cycled transmissions. Use only for test purposes
 */
#define LORAWAN_DUTYCYCLE_ON                        false

/*!
 *
 */
typedef enum
{
    LORAMAC_HANDLER_TX_ON_TIMER,
    LORAMAC_HANDLER_TX_ON_EVENT,
}LmHandlerTxEvents_t;

/*!
 * User application data
 */
static uint8_t AppDataBuffer[LORAWAN_APP_DATA_BUFFER_MAX_SIZE];

/*!
 * Timer to handle the application data transmission duty cycle
 */
static TimerEvent_t TxTimer;

/*!
 * Timer to handle the state of LED1
 */
static TimerEvent_t Led1Timer;

/*!
 * Timer to handle the state of LED2
 */
static TimerEvent_t Led2Timer;


static void OnMacProcessNotify( void );
static void OnNvmDataChange( LmHandlerNvmContextStates_t state, uint16_t size );
static void OnNetworkParametersChange( CommissioningParams_t* params );
static void OnMacMcpsRequest( LoRaMacStatus_t status, McpsReq_t *mcpsReq, TimerTime_t nextTxIn );
static void OnMacMlmeRequest( LoRaMacStatus_t status, MlmeReq_t *mlmeReq, TimerTime_t nextTxIn );
static void OnJoinRequest( LmHandlerJoinParams_t* params );
static void OnTxData( LmHandlerTxParams_t* params );
static void OnRxData( LmHandlerAppData_t* appData, LmHandlerRxParams_t* params );
static void OnSysTimeUpdate( bool isSynchronized, int32_t timeCorrection );
// static void StartTxProcess( LmHandlerTxEvents_t txEvent );
static uint32_t MakeRandom( uint16_t min, uint16_t max );



static void OnTxPeriodicityChanged( uint32_t periodicity );
static void OnTxFrameCtrlChanged( LmHandlerMsgTypes_t isTxConfirmed );

/*!
 * Function executed on TxTimer event
 */
// static void OnTxTimerEvent( void* context );

/*!
 * Function executed on Led 1 Timeout event
 */
static void OnLed1TimerEvent( void* context );

/*!
 * Function executed on Led 2 Timeout event
 */
static void OnLed2TimerEvent( void* context );

static LmHandlerCallbacks_t LmHandlerCallbacks =
{
    .GetBatteryLevel = BoardGetBatteryLevel,
    .GetTemperature = NULL,
    .GetRandomSeed = BoardGetRandomSeed,
    .OnMacProcess = OnMacProcessNotify,
    .OnNvmDataChange = OnNvmDataChange,
    .OnNetworkParametersChange = OnNetworkParametersChange,
    .OnMacMcpsRequest = OnMacMcpsRequest,
    .OnMacMlmeRequest = OnMacMlmeRequest,
    .OnJoinRequest = OnJoinRequest,
    .OnTxData = OnTxData,
    .OnRxData = OnRxData,
    .OnClassChange = NULL,
    .OnBeaconStatusChange = NULL,
    .OnSysTimeUpdate = OnSysTimeUpdate,
};

static LmHandlerParams_t LmHandlerParams =
{
    .Region = ACTIVE_REGION,
    .AdrEnable = LORAWAN_ADR_STATE,
    .IsTxConfirmed = LORAWAN_DEFAULT_CONFIRMED_MSG_STATE,
    .TxDatarate = LORAWAN_DEFAULT_DATARATE,
    .PublicNetworkEnable = LORAWAN_PUBLIC_NETWORK,
    .DutyCycleEnabled = LORAWAN_DUTYCYCLE_ON,
    .DataBufferMaxSize = LORAWAN_APP_DATA_BUFFER_MAX_SIZE,
    .DataBuffer = AppDataBuffer,
    .PingSlotPeriodicity = REGION_COMMON_DEFAULT_PING_SLOT_PERIODICITY,
};

static LmhpComplianceParams_t LmhpComplianceParams =
{
    .FwVersion.Value = FIRMWARE_VERSION,
    .OnTxPeriodicityChanged = OnTxPeriodicityChanged,
    .OnTxFrameCtrlChanged = OnTxFrameCtrlChanged,
    .OnPingSlotPeriodicityChanged = NULL,
};


/*!
 * Indicates if LoRaMacProcess call is pending.
 * 
 * \warning If variable is equal to 0 then the MCU can be set in low power mode
 */
static volatile uint8_t IsMacProcessPending = 0;

static volatile uint8_t IsTxFramePending = 0;

// TX period Setting
static volatile uint32_t TxPeriodicity = 1000;


/*
 * Indicates if the system time has been synchronized
 */
static volatile bool IsClockSynched = false;

/*
 * MC Session Started
 */
static volatile bool IsMcSessionStarted = false;

/*!
 * LED GPIO pins objects
 */
extern Gpio_t Led1; // Tx
extern Gpio_t Led2; // Rx
         
/*!
 * UART object used for command line interface handling
 */
extern Uart_t Uart2;

u_int32_t startTime = 0;
static uint32_t MakeRandom( uint16_t min, uint16_t max ){
    return (rand() % (max - min + 1)) + min;
}
/*!
 * Main application entry point.
 */
int main( void )
{
    // In main() function
    // SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
    BoardInitMcu( );
    BoardInitPeriph( );
    TimerInit( &Led1Timer, OnLed1TimerEvent );
    TimerSetValue( &Led1Timer, 25 );

    TimerInit( &Led2Timer, OnLed2TimerEvent );
    TimerSetValue( &Led2Timer, 100 );

    // Initialize transmission periodicity variable
    // TxPeriodicity = APP_TX_DUTYCYCLE + randr( -APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND );
    const Version_t appVersion = { .Value = FIRMWARE_VERSION };
    const Version_t gitHubVersion = { .Value = GITHUB_VERSION };
    DisplayAppInfo( "fuota-test-01", &appVersion, &gitHubVersion );

    if ( LmHandlerInit( &LmHandlerCallbacks, &LmHandlerParams ) != LORAMAC_HANDLER_SUCCESS )
    {
        printf( "LoRaMac wasn't properly initialized\n" );
        // Fatal error, endless loop.
        while ( 1 )
        {
        }
    }

    // Set system maximum tolerated rx error in milliseconds
    LmHandlerSetSystemMaxRxError( 20 );

    // The LoRa-Alliance Compliance protocol package should always be
    // initialized and activated.
    LmHandlerPackageRegister( PACKAGE_ID_COMPLIANCE, &LmhpComplianceParams );
    LmHandlerPackageRegister( PACKAGE_ID_CLOCK_SYNC, NULL );
    LmHandlerPackageRegister( PACKAGE_ID_REMOTE_MCAST_SETUP, NULL );
    LmHandlerPackageRegister( 3, NULL );

    IsClockSynched = false;

    while( 1 )
    {
        if(CliProcess( &Uart2 )){
            ACK_CNT = 0;
            IsTxFramePending = 1;
        }

        // Processes the LoRaMac events
        LmHandlerProcess( );

        // Process application uplinks management

        if( LmHandlerIsBusy( ) != true )
        {
            LmHandlerErrorStatus_t status = LORAMAC_HANDLER_ERROR;
            uint8_t isPending = 0;
            CRITICAL_SECTION_BEGIN( );
            isPending = IsTxFramePending;
            IsTxFramePending = 0;
            CRITICAL_SECTION_END( );
            if( isPending == 1 )
            {
                if( IsMcSessionStarted == false )
                {
                    if( IsClockSynched == false )
                    {
                        status = LmhpClockSyncAppTimeReq( );
                    }
                    else
                    {   
                        #if PAYLOAD == 30
                        for(int i = 0; i < 17; i++){
                            AppDataBuffer[i] = randr( 0, 255 );
                        }
                        #elif PAYLOAD == 50
                        for(int i = 0; i < 37; i++){
                            AppDataBuffer[i] = randr( 0, 255 );
                        }
                        #elif PAYLOAD == 100
                        for(int i = 0; i < 87; i++){
                            AppDataBuffer[i] = randr( 0, 255 );
                        }
                        #endif
                        
                        LmHandlerAppData_t appData =
                        {
                            .Buffer = AppDataBuffer,
                            #if PAYLOAD == 30
                                .BufferSize = 17,
                            #elif PAYLOAD == 50
                               .BufferSize = 37,
                            #elif PAYLOAD == 100
                                .BufferSize = 87,
                            #endif
                            
                            .Port = 1,
                        };

                        // Algorithm Zone
                        #ifdef Random
                            status = LmHandlerSend_custom( &appData, RandomProcess( ));
                        #elif defined CSMA
                            status = LmHandlerSend_custom( &appData, CSMAProcess( ));
                        #elif defined Qlearning
                            status = LmHandlerSend_custom( &appData, QLearningProcess( ));
                        #endif
                    }
                
                    if( status == LORAMAC_HANDLER_SUCCESS )
                    {
                        // Switch LED 1 ON
                        GpioWrite( &Led1, 1 );
                        TimerStart( &Led1Timer );
                    }
                }
            }
        }

        CRITICAL_SECTION_BEGIN( );
        if( IsMacProcessPending == 1 )
        {
            // Clear flag and prevent MCU to go into low power modes.
            IsMacProcessPending = 0;
        }
        else
        {
            // The MCU wakes up through events
            BoardLowPowerHandler( );
        }
        CRITICAL_SECTION_END( );
    }
}

static void OnMacProcessNotify( void )
{
    IsMacProcessPending = 1;
}

static void OnNvmDataChange( LmHandlerNvmContextStates_t state, uint16_t size )
{
    DisplayNvmDataChange( state, size );
}

static void OnNetworkParametersChange( CommissioningParams_t* params )
{
    DisplayNetworkParametersUpdate( params );
}

static void OnMacMcpsRequest( LoRaMacStatus_t status, McpsReq_t *mcpsReq, TimerTime_t nextTxIn )
{
    DisplayMacMcpsRequestUpdate( status, mcpsReq, nextTxIn );
}

static void OnMacMlmeRequest( LoRaMacStatus_t status, MlmeReq_t *mlmeReq, TimerTime_t nextTxIn )
{
    DisplayMacMlmeRequestUpdate( status, mlmeReq, nextTxIn );
}

static void OnJoinRequest( LmHandlerJoinParams_t* params )
{
    DisplayJoinRequestUpdate( params );
    if( params->Status == LORAMAC_HANDLER_ERROR )
    {
        LmHandlerJoin( );
    }
    else
    {
        LmHandlerRequestClass( LORAWAN_DEFAULT_CLASS );
    }
}

static void OnTxData( LmHandlerTxParams_t* params )
{
    DisplayTxUpdate( params );
    if(params->IsMcpsConfirm != 0 && IsClockSynched == true){
        IsTxFramePending = 1;
    }
}

static void OnRxData( LmHandlerAppData_t* appData, LmHandlerRxParams_t* params )
{
    DisplayRxUpdate( appData, params );
    ACK_CNT += 1;
}

static void OnSysTimeUpdate( bool isSynchronized, int32_t timeCorrection )
{
    IsClockSynched = isSynchronized;
    ACK_CNT = 0;
}

#ifdef Random
static uint8_t RandomProcess( void )
{
    DR = DRs[MakeRandom( 0, 1 )];
    backOFFTime = 10 * (MakeRandom(1, BACKOFF_MAX));     // backOFF 10ms
    DelayMs(backOFFTime);
    return DR;
}

#elif defined CSMA
static uint8_t CSMAProcess( void )
{
    MaxChanges = 2;
    uint8_t tmp = 0;
    DR = DRs[MakeRandom( 0, 1 )]; 
    backOFF = MakeRandom(1, BACKOFF_MAX); 
    bool aloha_mode = false;
    while (backOFF > 0 && !aloha_mode) {
        // DIFS: Check channel for DIFS_slots
        bool channelFreeDuringDIFS = true;
        for (uint8_t i = 0; i < 2; i++) {       // DIFS Phase has 2 BackOFF
            tmp = LmHandlerStartCad(DR);
            LmHandlerProcess( );
            if(tmp == 3) {
                continue;
                i --;
            }
            if (tmp) {
                channelFreeDuringDIFS = false;  // Channel is occupied during DIFS, hop to another.
                if(MaxChanges > 0){
                    DR = DRs[MakeRandom( 0, 1 )];
                    MaxChanges--;
                }else{ 
                    aloha_mode = true;
                }
                break;
            }
        }

        if (channelFreeDuringDIFS) {
            while (backOFF > 0){             
                tmp = LmHandlerStartCad(DR);
                LmHandlerProcess( );
                if(tmp == 3) continue;
                if (tmp) {
                    // Channel is busy during CAD, hop to another and return to DIFS state again.
                    if(MaxChanges > 0){
                        DR = DRs[MakeRandom( 0, 1 )];
                        MaxChanges--;
                    }else{
                        aloha_mode = true;
                    }
                    break;  // Exit loop. Go back to DIFS state.
                }
                backOFF--;  // Decrement BO by one if channel is free
            }
        }
    }
    return DR;
}

#elif defined Qlearning
static void ActionSelect(void){
    action = maxAction;

    if(envState == 3){    
        return;
    }

    float E = NVisit[envState];
    float prob = (float)MakeRandom(1,1000)/1000.0f;

    if(envState != 0){
        E *= 10;
    }

    if(prob > (300/E)){
        
        float max = Q_table[envState][0];
        for (int i = 1; i < maxAction; i++) {
            if (Q_table[envState][i] > max) max = Q_table[envState][i];
        }

        uint8_t tempMax[maxAction] = {0};
        uint8_t cnt = 0;
        for (int i = 0; i < maxAction; i++) {
            if (Q_table[envState][i] == max){
                tempMax[cnt] = i;
                cnt += 1;
            }
        }
        
        if(cnt == 1)
            action = tempMax[0];
        else{
            
            action = tempMax[MakeRandom(0,cnt)];
        }

    }else{
        action = MakeRandom(0, maxAction-1);

    }
    NVisit[envState] += 1;
    return;
}

static void UpdateQTable(float reward){
    int flag = action / BACKOFF_MAX;
    if(reward == -1){
        flag = !flag;
    }
    reward = reward * rewardArr[flag];
    float N = NSelect[envState][action];
    Q_table[envState][action] = Q_table[envState][action] + (1/N) * (reward - Q_table[envState][action]);
    NSelect[envState][action] += 1;
    return;
}

static uint8_t QLearningProcess( void ){
    action = maxAction;
    uint8_t localState = 0;
    int flag = MakeRandom( 0, 1 );
    int SFBackoffFlag = 1;           
    DR = DRs[flag];
    int cad = DRs[!flag];
    while(action == maxAction){
        envState = LmHandlerStartCad(cad);
        LmHandlerProcess( );
        ActionSelect();
    }
    localState = envState;
    if(action > (BACKOFF_MAX - 1)){
        DR = DRs[1];
        cad = DRs[0];
        backOFF = (action - (BACKOFF_MAX-1));
    }else{
        DR = DRs[0];
        cad = DRs[1];
        backOFF = action + 1;
    }
    
    while(backOFF != 0){
        localState = LmHandlerStartCad(cad);
        LmHandlerProcess( );
        if(localState == 3){
            sfcnt = 0;
            continue;
        }else if(localState == 1){
            backOFF -= 1;
            
        }else if(localState == 2){
            SFBackoffFlag  = 0; 
            #if PAYLOAD == 30
                sfcnt = 16;
            #elif PAYLOAD == 50
                sfcnt = 25;
            #elif PAYLOAD == 100
                sfcnt = 36;
            #endif
        }else{
            if(SFBackoffFlag){
                backOFF -= 1;
            }else{
                sfcnt -= 1;
                 if(sfcnt == 0){
                    SFBackoffFlag = 1; 
                 }
            }
        }
    }
    return DR;
}

#endif

static void OnTxPeriodicityChanged( uint32_t periodicity )
{
    TxPeriodicity = periodicity;

    if( TxPeriodicity == 0 )
    { // Revert to application default periodicity
        TxPeriodicity = APP_TX_DUTYCYCLE + randr( -APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND );
    }

    // Update timer periodicity
    TimerStop( &TxTimer );
    TimerSetValue( &TxTimer, TxPeriodicity );
    TimerStart( &TxTimer );
}

static void OnTxFrameCtrlChanged( LmHandlerMsgTypes_t isTxConfirmed )
{
    LmHandlerParams.IsTxConfirmed = isTxConfirmed;
}

/*!
 * Function executed on Led 1 Timeout event
 */
static void OnLed1TimerEvent( void* context )
{
    TimerStop( &Led1Timer );
    // Switch LED 1 OFF
    GpioWrite( &Led1, 0 );
}

/*!
 * Function executed on Led 2 Timeout event
 */
static void OnLed2TimerEvent( void* context )
{
    TimerStop( &Led2Timer );
    // Switch LED 2 ON
    GpioWrite( &Led2, 1 );
}