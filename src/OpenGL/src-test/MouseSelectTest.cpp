/** Generated with YAKINDU statechart tools
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText:  Copyright (C)  2026  Steffen A. Mork
 *               All rights reserved */
#include <string>
#include <list>
#include <algorithm>
#include "gtest/gtest.h"
#include "MouseSelect.h"
#include "sc_runner.h"
#include "sc_types.h"

#ifndef SC_UNUSED
#define SC_UNUSED(P) (void)P
#endif

namespace  {

class MouseSelectTest : public ::testing::Test{
	public:
		
	protected:
	MouseSelect* statechart;
	
	
	public:
	class ViewIs3DMock{
		typedef bool (ViewIs3DMock::*functiontype)();
		public:
		MouseSelectTest* owner;
		bool (ViewIs3DMock::*viewIs3DBehaviorDefault)();
		
		ViewIs3DMock(MouseSelectTest* owner) : 
			owner(owner),
			viewIs3DBehaviorDefault(0)
			{}
		
	
		bool is3D1(){
			return (false);
		}
	
		bool is3D2(){
			return (true);
		}
	
		bool is3DDefault(){
			bool defaultValue = false;
			return (defaultValue);
		}
	
		functiontype getBehavior(){
			return viewIs3DBehaviorDefault;
		}
		
		void setDefaultBehavior(bool (ViewIs3DMock::*defaultBehavior)()){
			viewIs3DBehaviorDefault = defaultBehavior;
		}
		
		void initializeBehavior() {
			setDefaultBehavior(&ViewIs3DMock::is3DDefault);
		}
		
		void reset() {
			initializeBehavior();
		}
	};
	ViewIs3DMock* viewIs3DMock;
	
	class ViewShowMock{
		typedef void (ViewShowMock::*functiontype)();
		public:
		MouseSelectTest* owner;
		void (ViewShowMock::*viewShowBehaviorDefault)();
		int callCount;
		
		ViewShowMock(MouseSelectTest* owner) : 
			owner(owner),
			viewShowBehaviorDefault(0),
			callCount(0)
			{}
		
	
		void show1(){
		}
	
		void showDefault(){
		}
	
		bool calledAtLeast(const int times){
			return (callCount >= times);
		}
		
		bool calledAtLeastOnce(){
			return (callCount>0);
		}
	
		void show() {
			++callCount;
		}
	
		functiontype getBehavior(){
			return viewShowBehaviorDefault;
		}
		
		void setDefaultBehavior(void (ViewShowMock::*defaultBehavior)()){
			viewShowBehaviorDefault = defaultBehavior;
		}
		
		void initializeBehavior() {
			setDefaultBehavior(&ViewShowMock::showDefault);
		}
		
		void reset() {
			initializeBehavior();
			callCount = 0;
		}
	};
	ViewShowMock* viewShowMock;
	
	class ViewHideMock{
		typedef void (ViewHideMock::*functiontype)();
		public:
		MouseSelectTest* owner;
		void (ViewHideMock::*viewHideBehaviorDefault)();
		int callCount;
		
		ViewHideMock(MouseSelectTest* owner) : 
			owner(owner),
			viewHideBehaviorDefault(0),
			callCount(0)
			{}
		
	
		void hide1(){
		}
	
		void hideDefault(){
		}
	
		bool calledAtLeast(const int times){
			return (callCount >= times);
		}
		
		bool calledAtLeastOnce(){
			return (callCount>0);
		}
	
		void hide() {
			++callCount;
		}
	
		functiontype getBehavior(){
			return viewHideBehaviorDefault;
		}
		
		void setDefaultBehavior(void (ViewHideMock::*defaultBehavior)()){
			viewHideBehaviorDefault = defaultBehavior;
		}
		
		void initializeBehavior() {
			setDefaultBehavior(&ViewHideMock::hideDefault);
		}
		
		void reset() {
			initializeBehavior();
			callCount = 0;
		}
	};
	ViewHideMock* viewHideMock;
	
	class ViewSetRectangleMock{
		struct parameters {
			int32_t x1;
			int32_t y1;
			int32_t x2;
			int32_t y2;
			int callCount;
			inline bool operator==(const parameters& other) {
				return (this->x1 == other.x1)&&(this->y1 == other.y1)&&(this->x2 == other.x2)&&(this->y2 == other.y2);
			}
		};
		public:
		MouseSelectTest* owner;
		std::list<ViewSetRectangleMock::parameters> paramCount;
		int callCount;
		
		ViewSetRectangleMock(MouseSelectTest* owner) : 
			owner(owner),
			callCount(0)
			{}
		
	
		bool calledAtLeast(const int times){
			return (callCount >= times);
		}
		
		bool calledAtLeastOnce(){
			return (callCount>0);
		}
	
		bool calledAtLeast(const int times, const int32_t x1, const int32_t y1, const int32_t x2, const int32_t y2){
			parameters p;
			p.x1 = x1;
			p.y1 = y1;
			p.x2 = x2;
			p.y2 = y2;
			
			std::list<ViewSetRectangleMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
			if(i != paramCount.end()) {
				return (i->callCount >= times);
			}else{
				return false;
			}
		}
		
		bool calledAtLeastOnce(const int32_t x1, const int32_t y1, const int32_t x2, const int32_t y2){
			parameters p;
			p.x1 = x1;
			p.y1 = y1;
			p.x2 = x2;
			p.y2 = y2;
			
			std::list<ViewSetRectangleMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
			if(i != paramCount.end()) {
				return (i->callCount > 0);
			}else{
				return false;
			}
		}
	
		void setRectangle(const int32_t x1, const int32_t y1, const int32_t x2, const int32_t y2) {
			++callCount;
			
			parameters p;
			p.x1 = x1;
			p.y1 = y1;
			p.x2 = x2;
			p.y2 = y2;
			
			std::list<ViewSetRectangleMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
			if(i != paramCount.end()) {
				p.callCount = (++i->callCount);
				paramCount.erase(i);
				
			}else{
				p.callCount = 1;
			}
			paramCount.push_back(p);
		}
		void reset() {
			callCount = 0;
			paramCount.clear();
		}
	};
	ViewSetRectangleMock* viewSetRectangleMock;
	
	class ViewSelectMock{
		struct parameters {
			int32_t x1;
			int32_t y1;
			int32_t x2;
			int32_t y2;
			int callCount;
			inline bool operator==(const parameters& other) {
				return (this->x1 == other.x1)&&(this->y1 == other.y1)&&(this->x2 == other.x2)&&(this->y2 == other.y2);
			}
		};
		public:
		MouseSelectTest* owner;
		std::list<ViewSelectMock::parameters> paramCount;
		int callCount;
		
		ViewSelectMock(MouseSelectTest* owner) : 
			owner(owner),
			callCount(0)
			{}
		
	
		bool calledAtLeast(const int times){
			return (callCount >= times);
		}
		
		bool calledAtLeastOnce(){
			return (callCount>0);
		}
	
		bool calledAtLeast(const int times, const int32_t x1, const int32_t y1, const int32_t x2, const int32_t y2){
			parameters p;
			p.x1 = x1;
			p.y1 = y1;
			p.x2 = x2;
			p.y2 = y2;
			
			std::list<ViewSelectMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
			if(i != paramCount.end()) {
				return (i->callCount >= times);
			}else{
				return false;
			}
		}
		
		bool calledAtLeastOnce(const int32_t x1, const int32_t y1, const int32_t x2, const int32_t y2){
			parameters p;
			p.x1 = x1;
			p.y1 = y1;
			p.x2 = x2;
			p.y2 = y2;
			
			std::list<ViewSelectMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
			if(i != paramCount.end()) {
				return (i->callCount > 0);
			}else{
				return false;
			}
		}
	
		void select(const int32_t x1, const int32_t y1, const int32_t x2, const int32_t y2) {
			++callCount;
			
			parameters p;
			p.x1 = x1;
			p.y1 = y1;
			p.x2 = x2;
			p.y2 = y2;
			
			std::list<ViewSelectMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
			if(i != paramCount.end()) {
				p.callCount = (++i->callCount);
				paramCount.erase(i);
				
			}else{
				p.callCount = 1;
			}
			paramCount.push_back(p);
		}
		void reset() {
			callCount = 0;
			paramCount.clear();
		}
	};
	ViewSelectMock* viewSelectMock;
	
	class ViewCursorPanningMock{
		public:
		MouseSelectTest* owner;
		int callCount;
		
		ViewCursorPanningMock(MouseSelectTest* owner) : 
			owner(owner),
			callCount(0)
			{}
		
	
		bool calledAtLeast(const int times){
			return (callCount >= times);
		}
		
		bool calledAtLeastOnce(){
			return (callCount>0);
		}
	
		void cursorPanning() {
			++callCount;
		}
		void reset() {
			callCount = 0;
		}
	};
	ViewCursorPanningMock* viewCursorPanningMock;
	
	class ViewCursorArrowMock{
		public:
		MouseSelectTest* owner;
		int callCount;
		
		ViewCursorArrowMock(MouseSelectTest* owner) : 
			owner(owner),
			callCount(0)
			{}
		
	
		bool calledAtLeast(const int times){
			return (callCount >= times);
		}
		
		bool calledAtLeastOnce(){
			return (callCount>0);
		}
	
		void cursorArrow() {
			++callCount;
		}
		void reset() {
			callCount = 0;
		}
	};
	ViewCursorArrowMock* viewCursorArrowMock;
	
	class GuiUpdateScrollingMock{
		public:
		MouseSelectTest* owner;
		int callCount;
		
		GuiUpdateScrollingMock(MouseSelectTest* owner) : 
			owner(owner),
			callCount(0)
			{}
		
	
		bool calledAtLeast(const int times){
			return (callCount >= times);
		}
		
		bool calledAtLeastOnce(){
			return (callCount>0);
		}
	
		void updateScrolling() {
			++callCount;
		}
		void reset() {
			callCount = 0;
		}
	};
	GuiUpdateScrollingMock* guiUpdateScrollingMock;
	
	class ViewMoveMock{
		struct parameters {
			int32_t dx;
			int32_t dy;
			int callCount;
			inline bool operator==(const parameters& other) {
				return (this->dx == other.dx)&&(this->dy == other.dy);
			}
		};
		public:
		MouseSelectTest* owner;
		std::list<ViewMoveMock::parameters> paramCount;
		int callCount;
		
		ViewMoveMock(MouseSelectTest* owner) : 
			owner(owner),
			callCount(0)
			{}
		
	
		bool calledAtLeast(const int times){
			return (callCount >= times);
		}
		
		bool calledAtLeastOnce(){
			return (callCount>0);
		}
	
		bool calledAtLeast(const int times, const int32_t dx, const int32_t dy){
			parameters p;
			p.dx = dx;
			p.dy = dy;
			
			std::list<ViewMoveMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
			if(i != paramCount.end()) {
				return (i->callCount >= times);
			}else{
				return false;
			}
		}
		
		bool calledAtLeastOnce(const int32_t dx, const int32_t dy){
			parameters p;
			p.dx = dx;
			p.dy = dy;
			
			std::list<ViewMoveMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
			if(i != paramCount.end()) {
				return (i->callCount > 0);
			}else{
				return false;
			}
		}
	
		void move(const int32_t dx, const int32_t dy) {
			++callCount;
			
			parameters p;
			p.dx = dx;
			p.dy = dy;
			
			std::list<ViewMoveMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
			if(i != paramCount.end()) {
				p.callCount = (++i->callCount);
				paramCount.erase(i);
				
			}else{
				p.callCount = 1;
			}
			paramCount.push_back(p);
		}
		void reset() {
			callCount = 0;
			paramCount.clear();
		}
	};
	ViewMoveMock* viewMoveMock;
	
	class MockGui : public MouseSelect::Gui::OperationCallback {
		public:
		MouseSelectTest* owner;
		MockGui(MouseSelectTest* owner) : owner(owner) {}
		virtual ~MockGui() = default;
		void updateScrolling() {
			owner->guiUpdateScrollingMock->updateScrolling();
		}
	};
	class MockView : public MouseSelect::View::OperationCallback {
		public:
		MouseSelectTest* owner;
		MockView(MouseSelectTest* owner) : owner(owner) {}
		virtual ~MockView() = default;
		bool is3D() {
			return (owner->viewIs3DMock->*(owner->viewIs3DMock->getBehavior()))();
		}
		void show() {
			owner->viewShowMock->show();
			return (owner->viewShowMock->*(owner->viewShowMock->getBehavior()))();
		}
		void hide() {
			owner->viewHideMock->hide();
			return (owner->viewHideMock->*(owner->viewHideMock->getBehavior()))();
		}
		void cursorPanning() {
			owner->viewCursorPanningMock->cursorPanning();
		}
		void cursorArrow() {
			owner->viewCursorArrowMock->cursorArrow();
		}
		void setRectangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
			owner->viewSetRectangleMock->setRectangle(x1, y1, x2, y2);
		}
		void select(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
			owner->viewSelectMock->select(x1, y1, x2, y2);
		}
		void move(int32_t dx, int32_t dy) {
			owner->viewMoveMock->move(dx, dy);
		}
	};
	
	//! The timers are managed by a timer service. */
	SctUnitRunner * runner;
	
	MockGui* guiMock;
	MockView* viewMock;
	
	virtual void SetUp() {
		statechart = new MouseSelect();
		runner = new SctUnitRunner(
		);
		viewIs3DMock = new ViewIs3DMock(this);
		viewIs3DMock->initializeBehavior();
		viewShowMock = new ViewShowMock(this);
		viewShowMock->initializeBehavior();
		viewHideMock = new ViewHideMock(this);
		viewHideMock->initializeBehavior();
		viewSetRectangleMock = new ViewSetRectangleMock(this);
		viewSelectMock = new ViewSelectMock(this);
		viewCursorPanningMock = new ViewCursorPanningMock(this);
		viewCursorArrowMock = new ViewCursorArrowMock(this);
		guiUpdateScrollingMock = new GuiUpdateScrollingMock(this);
		viewMoveMock = new ViewMoveMock(this);
		guiMock = new MockGui(this);
		viewMock = new MockView(this);
		statechart->gui().setOperationCallback(guiMock);
		statechart->view().setOperationCallback(viewMock);
	}
	virtual void TearDown() {
		delete viewMoveMock;
		delete guiUpdateScrollingMock;
		delete viewCursorArrowMock;
		delete viewCursorPanningMock;
		delete viewSelectMock;
		delete viewSetRectangleMock;
		delete viewHideMock;
		delete viewShowMock;
		delete viewIs3DMock;
		delete statechart;
		delete guiMock;
		guiMock = 0;
		delete viewMock;
		viewMock = 0;
		delete runner;
	}
};



TEST_F(MouseSelectTest, testSimple) {
	statechart->enter();
	
	EXPECT_TRUE(statechart->isActive());
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
	statechart->exit();
	
	EXPECT_TRUE(!statechart->isActive());
	
}
TEST_F(MouseSelectTest, testSelectionCancel) {
	viewIs3DMock->setDefaultBehavior(&ViewIs3DMock::is3D1);
	
	statechart->enter();
	
	statechart->gui
	().raiseOnSelect();
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Selection));
	
	statechart->gui
	().raiseOnDisable();
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
}
TEST_F(MouseSelectTest, testSelectionDisallowed) {
	viewIs3DMock->setDefaultBehavior(&ViewIs3DMock::is3D2);
	
	statechart->enter();
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
	statechart->gui
	().raiseOnSelect();
	
	statechart->gui
	().isRaisedSelectionEnd();
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
}
TEST_F(MouseSelectTest, testSelectingWithoutMove) {
	viewIs3DMock->setDefaultBehavior(&ViewIs3DMock::is3D1);
	
	
	
	SCT_point point;
	
	statechart->enter();
	
	statechart->gui
	().raiseOnSelect();
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Selection));
	
	point.x = 100;
	
	point.y = 100;
	
	statechart->gui
	().raiseMouseDown(point);
	
	EXPECT_TRUE(viewShowMock->calledAtLeastOnce());
	
	EXPECT_TRUE(viewSetRectangleMock->calledAtLeastOnce());
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Moving));
	
	statechart->gui
	().raiseMouseUp(point);
	
	EXPECT_TRUE(viewSetRectangleMock->calledAtLeastOnce());
	
	statechart->gui
	().isRaisedSelectionEnd();
	
	EXPECT_TRUE(viewHideMock->calledAtLeastOnce());
	
	EXPECT_TRUE(viewSelectMock->calledAtLeastOnce());
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
}
TEST_F(MouseSelectTest, testSelectingWithMove) {
	viewIs3DMock->setDefaultBehavior(&ViewIs3DMock::is3D1);
	
	SCT_point point;
	
	statechart->enter();
	
	statechart->gui
	().raiseOnSelect();
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Selection));
	
	point.x = 100;
	
	point.y = 100;
	
	statechart->gui
	().raiseMouseDown(point);
	
	EXPECT_TRUE(viewShowMock->calledAtLeastOnce());
	
	EXPECT_TRUE(viewSetRectangleMock->calledAtLeastOnce());
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Moving));
	
	point.x = 110;
	
	point.y = 120;
	
	statechart->gui
	().raiseMouseMove(point);
	
	statechart->gui
	().raiseMouseUp(point);
	
	EXPECT_TRUE(viewSetRectangleMock->calledAtLeastOnce());
	
	statechart->gui
	().isRaisedSelectionEnd();
	
	EXPECT_TRUE(viewHideMock->calledAtLeastOnce());
	
	EXPECT_TRUE(viewSelectMock->calledAtLeastOnce());
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
}
TEST_F(MouseSelectTest, testPanningWithoutMove) {
	viewIs3DMock->setDefaultBehavior(&ViewIs3DMock::is3D1);
	
	SCT_point point;
	
	statechart->enter();
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
	point.x = 100;
	
	point.y = 100;
	
	statechart->gui
	().raiseMouseDown(point);
	
	EXPECT_TRUE(viewCursorPanningMock->calledAtLeastOnce());
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Panning));
	
	statechart->gui
	().raiseMouseUp(point);
	
	EXPECT_TRUE(viewCursorArrowMock->calledAtLeastOnce());
	
	EXPECT_TRUE(guiUpdateScrollingMock->calledAtLeastOnce());
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
}
TEST_F(MouseSelectTest, testPanningWithMove) {
	viewIs3DMock->setDefaultBehavior(&ViewIs3DMock::is3D1);
	
	SCT_point point;
	
	statechart->enter();
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
	point.x = 100;
	
	point.y = 100;
	
	statechart->gui
	().raiseMouseDown(point);
	
	EXPECT_TRUE(viewCursorPanningMock->calledAtLeastOnce());
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Panning));
	
	point.x = 110;
	
	point.y = 120;
	
	statechart->gui
	().raiseMouseMove(point);
	
	EXPECT_TRUE(viewMoveMock->calledAtLeastOnce());
	
	statechart->gui
	().raiseMouseUp(point);
	
	EXPECT_TRUE(viewCursorArrowMock->calledAtLeastOnce());
	
	EXPECT_TRUE(guiUpdateScrollingMock->calledAtLeastOnce());
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
}
TEST_F(MouseSelectTest, testMouseMove) {
	viewIs3DMock->setDefaultBehavior(&ViewIs3DMock::is3D2);
	
	SCT_point point;
	
	statechart->enter();
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
	point.x = 100;
	
	point.y = 100;
	
	statechart->gui
	().raiseMouseDown(point);
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
	point.x = 110;
	
	point.y = 120;
	
	statechart->gui
	().raiseMouseMove(point);
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
	statechart->gui
	().raiseMouseUp(point);
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
}

}

