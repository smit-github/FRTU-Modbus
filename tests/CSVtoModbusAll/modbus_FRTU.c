#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <modbus.h>
#include <stdbool.h> 

#include "modbus_FRTU.h"
#include "modbus_Array_Defaults.h"
#include "modbus_FRTU_DI.h"
#include "modbus_FRTU_AI.h"
#include "modbus_FRTU_DO.h"


const int EXCEPTION_RC = 2;


int equal_dword(uint16_t *tab_reg, const uint32_t value) {
    return ((tab_reg[0] == (value >> 16)) && (tab_reg[1] == (value & 0xFFFF)));
}

const int NB_REPORT_SLAVE_ID = 10;
uint8_t *tab_rp_bits = NULL;
uint16_t *tab_rp_registers = NULL;
uint16_t *tab_rp_registers_bad = NULL;
modbus_t *ctx = NULL;
int i;
uint8_t value,NoOfData;
int nb_points;
int rc;
float real;
uint32_t old_response_to_sec;
uint32_t old_response_to_usec;
uint32_t new_response_to_sec;
uint32_t new_response_to_usec;
uint32_t old_byte_to_sec;
uint32_t old_byte_to_usec;
int use_backend;
int success = FALSE;
int old_slave;


int Configure_Modbus(uint8_t use_backend)
{

    if (use_backend == TCP) {
        ctx = modbus_new_tcp("127.0.0.1", 1502);
    } else if (use_backend == TCP_PI) {
        ctx = modbus_new_tcp_pi("::1", "1502");
    } else {
        ctx = modbus_new_rtu("/dev/ttyUSB0", 115200, 'E', 8, 1);
    }
    if (ctx == NULL) {
        fprintf(stderr, "Unable to allocate libmodbus context\n");
        return -1;
    }
}

void m_modbus_debug(bool Debug)
{
    modbus_set_debug(ctx, Debug);
}

void m_modbus_set_error_recovery(void)
{
    modbus_set_error_recovery(ctx,
                              MODBUS_ERROR_RECOVERY_LINK |
                              MODBUS_ERROR_RECOVERY_PROTOCOL);
}

int m_run_modbus(void)
{

	//m_modbus_set_slave(
	m_modbus_get_response_timeout_old();

	if (m_modbus_connect() == -1) 
	{
		fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
		m_modbus_free();
		return -1;
	}

	m_modbus_get_response_timeout_new();
}


void m_modbus_get_response_timeout_old(void)
{
	modbus_get_response_timeout(ctx, &old_response_to_sec, &old_response_to_usec);	
}

void m_modbus_get_response_timeout_new(void)
{
	modbus_get_response_timeout(ctx, &new_response_to_sec, &new_response_to_usec);
}


void m_modbus_set_slave(uint8_t ServerID)
{
	modbus_set_slave(ctx, ServerID);
}

void m_modbus_write_registers(int addr, int nb, const uint16_t *data)
{
	modbus_write_registers(ctx, addr, nb, data);
}

void m_modbus_close_and_free(void) 
{
    modbus_close(ctx);
    modbus_free(ctx);
    ctx = NULL;
}

int m_modbus_connect(void)
{
	return modbus_connect(ctx);
}

void m_modbus_free(void)
{
	modbus_free(ctx);
}

void m_modbus_close(void)
{
	modbus_close(ctx);
}

void m_modbus_close_and_free_memory(void) 
{
    /* Free the memory */
    free(tab_rp_bits);
    free(tab_rp_registers);

    /* Close the connection */
    m_modbus_close();
    m_modbus_free();
}



