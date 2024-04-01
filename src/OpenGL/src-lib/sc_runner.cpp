/*
 * Timer Service Implementation for SCTUnit
 */

#include "sc_runner.h"

/**
 * Implementation of a timer service that uses _virtual_ time to raise time events.
 * It is solely meant for use with sctunit.
 */

SctUnitRunner::SctUnitRunner(
		){
}

void SctUnitRunner::proceed_time(sc::time time_ms)
{
	(void) time_ms;
}

void SctUnitRunner::proceed_cycles(sc::integer cycles)
{
	(void) cycles;
}

void SctUnitRunner::cancel() noexcept
{
}
