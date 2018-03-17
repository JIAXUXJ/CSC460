#include <string.h>
#include <avr/io.h>
#include <avr/delay.h>
#include <time.h>
#include <avr/interrupt.h>

#include "active.h"
#include "queue.h"

/**
 * When creating a new task, it is important to initialize its stack just like
 * it has called "Enter_Kernel()"; so that when we switch to it later, we
 * can just restore its execution context on its stack.
 * (See file "cswitch.S" for details.)
 */

void setupTimer() {
    //Clear timer config.
    TCCR4A = 0;
    TCCR4B = 0;
    //Set to CTC (mode 4)
    TCCR4B |= (1<<WGM42);

    //Set prescaller to 256
    TCCR4B |= (1<<CS42);

    //Set TOP value (0.01 seconds)
    OCR4A = 625;

    //Enable interupt A for timer 3.
    TIMSK4 |= (1<<OCIE4A);

    //Set timer to 0 (optional here).
    TCNT4 = 0;

    Enable_Interrupt();
}

ISR(TIMER4_COMPA_vect)
{
    num_ticks++;
    Cp->request = TIMER;
    Enter_Kernel();
}

void idle_task() {
    for(;;) {};
}

extern void Enter_Kernel();

#define Disable_Interrupt()   asm volatile ("cli"::)
#define Enable_Interrupt()    asm volatile ("sei"::)

void Kernel_Create_Task_At( PD *p, voidfuncptr f , int arg, PRIORITY_LEVELS ttype)
{   
   unsigned char *sp;

// #ifdef DEBUG
//    int counter = 0;
// #endif

   //Changed -2 to -1 to fix off by one error.
   sp = (unsigned char *) &(p->workSpace[WORKSPACE-1]);

   /*----BEGIN of NEW CODE----*/
   //Initialize the workspace (i.e., stack) and PD here!

   //Clear the contents of the workspace
   memset(&(p->workSpace),0,WORKSPACE);

   //Notice that we are placing the address (16-bit) of the functions
   //onto the stack in reverse byte order (least significant first, followed
   //by most significant).  This is because the "return" assembly instructions 
   //(rtn and rti) pop addresses off in BIG ENDIAN (most sig. first, least sig. 
   //second), even though the AT90 is LITTLE ENDIAN machine.

   //Store terminate at the bottom of stack to protect against stack underrun.
   *(unsigned char *)sp-- = ((unsigned int)Task_Terminate) & 0xff;
   *(unsigned char *)sp-- = (((unsigned int)Task_Terminate) >> 8) & 0xff;
   *(unsigned char *)sp-- = 0x00;

   //Place return address of function at bottom of stack
   *(unsigned char *)sp-- = ((unsigned int)f) & 0xff;
   *(unsigned char *)sp-- = (((unsigned int)f) >> 8) & 0xff;
   *(unsigned char *)sp-- = 0x00;

// #ifdef DEBUG
//    //Fill stack with initial values for development debugging
//    //Registers 0 -> 31 and the status register
//    for (counter = 0; counter < 33; counter++)
//    {
//       *(unsigned char *)sp-- = counter;
//    }
// #else
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

   if (Tasks == MAXTHREAD) return;  /* Too many task! */

   /* find a DEAD PD that we can use  */
   for (x = 0; x < MAXTHREAD; x++) {
       if (Process[x].state == DEAD) break;
   }


    ++Tasks;
   Kernel_Create_Task_At( &(Process[x]), f,  arg, ttype);

    PD *p = &(Process[x]);
//    TODO enqueue
//    enqueue(&(Process[x]), f,  arg, ttype);
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




static void Dispatch()
{
    /* find the next READY task
      * Note: if there is no READY task, then this will loop forever!.
      */


//TODO: Handle BLOCKED better
    if (Cp->state != RUNNING ) {
        if (system_T.head && peek(&system_T)->state != RECEIVEBLOCK
                && peek(&system_T)->state != SENDBLOCK
                && peek(&system_T)->state != REPLYBLOCK) {
            Cp = peek(&system_T);
        } else if (periodic_T.len > 0 && num_ticks >= peek(&periodic_T)->next_start) {
            PD* p = peek(&periodic_T);
            // Check for overlap with another periodic task
            /* if (p->next != NULL && num_ticks >= p->next->next_start) { */
            /* 	OS_Abort(-2); */
            /* } */

            Cp = p;
        } else if (rr_T.head) {
            while (peek(&rr_T)->state != RECEIVEBLOCK
                   && peek(&rr_T)->state != SENDBLOCK
                   && peek(&rr_T)->state != REPLYBLOCK) {
                enqueue(&rr_T, dequeue(&rr_T));
            }
            Cp = peek(&rr_T);
        }
    }

    CurrentSp = Cp->sp;
    Cp->state = RUNNING;
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
    Tasks = 0;
    KernelActive = FALSE;
    //Reminder: Clear the memory for the task on creation.
    int x;
    for (x = 0; x < MAXTHREAD +1; x++) {
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
        /* we may have to initialize the interrupt vector for Enter_Kernel() here. */

        /* here we go...  */
        KernelActive = TRUE;
        setupTimer();
        for (;;) {
            Next_Kernel_Request();
        }
        /* NEVER RETURNS!!! */
    }
}


/**
  * For this example, we only support cooperatively multitasking, i.e.,
  * each task gives up its share of the processor voluntarily by calling
  * Task_Next().
  */
void Task_Create( voidfuncptr f)
{
   if (KernelActive ) {
     Disable_Interrupt();
     Cp ->request = CREATE;
     Cp->code = f;
     Enter_Kernel();
   } else { 
      /* call the RTOS function directly */
      Kernel_Create_Task( f );
   }
}

/**
  * The calling task gives up its share of the processor voluntarily.
  */
//void Task_Next()
//{
//   if (KernelActive) {
//     Disable_Interrupt();
//     Cp ->request = NEXT;
//     Enter_Kernel();
//  }
//}


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

/*============
  * A Simple Test 
  *============
  */

/**
  * A cooperative "Ping" task.
  * Added testing code for LEDs.
  */
void Ping() 
{
  // int  x ;
  // init_LED_D5();
  for(;;){
    //LED on
  // enable_LED(LED_D5_GREEN);

 //    for( x=0; x < 32000; ++x );   /* do nothing */
  // for( x=0; x < 32000; ++x );   /* do nothing */
  // for( x=0; x < 32000; ++x );   /* do nothing */

  //LED off
  // disable_LEDs();  
    
    /* printf( "*" );  */
    PORTB &= ~(1 << PORTB6);
    _delay_ms(500);
    PORTB |= 1 << PORTB6;
    Task_Next();
  }
}


/**
  * A cooperative "Pong" task.
  * Added testing code for LEDs.
  */
void Pong() 
{
  // int  x;
  // init_LED_D2();
  for(;;) {
  //LED on
  // enable_LED(LED_D2_GREEN);


 //    for( x=0; x < 32000; ++x );   /* do nothing */
  // for( x=0; x < 32000; ++x );   /* do nothing */
  // for( x=0; x < 32000; ++x );   /* do nothing */

  //LED off
  // disable_LEDs();

    /* printf( "." );  */
    PORTB &= ~(1 << PORTB7);
    _delay_ms(500);
    PORTB |= 1 << PORTB7;
    Task_Next();
  
  }
}


/**
  * This function creates two cooperative tasks, "Ping" and "Pong". Both
  * will run forever.
  */
void main() 
{
  DDRB |= 1 << DDB6;
  DDRB |= 1 << DDB7;
  OS_Init();
  Task_Create( Pong );
  Task_Create( Ping );
  OS_Start();
}
