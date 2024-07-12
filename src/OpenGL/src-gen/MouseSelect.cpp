/* Generated with YAKINDU statechart tools
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText:  Copyright (C)  2024  Steffen A. Mork
 *               All rights reserved */

#include "MouseSelect.h"

/*! \file
Implementation of the state machine 'MouseSelect'
*/




MouseSelect::MouseSelect() noexcept
{
	this->ifaceGui.parent = this;
	this->ifaceView.parent = this;
	for (sc::ushort state_vec_pos = 0; state_vec_pos < maxOrthogonalStates; ++state_vec_pos)
		stateConfVector[state_vec_pos] = MouseSelect::State::NO_STATE;
	
	clearInEvents();
	clearOutEvents();
}

MouseSelect::~MouseSelect()
{
	while (!incomingEventQueue.empty()) {
		auto nextEvent{incomingEventQueue.front()};
		incomingEventQueue.pop_front();
		delete nextEvent;
	}
}

MouseSelect::Gui::Gui(MouseSelect* parent_) noexcept :
	parent(parent_),
	ifaceGuiOperationCallback(nullptr)
{
}

MouseSelect::View::View(MouseSelect* parent_) noexcept :
	parent(parent_),
	ifaceViewOperationCallback(nullptr)
{
}



MouseSelect::EventInstance* MouseSelect::getNextEvent() noexcept
{
	MouseSelect::EventInstance* nextEvent = 0;

	if(!incomingEventQueue.empty()) {
		nextEvent = incomingEventQueue.front();
		incomingEventQueue.pop_front();
	}
	
	return nextEvent;
	
}					


	
bool MouseSelect::dispatchEvent(MouseSelect::EventInstance* event) noexcept
{
	if(event == nullptr) {
		return false;
	}
								
	switch(event->eventId)
	{
		
		case MouseSelect::Event::Gui_onSelect:
		{
			ifaceGui.onSelect_raised = true;
			break;
		}
		case MouseSelect::Event::Gui_onDisable:
		{
			ifaceGui.onDisable_raised = true;
			break;
		}
		case MouseSelect::Event::Gui_mouseDown:
		{
			MouseSelect::EventInstanceWithValue<SCT_point>* e = static_cast<MouseSelect::EventInstanceWithValue<SCT_point> *>(event);
			if(e != 0) {
				ifaceGui.mouseDown_value
				 = e->value;
				ifaceGui.mouseDown_raised = true;
			}
			break;
		}
		case MouseSelect::Event::Gui_mouseMove:
		{
			MouseSelect::EventInstanceWithValue<SCT_point>* e = static_cast<MouseSelect::EventInstanceWithValue<SCT_point> *>(event);
			if(e != 0) {
				ifaceGui.mouseMove_value
				 = e->value;
				ifaceGui.mouseMove_raised = true;
			}
			break;
		}
		case MouseSelect::Event::Gui_mouseUp:
		{
			MouseSelect::EventInstanceWithValue<SCT_point>* e = static_cast<MouseSelect::EventInstanceWithValue<SCT_point> *>(event);
			if(e != 0) {
				ifaceGui.mouseUp_value
				 = e->value;
				ifaceGui.mouseUp_raised = true;
			}
			break;
		}
		
		default:
			//pointer got out of scope
			delete event;
			return false;
	}
	//pointer got out of scope
	delete event;
	return true;
}



bool MouseSelect::isActive() const noexcept
{
	return stateConfVector[0] != MouseSelect::State::NO_STATE;
}

/* 
 * Always returns 'false' since this state machine can never become final.
 */
bool MouseSelect::isFinal() const noexcept
{
	   return false;
}

bool MouseSelect::check() const noexcept{
	if (this->ifaceGui.ifaceGuiOperationCallback == nullptr) {
		return false;
	}
	if (this->ifaceView.ifaceViewOperationCallback == nullptr) {
		return false;
	}
	return true;
}


bool MouseSelect::isStateActive(State state) const noexcept
{
	switch (state)
	{
		case MouseSelect::State::main_region_Normal :
		{
			return  (stateConfVector[scvi_main_region_Normal] == MouseSelect::State::main_region_Normal);
			break;
		}
		case MouseSelect::State::main_region_Selection :
		{
			return  (stateConfVector[scvi_main_region_Selection] == MouseSelect::State::main_region_Selection);
			break;
		}
		case MouseSelect::State::main_region_Moving :
		{
			return  (stateConfVector[scvi_main_region_Moving] == MouseSelect::State::main_region_Moving);
			break;
		}
		case MouseSelect::State::main_region_Panning :
		{
			return  (stateConfVector[scvi_main_region_Panning] == MouseSelect::State::main_region_Panning);
			break;
		}
		default:
		{
			/* State is not active*/
			return false;
			break;
		}
	}
}

MouseSelect::Gui& MouseSelect::gui() noexcept
{
	return ifaceGui;
}
void MouseSelect::Gui::setOperationCallback(OperationCallback* operationCallback) noexcept
{
	ifaceGuiOperationCallback = operationCallback;
}

/*! Raises the in event 'onSelect' of interface scope 'gui'. */
void MouseSelect::Gui::raiseOnSelect() {
	parent->incomingEventQueue.push_back(new MouseSelect::EventInstance(MouseSelect::Event::Gui_onSelect))
	;
	parent->runCycle();
}


/*! Raises the in event 'onDisable' of interface scope 'gui'. */
void MouseSelect::Gui::raiseOnDisable() {
	parent->incomingEventQueue.push_back(new MouseSelect::EventInstance(MouseSelect::Event::Gui_onDisable))
	;
	parent->runCycle();
}


/*! Raises the in event 'mouseDown' of interface scope 'gui'. */
void MouseSelect::Gui::raiseMouseDown(SCT_point mouseDown_) {
	parent->incomingEventQueue.push_back(new MouseSelect::EventInstanceWithValue<SCT_point>(MouseSelect::Event::Gui_mouseDown, mouseDown_))
	;
	parent->runCycle();
}


/*! Raises the in event 'mouseMove' of interface scope 'gui'. */
void MouseSelect::Gui::raiseMouseMove(SCT_point mouseMove_) {
	parent->incomingEventQueue.push_back(new MouseSelect::EventInstanceWithValue<SCT_point>(MouseSelect::Event::Gui_mouseMove, mouseMove_))
	;
	parent->runCycle();
}


/*! Raises the in event 'mouseUp' of interface scope 'gui'. */
void MouseSelect::Gui::raiseMouseUp(SCT_point mouseUp_) {
	parent->incomingEventQueue.push_back(new MouseSelect::EventInstanceWithValue<SCT_point>(MouseSelect::Event::Gui_mouseUp, mouseUp_))
	;
	parent->runCycle();
}


bool MouseSelect::Gui::isRaisedSelectionEnd() noexcept {
	return selectionEnd_raised;
}


sc::rx::Observable<void>& MouseSelect::Gui::getSelectionEnd() noexcept {
	return this->selectionEnd_observable;
}

MouseSelect::View& MouseSelect::view() noexcept
{
	return ifaceView;
}
void MouseSelect::View::setOperationCallback(OperationCallback* operationCallback) noexcept
{
	ifaceViewOperationCallback = operationCallback;
}
void MouseSelect::setP1(SCT_point p1_) noexcept
{
	this->p1 = p1_;
}
void MouseSelect::setP2(SCT_point p2_) noexcept
{
	this->p2 = p2_;
}

// implementations of all internal functions
/* Entry action for state 'Moving'. */
void MouseSelect::enact_main_region_Moving()
{
	/* Entry action for state 'Moving'. */
	ifaceView.ifaceViewOperationCallback->setRectangle(p1.x, p1.y, p2.x, p2.y);
}

/* Entry action for state 'Panning'. */
void MouseSelect::enact_main_region_Panning()
{
	/* Entry action for state 'Panning'. */
	setP1(p2);
}

/* Exit action for state 'Moving'. */
void MouseSelect::exact_main_region_Moving()
{
	/* Exit action for state 'Moving'. */
	ifaceView.ifaceViewOperationCallback->setRectangle(p1.x, p1.y, p2.x, p2.y);
}

/* 'default' enter sequence for state Normal */
void MouseSelect::enseq_main_region_Normal_default()
{
	/* 'default' enter sequence for state Normal */
	stateConfVector[0] = MouseSelect::State::main_region_Normal;
}

/* 'default' enter sequence for state Selection */
void MouseSelect::enseq_main_region_Selection_default()
{
	/* 'default' enter sequence for state Selection */
	stateConfVector[0] = MouseSelect::State::main_region_Selection;
}

/* 'default' enter sequence for state Moving */
void MouseSelect::enseq_main_region_Moving_default()
{
	/* 'default' enter sequence for state Moving */
	enact_main_region_Moving();
	stateConfVector[0] = MouseSelect::State::main_region_Moving;
}

/* 'default' enter sequence for state Panning */
void MouseSelect::enseq_main_region_Panning_default()
{
	/* 'default' enter sequence for state Panning */
	enact_main_region_Panning();
	stateConfVector[0] = MouseSelect::State::main_region_Panning;
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
	stateConfVector[0] = MouseSelect::State::NO_STATE;
}

/* Default exit sequence for state Selection */
void MouseSelect::exseq_main_region_Selection()
{
	/* Default exit sequence for state Selection */
	stateConfVector[0] = MouseSelect::State::NO_STATE;
}

/* Default exit sequence for state Moving */
void MouseSelect::exseq_main_region_Moving()
{
	/* Default exit sequence for state Moving */
	stateConfVector[0] = MouseSelect::State::NO_STATE;
	exact_main_region_Moving();
}

/* Default exit sequence for state Panning */
void MouseSelect::exseq_main_region_Panning()
{
	/* Default exit sequence for state Panning */
	stateConfVector[0] = MouseSelect::State::NO_STATE;
}

/* Default exit sequence for region main region */
void MouseSelect::exseq_main_region()
{
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of MouseSelect.main_region) at position 0... */
	switch(stateConfVector[ 0 ])
	{
		case MouseSelect::State::main_region_Normal :
		{
			exseq_main_region_Normal();
			break;
		}
		case MouseSelect::State::main_region_Selection :
		{
			exseq_main_region_Selection();
			break;
		}
		case MouseSelect::State::main_region_Moving :
		{
			exseq_main_region_Moving();
			break;
		}
		case MouseSelect::State::main_region_Panning :
		{
			exseq_main_region_Panning();
			break;
		}
		default:
			/* do nothing */
			break;
	}
}

/* Default react sequence for initial entry  */
void MouseSelect::react_main_region__entry_Default()
{
	/* Default react sequence for initial entry  */
	enseq_main_region_Normal_default();
}

sc::integer MouseSelect::react(const sc::integer transitioned_before) {
	/* State machine reactions. */
	return transitioned_before;
}

sc::integer MouseSelect::main_region_Normal_react(const sc::integer transitioned_before) {
	/* The reactions of state Normal. */
	sc::integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (0))
	{ 
		if (((ifaceGui.onSelect_raised)) && ((!(ifaceView.ifaceViewOperationCallback->is3D()))))
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
				ifaceGui.selectionEnd_raised = true;
				ifaceGui.selectionEnd_observable.next();
				enseq_main_region_Normal_default();
				react(0);
				transitioned_after = 0;
			}  else
			{
				if (((ifaceGui.mouseDown_raised)) && ((!(ifaceView.ifaceViewOperationCallback->is3D()))))
				{ 
					exseq_main_region_Normal();
					setP2(ifaceGui.mouseDown_value);
					ifaceView.ifaceViewOperationCallback->cursorPanning();
					enseq_main_region_Panning_default();
					react(0);
					transitioned_after = 0;
				} 
			}
		}
	} 
	/* If no transition was taken */
	if ((transitioned_after) == (transitioned_before))
	{ 
		/* then execute local reactions. */
		transitioned_after = react(transitioned_before);
	} 
	return transitioned_after;
}

sc::integer MouseSelect::main_region_Selection_react(const sc::integer transitioned_before) {
	/* The reactions of state Selection. */
	sc::integer transitioned_after = transitioned_before;
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
				setP1(ifaceGui.mouseDown_value);
				setP2(ifaceGui.mouseDown_value);
				ifaceView.ifaceViewOperationCallback->show();
				enseq_main_region_Moving_default();
				react(0);
				transitioned_after = 0;
			} 
		}
	} 
	/* If no transition was taken */
	if ((transitioned_after) == (transitioned_before))
	{ 
		/* then execute local reactions. */
		transitioned_after = react(transitioned_before);
	} 
	return transitioned_after;
}

sc::integer MouseSelect::main_region_Moving_react(const sc::integer transitioned_before) {
	/* The reactions of state Moving. */
	sc::integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (0))
	{ 
		if (ifaceGui.mouseUp_raised)
		{ 
			exseq_main_region_Moving();
			ifaceGui.selectionEnd_raised = true;
			ifaceGui.selectionEnd_observable.next();
			ifaceView.ifaceViewOperationCallback->hide();
			ifaceView.ifaceViewOperationCallback->select(p1.x, p1.y, p2.x, p2.y);
			ifaceGui.ifaceGuiOperationCallback->updateScrolling();
			enseq_main_region_Normal_default();
			react(0);
			transitioned_after = 0;
		}  else
		{
			if (ifaceGui.mouseMove_raised)
			{ 
				exseq_main_region_Moving();
				setP2(ifaceGui.mouseMove_value);
				enseq_main_region_Moving_default();
				react(0);
				transitioned_after = 0;
			} 
		}
	} 
	/* If no transition was taken */
	if ((transitioned_after) == (transitioned_before))
	{ 
		/* then execute local reactions. */
		transitioned_after = react(transitioned_before);
	} 
	return transitioned_after;
}

sc::integer MouseSelect::main_region_Panning_react(const sc::integer transitioned_before) {
	/* The reactions of state Panning. */
	sc::integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (0))
	{ 
		if (ifaceGui.mouseMove_raised)
		{ 
			exseq_main_region_Panning();
			setP2(ifaceGui.mouseMove_value);
			ifaceView.ifaceViewOperationCallback->move((p2.x - p1.x), (p2.y - p1.y));
			enseq_main_region_Panning_default();
			react(0);
			transitioned_after = 0;
		}  else
		{
			if (ifaceGui.mouseUp_raised)
			{ 
				exseq_main_region_Panning();
				ifaceView.ifaceViewOperationCallback->cursorArrow();
				ifaceGui.ifaceGuiOperationCallback->updateScrolling();
				enseq_main_region_Normal_default();
				react(0);
				transitioned_after = 0;
			} 
		}
	} 
	/* If no transition was taken */
	if ((transitioned_after) == (transitioned_before))
	{ 
		/* then execute local reactions. */
		transitioned_after = react(transitioned_before);
	} 
	return transitioned_after;
}

void MouseSelect::clearOutEvents() noexcept {
	ifaceGui.selectionEnd_raised = false;
}

void MouseSelect::clearInEvents() noexcept {
	ifaceGui.onSelect_raised = false;
	ifaceGui.onDisable_raised = false;
	ifaceGui.mouseDown_raised = false;
	ifaceGui.mouseMove_raised = false;
	ifaceGui.mouseUp_raised = false;
}

void MouseSelect::microStep() {
	switch(stateConfVector[ 0 ])
	{
		case MouseSelect::State::main_region_Normal :
		{
			main_region_Normal_react(-1);
			break;
		}
		case MouseSelect::State::main_region_Selection :
		{
			main_region_Selection_react(-1);
			break;
		}
		case MouseSelect::State::main_region_Moving :
		{
			main_region_Moving_react(-1);
			break;
		}
		case MouseSelect::State::main_region_Panning :
		{
			main_region_Panning_react(-1);
			break;
		}
		default:
			/* do nothing */
			break;
	}
}

void MouseSelect::runCycle() {
	/* Performs a 'run to completion' step. */
	if (isExecuting)
	{ 
		return;
	} 
	isExecuting = true;
	clearOutEvents();
	dispatchEvent(getNextEvent());
	do
	{ 
		microStep();
		clearInEvents();
	} while (dispatchEvent(getNextEvent()));
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
	stateConfVector[0] = MouseSelect::State::NO_STATE;
	isExecuting = false;
}

/* Can be used by the client code to trigger a run to completion step without raising an event. */
void MouseSelect::triggerWithoutEvent() {
	runCycle();
}


