#ifndef __WEBINSPECTORPROTOCOL_H
#define __WEBINSPECTORPROTOCOL_H

#include <generics/generics.h>
#include <websocket/websocket.h>

namespace WPEFramework {
namespace WebInspector {
    class RemoteObject : public Core::JSON::Container {
    public:
        typedef enum {
            array,
            date,
            node,
            null,
            regexp
        } enumSubType;

        typedef enum {
            boolean,
            function,
            number,
            object,
            string,
            undefined
        } enumType;

    private:
        RemoteObject(const RemoteObject&);
        RemoteObject& operator=(const RemoteObject&);

    public:
        RemoteObject()
        {
            Add(_T("className"), &className);
            Add(_T("description"), &description);
            Add(_T("objectId"), &objectId);
            //          Add (_T("subType"), &subType);
            Add(_T("type"), &type);
            Add(_T("value"), &value);
        }
        ~RemoteObject()
        {
        }

    public:
        Core::JSON::String className;
        Core::JSON::String description;
        Core::JSON::String objectId;
        //      Core::JSON::EnumType<enumSubType>   subType;
        Core::JSON::EnumType<enumType> type;
        Core::JSON::String value;
    };

    class Result : public Core::JSON::Container {
    private:
        Result(const Result&);
        Result& operator=(const Result&);

    public:
        Result()
        {
            Add(_T("result"), &result);
            Add(_T("wasThrown"), &wasThrown);
        }
        ~Result()
        {
        }

    public:
        Core::JSON::String result;
        Core::JSON::Boolean wasThrown;
    };

    class Response : public Core::JSON::Container {
    private:
        Response(const Response&);
        Response& operator=(const Response&);

    public:
        Response()
        {
            Add(_T("id"), &id);
            Add(_T("error"), &error);
            Add(_T("result"), &result);
        }
        ~Response()
        {
        }

    public:
        Core::JSON::DecUInt32 id;
        Core::JSON::String error;
        Result result;
    };

    class CallArgument : public Core::JSON::Container {
    private:
        CallArgument& operator=(const CallArgument&);

    public:
        CallArgument()
        {
            Add(_T("objectId"), &objectId);
            Add(_T("value"), &value);
        }
        CallArgument(const CallArgument& copy)
            : Core::JSON::Container()
            , objectId(copy.objectId)
            , value(copy.value)
        {
            Add(_T("objectId"), &objectId);
            Add(_T("value"), &value);
        }
        ~CallArgument()
        {
        }

    public:
        Core::JSON::String objectId;
        Core::JSON::String value;
    };

    class Params : public Core::JSON::Container {
    private:
        Params(const Params&);
        Params& operator=(const Params&);

    public:
        Params()
        {
            Add(_T("objectId"), &objectId);
            Add(_T("functionDeclaration"), &functionDeclaration);
            Add(_T("arguments"), &arguments);
            Add(_T("returnByValue"), &returnByValue);
            Add(_T("ignoreCache"), &ignoreCache);
            Add(_T("scriptToEvaluateOnLoad"), &scriptToEvaluateOnLoad);
        }
        ~Params()
        {
        }

    public:
        Core::JSON::String objectId;
        Core::JSON::String functionDeclaration;
        Core::JSON::ArrayType<CallArgument> arguments;
        Core::JSON::Boolean returnByValue;
        Core::JSON::Boolean ignoreCache;
        Core::JSON::String scriptToEvaluateOnLoad;
    };

    class Request : public Core::JSON::Container {
        JSONTYPEID(request);

    private:
        Request(const Request&);
        Request& operator=(const Request&);

    public:
        Request()
        {
            Add(_T("id"), &id);
            Add(_T("method"), &method);
            Add(_T("params"), &params);
        }
        ~Request()
        {
        }

    public:
        Core::JSON::DecUInt32 id;
        Core::JSON::String method;
        Params params;
    };

    class Notification : public Core::JSON::Container {
        JSONTYPEID(notification);

    private:
        Notification(const Notification&);
        Notification& operator=(const Notification&);

    public:
        Notification()
        {
            Add(_T("method"), &method);
            Add(_T("params"), &params);
        }
        ~Notification()
        {
        }

    public:
        Core::JSON::String method;
        Params params;
    };
}
} // WPEFramework::WebInspector

ENUM_CONVERSION_BEGIN(WPEFramework::WebInspector::RemoteObject::enumSubType)

    { WPEFramework::WebInspector::RemoteObject::array, _TXT("array") },
    { WPEFramework::WebInspector::RemoteObject::date, _TXT("date") },
    { WPEFramework::WebInspector::RemoteObject::node, _TXT("node") },
    { WPEFramework::WebInspector::RemoteObject::null, _TXT("null") },
    { WPEFramework::WebInspector::RemoteObject::regexp, _TXT("regexp") },

ENUM_CONVERSION_END(WPEFramework::WebInspector::RemoteObject::enumSubType)

ENUM_CONVERSION_BEGIN(WPEFramework::WebInspector::RemoteObject::enumType)

    { WPEFramework::WebInspector::RemoteObject::boolean, _TXT("boolean") },
    { WPEFramework::WebInspector::RemoteObject::function, _TXT("function") },
    { WPEFramework::WebInspector::RemoteObject::number, _TXT("number") },
    { WPEFramework::WebInspector::RemoteObject::object, _TXT("object") },
    { WPEFramework::WebInspector::RemoteObject::string, _TXT("string") },
    { WPEFramework::WebInspector::RemoteObject::undefined, _TXT("undefined") },

ENUM_CONVERSION_END(WPEFramework::WebInspector::RemoteObject::enumType)

#endif // __WEBINSPECTORPROTOCOL_H