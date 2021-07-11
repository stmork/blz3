/*
 * Timer Service Implementation for SCTUnit
 */

#include "sc_runner.h"

/**
 * Implementation of a timer service that uses _virtual_ time to raise time events.
 * It is solely meant for use with sctunit.
 */

SctUnitRunner::SctUnitRunner(
		sc::StatemachineInterface * p_statemachine,
		bool p_event_driven,
		sc_integer p_cycle_period
		) :
		statemachine(p_statemachine),
		event_driven(p_event_driven),
		cycle_period(p_event_driven ? -1 : p_cycle_period),
		current_time_ms(0),
		timer_queue()
{
	if(!event_driven) {
		SctTimer runCycle(
			cycle_period,
			true,
			0,
			-1,
			true
		);
		insert_timer(runCycle);
	}
}

void SctUnitRunner::proceed_time(sc_integer time_ms)
{
	sc_integer stop_time_ms = current_time_ms + time_ms;
	bool processed_timer = false;

	do {
		
		// first assume we won't process a timer
		processed_timer = false;

		// and then check if there is a timer to process
		if( ! timer_queue.empty()) {
			SctTimer next = timer_queue.front();

			if(next.abs_time_ms <= stop_time_ms) {
				timer_queue.pop_front();
				current_time_ms = next.abs_time_ms;

				/* Repeat and reinsert timer? */
				if(next.periodic) {
					insert_timer(next);
				}
				processed_timer = true;
			}
		}

	} while ( processed_timer );

	// As a postcondition the current time is the time after proceeding the specified period.
	current_time_ms = stop_time_ms;
}

void SctUnitRunner::proceed_cycles(sc_integer cycles)
{
	sc_integer elapsed_cycles = 0;
		
	while(elapsed_cycles < cycles) {
		if(timer_queue.empty()) {
			return;
		}
		SctTimer next = timer_queue.front();
		
		timer_queue.pop_front();
		
		current_time_ms = next.abs_time_ms;
		
		std::list<SctTimer>::iterator i_timer;
		
		/* Repeat and reinsert timer? */
		if(next.periodic) {
			insert_timer(next);
		}
	}
}

void SctUnitRunner::cancel()
{
	std::list<SctTimer>::iterator i_timer = timer_queue.begin();
	while(i_timer != timer_queue.end()) {
		i_timer = timer_queue.erase(i_timer);
	}
}
void SctUnitRunner::insert_timer(SctTimer timer) 
{
	timer.abs_time_ms = current_time_ms + timer.rel_time_ms;
	
	std::list<SctTimer>::iterator i_timer;
	
	/* Either the list is empty, so we put the new timer in directly... */
	if(timer_queue.empty()) {
		timer_queue.push_front(timer);
		return;
	}
	
	
	/* Or we put it before some other timer that needs to be raised after this one */
	for(i_timer = timer_queue.begin(); i_timer != timer_queue.end(); ++i_timer) {
		if(timer.compare(&(*i_timer)) < 0) {
			timer_queue.insert(i_timer, timer);
			return;
		}
	}
	
	/* Or it gets put into the back of the list. */
	timer_queue.push_back(timer);
}

SctUnitRunner::SctTimer::SctTimer(
		sc_integer p_time_ms,
		bool p_periodic,
		sc_eventid p_evid,
		sc_integer p_priority,
		sc_boolean p_is_runcycle
		) :
		rel_time_ms(p_time_ms),
		abs_time_ms(0),
		periodic(p_periodic),
		pt_evid(p_evid),
		priority(p_priority),
		is_runcycle(p_is_runcycle)
{}

sc_integer SctUnitRunner::SctTimer::compare(SctTimer * other)
{
	sc_integer result = abs_time_ms - other->abs_time_ms;
	if(result != 0) {
		return result;
	} else {
		// bigger priority needs to be raised first
		result = other->priority - priority;
		return result;
	}
}
