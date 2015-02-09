#ifndef _STDAFX_H_
#define _STDAFX_H_

#include <stdio.h>

extern volatile unsigned *module;
extern volatile unsigned *data;
extern volatile unsigned *cfg;
extern volatile unsigned *count;
extern volatile unsigned *data_read;
extern volatile unsigned *st_read;
extern volatile unsigned *count_read;

#include "Tools.h"
#include "platform.h"
#include "phy_cmd.h"

void ReadStatusReg();

#endif
