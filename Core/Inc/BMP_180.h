/*
 * BMP_180.h
 *
 *  Created on: Dec 7, 2023
 *      Author: Mega-PC
 */

#ifndef INC_BMP_180_H_
#define INC_BMP_180_H_

#include "main.h"

void BMP180_Start (void);
uint32_t Get_UPress (int oss);
float BMP180_GetPress (int oss);

#endif /* INC_BMP_180_H_ */
