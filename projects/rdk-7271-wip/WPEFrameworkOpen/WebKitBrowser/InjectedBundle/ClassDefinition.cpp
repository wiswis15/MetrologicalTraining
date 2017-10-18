#include <memory.h>
#include "ClassDefinition.h"

#include <iostream>
using std::cerr;
using std::endl;

namespace WPEFramework {
namespace JavaScript {

    // Constructor: uses identifier to build class and extension name.
    ClassDefinition::ClassDefinition(const string& identifier)
    {
        ClassName = Core::ToString(identifier);
        // Make upper case.
        transform(ClassName.begin(), ClassName.end(), ClassName.begin(), ::toupper);

        ExtName = Core::ToString(identifier);
        // Make lower case.
        transform(ExtName.begin(), ExtName.end(), ExtName.begin(), ::tolower);
    }

    // Adds JS function to class.
    /* static */ void ClassDefinition::Add(const JavaScriptFunction* javaScriptFunction)
    {
        ASSERT(std::find(CustomFunctions.begin(), CustomFunctions.end(), javaScriptFunction) == CustomFunctions.end());

        CustomFunctions.push_back(javaScriptFunction);
    }

    // Removes JS function from class.
    void ClassDefinition::Remove(const JavaScriptFunction* javaScriptFunction)
    {
        // Try to find function in class.
        std::vector<const JavaScriptFunction*>& funcVector = CustomFunctions;
        std::vector<const JavaScriptFunction*>::iterator i = find(funcVector.begin(), funcVector.end(), javaScriptFunction);

        ASSERT(i != funcVector.end());

        if (i != funcVector.end()) {
            // Remove function from function vector.
            funcVector.erase(i);
        }
    }

    // Returns iterator for functions in class.
    ClassDefinition::FunctionIterator ClassDefinition::GetFunctions()
    {
        return FunctionIterator(CustomFunctions);
    }
}
}
