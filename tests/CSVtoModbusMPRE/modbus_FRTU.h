/* tests/unit-test.h.  Generated from unit-test.h.in by configure.  */
/*
 * Copyright © 2008-2014 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/* Constants defined by configure.ac */


#include <stdint.h>
#include <stdbool.h> 

#define SERVER_ID         7
//#define TEST_DO
#ifdef TEST_DO
	#define SERVER_ID         7
#endif

//#define TEST_DI
#ifdef TEST_DI
	#define SERVER_ID         3
#endif

//#define TEST_AI
#ifdef TEST_AI
	#define SERVER_ID         1
#endif


#define INVALID_SERVER_ID 18

#define BYTEperWORD 2

enum {
    TCP,
    TCP_PI,
    RTU
};

int Configure_Modbus(uint8_t use_backend);
void m_modbus_set_error_recovery(void);
void m_modbus_debug(bool Debug);
void m_modbus_set_slave(uint8_t ServerID);
int m_run_modbus(void);
void m_modbus_write_registers(int addr, int nb, const uint16_t *data);
void m_modbus_close_and_free_memory(void) ;
void m_modbus_close(void);

void m_modbus_close_and_free(void) ;
void m_modbus_free(void) ;
void m_modbus_get_response_timeout_new(void);
void m_modbus_get_response_timeout_old(void);
int m_modbus_connect(void);



