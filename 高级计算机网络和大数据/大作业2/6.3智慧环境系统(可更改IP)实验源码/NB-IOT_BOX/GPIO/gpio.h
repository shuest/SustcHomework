#ifndef _GPIO_H_
#define _GPIO_H_

#include "stm32f10x.h"
#include "stm32f10x_tim.h"

void blink_BLUE(void);
void blink_RED(void);
void GPIO_init(void);
void TIM3_Configuration(uint16_t period,uint16_t prescaler);
void TIM2_Configuration(uint16_t period,uint16_t prescaler);
void Watchdog(void);
void WatchDogInit(void);
void NB_BoxInit(void);
void DisPlay(void);
void CycleCheck(void);

#define RESET_WDI_H()         GPIO_SetBits(GPIOC,GPIO_Pin_1)       
#define RESET_WDI_L()         GPIO_ResetBits(GPIOC,GPIO_Pin_1)

	/*12864 GPIO */
#define LCD12864_PW_EN_H()         GPIO_SetBits(GPIOE,GPIO_Pin_7)
#define LCD12864_PW_EN_L()         GPIO_ResetBits(GPIOE,GPIO_Pin_7)

#define LCD12864_BL_EN_H()         GPIO_SetBits(GPIOE,GPIO_Pin_8)
#define LCD12864_BL_EN_L()         GPIO_ResetBits(GPIOE,GPIO_Pin_8)

#define LCD12864_RST_H()           GPIO_SetBits(GPIOE,GPIO_Pin_9)
#define LCD12864_RST_L()           GPIO_ResetBits(GPIOE,GPIO_Pin_9)

#define LCD12864_RS_CS_H()         GPIO_SetBits(GPIOE,GPIO_Pin_10)       
#define LCD12864_RS_CS_L()         GPIO_ResetBits(GPIOE,GPIO_Pin_10)

#define LCD12864_SDI_H()		      GPIO_SetBits(GPIOE, GPIO_Pin_11)           
#define LCD12864_SDI_L() 		      GPIO_ResetBits(GPIOE, GPIO_Pin_11)

#define LCD12864_SCLK_H()         GPIO_SetBits(GPIOE,GPIO_Pin_12)          
#define LCD12864_SCLK_L()         GPIO_ResetBits(GPIOE,GPIO_Pin_12)

/*UM402 LORA GPIO */
#define LORA_SETA_H()         GPIO_SetBits(GPIOB,GPIO_Pin_7)       
#define LORA_SETA_L()         GPIO_ResetBits(GPIOB,GPIO_Pin_7)

#define LORA_AUX_H()		      GPIO_SetBits(GPIOB, GPIO_Pin_6)           
#define LORA_AUX_L() 		      GPIO_ResetBits(GPIOB, GPIO_Pin_6)

#define LORA_SETB_H()         GPIO_SetBits(GPIOB,GPIO_Pin_5)          
#define LORA_SETB_L()         GPIO_ResetBits(GPIOB,GPIO_Pin_5)

#define LORA_Pwor_H()         GPIO_SetBits(GPIOD,GPIO_Pin_5)          
#define LORA_Pwor_L()         GPIO_ResetBits(GPIOD,GPIO_Pin_5)

	/*按键 GPIO*/	
#define AUTO           GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define MANUAL         GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)
#define LOCK           GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)
#define UNLOCK         GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)

#define LOCK_CHECK     GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)
#define REMOVE_ALARM   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)
#define LIFT_ROD       GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)

#define ModeChoice      		
	
#define LED_BLUE_H()         GPIO_SetBits(GPIOA,GPIO_Pin_6)       
#define LED_BLUE_L()         GPIO_ResetBits(GPIOA,GPIO_Pin_6)

#define LED_RED_H()         GPIO_SetBits(GPIOA,GPIO_Pin_7)       
#define LED_RED_L()         GPIO_ResetBits(GPIOA,GPIO_Pin_7)

#define Read_LED_RED()      GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_7)  

#define Read_BEEP()         GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_4)
	
#define BEEP_H()         GPIO_SetBits(GPIOC,GPIO_Pin_4)       
#define BEEP_L()         GPIO_ResetBits(GPIOC,GPIO_Pin_4)
	
/*复位引脚*/
#define RESET_WDI_H()         GPIO_SetBits(GPIOC,GPIO_Pin_1)       
#define RESET_WDI_L()         GPIO_ResetBits(GPIOC,GPIO_Pin_1)

#define GPRS_ON_H()         GPIO_SetBits(GPIOA,GPIO_Pin_4)          
#define GPRS_ON_L()         GPIO_ResetBits(GPIOA,GPIO_Pin_4)

#define Start_Address                                   (uint16_t)(0x01)
#define ID_Address                                      (uint16_t)(0x20)
#define Ip_Address                                      (uint16_t)(0x40)
#define TCP_Address                                     (uint16_t)(0x50)
#define UDP_Address                                     (uint16_t)(0x60)
#define PsWord_Address                                  (uint16_t)(0x70)
#define GPSCycle_Address                                (uint16_t)(0x80)
#define GPSMode_Address                                 (uint16_t)(0x8a)
#define SensorCycle_Address                             (uint16_t)(0x90)
#define SensorMode_Address                              (uint16_t)(0x9a)
#define HeartBeat_Address                               (uint16_t)(0xa0)
#define Lora_Address                                    (uint16_t)(0xb0)  
#define LockState_Address                               (uint16_t)(0xd0)
#define Operation_Address                               (uint16_t)(0xd5)
#define WarningState_Address                            (uint16_t)(0xe0)
#define WarningCount_Address                            (uint16_t)(0xe1)
#define WarningTime_Address                             (uint16_t)(0xe2)
#define PoleState_Address                               (uint16_t)(0xea)
#define GPRS_YorNFirst                                  (uint16_t)(0xf0)
#define SetNBScrambling_Address                         (uint16_t)(0xf5)

#define PDADataNum_Address                              (uint16_t)(0x0400)
#define PDAData_Address                                 (uint16_t)(0x0401)

#define PDACloseLockDataNum_Address                          (uint16_t)(0x0800)
#define PDACloseLockData_Address                             (uint16_t)(0x0801)

#define PDAOpenLockDataNum_Address                          (uint16_t)(0x0A00)
#define PDAOpenLockData_Address                             (uint16_t)(0x0A01)

//#define LockEquip 1
#define SensorEquip 1
//#define ICCard 1

void ReadParaFromEEprom(void);

extern uint8_t NBIOT;
extern uint8_t GPRS;
extern uint8_t IP_Address[15];                   //IP地址
extern uint8_t TCP_Port[5];                      //TCP端口号
extern uint8_t UDP_Port[5];                      //UDP团口号
extern int UDPPort;                             //UDP端口号
extern uint8_t PDA_Password_ID[10];        //本机PDA密码
extern uint16_t GPS_UploadCycle;                //GPS上传周期
extern uint8_t GPS_WorkMode;                     ///GPS工作模式
extern uint16_t Sensor_UploadCycle;             //传感器上传周期
extern uint8_t Sensor_WorkMode;                  //传感器工作模式
extern uint16_t HeartBeatCycleTime;                //心跳包周期时间
extern uint8_t LockState;                          //锁的当前状态;
extern uint8_t PoleState;                          //杆的当前状态
extern uint32_t OperationNum;                       //操作次数
extern uint8_t WarningState;                       //报警状态
extern uint8_t WainingCount;                       //报警次数
extern uint8_t WarningTime[7];                   //报警时间
extern uint8_t PDADataNum;                         //温湿度信息数量
extern uint8_t USART4_UartReceiving;
extern uint8_t USART4_UartDataFinishFlag;
extern uint16_t USART4_RxCount;
extern uint16_t USART4_ANum;
extern uint8_t USART4_RxBuf[255];
extern void ClearScreen23(void);
extern uint32_t NMEA_Str2num(uint8_t *buf,uint8_t *dx);

#endif

