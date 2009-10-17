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

#include <cppunit/Exception.h>
#include <cppunit/Test.h>
#include <cppunit/TestFailure.h>
#include "JUnitXmlOutputter.h"
#include <cppunit/XmlOutputterHook.h>
#include <cppunit/tools/XmlDocument.h>
#include <cppunit/tools/XmlElement.h>
#include <stdlib.h>
#include <algorithm>


CPPUNIT_NS_BEGIN


JUnitXmlOutputter::JUnitXmlOutputter( JUnitTestResultCollector *result,
                            OStream &stream,
                            std::string encoding )
  : m_result( result )
  , m_stream( stream )
  , m_xml( new XmlDocument( encoding ) )
  , m_hostname("localhost")
  , m_name("anonymous")
{
}


JUnitXmlOutputter::~JUnitXmlOutputter()
{
  delete m_xml;
}


void 
JUnitXmlOutputter::addHook( XmlOutputterHook *hook )
{
  m_hooks.push_back( hook );
}


void 
JUnitXmlOutputter::removeHook( XmlOutputterHook *hook )
{
  m_hooks.erase( std::find( m_hooks.begin(), m_hooks.end(), hook ) );
}


void 
JUnitXmlOutputter::write()
{
  setRootNode();
  m_stream  <<  m_xml->toString();
}


void 
JUnitXmlOutputter::setStyleSheet( const std::string &styleSheet )
{
  m_xml->setStyleSheet( styleSheet );
}


void
JUnitXmlOutputter::setStandalone( bool standalone )
{
  m_xml->setStandalone( standalone );
}
 

void
JUnitXmlOutputter::setRootNode()
{
    XmlElement *rootNode = new XmlElement( "testsuite" );
    m_xml->setRootElement( rootNode );

    for ( Hooks::iterator it = m_hooks.begin(); it != m_hooks.end(); ++it ) {
        (*it)->beginDocument( m_xml );
    }

    FailedTests failedTests;
    fillFailedTestsMap( failedTests );

    char buf[100];

    double sec = (double) m_result->getAllTestsDuration() / CLOCKS_PER_SEC;
    sprintf(buf, "%0.04f", sec);
    rootNode->addAttribute("time", buf);

    time_t mytimet;
    time(&mytimet);
    struct tm * mytime = localtime(&mytimet);
    strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", mytime);
    rootNode->addAttribute( "timestamp", buf );

    int countErrors = 0, countFailures = 0;
    for (FailedTests::iterator i = failedTests.begin(); i != failedTests.end(); ++i) {
        if (i->second->isError()) 
            ++countErrors;
        else
            ++countFailures;
    }

    sprintf(buf, "%d", countErrors);
    rootNode->addAttribute("errors", buf);

    sprintf(buf, "%d", countErrors);
    rootNode->addAttribute("failures", buf);

    sprintf(buf, "%d", m_result->tests().size());
    rootNode->addAttribute("tests", buf);

    rootNode->addAttribute("hostname", m_hostname);
    rootNode->addAttribute("name", m_name);

    addAllTests( failedTests, rootNode );

    for ( Hooks::iterator itEnd = m_hooks.begin(); itEnd != m_hooks.end(); ++itEnd ) {
        (*itEnd)->endDocument( m_xml );
    }
}


void 
JUnitXmlOutputter::fillFailedTestsMap( FailedTests &failedTests )
{
  const JUnitTestResultCollector::TestFailures &failures = m_result->failures();
  JUnitTestResultCollector::TestFailures::const_iterator itFailure = failures.begin();
  while ( itFailure != failures.end() )
  {
    TestFailure *failure = *itFailure++;
    failedTests.insert( std::pair<Test* const, TestFailure*>(failure->failedTest(), failure ) );
  }
}


void
JUnitXmlOutputter::addAllTests( FailedTests &failedTests,
                              XmlElement *rootNode )
{
  const TestResultCollector::Tests &tests = m_result->tests();
  for ( unsigned int testNumber = 0; testNumber < tests.size(); ++testNumber )
  {
    Test * test = tests[testNumber];
    clock_t duration = 0;
    if ( failedTests.find( test ) == failedTests.end() )
      addSuccessfulTest( test, duration, testNumber+1, rootNode );
    else
      addFailedTest( test, duration, failedTests[test], testNumber+1, rootNode );
  }
}


void
JUnitXmlOutputter::addFailedTest( Test * test, 
                                  clock_t duration,
                                  TestFailure *failure,
                                  int testNumber,
                                  XmlElement *rootNode )
{
    XmlElement *testElement = addBaseElement(test, duration, testNumber, rootNode);

    Exception *thrownException = failure->thrownException();

    XmlElement *failureElement = new XmlElement( "failure", thrownException->what() );
    testElement->addElement( failureElement );

    failureElement->addAttribute( "type", failure->isError() ? "error" : "assertion" );
    failureElement->addAttribute( "message", thrownException->message().shortDescription() );
    
    if ( failure->sourceLine().isValid() ) {
        addFailureLocation( failure, failureElement );
    }

    for ( Hooks::iterator it = m_hooks.begin(); it != m_hooks.end(); ++it ) {
        (*it)->successfulTestAdded( m_xml, testElement, test );
    }
}


void
JUnitXmlOutputter::addFailureLocation( TestFailure *failure,
                                       XmlElement *testElement )
{
  XmlElement *locationNode = new XmlElement( "location" );
  testElement->addElement( locationNode );
  SourceLine sourceLine = failure->sourceLine();
  locationNode->addElement( new XmlElement( "file", sourceLine.fileName() ) );
  locationNode->addElement( new XmlElement( "line", sourceLine.lineNumber() ) );
}


void
JUnitXmlOutputter::addSuccessfulTest( Test * test, 
                                      clock_t duration,
                                      int testNumber,
                                      XmlElement *rootNode )
{
    XmlElement *testElement = addBaseElement( test, duration, testNumber, rootNode );
    for ( Hooks::iterator it = m_hooks.begin(); it != m_hooks.end(); ++it ) {
        (*it)->successfulTestAdded( m_xml, testElement, test );
    }
}

XmlElement *
JUnitXmlOutputter::addBaseElement( Test * test, 
                                   clock_t duration,
                                   int testNumber,
                                   XmlElement *rootNode )
{
    XmlElement *testElement = new XmlElement( "testcase" );
    rootNode->addElement( testElement );
    testElement->addAttribute( "id", testNumber );
    std::string::size_type n = test->getName().find("::");
    if (n >= 0) {
        testElement->addAttribute( "class", std::string(test->getName(), 0, n) );
        testElement->addAttribute( "name",  std::string(test->getName(), n+2) );
    }
    else {
        testElement->addAttribute( "class", "" );
        testElement->addAttribute( "name",  test->getName() );
    }

    std::stringstream timing;
    double sec = (double) duration / CLOCKS_PER_SEC;
    timing.precision( 4 );
    timing << std::fixed << sec;

    testElement->addAttribute( "time", timing.str() );
    return testElement;
}


CPPUNIT_NS_END
