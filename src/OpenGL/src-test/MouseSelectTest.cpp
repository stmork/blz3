/** Generated with YAKINDU statechart tools
 *
 *   (C) Copyright  2001 - 2022  Steffen A. Mork
 *               All rights reserved */
#include <string>
#include "gtest/gtest.h"
#include "MouseSelect.h"
#include "sc_runner.h"
#include "sc_types.h"

#define SC_UNUSED(P) (void)P

namespace  {

MouseSelect* statechart;


class ViewIs3DMock{
	typedef bool (ViewIs3DMock::*functiontype)();
	public:
	bool (ViewIs3DMock::*viewIs3DBehaviorDefault)();

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
static ViewIs3DMock* viewIs3DMock;

class ViewShowMock{
	typedef void (ViewShowMock::*functiontype)();
	public:
	void (ViewShowMock::*viewShowBehaviorDefault)();
	int callCount;

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
static ViewShowMock* viewShowMock;

class ViewHideMock{
	typedef void (ViewHideMock::*functiontype)();
	public:
	void (ViewHideMock::*viewHideBehaviorDefault)();
	int callCount;

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
static ViewHideMock* viewHideMock;

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
	std::list<ViewSetRectangleMock::parameters> paramCount;
	int callCount;

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
static ViewSetRectangleMock* viewSetRectangleMock;

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
	std::list<ViewSelectMock::parameters> paramCount;
	int callCount;

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
static ViewSelectMock* viewSelectMock;

class ViewCursorPanningMock{
	public:
	int callCount;

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
static ViewCursorPanningMock* viewCursorPanningMock;

class ViewCursorArrowMock{
	public:
	int callCount;

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
static ViewCursorArrowMock* viewCursorArrowMock;

class GuiUpdateScrollingMock{
	public:
	int callCount;

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
static GuiUpdateScrollingMock* guiUpdateScrollingMock;

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
	std::list<ViewMoveMock::parameters> paramCount;
	int callCount;

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
static ViewMoveMock* viewMoveMock;

class MockGui : public MouseSelect::Gui::OperationCallback {
	public:
	void updateScrolling() {
		guiUpdateScrollingMock->updateScrolling();
	}
};
class MockView : public MouseSelect::View::OperationCallback {
	public:
	bool is3D() {
		return (viewIs3DMock->*(viewIs3DMock->getBehavior()))();
	}
	void show() {
		viewShowMock->show();
		return (viewShowMock->*(viewShowMock->getBehavior()))();
	}
	void hide() {
		viewHideMock->hide();
		return (viewHideMock->*(viewHideMock->getBehavior()))();
	}
	void cursorPanning() {
		viewCursorPanningMock->cursorPanning();
	}
	void cursorArrow() {
		viewCursorArrowMock->cursorArrow();
	}
	void setRectangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
		viewSetRectangleMock->setRectangle(x1, y1, x2, y2);
	}
	void select(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
		viewSelectMock->select(x1, y1, x2, y2);
	}
	void move(int32_t dx, int32_t dy) {
		viewMoveMock->move(dx, dy);
	}
};

//! The timers are managed by a timer service. */
static SctUnitRunner * runner;

class MouseSelectTest : public ::testing::Test{
	protected:
	virtual void SetUp() {
		statechart = new MouseSelect();
		runner = new SctUnitRunner(
			statechart,
			true,
			200
		);
	}
	virtual void TearDown() {
		delete statechart;
		delete runner;
	}
};


TEST_F(MouseSelectTest, testSimple) {
	
	MockGui guiMock;
	MockView viewMock;
	statechart->gui()->setOperationCallback(&guiMock);
	statechart->view()->setOperationCallback(&viewMock);
	statechart->enter();
	
	EXPECT_TRUE(statechart->isActive());
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
	statechart->exit();
	
	EXPECT_TRUE(!statechart->isActive());
	
	
}
TEST_F(MouseSelectTest, testSelectionCancel) {
	viewIs3DMock = new ViewIs3DMock();
	viewIs3DMock->initializeBehavior();
	
	MockGui guiMock;
	MockView viewMock;
	statechart->gui()->setOperationCallback(&guiMock);
	statechart->view()->setOperationCallback(&viewMock);
	viewIs3DMock->setDefaultBehavior(&ViewIs3DMock::is3D1);
	
	statechart->enter();
	
	statechart->gui
	()->raiseOnSelect();
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Selection));
	
	statechart->gui
	()->raiseOnDisable();
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
	
	viewIs3DMock->reset();
}
TEST_F(MouseSelectTest, testSelectionDisallowed) {
	viewIs3DMock = new ViewIs3DMock();
	viewIs3DMock->initializeBehavior();
	
	MockGui guiMock;
	MockView viewMock;
	statechart->gui()->setOperationCallback(&guiMock);
	statechart->view()->setOperationCallback(&viewMock);
	viewIs3DMock->setDefaultBehavior(&ViewIs3DMock::is3D2);
	
	statechart->enter();
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
	statechart->gui
	()->raiseOnSelect();
	
	statechart->gui
	()->isRaisedSelectionEnd();
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
	
	viewIs3DMock->reset();
}
TEST_F(MouseSelectTest, testSelectingWithoutMove) {
	viewIs3DMock = new ViewIs3DMock();
	viewIs3DMock->initializeBehavior();
	viewShowMock = new ViewShowMock();
	viewShowMock->initializeBehavior();
	viewHideMock = new ViewHideMock();
	viewHideMock->initializeBehavior();
	viewSetRectangleMock = new ViewSetRectangleMock();
	viewSelectMock = new ViewSelectMock();
	
	MockGui guiMock;
	MockView viewMock;
	statechart->gui()->setOperationCallback(&guiMock);
	statechart->view()->setOperationCallback(&viewMock);
	viewIs3DMock->setDefaultBehavior(&ViewIs3DMock::is3D1);
	
	
	
	SCT_point point;
	
	statechart->enter();
	
	statechart->gui
	()->raiseOnSelect();
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Selection));
	
	point.x = 100;
	
	point.y = 100;
	
	statechart->gui
	()->raiseMouseDown(point);
	
	EXPECT_TRUE(viewShowMock->calledAtLeastOnce());
	
	EXPECT_TRUE(viewSetRectangleMock->calledAtLeastOnce());
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Moving));
	
	statechart->gui
	()->raiseMouseUp(point);
	
	EXPECT_TRUE(viewSetRectangleMock->calledAtLeastOnce());
	
	statechart->gui
	()->isRaisedSelectionEnd();
	
	EXPECT_TRUE(viewHideMock->calledAtLeastOnce());
	
	EXPECT_TRUE(viewSelectMock->calledAtLeastOnce());
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
	
	viewIs3DMock->reset();
	viewShowMock->reset();
	viewHideMock->reset();
	viewSetRectangleMock->reset();
	viewSelectMock->reset();
}
TEST_F(MouseSelectTest, testSelectingWithMove) {
	viewIs3DMock = new ViewIs3DMock();
	viewIs3DMock->initializeBehavior();
	viewShowMock = new ViewShowMock();
	viewShowMock->initializeBehavior();
	viewSetRectangleMock = new ViewSetRectangleMock();
	viewHideMock = new ViewHideMock();
	viewHideMock->initializeBehavior();
	viewSelectMock = new ViewSelectMock();
	
	MockGui guiMock;
	MockView viewMock;
	statechart->gui()->setOperationCallback(&guiMock);
	statechart->view()->setOperationCallback(&viewMock);
	viewIs3DMock->setDefaultBehavior(&ViewIs3DMock::is3D1);
	
	SCT_point point;
	
	statechart->enter();
	
	statechart->gui
	()->raiseOnSelect();
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Selection));
	
	point.x = 100;
	
	point.y = 100;
	
	statechart->gui
	()->raiseMouseDown(point);
	
	EXPECT_TRUE(viewShowMock->calledAtLeastOnce());
	
	EXPECT_TRUE(viewSetRectangleMock->calledAtLeastOnce());
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Moving));
	
	point.x = 110;
	
	point.y = 120;
	
	statechart->gui
	()->raiseMouseMove(point);
	
	statechart->gui
	()->raiseMouseUp(point);
	
	EXPECT_TRUE(viewSetRectangleMock->calledAtLeastOnce());
	
	statechart->gui
	()->isRaisedSelectionEnd();
	
	EXPECT_TRUE(viewHideMock->calledAtLeastOnce());
	
	EXPECT_TRUE(viewSelectMock->calledAtLeastOnce());
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
	
	viewIs3DMock->reset();
	viewShowMock->reset();
	viewSetRectangleMock->reset();
	viewHideMock->reset();
	viewSelectMock->reset();
}
TEST_F(MouseSelectTest, testPanningWithoutMove) {
	viewIs3DMock = new ViewIs3DMock();
	viewIs3DMock->initializeBehavior();
	viewCursorPanningMock = new ViewCursorPanningMock();
	viewCursorArrowMock = new ViewCursorArrowMock();
	guiUpdateScrollingMock = new GuiUpdateScrollingMock();
	
	MockGui guiMock;
	MockView viewMock;
	statechart->gui()->setOperationCallback(&guiMock);
	statechart->view()->setOperationCallback(&viewMock);
	viewIs3DMock->setDefaultBehavior(&ViewIs3DMock::is3D1);
	
	SCT_point point;
	
	statechart->enter();
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
	point.x = 100;
	
	point.y = 100;
	
	statechart->gui
	()->raiseMouseDown(point);
	
	EXPECT_TRUE(viewCursorPanningMock->calledAtLeastOnce());
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Panning));
	
	statechart->gui
	()->raiseMouseUp(point);
	
	EXPECT_TRUE(viewCursorArrowMock->calledAtLeastOnce());
	
	EXPECT_TRUE(guiUpdateScrollingMock->calledAtLeastOnce());
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
	
	viewIs3DMock->reset();
	viewCursorPanningMock->reset();
	viewCursorArrowMock->reset();
	guiUpdateScrollingMock->reset();
}
TEST_F(MouseSelectTest, testPanningWithMove) {
	viewIs3DMock = new ViewIs3DMock();
	viewIs3DMock->initializeBehavior();
	viewCursorPanningMock = new ViewCursorPanningMock();
	viewMoveMock = new ViewMoveMock();
	viewCursorArrowMock = new ViewCursorArrowMock();
	guiUpdateScrollingMock = new GuiUpdateScrollingMock();
	
	MockGui guiMock;
	MockView viewMock;
	statechart->gui()->setOperationCallback(&guiMock);
	statechart->view()->setOperationCallback(&viewMock);
	viewIs3DMock->setDefaultBehavior(&ViewIs3DMock::is3D1);
	
	SCT_point point;
	
	statechart->enter();
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
	point.x = 100;
	
	point.y = 100;
	
	statechart->gui
	()->raiseMouseDown(point);
	
	EXPECT_TRUE(viewCursorPanningMock->calledAtLeastOnce());
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Panning));
	
	point.x = 110;
	
	point.y = 120;
	
	statechart->gui
	()->raiseMouseMove(point);
	
	EXPECT_TRUE(viewMoveMock->calledAtLeastOnce());
	
	statechart->gui
	()->raiseMouseUp(point);
	
	EXPECT_TRUE(viewCursorArrowMock->calledAtLeastOnce());
	
	EXPECT_TRUE(guiUpdateScrollingMock->calledAtLeastOnce());
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
	
	viewIs3DMock->reset();
	viewCursorPanningMock->reset();
	viewMoveMock->reset();
	viewCursorArrowMock->reset();
	guiUpdateScrollingMock->reset();
}
TEST_F(MouseSelectTest, testMouseMove) {
	viewIs3DMock = new ViewIs3DMock();
	viewIs3DMock->initializeBehavior();
	
	MockGui guiMock;
	MockView viewMock;
	statechart->gui()->setOperationCallback(&guiMock);
	statechart->view()->setOperationCallback(&viewMock);
	viewIs3DMock->setDefaultBehavior(&ViewIs3DMock::is3D2);
	
	SCT_point point;
	
	statechart->enter();
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
	point.x = 100;
	
	point.y = 100;
	
	statechart->gui
	()->raiseMouseDown(point);
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
	point.x = 110;
	
	point.y = 120;
	
	statechart->gui
	()->raiseMouseMove(point);
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
	statechart->gui
	()->raiseMouseUp(point);
	
	EXPECT_TRUE(statechart->isStateActive(MouseSelect::State::main_region_Normal));
	
	
	viewIs3DMock->reset();
}

}

