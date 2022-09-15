/*
 * Timer Service for SCTUnit
 */

#ifndef SC_TIMER_SERVICE_H_
#define SC_TIMER_SERVICE_H_

#include <list>

#include "sc_types.h"
#include "sc_statemachine.h"

class SctUnitRunner {
	public:
		SctUnitRunner(sc::StatemachineInterface * statemachine_, bool event_driven_, sc::integer cycle_period_);
		virtual ~SctUnitRunner(){};
		void proceed_time(sc::integer time_ms);
		void proceed_cycles(sc::integer cycles);
		virtual void cancel();
	private:
		class SctTimer;
		sc::StatemachineInterface * statemachine;
		
		bool event_driven;
		sc::integer cycle_period;
		sc::integer current_time_ms;

		std::list<SctTimer> timer_queue;
		void insert_timer(SctTimer timer);
};

class SctUnitRunner::SctTimer {
	friend class SctUnitRunner;
	public:
		SctTimer(sc::integer time_ms, bool periodic_, sc::eventid evid, sc::integer priority_, bool is_runcycle_);
		~SctTimer(){}
		
		sc::integer compare(SctTimer * other);
		
	private:
		sc::integer rel_time_ms;
		sc::integer abs_time_ms;
		bool periodic;
		sc::eventid pt_evid;
		sc::integer priority;
		bool is_runcycle;
};

#endif /* SC_TIMER_SERVICE_H_ */

