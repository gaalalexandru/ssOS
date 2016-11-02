// os_hw.h - ssOS Target HW specific code - Interrupt Service Routines, Target configurations
// ssOS - stupid simple Operating System
// A very simple real time operating system with minimal features.
// For copyright, configuration and usage read readme.txt

#ifndef __OSHW_H
#define __OSHW_H  1

#include "os_core.h"

#ifdef TARGET_TM4C
//TM4C specific code
#include "BSP_TM4C.h"
#endif //TARGET_TM4C

#ifdef TARGET_MSP432
//MSP432 specific code
#include "BSP_MSP432.h"
#endif //TARGET_TM4C

#endif