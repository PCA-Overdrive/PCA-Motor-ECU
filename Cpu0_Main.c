/**********************************************************************************************************************
 * \file Cpu0_Main.c
 * \brief Motor ECU Main - CAN RX -> Vehicle UART TX
 *
 * 역할:
 *  - CAN 0x100 VehicleControlCmd 수신
 *  - driveCmd / steeringCmd를 차량 UART 프레임으로 변환
 *  - ASCLIN0 TX(P15.2)로 차량 제어보드에 송신
 *********************************************************************************************************************/

#include "Ifx_Types.h"
#include "IfxAsclin_Asc.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "IfxPort.h"
#include "Ifx_Cfg_Ssw.h"

#include "McmcanFd.h"   /* CAN 송수신 모듈 */
#include "CanMsg.h"     /* VehicleControlCmd_t */

/* ============================================================
   ASCLIN0 UART
   TX : P15.2 / RX : P15.3 / Baudrate : 9600
   ============================================================ */

static IfxAsclin_Asc g_asc;
IFX_ALIGN(4) IfxCpu_syncEvent g_cpuSyncEvent = 0;

static uint8 frame[11];

#define TX_BUFFER_SIZE  (64 + sizeof(Ifx_Fifo) + 8)
#define RX_BUFFER_SIZE  (64 + sizeof(Ifx_Fifo) + 8)

IFX_ALIGN(4) static uint8 g_txBuffer[TX_BUFFER_SIZE];
IFX_ALIGN(4) static uint8 g_rxBuffer[RX_BUFFER_SIZE];

#define ISR_PRIORITY_ASCLIN_TX  10
#define ISR_PRIORITY_ASCLIN_RX  11
#define ISR_PRIORITY_ASCLIN_ER  12

/* ============================================================
   디버그용 변수 - ADS Watch 창에서 확인
   ============================================================ */

volatile uint8 g_dbg_driveCmd    = 0U;
volatile uint8 g_dbg_steeringCmd = 0U;

/* ============================================================
   ASCLIN ISR
   ============================================================ */

IFX_INTERRUPT(asclin0TxISR, 0, ISR_PRIORITY_ASCLIN_TX)
{
    IfxAsclin_Asc_isrTransmit(&g_asc);
}

IFX_INTERRUPT(asclin0RxISR, 0, ISR_PRIORITY_ASCLIN_RX)
{
    IfxAsclin_Asc_isrReceive(&g_asc);
}

IFX_INTERRUPT(asclin0ErISR, 0, ISR_PRIORITY_ASCLIN_ER)
{
    IfxAsclin_Asc_isrError(&g_asc);
}

/* ============================================================
   Private Function Prototypes
   ============================================================ */

static void initUART(void);
static void uartSendBytes(uint8 *data, uint32 len);
static void convertThrottle(uint8 speed, char *out);
static void convertSteering(uint8 steer, char *out);
static void sendCarFrame(uint8 speed, uint8 steer);

/* ============================================================
   UART 초기화
   ============================================================ */

static void initUART(void)
{
    IfxAsclin_Asc_Config ascConfig;

    IfxAsclin_Asc_initModuleConfig(&ascConfig, &MODULE_ASCLIN0);

    ascConfig.baudrate.baudrate  = 9600;
    ascConfig.baudrate.prescaler = 1;

    ascConfig.interrupt.txPriority    = ISR_PRIORITY_ASCLIN_TX;
    ascConfig.interrupt.rxPriority    = ISR_PRIORITY_ASCLIN_RX;
    ascConfig.interrupt.erPriority    = ISR_PRIORITY_ASCLIN_ER;
    ascConfig.interrupt.typeOfService = IfxSrc_Tos_cpu0;

    ascConfig.txBuffer     = g_txBuffer;
    ascConfig.txBufferSize = 64;
    ascConfig.rxBuffer     = g_rxBuffer;
    ascConfig.rxBufferSize = 64;

    const IfxAsclin_Asc_Pins pins =
    {
        NULL,                           IfxPort_InputMode_pullUp,
        &IfxAsclin0_RXB_P15_3_IN,      IfxPort_InputMode_pullUp,
        NULL,                           IfxPort_OutputMode_pushPull,
        &IfxAsclin0_TX_P15_2_OUT,      IfxPort_OutputMode_pushPull,
        IfxPort_PadDriver_cmosAutomotiveSpeed1
    };

    ascConfig.pins = &pins;

    IfxAsclin_Asc_initModule(&g_asc, &ascConfig);
}

/* ============================================================
   UART 바이트 배열 송신
   ============================================================ */

static void uartSendBytes(uint8 *data, uint32 len)
{
    uint32 i;
    for(i = 0U; i < len; i++)
    {
        IfxAsclin_Asc_flushTx(&g_asc, TIME_INFINITE);
        IfxAsclin_Asc_blockingWrite(&g_asc, data[i]);
    }
}

/* ============================================================
   0~255 → Throttle 3자리 ASCII HEX 변환
   중립 = 0x3B3 / 최대 전진 = 0x440 / 최대 후진 = 0x36D
   speed < 127 : 전진 / speed > 127 : 후진 / 127 : 중립
   ============================================================ */

static void convertThrottle(uint8 speed, char *out)
{
    int value;

    if(speed < 127U)
    {
        value = 0x3B3 + ((127 - speed) * (0x440 - 0x3B3)) / 127;
    }
    else if(speed > 127U)
    {
        value = 0x3B3 - ((speed - 127) * (0x3B3 - 0x36D)) / 128;
    }
    else
    {
        value = 0x3B3;
    }

    const char hex[] = "0123456789ABCDEF";
    out[0] = hex[(value >> 8) & 0xF];
    out[1] = hex[(value >> 4) & 0xF];
    out[2] = hex[(value >> 0) & 0xF];
    out[3] = '\0';
}

/* ============================================================
   0~255 → Steering 3자리 ASCII HEX 변환
   중립 = 0x381 / 최대 좌 = 0x253 / 최대 우 = 0x505
   steer < 127 : 좌 / steer > 127 : 우 / 127 : 중립
   ============================================================ */

static void convertSteering(uint8 steer, char *out)
{
    int value;

    if(steer < 127U)
    {
        value = 0x381 - ((127 - steer) * (0x381 - 0x253)) / 127;
    }
    else if(steer > 127U)
    {
        value = 0x381 + ((steer - 127) * (0x505 - 0x381)) / 128;
    }
    else
    {
        value = 0x381;
    }

    const char hex[] = "0123456789ABCDEF";
    out[0] = hex[(value >> 8) & 0xF];
    out[1] = hex[(value >> 4) & 0xF];
    out[2] = hex[(value >> 0) & 0xF];
    out[3] = '\0';
}

/* ============================================================
   차량 UART Frame 생성 및 송신
   Frame: [NULL][STX][T0][T1][T2][S0][S1][S2]['0']['1'][ETX]
   ============================================================ */

static void sendCarFrame(uint8 speed, uint8 steer)
{
    char throttleStr[4];
    char steeringStr[4];

    convertThrottle(speed, throttleStr);
    convertSteering(steer, steeringStr);

    frame[0]  = 0x00;
    frame[1]  = 0x02;
    frame[2]  = throttleStr[0];
    frame[3]  = throttleStr[1];
    frame[4]  = throttleStr[2];
    frame[5]  = steeringStr[0];
    frame[6]  = steeringStr[1];
    frame[7]  = steeringStr[2];
    frame[8]  = '0';
    frame[9]  = '1';
    frame[10] = 0x03;

    uartSendBytes(frame, 11U);
}

/* ============================================================
   MAIN
   ============================================================ */

int core0_main(void)
{
    VehicleControlCmd_t cmd;

    IfxCpu_enableInterrupts();

    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 3);


    /* CAN 초기화 (NODE_MOTOR_ECU 빌드) */
    McmcanFd_Init();

    /* UART 초기화 */
    initUART();

    /* 시작 시 중립 프레임 1회 송신 */
    sendCarFrame(127U, 127U);

    while(1)
    {
        /* CAN 0x100 수신 시 차량 UART 프레임 송신 */
        if(McmcanFd_RecvVehicleControl(&cmd) == TRUE)
        {
            g_dbg_driveCmd    = cmd.driveCmd;    /* ← 디버그 복사 */
            g_dbg_steeringCmd = cmd.steeringCmd;
            sendCarFrame(cmd.driveCmd, cmd.steeringCmd);
        }
    }

    return 0;
}
