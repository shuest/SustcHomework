#include "Queue.h"


sp_queue queue_init()
{
    sp_queue q;
    q.front = q.rear = 0;
    return q;
}

int queue_empty(sp_queue q)
{
    return q.front == q.rear;
}

int queue_en(sp_queue *q, uint8_t* e,uint8_t len)
{
    uint8_t i = 0;
    /* 队满 */
    if (q -> rear == MAX_QUEUE_SIZE)
        return false;
    for(i = 0; i < len;i++)
    {
      /* 入队 */
      q -> sp_queue_array[q -> rear] = *e;
      q -> rear++;
      e++;
      if(q -> rear == MAX_QUEUE_SIZE)
        q -> rear = 0;
    }
    
    return true;
}

int queue_de(sp_queue *q, uint8_t *e,uint8_t len)
{
    uint8_t i = 0;
    /* 队空 */
    if(queue_empty(*q))
        return false;
    for(i = 0; i < len;i++)
    {
      /* 出队 */    
      *e = q -> sp_queue_array[q -> front];
      q -> front++;
      e++;
      if(q -> front == MAX_QUEUE_SIZE)
        q -> front = 0;
    }
    return true;
}

void queue_clear(sp_queue *q)
{
    q -> front = q -> rear = 0;
}

int get_front(sp_queue q, uint8_t *e)
{
    /* 队空 */
    if(q.front == q.rear)
        return false;

    /* 获取队头元素 */
    *e = q.sp_queue_array[q.front];
    return true;
}

//获取队长
int queue_len(sp_queue q)
{
    return (q.rear - q.front);
}

//遍历队
void queue_traverse(sp_queue q, void (*visit)(sp_queue q))
{
    visit(q);
}

void visit(sp_queue q)
{
    /* 队空 */
    if (q.front == q.rear)
      return;

    int temp = q.front;
    while(temp != q.rear)
    {
        temp += 1;
    }
}



//int main()
//{
//    sp_queue q = queue_init();
//    queue_en(&q, 1);
//    queue_en(&q, 2);
//    printf("length=%d\n", queue_len(q));
//    queue_en(&q, 3);
//    printf("length=%d\n", queue_len(q));
//    queue_en(&q, 4);
//    printf("length=%d\n", queue_len(q));
//    queue_en(&q, 5);
//    printf("length=%d\n", queue_len(q));
//    queue_en(&q, 6);
//    printf("length=%d\n", queue_len(q));
//    queue_traverse(q,visit);
//    datatype *e = (datatype *)malloc(sizeof(*e));
//    queue_de(&q,e);
//    printf("queue_de(),e=%d length=%d\n", *e, queue_len(q));
//    queue_traverse(q, visit);
//    queue_clear(&q);
//    queue_traverse(q, visit);
//    printf("length:%d\n", queue_len(q));
//}