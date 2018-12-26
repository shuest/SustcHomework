#include "usart.h"
#include "NB-IOT.h"

char receive_buf[MAX_len] = {0};
char send_buf[MAX_len] = {0};
int len = 0;
char IOT_ReceiveBuf[MAX_len] = 0;
char MAC_address[MAX_len] = {0};

void refreshBuf()
{
  int i = 0;
  for(i = 0;i <= len;i++)
  {
      receive_buf[i] = 0;
  }
}

void UART_Receive()
{
  char c_flag = 0;
  int lent = 0;
  int i = 0;
  lent = MAX_len;
  for(i = 0;i < lent;i++)
  {
    scanf("%c",&receive_buf[i]);
    if(receive_buf[i] == '\r')
    {
      if(c_flag > 0)
      {
          lent = i;
      }
      c_flag++;
    }
  }
  len = lent;
}

void UART_Receive2()
{
  char c_flag = 0;
  int lent = 0;
  int i = 0;
  lent = MAX_len;
  for(i = 0;i < lent;i++)
  {
    scanf("%c",&receive_buf[i]);
    if(receive_buf[i] == '\r')
    {
      if(c_flag > 2)
      {
          lent = i;
      }
      c_flag++;
    }
  }
  len = lent;
}

void UART_Receive3()
{
  char c_flag = 0;
  int lent = 0;
  int i = 0;
  lent = MAX_len;
  for(i = 0;i < lent;i++)
  {
    scanf("%c",&receive_buf[i]);
    if(receive_buf[i] == '\r')
    {
      if(c_flag > 2)
      {
          lent = i;
      }
      c_flag++;
    }
  }
  len = lent;
}

void UART_ReceivePoint()
{
  int lent = 0;
  int i = 0;
  lent = MAX_len;
  for(i = 0;i < lent;i++)
  {
    scanf("%c",&receive_buf[i]);
    if(receive_buf[i] == ',')
    {
        lent = i + 3;
    }
  }
  len = lent;
}

int NB_IOT_ConNet()
{
  printf("AT+NCONFIG?\r\n");                                    //����UE(�û�����)����״��   ��ȷ����TURE ���󷵻�FALSE
  UART_Receive();
  
  if(memcmp("TRUE",&receive_buf[len - 4],4) == 0)               
  {
      refreshBuf();
      printf("AT+NBAND?\r\n");                                  //����֧�ֵ�Ƶ�� Ŀǰֻ֧����������   5��850 MHz��  8(900 MHx)   20(800 MHz)      ��ȷ���ص�ǰ��Ƶ��
      UART_Receive();
      
      if(memcmp("5",&receive_buf[len - 1],1) == 0)
      {
          refreshBuf();
          printf("AT+CFUN=1\r\n");                              //����ģ�鹦�� 0��С����   1 ȫ���� 2 3 4 ����129�ο��ֲ�  ��ȷ����OK
          UART_Receive();
          printf("AT+CIMI\r\n");                                //�����ƶ�̨�豸ʶ������ ��ȷ���� ID
          UART_Receive();
          refreshBuf();
          printf("AT+CSQ\r\n");                                 //��ȡ�ź�ǿ�� ��һ������Ϊ�ź�ǿ��  �ڶ������� Ŀǰһֱ��99
          UART_ReceivePoint();
          if((receive_buf[len - 4] == 0x39) && (receive_buf[len - 5] == 0x39))          //���ź�
          {
              return 0;                                           
          }
          else
          {
              refreshBuf();
              printf("AT+CGDCONT=1,\"IP\",\"HUAWEI.COM\"\r\n");       //����PDP������  ����2 Ŀǰֻ֧��IP��IPV4��   ������ APN ͨ��APN��ȷ��Ҫ�����Ǹ�����
              UART_Receive();
              printf("AT+CGATT=1\r\n");                         //���ͻ����GPRSҵ��  0 ����  1 ����
              UART_Receive();
              printf("AT+COPS=1,2,\"46011\"\r\n");                //ѡ����Ӫ�� ����1 ģʽѡ�� 0 �Զ� 1�ֶ� 2������ע����Ŀǰֻ֧��������ģʽ��  ����2 ��ʽ  2 ���֣�ֻ֧��2��  ����3 GSM������
              UART_Receive();
              printf("AT+CGATT?\r\n");                          //���ͻ����GPRSҵ��  0 ����  1 ����
              UART_Receive();
              if(receive_buf[len - 1] == 0x31)                  //����ֵ 1 �����ɹ�  0 ����ʧ��
                return 1;
              else
                return 0;
          }
      }
      else
      {
          refreshBuf();
          printf("AT+NBAND=5\r\n");                             //��ȷ����OK
          UART_Receive();
      }
  }
  else
  {
      refreshBuf();
      printf("AT+NCONFIG=AUTOCONNECT,TRUE\r\n");                //��ȷ����OK
      UART_Receive();
  }
}

int NB_IOT_CreatSocket(int port)
{
      int Port = 0;
      
      Port = port;
      refreshBuf();
      printf("AT+NSOCR=DGRAM,17,%d,1\r\n",Port);            //����һ��Socket  ����1 Sockst���� Ŀǰֻ֧��DGRAM   ����2 ���ӷ�ʽ UDP=17 Ŀǰֻ֧��UDP  ����3 �˿ں�   ����4  һ������ȥ����һ��Socket 
      UART_Receive();
      return 1;
}

int NB_IOT_SendMsg(char *ip,int port,int length,char dataBuf[])
{       
      char IP[15] = {0};
      int Port = 0,Length = 0,i = 0;
      char DataBuf[MAX_len] = {0};
      char test[MAX_len] = {0};
      char high = 0;
      char low = 0;
      
      while(*ip != 0)
      {
          IP[i] = *ip;
          i++;
          ip++;
      }
      Port = port;
      Length = length;
      
      for(i = 0;i < Length;i++)
      {
          test[i] = dataBuf[i];
      }
      
      for(i = 0;i < Length;i++)
      {
          high = test[i] >> 4;
          low = test[i] & 0x0f;
          DataBuf[2 * i] = high + 0x30;
          DataBuf[2 * i + 1] = low + 0x30;
      }    
      
      refreshBuf();
      //printf("AT+NSOST=0,139.196.218.156,5050,1,BB\r\n");//����UDP���� ����1 AT+NSOCR�ķ���ֵ  ����2 ��������IP��ַ(ֻ֧��IPV4)  ����3 �˿ں� ����4 �������ݳ���(���Ϊ512 byte)  ����5 ��������(ֻ֧��16����)   ����ֵΪ�ɹ�������ֽ���
      printf("AT+NSOST=0,%s,%d,%d,%s\r\n",IP,Port,Length,DataBuf);
      UART_Receive3();
      if((memcmp("OK",&receive_buf[len - 2],2) == 0) || (receive_buf[len - 1] == Length))
      {
          return 1;
      }
      else
      {
          return 0;
      }
        
}

int HaveReceive(int length)
{
  refreshBuf();
  UART_Receive();
  if((memcmp("NSONMI",&receive_buf[len - 10],6) == 0) || (memcmp("NSONMI",&receive_buf[len - 11],6) == 0))
    return 1;
  else
    return 0;
}

int NB_IOT_ReceiveMsg(int length)
{
      int Length = 0,i = 0;
      char DataBuf[MAX_len] = {0};
      char high = 0;
      char low = 0;
      
      Length = length;
      
      refreshBuf();
      printf("AT+NSORF=0,128\r\n");                      //��������  ����1 AT+NSOCR�ķ���ֵ ����2 �����յ÷��س��ȣ�10���ƣ�  ����ֵΪ ���ӵ�IP��ַ �˿ں� ���ݳ��� ��������
      UART_Receive3();
      if(memcmp("OK",&receive_buf[len - 2],2) == 0)            
      {
         
          for(i = 0;i < (2 * length);i++)
          {
              DataBuf[i] = receive_buf[len - 8 - (2 * Length) + i];
          }
          
          for(i = 0;i < Length;i++)
          {          
              high = DataBuf[2 * i] - 0x30;
              low = DataBuf[2 * i + 1] - 0x30;
              
              IOT_ReceiveBuf[i] = (high << 4) + low;
          }
      
          return 1;             
      }
      else
      {
          return 0;
      }
}

int NB_IOT_CloseSocet()
{
      refreshBuf();
      printf("AT+NSOCL=0\r\n");                         //�ر�Sockst
      UART_Receive();
      if(memcmp("OK",&receive_buf[len - 2],2) == 0)
        return 1;
      else
        return 0;
}

int receive_Mac()
{
      int i = 0;
      refreshBuf();
      printf("AT+CGSN=1\r\n");                         //��ȡ��Ʒ���к�
      UART_Receive3();
      if(memcmp("OK",&receive_buf[len - 2],2) == 0)
      {
          for(i = 0;i < 15;i++)
          {
              dataBuf[i + 3] = receive_buf[len - 21 + i];
              MAC_address[i] = receive_buf[len - 21 + i];
          }
          dataBuf[i] = 0x00;
          MAC_address[i] = 0x00;
          return 1;
      }
      else
        return 0;
}

void Do_option()
{
    int i = 0;
    
    RX_Flag = 0;
    UART_Receive();
    if(receive_buf[0] == 0xfc)
    {
        send_buf[0] = 0xfe;
        for(i = 0;i < (receive_buf[1] + 1);i++)
        {
            send_buf[i + 1] = receive_buf[i + 1];
        }
        switch(receive_buf[2])
        {
          case 0x02:                                     //�ɼ��ڲ���ʪ������
          {
             // if(memcmp(MAC_address,&receive_buf[len - 21],16) == 0)
            if(receive_buf[21] == 0x00)
            {
                delay_time = 0;
            }
            else if(receive_buf[21] == 0x01)
            {
                delay_time = (receive_buf[22] << 8) + receive_buf[23];
            }
              break;
          }
          case 0x03:                                    //�ڲ�����������
          {
              
              break;
          }
          case 0x04:                                    //�ڲ��������        
          {
       //       delay_s(delay_time);
              send_buf[24] = 0;
              send_buf[25] = 0;
              /*��·����*/  
              if(((receive_buf[24] & 0x01) == 0x01) && (((receive_buf[25] & 0x01)) == 0x01))
              {
                  GPIO_SetBits(GPIOB,GPIO_Pin_8);
              }
              else if(((receive_buf[24] & 0x01) == 0x01) && (((receive_buf[25] & 0x01)) == 0x00))
              {
                  GPIO_ResetBits(GPIOB,GPIO_Pin_8);
              }
              
              if(((receive_buf[24] & 0x02) == 0x02) && (((receive_buf[25] & 0x02)) == 0x02))
              {
                  GPIO_SetBits(GPIOB,GPIO_Pin_9);
              }
              else if(((receive_buf[24] & 0x02) == 0x02) && (((receive_buf[25] & 0x02)) == 0x00))
              {
                  GPIO_SetBits(GPIOB,GPIO_Pin_9);
              }
              if(((receive_buf[24] & 0x04 ) == 0x04) && (((receive_buf[25] & 0x04)) == 0x04))
              {
                  GPIO_SetBits(GPIOC,GPIO_Pin_13);
              }
              else if(((receive_buf[24] & 0x04 ) == 0x04) && (((receive_buf[25] & 0x04)) == 0x00))
              {
                  GPIO_SetBits(GPIOC,GPIO_Pin_13);
              }
              /*��·����*/  
              
              send_buf[24] = 0x07;
              
              /*��������״̬*/
              if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_8))
                  send_buf[25] |= 0x01;
              else
                  send_buf[25] &= 0xfe;
              if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_9))
                  send_buf[25] |= 0x02;
              else
                  send_buf[25] &= 0xfd;
              if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_13))
                  send_buf[25] |= 0x04;
              else
                  send_buf[25] &= 0xfb;
              /*��������״̬*/  
              
 //           NB_IOT_SendMsg(ip,port,(receive_buf[1]+2),send_buf);
              
              break;
          }
          case 0x10:                                    //modbus�豸
          {
              break;
          }
          default:
              break;
        }
    }
    else
      return;
}

void regist()                                           //��ʼ���ӷ���������ע��
{
    int i = 0;
    send_buf[0] = 0xfe;
    send_buf[1] = 0x16;
    send_buf[2] = 0x01;
    for(i = 0;i < 16;i++)
    {
      send_buf[i + 2] = MAC_address[i];
    }
    send_buf[19] = 0x01;
    send_buf[20] = 0x01;
    send_buf[21] = 0x01;
    send_buf[22] = 0x01;
    send_buf[23] = 0x01;
    
    NB_IOT_SendMsg(ip,port,(receive_buf[1]+2),send_buf);
}