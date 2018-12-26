#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "stdio.h"
#include "stm32f10x.h"
#include "stdlib.h"

/* ˳����нӿڶ���ͷ�ļ�*/
#define true 1
#define false 0


/* �ӵ���󳤶� */
#define MAX_QUEUE_SIZE 300

/* ��̬�������ݽṹ */
typedef struct queue{
    uint8_t sp_queue_array[MAX_QUEUE_SIZE];
    /* ��ͷ */
    int front;
    /* ��β */
    int rear;
}sp_queue;


/* ��̬˳�����Ľӿڶ��� */


/* ��̬���ĳ�ʼ�� */
sp_queue queue_init();

/* �ж϶����Ƿ�Ϊ��,��Ϊ��
 * ����true
 * ���򷵻�false
*/
int queue_empty(sp_queue q);


/* ����Ԫ��eΪ��q�Ķ�β��Ԫ�� 
 * ����ɹ�����true
 * ��������false
*/
int queue_en(sp_queue *q, uint8_t* e,uint8_t len);


/* ��ͷԪ�س���
 * ��e���س���Ԫ��,������true
 * ���ӿշ���false
*/
int queue_de(sp_queue *q, uint8_t *e,uint8_t len);

/* ��ն� */
void queue_clear(sp_queue *q);


/* ��ö�ͷԪ��
 * ���зǿ�,��e���ض�ͷԪ��,������true
 * ���򷵻�false
*/
int get_front(sp_queue, uint8_t *e );


/* ��öӳ� */
int queue_len(sp_queue q);

/* ������ */
void queue_traverse(sp_queue q, void(*visit)(sp_queue q));


void visit(sp_queue s);

#endif
