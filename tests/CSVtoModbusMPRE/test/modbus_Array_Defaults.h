/*
 * Copyright © 2008-2014 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <modbus.h>



extern const uint16_t DI_FRTU_PART1_REGISTERS_ADDRESS;
extern const uint16_t DI_FRTU_PART1_REGISTERS_NB;
extern const uint16_t DI_FRTU_PART1_REGISTERS_NB_MAX;
extern const uint16_t DI_FRTU_PART1_REGISTERS_TAB[];

extern const uint16_t DI_FRTU_PART2_REGISTERS_ADDRESS;
extern const uint16_t DI_FRTU_PART2_REGISTERS_NB;
extern const uint16_t DI_FRTU_PART2_REGISTERS_NB_MAX;
extern const uint16_t DI_FRTU_PART2_REGISTERS_TAB[];

extern const uint16_t DI_FRTU_PART3_REGISTERS_ADDRESS;
extern const uint16_t DI_FRTU_PART3_REGISTERS_NB;
extern const uint16_t DI_FRTU_PART3_REGISTERS_NB_MAX;
extern const uint16_t DI_FRTU_PART3_REGISTERS_TAB[];

extern const uint16_t DI_FRTU_PART4_REGISTERS_ADDRESS;
extern const uint16_t DI_FRTU_PART4_REGISTERS_NB;
extern const uint16_t DI_FRTU_PART4_REGISTERS_NB_MAX;
extern const uint16_t DI_FRTU_PART4_REGISTERS_TAB[];

extern const uint16_t AI_FRTU_PART1_REGISTERS_ADDRESS;
extern const uint16_t AI_FRTU_PART1_REGISTERS_NB;
extern const uint16_t AI_FRTU_PART1_REGISTERS_NB_MAX;
extern const uint16_t AI_FRTU_PART1_REGISTERS_TAB[];

extern const uint16_t AI_FRTU_PART2_REGISTERS_ADDRESS;
extern const uint16_t AI_FRTU_PART2_REGISTERS_NB;
extern const uint16_t AI_FRTU_PART2_REGISTERS_NB_MAX;
extern const uint16_t AI_FRTU_PART2_REGISTERS_TAB[];

extern const uint16_t AI_FRTU_PART3_REGISTERS_ADDRESS;
extern const uint16_t AI_FRTU_PART3_REGISTERS_NB;
extern const uint16_t AI_FRTU_PART3_REGISTERS_NB_MAX;
extern const uint16_t AI_FRTU_PART3_REGISTERS_TAB[];

