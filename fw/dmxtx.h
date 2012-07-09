#ifndef __DMXTX_H__
#define __DMXTX_H__

#include "board.h"
#include <avr/interrupt.h>


#define DMXTXSFBS 0
#define DMXTXMABS 1
#define DMXTXTXS 2

#define DMXTXBSZ 64
#define DMXTXSFBT 240
#define DMXTXMABT 30



void dmxtx_init(void);



#endif
