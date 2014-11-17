#include "CppUnitWrapper.h"
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/Outputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/XmlOutputterHook.h>
#include <cppunit/tools/XmlElement.h>
#include <cppunit/tools/XmlDocument.h>

#include <fstream>
#include <string>
#include <ctime>

#include <sys/times.h>


class MyXmlOutputterHook : public CPPUNIT_NS::XmlOutputterHook {
public:
	MyXmlOutputterHook (std::string inProjectName) : 
        CPPUNIT_NS::XmlOutputterHook (),
        mProjectName (inProjectName)
	{
	};

	virtual ~MyXmlOutputterHook() {
	};

// Do not add extra info to Test.xml when executed under Jenkins,
//  since Jenkins will choke on it!
#ifndef __JENKINS
	virtual void beginDocument (CPPUNIT_NS::XmlDocument* inDocument) {
		if (!inDocument) {
			return;
		}

		CPPUNIT_NS::XmlElement* theElement = new CPPUNIT_NS::XmlElement ("TestSuiteInfo", "");
		theElement->addElement (new CPPUNIT_NS::XmlElement ("Project", mProjectName));

		const time_t rawtime = time (0);
		theElement->addElement (new CPPUNIT_NS::XmlElement ("Date", ctime (&rawtime)));
		inDocument->rootElement ().addElement (theElement);
	};
#endif  //  __JENKINS

private:
	std::string mProjectName;

};

CppUnitWrapper::CppUnitWrapper () : 
    CPPUNIT_NS::TextUi::TestRunner (),
    mOutputterHook (0),
    theStream (0)
{
	// Get the top level suite from the registry
	CPPUNIT_NS::Test* suite = CPPUNIT_NS::TestFactoryRegistry::getRegistry ().makeTest ();
	// Adds the test to the list of test to run
	addTest (suite);
}

CppUnitWrapper::~CppUnitWrapper () {
	if (theStream) 	{

		if (theStream ->is_open ()) {
			// Close the xml-format outputter stream
			theStream->close ();
			delete theStream;
			theStream = 0;
		}
	}

	if (mOutputterHook) {
		delete mOutputterHook;
		mOutputterHook = 0;
	}
}

void CppUnitWrapper::setOutputType (const enum eOUTPUTTYPES output) {

	// Gets cleaned by destructor of 'runner'
	CppUnit::Outputter* theOutputter = 0;

	// Change the default outputter to a
	if (output == kStdoutOutput) {

		//	compiler error format outputter
		theOutputter = new CppUnit::CompilerOutputter (&(this->result()), CPPUNIT_NS::stdCOut ());
	}
	else if (output == kXmlOutput)	{

		//	xml-format outputter
		theStream = new std::ofstream ("Test.xml");
		theOutputter = new CppUnit::XmlOutputter (&(this->result ()), *theStream);
		CPPUNIT_NS::XmlOutputter* xmlOutputter = dynamic_cast<CPPUNIT_NS::XmlOutputter*>(theOutputter);

		// Add stylesheet if possible
		mOutputterHook = new MyXmlOutputterHook ("UnitTestDemo");
		if (xmlOutputter && mOutputterHook) {

			xmlOutputter->setStyleSheet ("testreport.xsl");
			xmlOutputter->addHook (mOutputterHook);
		}
	}

	// set outputter
	if (theOutputter) {
		setOutputter (theOutputter);
	}
}
