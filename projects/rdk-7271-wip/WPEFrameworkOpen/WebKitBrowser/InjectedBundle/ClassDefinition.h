#ifndef __CUSTOMCLASS_H
#define __CUSTOMCLASS_H

#include "Module.h"

#include "JavaScriptFunction.h"

#include <WPE/WebKit.h>
#include <vector>

namespace WPEFramework {
namespace JavaScript {

    class ClassDefinition {
    private:
        typedef std::map<string, ClassDefinition> ClassMap;
        typedef std::vector<const JavaScriptFunction*> FunctionVector;

    public:
        typedef Core::IteratorMapType<ClassMap, ClassDefinition&, string> Iterator;
        typedef Core::IteratorType<FunctionVector, const JavaScriptFunction*> FunctionIterator;

    private:
        class Administrator {
        private:
            Administrator(const Administrator&);
            Administrator& operator=(const Administrator&);

        public:
            Administrator()
            {
            }
            ~Administrator()
            {
            }

        public:
            Iterator ClassNames()
            {
                return (Iterator(_classDefinitions));
            }
            ClassDefinition& Instance(const string& className)
            {
                ClassMap::iterator index(_classDefinitions.find(className));
                ClassDefinition* result = nullptr;

                if (index == _classDefinitions.end()) {
                    ClassDefinition newEntry(className);

                    std::pair<string, ClassDefinition> insertEntry(className, newEntry);

                    result = &((_classDefinitions.insert(insertEntry)).first->second);
                }
                else {
                    result = &(index->second);
                }
                return (*result);
            }

        private:
            ClassMap _classDefinitions;
        };

    public:
        ClassDefinition(const ClassDefinition& copy)
            : CustomFunctions()
            , ClassName(copy.ClassName)
            , ExtName(copy.ExtName)
        {
        }
        inline static ClassDefinition& Instance(const string& className)
        {
            return (GetAdministrator().Instance(className));
        }

        void Add(const JavaScriptFunction* javaScriptFunction);
        void Remove(const JavaScriptFunction* javaScriptFunction);

        static Iterator GetClassDefinitions()
        {
            return GetAdministrator().ClassNames();
        }
        FunctionIterator GetFunctions();
        const string& GetClassName() const
        {
            return ClassName;
        }
        const string& GetExtName() const
        {
            return ExtName;
        }

    private:
        ClassDefinition() = delete;
        ClassDefinition(const string& identifier);
        inline static Administrator& GetAdministrator()
        {
            static Administrator administrator;

            return (administrator);
        }

    private:
        std::vector<const JavaScriptFunction*> CustomFunctions;

        // JavaScript class properties.
        std::string ClassName;
        std::string ExtName;
    };
}
}

#endif // __CUSTOMCLASS_H
