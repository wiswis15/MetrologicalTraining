#ifndef __ISTATECONTROL_H
#define __ISTATECONTROL_H

#include "Module.h"
#include "IShell.h"

namespace WPEFramework {
namespace PluginHost {

    // This interface gives direct access to change occuring on the remote object
    struct IStateControl : virtual public Core::IUnknown {

		enum { ID = 0x0000002A };

		enum command {
			SUSPEND = 0x0001,
			RESUME = 0x0002
		};

		enum state {
			UNINITIALIZED = 0x0000,
			SUSPENDED = 0x0001,
			RESUMED = 0x0002,
			EXITED =  0x0003
		};

		struct INotification : virtual public Core::IUnknown {
			enum { ID = 0x0000002B };

			virtual ~INotification() {}

			virtual void StateChange(const IStateControl::state state) = 0;
		};

	class Command : public Core::EnumerateType<command> {
	public:
	    inline Command() : Core::EnumerateType<command>() {
	    }
	    inline Command(const command value) : Core::EnumerateType<command>(value) {
	    }
	    inline Command(const Command& copy) : Core::EnumerateType<command>(copy) {
	    }
	    inline ~Command() {
	    }

	public:
	    string Data() const;
	};

	class State : public Core::EnumerateType<state> {
	public:
	    inline State() : Core::EnumerateType<state>() {
	    }
	    inline State(const state value) : Core::EnumerateType<state>(value) {
	    }
	    inline State(const State& copy) : Core::EnumerateType<state>(copy) {
	    }
	    inline ~State() {
	    }

	public:
	    string Data() const;
	};


		virtual ~IStateControl() {}

        virtual uint32 Configure(PluginHost::IShell* framework) = 0;
        virtual state State() const = 0;
        virtual uint32 Request(const command state) = 0;

        virtual void Register(IStateControl::INotification* notification) = 0;
        virtual void Unregister(IStateControl::INotification* notification) = 0;
    };
}
}

#endif // __ISTATECONTROL_H
