#include <iostream>
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pruss/prussdrv.h>
#include <pruss/pruss_intc_mapping.h>

using namespace std;

enum PruStatus {OFF,STANDBY,RUNNING};
class Pru
{
	private:
		static const char *path_to_bin;
		static bool instanceFlag;
		static Pru *pru;
		Pru(){}
		static uint32_t *ram0;
		static uint32_t *ram1;
	protected:
		static PruStatus status;
		static void pru_ram_setup();
		bool pru_cleanup();
		static bool pru_setup();
	public:
		static bool pruStart();
		static bool pruWaitHalt();
		static Pru* getInstance(const char 
				*path_to_bin);
		static Pru* getInstance();
		static void writeRAM(uint32_t value,
				uint32_t pos,
				short int prunum);
		static void readRAM(uint32_t *value,
				uint32_t pos,
				short int prunum);
		PruStatus getStatus();
		~Pru();
};
