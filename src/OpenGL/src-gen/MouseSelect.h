/* Generated with YAKINDU statechart tools
 *
 *   (C) Copyright  2001 - 2021  Steffen A. Mork
 *               All rights reserved */

#ifndef MOUSESELECT_H_
#define MOUSESELECT_H_

/*!
 * Forward declaration for the MouseSelect state machine.
 */
class MouseSelect;


#include "../point.h"
#include <deque>
#include "../src-lib/sc_types.h"
#include "../src-lib/sc_rxcpp.h"
#include "../src-lib/sc_statemachine.h"

/*! \file Header of the state machine 'MouseSelect'.
*/


#ifndef SCT_EVENTS_MOUSESELECT_H
#define SCT_EVENTS_MOUSESELECT_H
#ifndef SC_INVALID_EVENT_VALUE
#define SC_INVALID_EVENT_VALUE 0
#endif

namespace mouseselect_events
{
	typedef enum
	{
		invalid_event = SC_INVALID_EVENT_VALUE,
		Gui_onSelect,
		Gui_onDisable,
		Gui_mouseDown,
		Gui_mouseMove,
		Gui_mouseUp
	} MouseSelectEventName;

	class SctEvent
	{
	public:
		explicit SctEvent(MouseSelectEventName n_name) : name(n_name) {};
		virtual ~SctEvent() = default;
		const MouseSelectEventName name;

	};

	template <typename T>
	class TypedSctEvent : public SctEvent
	{
	public:
		explicit TypedSctEvent(MouseSelectEventName n_name, T n_value) :
			SctEvent(n_name),
			value(n_value) {};
		virtual ~TypedSctEvent() = default;
		const T value;
	};

	class SctEvent_Gui_onSelect : public SctEvent
	{
	public:
		explicit SctEvent_Gui_onSelect(MouseSelectEventName n_name) : SctEvent(n_name) {};
	};
	class SctEvent_Gui_onDisable : public SctEvent
	{
	public:
		explicit SctEvent_Gui_onDisable(MouseSelectEventName n_name) : SctEvent(n_name) {};
	};
	class SctEvent_Gui_mouseDown : public TypedSctEvent<SCT_point>
	{
	public:
		explicit SctEvent_Gui_mouseDown(MouseSelectEventName n_name, SCT_point n_value) :
			TypedSctEvent(n_name, n_value) {};
	};
	class SctEvent_Gui_mouseMove : public TypedSctEvent<SCT_point>
	{
	public:
		explicit SctEvent_Gui_mouseMove(MouseSelectEventName n_name, SCT_point n_value) :
			TypedSctEvent(n_name, n_value) {};
	};
	class SctEvent_Gui_mouseUp : public TypedSctEvent<SCT_point>
	{
	public:
		explicit SctEvent_Gui_mouseUp(MouseSelectEventName n_name, SCT_point n_value) :
			TypedSctEvent(n_name, n_value) {};
	};

}
#endif /* SCT_EVENTS_MOUSESELECT_H */


/*! Define indices of states in the StateConfVector */
#define SCVI_MAIN_REGION_NORMAL 0
#define SCVI_MAIN_REGION_SELECTION 0
#define SCVI_MAIN_REGION_MOVING 0
#define SCVI_MAIN_REGION_PANNING 0


class MouseSelect : public sc::StatemachineInterface
{
public:
	MouseSelect();

	virtual ~MouseSelect();

	/*! Enumeration of all states */
	typedef enum
	{
		MouseSelect_last_state,
		main_region_Normal,
		main_region_Selection,
		main_region_Moving,
		main_region_Panning
	} MouseSelectStates;

	static const sc_integer numStates = 4;

	//! Inner class for gui interface scope.
	class Gui
	{
	public:
		explicit Gui(MouseSelect * parent);

		/*! Raises the in event 'onSelect' that is defined in the interface scope 'gui'. */
		void raiseOnSelect();


		/*! Raises the in event 'onDisable' that is defined in the interface scope 'gui'. */
		void raiseOnDisable();


		/*! Raises the in event 'mouseDown' that is defined in the interface scope 'gui'. */
		void raiseMouseDown(SCT_point value);


		/*! Raises the in event 'mouseMove' that is defined in the interface scope 'gui'. */
		void raiseMouseMove(SCT_point value);


		/*! Raises the in event 'mouseUp' that is defined in the interface scope 'gui'. */
		void raiseMouseUp(SCT_point value);


		/*! Gets the observable of the out event 'selectionEnd' that is defined in the interface scope 'gui'. */
		sc::rx::Observable<void> * getSelectionEnd();



		//! Inner class for gui interface scope operation callbacks.
		class OperationCallback
		{
		public:
			virtual ~OperationCallback() = 0;

			virtual void updateScrolling() = 0;


		};

		/*! Set the working instance of the operation callback interface 'OperationCallback'. */
		void setOperationCallback(OperationCallback * operationCallback);


	private:
		friend class MouseSelect;

		/*! Raises the in event 'onSelect' that is defined in the interface scope 'gui'. */
		void internal_raiseOnSelect();
		sc_boolean onSelect_raised;
		/*! Raises the in event 'onDisable' that is defined in the interface scope 'gui'. */
		void internal_raiseOnDisable();
		sc_boolean onDisable_raised;
		/*! Raises the in event 'mouseDown' that is defined in the interface scope 'gui'. */
		void internal_raiseMouseDown(SCT_point value);
		sc_boolean mouseDown_raised;
		SCT_point mouseDown_value;
		/*! Raises the in event 'mouseMove' that is defined in the interface scope 'gui'. */
		void internal_raiseMouseMove(SCT_point value);
		sc_boolean mouseMove_raised;
		SCT_point mouseMove_value;
		/*! Raises the in event 'mouseUp' that is defined in the interface scope 'gui'. */
		void internal_raiseMouseUp(SCT_point value);
		sc_boolean mouseUp_raised;
		SCT_point mouseUp_value;
		sc::rx::Observable<void> selectionEnd_observable;
		sc_boolean selectionEnd_raised;
		void dispatch_event(mouseselect_events::SctEvent * event);

		MouseSelect * parent;



		OperationCallback * ifaceGuiOperationCallback;


	};

	/*! Returns an instance of the interface class 'Gui'. */
	Gui * gui();

	//! Inner class for view interface scope.
	class View
	{
	public:
		explicit View(MouseSelect * n_parent);









		//! Inner class for view interface scope operation callbacks.
		class OperationCallback
		{
		public:
			virtual ~OperationCallback() = 0;

			virtual sc_boolean is3D() = 0;

			virtual void show() = 0;

			virtual void hide() = 0;

			virtual void cursorPanning() = 0;

			virtual void cursorArrow() = 0;

			virtual void setRectangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2) = 0;

			virtual void select(int32_t x1, int32_t y1, int32_t x2, int32_t y2) = 0;

			virtual void move(int32_t dx, int32_t dy) = 0;


		};

		/*! Set the working instance of the operation callback interface 'OperationCallback'. */
		void setOperationCallback(OperationCallback * operationCallback);


	private:
		friend class MouseSelect;

		void dispatch_event(mouseselect_events::SctEvent * event);

		MouseSelect * parent;



		OperationCallback * ifaceViewOperationCallback;


	};

	/*! Returns an instance of the interface class 'View'. */
	View * view();


	/*
	 * Functions inherited from StatemachineInterface
	 */
	virtual void enter() override;

	virtual void exit() override;

	/*!
	 * Checks if the state machine is active (until 2.4.1 this method was used for states).
	 * A state machine is active if it has been entered. It is inactive if it has not been entered at all or if it has been exited.
	 */
	virtual sc_boolean isActive() const override;


	/*!
	* Checks if all active states are final.
	* If there are no active states then the state machine is considered being inactive. In this case this method returns false.
	*/
	virtual sc_boolean isFinal() const override;

	/*!
	 * Checks if member of the state machine must be set. For example an operation callback.
	 */
	sc_boolean check();


	/*! Checks if the specified state is active (until 2.4.1 the used method for states was calles isActive()). */
	sc_boolean isStateActive(MouseSelectStates state) const;



protected:


private:
	MouseSelect(const MouseSelect & rhs);
	MouseSelect & operator=(const MouseSelect &);

	SCT_point p1;
	SCT_point p2;
	void internal_dispatch_event(mouseselect_events::SctEvent * event);


	//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
	static const sc_ushort maxOrthogonalStates = 1;



	MouseSelectStates stateConfVector[maxOrthogonalStates];


	Gui ifaceGui;
	View ifaceView;


	sc_boolean isExecuting;

	/*! Gets the value of the variable 'p1' that is defined in the internal scope. */
	SCT_point getP1() const;

	/*! Sets the value of the variable 'p1' that is defined in the internal scope. */
	void setP1(SCT_point value);

	/*! Gets the value of the variable 'p2' that is defined in the internal scope. */
	SCT_point getP2() const;

	/*! Sets the value of the variable 'p2' that is defined in the internal scope. */
	void setP2(SCT_point value);


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
	sc_integer react(const sc_integer transitioned_before);
	sc_integer main_region_Normal_react(const sc_integer transitioned_before);
	sc_integer main_region_Selection_react(const sc_integer transitioned_before);
	sc_integer main_region_Moving_react(const sc_integer transitioned_before);
	sc_integer main_region_Panning_react(const sc_integer transitioned_before);
	void clearInEvents();
	void microStep();
	void runCycle();


	mouseselect_events::SctEvent * getNextEvent();
	void dispatch_event(mouseselect_events::SctEvent * event);
	std::deque<mouseselect_events::SctEvent *> inEventQueue;


};


inline MouseSelect::Gui::OperationCallback::~OperationCallback() {}
inline MouseSelect::View::OperationCallback::~OperationCallback() {}


#endif /* MOUSESELECT_H_ */
