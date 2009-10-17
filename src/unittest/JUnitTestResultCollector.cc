/*
    The licence text below is the boilerplate "MIT Licence" used from:
    http://www.opensource.org/licenses/mit-license.php

    Copyright (c) 2009, Brodie Thiesfield

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is furnished
    to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
    FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
    COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
    IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <cppunit/TestFailure.h>
#include "JUnitTestResultCollector.h"

#include <algorithm>

CPPUNIT_NS_BEGIN


JUnitTestResultCollector::JUnitTestResultCollector( SynchronizationObject *syncObject )
    : TestResultCollector( syncObject )
{
    reset();
}

JUnitTestResultCollector::~JUnitTestResultCollector()
{
}

void 
JUnitTestResultCollector::reset()
{
    TestResultCollector::reset();

    ExclusiveZone zone( m_syncObject ); 
    m_timings.clear();
    m_startAll = 0;
    m_durationAll = 0;
}

void JUnitTestResultCollector::startTestRun(Test * /*test*/, TestResult * /*eventManager*/ ) 
{
    m_startAll = clock();
}

void 
JUnitTestResultCollector::startTest( Test *test )
{
    TestResultCollector::startTest( test );

    ExclusiveZone zone (m_syncObject); 
    m_timings.push_back( test );
}

void 
JUnitTestResultCollector::endTest( Test *test )
{
    TestResultCollector::endTest( test );

    ExclusiveZone zone (m_syncObject); 
    TimingResults::iterator i = std::find(m_timings.begin(), m_timings.end(), test);
    if (i != m_timings.end()) {
        i->duration = clock() - i->start;
    }
}

void JUnitTestResultCollector::endTestRun(Test * /*test*/, TestResult * /*eventManager*/ ) 
{
    m_durationAll = clock() - m_startAll;
}

clock_t 
JUnitTestResultCollector::getTestDuration( Test * test )
{
    ExclusiveZone zone (m_syncObject); 
  
    TimingResults::iterator i = std::find(m_timings.begin(), m_timings.end(), test);
    if (i != m_timings.end()) {
        return i->duration;
    }
    return 0;
}

CPPUNIT_NS_END

