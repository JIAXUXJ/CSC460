#include <string.h>
#include <avr/io.h>
#include <avr/delay.h>
#include <time.h>
#include <avr/interrupt.h>
#include "active.h"
#include "avr_console.h"
#include "os.h"
#include "basic_test.h"

extern void a_main();


/**
 * When creating a new task, it is important to initialize its stack just like
 * it has called "Enter_Kernel()"; so that when we switch to it later, we
 * can just restore its execution context on its stack.
 * (See file "cswitch.S" for details.)
 */

void setup_Timer(){
    //set up timer with prescaler = 64 and CTC mode
    TCCR1B |= (1 << WGM12)|(1 << CS11)|(1 << CS10);

    // initialize counter
    TCNT1 = 0;

    // initialize compare value, required delay = 10ms
    OCR1A = 2499;

    // enable compare interrupt
    TIMSK1 |= (1 << OCIE1A);

    // enable global interrupts
    sei();
}
/************************************************************************/
// Increment the timer value every 1 ms
ISR(TIMER1_COMPA_vect)
{
    // toggle led here
    // PORTC ^= (1 << 0);
    DDRB |= 1 << DDB4;
    PORTB |= 1 << PORTB4;
    //PORTC ^= (1 << DDC6);
    Task_Next();
    PORTB &= ~(1 << PORTB4);
}

void enqueue(task_queue_t * list, PD * task) {

    if (list->len == 0) {
        printf("hello\n");
        printf("length is %02x\n" + (list->len)&0xFF);
        list->head = list->tail = task;
        printf("length is %u\n" + list->len);
        printf("got here enqueue2\n");
    } else {
        printf(list->len);
        printf("got here enqueue1\n");
        PD * temp;
        temp = list->tail;
        temp->next = task;
        list->tail = temp;
    }
    list->tail = task;
    task->next = NULL;
    printf(list);
    printf("got here enqueue3\n");
    list->len++;
    printf(list->len);
}
PD * dequeue(task_queue_t * list) {
    if (list->len == 0) {
        return NULL;
    } else if (list->len == 1) {
        PD * p = list->head;
        list->head = list->tail = NULL;
        list->len = 0;
        return p;
    } else {
        PD * p = list->head;
        list->head = p->next;
        list->len--;
        return p;
    }
}
void Enqueue_periodic_offset(task_queue_t * list, PD * task) {
    if (list->len == 0) {
        list->head = list->tail = task;
    } else {
        PD * p = list->head;
        PD * p_prev = NULL;
        while (p != NULL && p->next_start < task->next_start) {
            p_prev = p;
            p = p->next;
        }

        if (p_prev == NULL) {
            // Insert as first element in list
            list->head = task;
            task->next = p;
        } else if (p == NULL) {
            // Insert as last element in list
            list->tail = task;
            p_prev->next = task;
            task->next = NULL;
        } else {
            // Somewhere in the middle, insert between p_prev and p
            p_prev->next = task;
            task->next = p;
        }
    }
    list->len++;
}

PD * peek(task_queue_t * list) {
    return list->head;
}

void queue_init(task_queue_t * list) {
    list->head = list->tail = NULL;
    list->len = 0;
}
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
        printf("create a task here\n");
        /* call the RTOS function directly */
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
void Kernel_Create_Task_At( PD *p, voidfuncptr f , int arg, PRIORITY_LEVELS ttype, int x)
{
    unsigned char *sp;

    //Changed -2 to -1 to fix off by one error.
    sp = (unsigned char *) &(p->workSpace[WORKSPACE-1]);

    /*----BEGIN of NEW CODE----*/
    memset(&(p->workSpace),0,WORKSPACE);
    *(unsigned char *)sp-- = ((unsigned int)Task_Terminate) & 0xff;
    *(unsigned char *)sp-- = (((unsigned int)Task_Terminate) >> 8) & 0xff;
    *(unsigned char *)sp-- = 0x00;

    //Place return address of function at bottom of stack
    *(unsigned char *)sp-- = ((unsigned int)f) & 0xff;
    *(unsigned char *)sp-- = (((unsigned int)f) >> 8) & 0xff;
    *(unsigned char *)sp-- = 0x00;

    //Place stack pointer at top of stack
    sp = sp - 34;
// #endif

    p->sp = sp;   /* stack pointer into the "workSpace" */
    p->code = f;    /* function to be executed as a task */
    p->request = NONE;
    p->arg = arg;
    p->taskType = ttype;
//    p->rtnVal = 0;
    p->rtnVal = x+1;
    p->ticks_remaining = 0;
    /*----END of NEW CODE----*/

    p->state = READY;

}


/**
  *  Create a new task
  */
PD * Kernel_Create_Task( voidfuncptr f, int arg, PRIORITY_LEVELS ttype)
{
    int x;

    if (Tasks == MAXTHREAD) return NULL;  /* Too many task! */

    /* find a DEAD PD that we can use  */
    for (x = 0; x < MAXTHREAD; x++) {
        if (Process[x].state == DEAD) break;
    }

    ++Tasks;
    Kernel_Create_Task_At( &(Process[x]), f,  arg, ttype, x);

    PD *p = &(Process[x]);
    return p;

}


/**
  * This internal kernel function is a part of the "scheduler". It chooses the 
  * next task to run, i.e., Cp.
  */
static void Dispatch()
{
    /* find the next READY task
      * Note: if there is no READY task, then this will loop forever!.
      */
    while(Process[NextP].state != READY) {
        NextP = (NextP + 1) % MAXTHREAD;
    }

    Cp = &(Process[NextP]);
    CurrentSp = Cp->sp;
    Cp->state = RUNNING;

    NextP = (NextP + 1) % MAXTHREAD;
}

/**
  * This internal kernel function is the "main" driving loop of this full-served
  * model architecture. Basically, on OS_Start(), the kernel repeatedly
  * requests the next user task's next system call and then invokes the
  * corresponding kernel function on its behalf.
  *
  * This is the main loop of our kernel, called by OS_Start().
  */
static void Next_Kernel_Request()
{
    Dispatch();  /* select a new task to run */

    while(1) {
        Cp->request = NONE; /* clear its request */

        /* activate this newly selected task */
        CurrentSp = Cp->sp;
        Exit_Kernel();    /* or CSwitch() */

        /* if this task makes a system call, it will return to here! */

        /* save the Cp's stack pointer */
        Cp->sp = CurrentSp;

        switch(Cp->request){

//       case CREATE:
//           Kernel_Create_Task( Cp->code );
//           break;

            case NEXT:

                switch (Cp->taskType) {
                    case SYSTEM:
                        enqueue(&system_T, dequeue(&system_T));
                        break;
                    case PERIODIC:
                        dequeue(&periodic_T);
                        Cp->next_start = Cp->next_start + Cp->period;
                        Cp->ticks_remaining = Cp->wcet;
                        Enqueue_periodic_offset(&periodic_T, (PD *) Cp);

                        break;
                    case RR:
                        Cp->ticks_remaining = 1;
                        enqueue(&rr_T, dequeue(&rr_T));
                        break;
                }
                if (Cp->state != RECEIVEBLOCK && Cp->state != SENDBLOCK && Cp->state != REPLYBLOCK) Cp->state = READY;
                Dispatch();
                break;


            case NONE:
                /* NONE could be caused by a timer interrupt */
                if (Cp->state != RECEIVEBLOCK && Cp->state != SENDBLOCK && Cp->state != REPLYBLOCK) Cp->state = READY;
                Dispatch();
                break;

            case TIMER:
                switch (Cp->taskType) {
                    case SYSTEM: // drop down
                        break;
                    case PERIODIC: // drop down
                        Cp->ticks_remaining--;
                        if (Cp->ticks_remaining <= 0) {
                            OS_Abort(-1);
                        }
                        break;
                    case RR:
                        Cp->ticks_remaining--;
                        if (Cp->ticks_remaining <= 0) {
                            // Reset ticks and move to back
                            Cp->ticks_remaining = 1;
                            enqueue(&rr_T, dequeue(&rr_T));
                        }
                        break;
                }
                if (Cp->state != RECEIVEBLOCK && Cp->state != SENDBLOCK && Cp->state != REPLYBLOCK) Cp->state = READY;
                Dispatch();
                break;

            case TERMINATE:
                /* deallocate all resources used by this task */
                switch (Cp->taskType) {
                    case SYSTEM:
                        dequeue(&system_T);
                        break;
                    case PERIODIC:
                        dequeue(&periodic_T);
                        break;
                    case RR:
                        dequeue(&rr_T);
                        break;
                }
                Cp->state = DEAD;
                Dispatch();
                break;

            default:
                /* Houston! we have a problem here! */
                break;
        }
    }
}


/*================
  * RTOS  API  and Stubs
  *================
  */

/**
  * This function initializes the RTOS and must be called before any other
  * system calls.
  */
void OS_Init()
{
    int x;

    Tasks = 0;
    KernelActive = 0;
    NextP = 0;
    //Reminder: Clear the memory for the task on creation.
    for (x = 0; x < MAXTHREAD; x++) {
        memset(&(Process[x]),0,sizeof(PD));
        Process[x].state = DEAD;
    }
    queue_init(&system_T);
    queue_init(&periodic_T);
    queue_init(&rr_T);
}


/**
  * This function starts the RTOS after creating a few tasks.
  */
void OS_Start()
{
    if ( (! KernelActive) && (Tasks > 0)) {
        Disable_Interrupt();
        /* we may have to initialize the interrupt vector for Enter_Kernel() here. */

        /* here we go...  */
        KernelActive = 1;
//        Next_Kernel_Request();
        /* NEVER RETURNS!!! */
    }
}


/**
  * For this example, we only support cooperatively multitasking, i.e.,
  * each task gives up its share of the processor voluntarily by calling
  * Task_Next().
  */

/**
  * The calling task gives up its share of the processor voluntarily.
  */
void Task_Next()
{
    if (KernelActive) {
        Disable_Interrupt();
        Cp ->request = NEXT;
        Enter_Kernel();
    }
}


/**
  * The calling task terminates itself.
  */
void Task_Terminate()
{
    if (KernelActive) {
        Disable_Interrupt();
        Cp -> request = TERMINATE;
        Enter_Kernel();
        /* never returns here! */
    }
}

void main()
{
    uart_init();
    stdout = &uart_output;
    stdin = &uart_input;
    DDRB |= 1 << DDB6;
    DDRB |= 1 << DDB7;
    OS_Init();
    Task_Create_System(a_main, 0);
    OS_Start();
}

