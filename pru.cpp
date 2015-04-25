#include "pru.h"
#define PRU_NUM 0


/* pru.cpp
 *
 *Implementation of Pru class based on pru-hello
 *by dhenke@mythopoeic.org 
 *
 * Author: Igor Nunes
 *
 * Created: April 25, 2015
 * 
 */

bool Pru::instanceFlag = false;
PruStatus Pru::status  = OFF;
Pru* Pru::pru = NULL;
const char * Pru::path_to_bin = NULL;
uint32_t *Pru::ram0 = 0x0;
uint32_t *Pru::ram1 = 0x0;


bool Pru::pru_cleanup(void) {
	int rtn = 0;

	/* clear the event (if asserted) */
	if(prussdrv_pru_clear_event(PRU_EVTOUT_0, PRU0_ARM_INTERRUPT))
	{
		fprintf(stderr, "prussdrv_pru_clear_event() failed\n");
		rtn = -1;
	}

	/* halt and disable the PRU (if running) */
	if((rtn = prussdrv_pru_disable(PRU_NUM)) != 0)
	{
		fprintf(stderr, "prussdrv_pru_disable() failed\n");
		rtn = -1;
	}

	/* release the PRU clocks and disable prussdrv module */
	if((rtn = prussdrv_exit()) != 0) {
		fprintf(stderr, "prussdrv_exit() failed\n");
	rtn = -1;
	}

	return rtn>=0?true:false;
}
void Pru::pru_ram_setup()
{
	static void *ramshort0, *ramshort1;
	prussdrv_map_prumem (PRUSS0_PRU0_DATARAM, &ramshort0);
	prussdrv_map_prumem (PRUSS0_PRU1_DATARAM, &ramshort1);
	ram0 = (uint32_t*)ramshort0;
	ram1 = (uint32_t*)ramshort1;
	return;
}

bool Pru::pru_setup()
{
	int rtn;
	tpruss_intc_initdata intc = PRUSS_INTC_INITDATA;

	if(!path_to_bin) 
	{
		fprintf(stderr, "pru_setup(): path is NULL\n");
		return false;
	}

	/* initialize PRU */
	if((rtn = prussdrv_init()) != 0) 
	{
		fprintf(stderr, "prussdrv_init() failed\n");
		return false;
	}

	/* open the interrupt */
	if((rtn = prussdrv_open(PRU_EVTOUT_0)) != 0) 
	{
		fprintf(stderr, "prussdrv_open() failed\n");
		return false;
	}

	/* initialize interrupt */
	if((rtn = prussdrv_pruintc_init(&intc)) != 0) 
	{
		fprintf(stderr, "prussdrv_pruintc_init() failed\n");
		return false;
	}
	pru_ram_setup();
	return true;
}

bool Pru::pruStart()
{
	if(status != STANDBY) return false;

	/* load and run the PRU program */
	if((prussdrv_exec_program(PRU_NUM, path_to_bin)) < 0) 
	{
		fprintf(stderr, "prussdrv_exec_program() failed\n");
		return false;
   	}

   return true;
}
bool Pru::pruWaitHalt()
{
	int rtn = prussdrv_pru_wait_event(PRU_EVTOUT_0);
	if(rtn < 0)
	{
		fprintf(stderr, "prussdirv_pru_wait_event() failed\n");
		return false;
	}
	status = STANDBY;
	return true;
}
Pru* Pru::getInstance(const char *path)
{
	path_to_bin = path;
	if(! instanceFlag)
	{
		status  = OFF;
		instanceFlag = true;
		if(!path_to_bin)
		{
			cout << "Empty path" << endl;
			return pru = new Pru();
		}
	/* prussdrv_init() will segfault if called with EUID != 0 */ 
		if(geteuid()) {
			fprintf(stderr,	" must be run as root to use prussdrv\n");
			return pru = new Pru();
   		}
		if(!pru_setup())
		{
			cout << "Error initializing pru" <<endl;
			return pru = new Pru();
		}
		status = STANDBY;
		cout << "Creating new instance..." << endl;
		pru = new Pru();
		return pru;
	}
	else
	{
		if(path_to_bin)
			cout << "Cannot instantiate new pru\n Returning available one..."<< endl;
		return pru;
	}
}

Pru* Pru::getInstance()
{
	if(! instanceFlag)
	{
		status = OFF;
		instanceFlag = true;
		pru = new Pru();
		return pru;
	}
	else
	{
		return pru;
	}
}

PruStatus Pru::getStatus()
{
	return status;
}

 void Pru::writeRAM(uint32_t value, uint32_t pos, short int prunum)
{
	if(!status) return;
	pos = pos % 4096;
	if (prunum == 0)
		ram0[pos] = value;
	else if (prunum == 1)
		ram1[pos] = value;
	return;
}
void Pru::readRAM(uint32_t *value, uint32_t pos, short int prunum)
{
	if(!status) return;
	pos = pos % 4096;
	if (prunum == 0)
		*value = ram0[pos];
	else if (prunum == 1)
		*value = ram1[pos];
	return;
}

Pru::~Pru()
{
	if(!status)
	{
		this->pru_cleanup();
		instanceFlag = false;
		status = OFF;
	}
}
