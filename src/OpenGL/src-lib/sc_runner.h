/*
 * Timer Service for SCTUnit
 */

#ifndef SC_UNIT_TIMER_SERVICE_H_
#define SC_UNIT_TIMER_SERVICE_H_


#include "sc_types.h"
#include "sc_eventdriven.h"

class SctUnitRunner
{
public:
	SctUnitRunner();
	virtual ~SctUnitRunner()
	{
	};
	void proceed_time(sc::time time_ms);
	void proceed_cycles(sc::integer cycles);
	virtual void cancel() noexcept;
private:
};
#endif /* SC_UNIT_TIMER_SERVICE_H_ */

