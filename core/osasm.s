;/*****************************************************************************/
; OSasm.s: low-level OS commands, written in assembly                       */
; Runs on LM4F120/TM4C123/MSP432
; A very simple real time operating system with minimal features.
; Daniel Valvano
; February 8, 2016
;
; This example accompanies the book
;   "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
;   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2016
;   "Embedded Systems: Real-Time Operating Systems for ARM Cortex-M Microcontrollers",
;   ISBN: 978-1466468863, , Jonathan Valvano, copyright (c) 2016;
;  Programs 4.4 through 4.12, section 4.2
;
;Copyright 2016 by Jonathan W. Valvano, valvano@mail.utexas.edu
;    You may use, edit, run or distribute this file
;    as long as the above copyright notice remains
; THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
; OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; For more information about my classes, my research, and my books, see
; http://users.ece.utexas.edu/~valvano/
; */

        AREA |.text|, CODE, READONLY, ALIGN=2
        THUMB
        REQUIRE8
        PRESERVE8

        EXTERN  RunPt            ; currently running thread
        EXPORT  StartOS
        EXPORT  SysTick_Handler
        IMPORT  Scheduler


SysTick_Handler                ; 1) Saves R0-R3,R12,LR,PC,PSR
    CPSID   I                  ; 2) Prevent interrupt during switch
    PUSH {R4-R11}		;saves registers R4 to R11
    LDR R0,=RunPt		;load address of RunPt to R0
    LDR R1,[R0]		;R1 = RunPt
    STR SP,[R1]		;save current SP to tcbs.sp
		;LDR R1,[R1,#4]		;move to next tcbs
		;STR R1,[R0]		;R1 = RunPt, for the upcomming tcbs
    PUSH {R0,LR}
    BL Scheduler
    POP {R0,LR}
    LDR R1,[R0]		; 6) R1 = RunPt, new thread
    LDR SP,[R1]		;load new SP, SP = RunPt.sp
    POP {R4-R11}		;load registers from stack
    CPSIE   I                  ; 9) tasks run with interrupts enabled
    BX      LR                 ; 10) restore R0-R3,R12,LR,PC,PSR

StartOS
    LDR     R0, =RunPt         ; currently running thread, load address of RunPt to R0
    LDR     R2, [R0]           ; R2 = value of RunPt
    LDR     SP, [R2]           ; new thread SP, SP = RunPt->stackPointer, load address of RunPt to R0
    POP     {R4-R11}           ; restore regs r4-11
    POP     {R0-R3}            ; restore regs r0-3
    POP     {R12}
    ADD     SP,SP,#4           ; discard LR from initial stack, move to next element
    POP     {LR}               ; start location
    ADD     SP,SP,#4           ; discard PSR, move to next element
    CPSIE   I                  ; Enable interrupts at processor level
    BX      LR                 ; start first thread

    ALIGN
    END
