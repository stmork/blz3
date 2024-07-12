/* Generated with YAKINDU statechart tools
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText:  Copyright (C)  2024  Steffen A. Mork
 *               All rights reserved */

#ifndef MOUSESELECT_H_
#define MOUSESELECT_H_

/*!
Forward declaration for the MouseSelect state machine.
*/
class MouseSelect;


#include <point.h>
#include <deque>
#include "sc_types.h"
#include "sc_rxcpp.h"
#include "sc_statemachine.h"
#include "sc_eventdriven.h"
#include <string.h>

/*! \file
Header of the state machine 'MouseSelect'.
*/


class MouseSelect : public sc::EventDrivenInterface
{
public:
	MouseSelect() noexcept;

	virtual ~MouseSelect();



	/*! Enumeration of all states. */
	enum class State
	{
		NO_STATE,
		main_region_Normal,
		main_region_Selection,
		main_region_Moving,
		main_region_Panning
	};

	/*! The number of states. */
	static constexpr const sc::integer numStates {4};
	static constexpr const sc::integer scvi_main_region_Normal {0};
	static constexpr const sc::integer scvi_main_region_Selection {0};
	static constexpr const sc::integer scvi_main_region_Moving {0};
	static constexpr const sc::integer scvi_main_region_Panning {0};

	/*! Enumeration of all events which are consumed. */
	enum class Event
	{
		NO_EVENT,
		Gui_onSelect,
		Gui_onDisable,
		Gui_mouseDown,
		Gui_mouseMove,
		Gui_mouseUp
	};

	class EventInstance
	{
	public:
		explicit  EventInstance(Event id) noexcept : eventId(id) {}
		virtual ~EventInstance() = default;
		const Event eventId;
	};
	template <typename T>
	class EventInstanceWithValue : public EventInstance
	{
	public:
		explicit  EventInstanceWithValue(Event id, T val) noexcept :
			EventInstance(id),
			value(val)
		{}
		virtual ~EventInstanceWithValue() = default;
		const T value;
	};


	//! Inner class for gui interface scope.
	class Gui
	{
	public:
		explicit Gui(MouseSelect * parent) noexcept;








		/*! Raises the in event 'onSelect' of interface scope 'gui'. */
		void raiseOnSelect();
		/*! Raises the in event 'onDisable' of interface scope 'gui'. */
		void raiseOnDisable();
		/*! Raises the in event 'mouseDown' of interface scope 'gui'. */
		void raiseMouseDown(SCT_point mouseDown_);
		/*! Raises the in event 'mouseMove' of interface scope 'gui'. */
		void raiseMouseMove(SCT_point mouseMove_);
		/*! Raises the in event 'mouseUp' of interface scope 'gui'. */
		void raiseMouseUp(SCT_point mouseUp_);
		/*! Check if event 'selectionEnd' of interface scope 'gui' is raised. */
		bool isRaisedSelectionEnd() noexcept;
		/*! Get observable for event 'selectionEnd' of interface scope 'gui'. */
		sc::rx::Observable<void> & getSelectionEnd() noexcept;

		//! Inner class for gui interface scope operation callbacks.
		class OperationCallback
		{
		public:
			virtual ~OperationCallback() = 0;

			virtual void updateScrolling() = 0;


		};

		/*! Set the working instance of the operation callback interface 'OperationCallback'. */
		void setOperationCallback(OperationCallback * operationCallback) noexcept;


	private:
		friend class MouseSelect;


		/*! Indicates event 'onSelect' of interface scope 'gui' is active. */
		bool onSelect_raised {false};
		/*! Indicates event 'onDisable' of interface scope 'gui' is active. */
		bool onDisable_raised {false};
		/*! Indicates event 'mouseDown' of interface scope 'gui' is active. */
		bool mouseDown_raised {false};
		/*! Value of event 'mouseDown' of interface scope 'gui'. */
		SCT_point mouseDown_value {};
		/*! Indicates event 'mouseMove' of interface scope 'gui' is active. */
		bool mouseMove_raised {false};
		/*! Value of event 'mouseMove' of interface scope 'gui'. */
		SCT_point mouseMove_value {};
		/*! Indicates event 'mouseUp' of interface scope 'gui' is active. */
		bool mouseUp_raised {false};
		/*! Value of event 'mouseUp' of interface scope 'gui'. */
		SCT_point mouseUp_value {};
		/*! Indicates event 'selectionEnd' of interface scope 'gui' is active. */
		bool selectionEnd_raised {false};
		/*! Observable for event 'selectionEnd' of interface scope 'gui'. */
		sc::rx::Observable<void> selectionEnd_observable = sc::rx::Observable<void> {};

		MouseSelect * parent;



		OperationCallback * ifaceGuiOperationCallback;


	};

	/*! Returns an instance of the interface class 'Gui'. */
	Gui & gui() noexcept;

	//! Inner class for view interface scope.
	class View
	{
	public:
		explicit View(MouseSelect * parent) noexcept;










		//! Inner class for view interface scope operation callbacks.
		class OperationCallback
		{
		public:
			virtual ~OperationCallback() = 0;

			virtual bool is3D() = 0;

			virtual void show() = 0;

			virtual void hide() = 0;

			virtual void cursorPanning() = 0;

			virtual void cursorArrow() = 0;

			virtual void setRectangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2) = 0;

			virtual void select(int32_t x1, int32_t y1, int32_t x2, int32_t y2) = 0;

			virtual void move(int32_t dx, int32_t dy) = 0;


		};

		/*! Set the working instance of the operation callback interface 'OperationCallback'. */
		void setOperationCallback(OperationCallback * operationCallback) noexcept;


	private:
		friend class MouseSelect;



		MouseSelect * parent;



		OperationCallback * ifaceViewOperationCallback;


	};

	/*! Returns an instance of the interface class 'View'. */
	View & view() noexcept;


	/*! Can be used by the client code to trigger a run to completion step without raising an event. */
	void triggerWithoutEvent() override;

	/*
	 * Functions inherited from StatemachineInterface
	 */
	void enter() override;

	void exit() override;

	/*!
	 * Checks if the state machine is active (until 2.4.1 this method was used for states).
	 * A state machine is active if it has been entered. It is inactive if it has not been entered at all or if it has been exited.
	 */
	bool isActive() const noexcept override;


	/*!
	* Checks if all active states are final.
	* If there are no active states then the state machine is considered being inactive. In this case this method returns false.
	*/
	bool isFinal() const noexcept override;

	/*!
	 * Checks if member of the state machine must be set. For example an operation callback.
	 */
	bool check() const noexcept;


	/*! Checks if the specified state is active (until 2.4.1 the used method for states was calles isActive()). */
	bool isStateActive(State state) const noexcept;



protected:


	std::deque<EventInstance *> incomingEventQueue;

	EventInstance * getNextEvent() noexcept;

	bool dispatchEvent(EventInstance * event) noexcept;



private:
	MouseSelect(const MouseSelect & rhs);
	MouseSelect & operator=(const MouseSelect &);

	SCT_point p1 {};
	SCT_point p2 {};



	//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
	static const sc::ushort maxOrthogonalStates {1};



	State stateConfVector[maxOrthogonalStates];


	Gui ifaceGui {Gui{nullptr}};
	View ifaceView {View{nullptr}};


	bool isExecuting {false};



	// prototypes of all internal functions

	void enact_main_region_Moving();
	void enact_main_region_Panning();
	void exact_main_region_Moving();
	void enseq_main_region_Normal_default();
	void enseq_main_region_Selection_default();
	void enseq_main_region_Moving_default();
	void enseq_main_region_Panning_default();
	void enseq_main_region_default();
	void exseq_main_region_Normal();
	void exseq_main_region_Selection();
	void exseq_main_region_Moving();
	void exseq_main_region_Panning();
	void exseq_main_region();
	void react_main_region__entry_Default();
	sc::integer react(const sc::integer transitioned_before);
	sc::integer main_region_Normal_react(const sc::integer transitioned_before);
	sc::integer main_region_Selection_react(const sc::integer transitioned_before);
	sc::integer main_region_Moving_react(const sc::integer transitioned_before);
	sc::integer main_region_Panning_react(const sc::integer transitioned_before);
	void clearOutEvents() noexcept;
	void clearInEvents() noexcept;
	void microStep();
	void runCycle();


	/*! Sets the value of the variable 'p1' that is defined in the internal scope. */
	void setP1(SCT_point p1) noexcept;
	/*! Sets the value of the variable 'p2' that is defined in the internal scope. */
	void setP2(SCT_point p2) noexcept;



};


inline MouseSelect::Gui::OperationCallback::~OperationCallback() {}
inline MouseSelect::View::OperationCallback::~OperationCallback() {}


#endif /* MOUSESELECT_H_ */
