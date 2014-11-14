#ifndef	__CPPUNITWRAPPER_H
#define	__CPPUNITWRAPPER_H

#include "cppunit/ui/text/TestRunner.h"
#include "cppunit/XmlOutputterHook.h"
#include <fstream>


class CppUnitWrapper : public CPPUNIT_NS::TextUi::TestRunner
{
public:
	CppUnitWrapper (void);
	virtual ~CppUnitWrapper (void);

	enum eOUTPUTTYPES {
		kDefaultOutput,
		kStdoutOutput,
		kXmlOutput
	};

	void setOutputType (const enum eOUTPUTTYPES output);

private:
	CPPUNIT_NS::XmlOutputterHook* mOutputterHook;
	std::ofstream* theStream;

};

#endif	//	__CPPUNITWRAPPER_H
