/* Generated with YAKINDU statechart tools
 *
 *   (C) Copyright  2001 - 2021  Steffen A. Mork
 *               All rights reserved */

#include "MouseSelect.h"

/*! \file Implementation of the state machine 'MouseSelect'
*/




MouseSelect::MouseSelect()  :
p1(),
p2(),
ifaceGui(sc_null),
ifaceView(sc_null),
isExecuting(false)
{
	this->ifaceGui.parent = this;
	this->ifaceView.parent = this;
	for (sc_ushort i = 0; i < maxOrthogonalStates; ++i)
		stateConfVector[i] = MouseSelect_last_state;
	
	clearInEvents();
}

MouseSelect::~MouseSelect()
{
}

MouseSelect::Gui::Gui(MouseSelect* parent) :
onSelect_raised(false),
onDisable_raised(false),
mouseDown_raised(false),
mouseDown_value(),
mouseMove_raised(false),
mouseMove_value(),
mouseUp_raised(false),
mouseUp_value(),
selectionEnd_raised(false),
parent(parent)
{
}

MouseSelect::View::View(MouseSelect* parent) :
parent(parent),
ifaceViewOperationCallback(sc_null)
{
}


using namespace mouseselect_events;

SctEvent* MouseSelect::getNextEvent()
{
	SctEvent* nextEvent = 0;
	
	if(!inEventQueue.empty()) {
		nextEvent = inEventQueue.front();
		inEventQueue.pop_front();
	}
	
	return nextEvent;
}

void MouseSelect::dispatch_event(SctEvent * event)
{
	if(event == 0) {
		return;
	}
	switch(event->name)
	{
		case Gui_onSelect:
		case Gui_onDisable:
		case Gui_mouseDown:
		case Gui_mouseMove:
		case Gui_mouseUp:
		{
			ifaceGui.dispatch_event(event);
			break;
		}
		default:
			break;
	}
}

void MouseSelect::Gui::dispatch_event(SctEvent * event)
{
	switch(event->name)
	{
		case Gui_onSelect:
		{
			internal_raiseOnSelect();
			break;
		}
		case Gui_onDisable:
		{
			internal_raiseOnDisable();
			break;
		}
		case Gui_mouseDown:
		{
			SctEvent_Gui_mouseDown * e = static_cast<SctEvent_Gui_mouseDown*>(event);
			if(e != 0) {
				internal_raiseMouseDown(e->value);
			}
			break;
		}
		case Gui_mouseMove:
		{
			SctEvent_Gui_mouseMove * e = static_cast<SctEvent_Gui_mouseMove*>(event);
			if(e != 0) {
				internal_raiseMouseMove(e->value);
			}
			break;
		}
		case Gui_mouseUp:
		{
			SctEvent_Gui_mouseUp * e = static_cast<SctEvent_Gui_mouseUp*>(event);
			if(e != 0) {
				internal_raiseMouseUp(e->value);
			}
			break;
		}
		default:
			break;
	}
}
void MouseSelect::View::dispatch_event(SctEvent * event)
{
	switch(event->name)
	{
		default:
			break;
	}
}
void MouseSelect::internal_dispatch_event(SctEvent * event)
{
	switch(event->name)
	{
		default:
			break;
	}
}



sc_boolean MouseSelect::isActive() const
{
	return stateConfVector[0] != MouseSelect_last_state;
}

/* 
 * Always returns 'false' since this state machine can never become final.
 */
sc_boolean MouseSelect::isFinal() const
{
   return false;}

sc_boolean MouseSelect::check() {
	if (this->ifaceView.ifaceViewOperationCallback == sc_null) {
		return false;
	}
	return true;
}


sc_boolean MouseSelect::isStateActive(MouseSelectStates state) const
{
	switch (state)
	{
		case main_region_Normal : 
			return (sc_boolean) (stateConfVector[SCVI_MAIN_REGION_NORMAL] == main_region_Normal
			);
		case main_region_Selection : 
			return (sc_boolean) (stateConfVector[SCVI_MAIN_REGION_SELECTION] == main_region_Selection
			);
		case main_region_Moving : 
			return (sc_boolean) (stateConfVector[SCVI_MAIN_REGION_MOVING] == main_region_Moving
			);
		default: return false;
	}
}

MouseSelect::Gui* MouseSelect::gui()
{
	return &ifaceGui;
}
/* Functions for event onSelect in interface Gui */
void MouseSelect::Gui::raiseOnSelect()
{
	parent->inEventQueue.push_back(new SctEvent_Gui_onSelect(Gui_onSelect));
	parent->runCycle();
}
void MouseSelect::Gui::internal_raiseOnSelect()
{
	onSelect_raised = true;
}
/* Functions for event onDisable in interface Gui */
void MouseSelect::Gui::raiseOnDisable()
{
	parent->inEventQueue.push_back(new SctEvent_Gui_onDisable(Gui_onDisable));
	parent->runCycle();
}
void MouseSelect::Gui::internal_raiseOnDisable()
{
	onDisable_raised = true;
}
/* Functions for event mouseDown in interface Gui */
void MouseSelect::Gui::raiseMouseDown(SCT_point value)
{
	parent->inEventQueue.push_back(new SctEvent_Gui_mouseDown(Gui_mouseDown, value));
	parent->runCycle();
}
void MouseSelect::Gui::internal_raiseMouseDown(SCT_point value)
{
	mouseDown_value = value;
	mouseDown_raised = true;
}
/* Functions for event mouseMove in interface Gui */
void MouseSelect::Gui::raiseMouseMove(SCT_point value)
{
	parent->inEventQueue.push_back(new SctEvent_Gui_mouseMove(Gui_mouseMove, value));
	parent->runCycle();
}
void MouseSelect::Gui::internal_raiseMouseMove(SCT_point value)
{
	mouseMove_value = value;
	mouseMove_raised = true;
}
/* Functions for event mouseUp in interface Gui */
void MouseSelect::Gui::raiseMouseUp(SCT_point value)
{
	parent->inEventQueue.push_back(new SctEvent_Gui_mouseUp(Gui_mouseUp, value));
	parent->runCycle();
}
void MouseSelect::Gui::internal_raiseMouseUp(SCT_point value)
{
	mouseUp_value = value;
	mouseUp_raised = true;
}
/* Functions for event selectionEnd in interface Gui */
sc::rx::Observable<void>* MouseSelect::Gui::getSelectionEnd()
{
	return &(this->selectionEnd_observable);
}
MouseSelect::View* MouseSelect::view()
{
	return &ifaceView;
}
void MouseSelect::View::setOperationCallback(OperationCallback* operationCallback)
{
	ifaceViewOperationCallback = operationCallback;
}
SCT_point MouseSelect::getP1() const
{
	return p1;
}

void MouseSelect::setP1(SCT_point value)
{
	this->p1 = value;
}

SCT_point MouseSelect::getP2() const
{
	return p2;
}

void MouseSelect::setP2(SCT_point value)
{
	this->p2 = value;
}


// implementations of all internal functions

/* Entry action for state 'Moving'. */
void MouseSelect::enact_main_region_Moving()
{
	/* Entry action for state 'Moving'. */
	ifaceView.ifaceViewOperationCallback->drawRect(p1.x, p1.y, p2.x, p2.y);
}

/* Exit action for state 'Moving'. */
void MouseSelect::exact_main_region_Moving()
{
	/* Exit action for state 'Moving'. */
	ifaceView.ifaceViewOperationCallback->drawRect(p1.x, p1.y, p2.x, p2.y);
}

/* 'default' enter sequence for state Normal */
void MouseSelect::enseq_main_region_Normal_default()
{
	/* 'default' enter sequence for state Normal */
	stateConfVector[0] = main_region_Normal;
}

/* 'default' enter sequence for state Selection */
void MouseSelect::enseq_main_region_Selection_default()
{
	/* 'default' enter sequence for state Selection */
	stateConfVector[0] = main_region_Selection;
}

/* 'default' enter sequence for state Moving */
void MouseSelect::enseq_main_region_Moving_default()
{
	/* 'default' enter sequence for state Moving */
	enact_main_region_Moving();
	stateConfVector[0] = main_region_Moving;
}

/* 'default' enter sequence for region main region */
void MouseSelect::enseq_main_region_default()
{
	/* 'default' enter sequence for region main region */
	react_main_region__entry_Default();
}

/* Default exit sequence for state Normal */
void MouseSelect::exseq_main_region_Normal()
{
	/* Default exit sequence for state Normal */
	stateConfVector[0] = MouseSelect_last_state;
}

/* Default exit sequence for state Selection */
void MouseSelect::exseq_main_region_Selection()
{
	/* Default exit sequence for state Selection */
	stateConfVector[0] = MouseSelect_last_state;
}

/* Default exit sequence for state Moving */
void MouseSelect::exseq_main_region_Moving()
{
	/* Default exit sequence for state Moving */
	stateConfVector[0] = MouseSelect_last_state;
	exact_main_region_Moving();
}

/* Default exit sequence for region main region */
void MouseSelect::exseq_main_region()
{
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of MouseSelect.main_region) at position 0... */
	switch(stateConfVector[ 0 ])
	{
		case main_region_Normal :
		{
			exseq_main_region_Normal();
			break;
		}
		case main_region_Selection :
		{
			exseq_main_region_Selection();
			break;
		}
		case main_region_Moving :
		{
			exseq_main_region_Moving();
			break;
		}
		default: break;
	}
}

/* Default react sequence for initial entry  */
void MouseSelect::react_main_region__entry_Default()
{
	/* Default react sequence for initial entry  */
	enseq_main_region_Normal_default();
}

sc_integer MouseSelect::react(const sc_integer transitioned_before) {
	/* State machine reactions. */
	return transitioned_before;
}

sc_integer MouseSelect::main_region_Normal_react(const sc_integer transitioned_before) {
	/* The reactions of state Normal. */
	sc_integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (0))
	{ 
		if (((ifaceGui.onSelect_raised)) && ((!ifaceView.ifaceViewOperationCallback->is3D())))
		{ 
			exseq_main_region_Normal();
			enseq_main_region_Selection_default();
			react(0);
			transitioned_after = 0;
		}  else
		{
			if (((ifaceGui.onSelect_raised)) && ((ifaceView.ifaceViewOperationCallback->is3D())))
			{ 
				exseq_main_region_Normal();
				ifaceGui.selectionEnd_observable.next();
				ifaceGui.selectionEnd_raised = true;
				enseq_main_region_Normal_default();
				react(0);
				transitioned_after = 0;
			} 
		}
	} 
	/* If no transition was taken then execute local reactions */
	if ((transitioned_after) == (transitioned_before))
	{ 
		transitioned_after = react(transitioned_before);
	} 
	return transitioned_after;
}

sc_integer MouseSelect::main_region_Selection_react(const sc_integer transitioned_before) {
	/* The reactions of state Selection. */
	sc_integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (0))
	{ 
		if (ifaceGui.onDisable_raised)
		{ 
			exseq_main_region_Selection();
			enseq_main_region_Normal_default();
			react(0);
			transitioned_after = 0;
		}  else
		{
			if (ifaceGui.mouseDown_raised)
			{ 
				exseq_main_region_Selection();
				p1 = ifaceGui.mouseDown_value;
				p2 = ifaceGui.mouseDown_value;
				enseq_main_region_Moving_default();
				react(0);
				transitioned_after = 0;
			} 
		}
	} 
	/* If no transition was taken then execute local reactions */
	if ((transitioned_after) == (transitioned_before))
	{ 
		transitioned_after = react(transitioned_before);
	} 
	return transitioned_after;
}

sc_integer MouseSelect::main_region_Moving_react(const sc_integer transitioned_before) {
	/* The reactions of state Moving. */
	sc_integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (0))
	{ 
		if (ifaceGui.mouseUp_raised)
		{ 
			exseq_main_region_Moving();
			ifaceGui.selectionEnd_observable.next();
			ifaceGui.selectionEnd_raised = true;
			ifaceView.ifaceViewOperationCallback->select(p1.x, p1.y, p2.x, p2.y);
			enseq_main_region_Normal_default();
			react(0);
			transitioned_after = 0;
		}  else
		{
			if (ifaceGui.mouseMove_raised)
			{ 
				exseq_main_region_Moving();
				p2 = ifaceGui.mouseMove_value;
				enseq_main_region_Moving_default();
				react(0);
				transitioned_after = 0;
			} 
		}
	} 
	/* If no transition was taken then execute local reactions */
	if ((transitioned_after) == (transitioned_before))
	{ 
		transitioned_after = react(transitioned_before);
	} 
	return transitioned_after;
}

void MouseSelect::clearInEvents() {
	ifaceGui.onSelect_raised = false;
	ifaceGui.onDisable_raised = false;
	ifaceGui.mouseDown_raised = false;
	ifaceGui.mouseMove_raised = false;
	ifaceGui.mouseUp_raised = false;
}

void MouseSelect::microStep() {
	switch(stateConfVector[ 0 ])
	{
		case main_region_Normal :
		{
			main_region_Normal_react(-1);
			break;
		}
		case main_region_Selection :
		{
			main_region_Selection_react(-1);
			break;
		}
		case main_region_Moving :
		{
			main_region_Moving_react(-1);
			break;
		}
		default: break;
	}
}

void MouseSelect::runCycle() {
	/* Performs a 'run to completion' step. */
	if (isExecuting)
	{ 
		return;
	} 
	isExecuting = true;
	dispatch_event(getNextEvent());
	do
	{ 
		microStep();
		clearInEvents();
		dispatch_event(getNextEvent());
	} while (((((ifaceGui.onSelect_raised) || (ifaceGui.onDisable_raised)) || (ifaceGui.mouseDown_raised)) || (ifaceGui.mouseMove_raised)) || (ifaceGui.mouseUp_raised));
	isExecuting = false;
}

void MouseSelect::enter() {
	/* Activates the state machine. */
	if (isExecuting)
	{ 
		return;
	} 
	isExecuting = true;
	/* Default enter sequence for statechart MouseSelect */
	enseq_main_region_default();
	isExecuting = false;
}

void MouseSelect::exit() {
	/* Deactivates the state machine. */
	if (isExecuting)
	{ 
		return;
	} 
	isExecuting = true;
	/* Default exit sequence for statechart MouseSelect */
	exseq_main_region();
	isExecuting = false;
}



