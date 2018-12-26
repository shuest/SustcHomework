#ifndef _NBIOT_H_
#define _NBIOT_H_

#include "stm32f10x.h"

#define MAX_len 128

extern uint8_t CardID[4];

extern uint8_t SwipingCardFlag; 

/*函数声明*/
void NB_AutoConnect(void);
void refreshBuf(uint8_t *buf);
void NB_IOT_YorNCloseScrambling(uint8_t yORn);
uint8_t NB_IOT_ConNet(void);
void regist(void);
void NB_SendData(void);
void NB_ReceiveData(void);
void NB_CloseConnect(void);

uint8_t AT_Test(void);
uint8_t NB_IOT_CloseSocet(void);
uint8_t receive_Mac(void);
uint8_t SetFunc(int function);
uint8_t Scrambling(uint8_t yORn);
uint8_t ReBoot(void);
uint8_t AutoNet(void);
uint8_t SetBand(void);
uint8_t CheckNet(void);
uint8_t NB_IOT_CreatSocket(int port);
uint8_t NB_IOT_SendMsg(uint8_t *ip,int port,int length,uint8_t dataBuf[]);
uint8_t HaveReceive(void);
void NB_IOT_ReceiveMsg(void);

void NB_Init(void);
void SetScrambling(uint8_t Num);
void NB_ReceiveData(void);
void NB_DO(void);
void DeviceJoinNet(void);

void CopyID(uint8_t *buf);
void SendHeartBeatPackage(void);
void SwipingCard(void);
void WarningUpload(void);

void PDAOperationUpload(void);
uint8_t CP_NBReceiveData(uint8_t *string);

/*函数声明*/

#endif