/* Generated with YAKINDU statechart tools
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText:  Copyright (C)  2024  Steffen A. Mork
 *               All rights reserved */

#ifndef SC_EVENTDRIVEN_H_
#define SC_EVENTDRIVEN_H_

#include "sc_statemachine.h"

namespace sc
{

	/*! \file
	Interface for event-driven state machines.
	*/
	class EventDrivenInterface : public sc::StatemachineInterface
	{
	public:

		virtual ~EventDrivenInterface() = 0;

		/*! Start a run-to-completion step without any event.
		*/
		virtual void triggerWithoutEvent() = 0;
	};

	inline EventDrivenInterface::~EventDrivenInterface() {}

} /* namespace sc */

#endif /* SC_EVENTDRIVEN_H_ */
