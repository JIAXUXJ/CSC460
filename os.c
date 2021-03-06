#include "os.h"
#include <string.h>
#include <avr/io.h>
#include <time.h>
#include <avr/interrupt.h>
#include "active.h"
#include "queue.h"
#include <util/delay.h>


PID   Task_Create_System(void (*f)(void), int arg){
   if (KernelActive ) {
     Disable_Interrupt();
     Cp ->request = CREATE;
     Cp->code = f;
     Cp->arg = arg;
     Cp->taskType = SYSTEM;
     Enter_Kernel();
     return Cp->rtnVal;
   } else { 
      /* call the RTOS function directly */
       unsigned char ttype;
       ttype = SYSTEM;
      PD *P = Kernel_Create_Task( f , arg, SYSTEM);
       enqueue(&system_T, P);
       return P->rtnVal;
   }
}


PID Task_Create_RR(voidfuncptr f, int arg) {
    PD * p = Kernel_Create_Task(f, arg, RR);
    if (p == NULL) return 0;
    p->arg = arg;
    enqueue(&rr_T, p);
    return p->rtnVal;
}


PID Task_Create_Period(voidfuncptr f, int arg, TICK period, TICK wcet, TICK offset){
    PD * p = Kernel_Create_Task(f, arg, PERIODIC);
    if (p == NULL) return 0;
    p->arg = arg;

    p->period = period;
    p->wcet = wcet;
    p->next_start = num_ticks + offset;
    p->ticks_remaining = wcet;

    Enqueue_periodic_offset(&periodic_T, p);

    return p->rtnVal;
}

void Task_Next()
{
    Disable_Interrupt();
    Cp->state = READY;
    Cp->request = NEXT;
    Enter_Kernel();
}


int Task_GetArg() {
    return Cp->arg;
}


PID  Task_Pid(){
    return Cp->rtnVal;
}

struct PD *ProcessOf(PID id)
{
    unsigned int i;

    if(id>0){
        i=(id-1)%MAXTHREAD;
        if(Process[i].state != DEAD)
            return &(Process[i]);
    }
    return NULL;
}


void Msg_Send( PID  id, MTYPE t, unsigned int *v ){

    PD *receiver;
    receiver = ProcessOf(id);

    if(receiver==NULL)return;

    Cp->recipient = receiver;
    if(receiver->state == RECEIVEBLOCK){
        receiver->message->m_val = *v;
        receiver->message->m_type = t;
        Cp->state = REPLYBLOCK;
        enqueue(&(receiver->replies), Cp);
//        TODO
        //suspend
    }else{
        Cp->state = SENDBLOCK;
        Cp->message->m_val = *v;
        Cp->message->m_type = t;
        enqueue(&(receiver->senders), Cp);
        Dispatch();
    }
}


PID  Msg_Recv( MASK m, unsigned int *v ) {
    PD *firstSender;
    firstSender = dequeue(&(Cp->senders));
    if (firstSender == NULL) {
        Cp->state = RECEIVEBLOCK;
        Dispatch();
        return NULL;
    }

    Cp->message->m_id = firstSender->rtnVal;
    Cp->message->m_val = *v;

    if ((m & firstSender->message->m_type) != 0) {
        if (m == "GET") {
            dequeue(&buffer_Q);

            reply(id, Cp->message->m_val);
        } else if (m == "PUT") {
            deposit(Cp->message->m_val, buffer);

            enqueue(buffer_Q, *v);
            reply(id, NULL);
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }

    firstSender->state = REPLYBLOCK;
    enqueue(&(Cp->replies), firstSender);
}



//TODO
void Msg_Rply( PID  id, unsigned int r){
    PD *sender;
    sender = ProcessOf(id);
    if(sender == NULL || sender->state != REPLYBLOCK || sender->recipient == NULL){
        return;
    }
    dequeue(Cp->replies);
    sender->message->m_val = r;
    sender->recipient = NULL;
    sender->state = READY;

//    if(r == 0){
//        Msg_Send
//    }
}

void Msg_ASend( PID  id, MTYPE t, unsigned int v) {
    PD *receiver;
    receiver = ProcessOf(id);

    if(receiver==NULL){
        OS_Abort(-5);
    };

//    Cp->recipient = receiver;
    if(receiver->state == RECEIVEBLOCK){
        receiver->message->m_val = v;
        receiver->message->m_type = t;
//        TODO
        //suspend
    }else{
        Dispatch();
        OS_Abort(-5);
    }
}

void OS_Abort(unsigned int error) {
    //TODO: blink lights to indicate errors
    Disable_Interrupt();

//    UART_print("\nOS Aborted: %d\n", error);
    for(;;) {
        toggle_LED_B7();
        _delay_ms(350);
    }
}

unsigned int Now() {
    unsigned int tmp = num_ticks * MSECPERTICK;
    return tmp + TCNT4/62;
}


