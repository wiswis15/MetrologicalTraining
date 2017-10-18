#ifndef __REMOTEADMINISTRATOR_H
#define __REMOTEADMINISTRATOR_H

#include "Module.h"
#include <interfaces/IKeyHandler.h>

namespace WPEFramework {
namespace Remotes {

    class RemoteAdministrator {
    private:
        RemoteAdministrator()
            : _adminLock()
            , _callback(nullptr)
            , _remotes()
        {
        }
        RemoteAdministrator(const RemoteAdministrator&);
        RemoteAdministrator& operator=(const RemoteAdministrator&);

    public:
        typedef Core::IteratorType< const std::list<Exchange::IKeyProducer*>, const Exchange::IKeyProducer*, std::list<Exchange::IKeyProducer*>::const_iterator > Iterator;

        static RemoteAdministrator& Instance()
        {
            static RemoteAdministrator _administrator;

            return (_administrator);
        }
        ~RemoteAdministrator()
        {
        }

    public:
        inline Iterator Producers() {
            return (Iterator(_remotes));
        }
        uint32 Error(const string& device)
        {
            uint32 result = Core::ERROR_UNAVAILABLE;

            _adminLock.Lock();

            std::list<Exchange::IKeyProducer*>::iterator index(_remotes.begin());

            while ((index != _remotes.end()) && (result == Core::ERROR_UNAVAILABLE)) {
                if (device == (*index)->Name()) {
                    result = (*index)->Error();
                    index = _remotes.end();
                }
                else {
                    index++;
                }
            }

            _adminLock.Unlock();

            return (result);
        }
        bool Pairing(const string& device)
        {
            bool result = true;

            _adminLock.Lock();

            std::list<Exchange::IKeyProducer*>::iterator index(_remotes.begin());

            while (index != _remotes.end()) {
                if (device.empty() == true) {
                    result = (*index)->Pairing() && result;
                    index++;
                }
                else if (device == (*index)->Name()) {
                    result = (*index)->Pairing();
                    index = _remotes.end();
                }
                else {
                    index++;
                }
            }

            _adminLock.Unlock();

            return (result);
        }
        string MetaData(const string& device)
        {
            string result;

            _adminLock.Lock();

            std::list<Exchange::IKeyProducer*>::iterator index(_remotes.begin());

            while (index != _remotes.end()) {
                string entry;

                if (device.empty() == true) {
                    entry = '\"' + string((*index)->Name()) + _T("\":") + (*index)->MetaData();
                    index++;
                }
                else if (device == (*index)->Name()) {
                    entry = '\"' + string((*index)->Name()) + _T("\":") + (*index)->MetaData();
                    index = _remotes.end();
                }
                else {
                    index++;
                }

                if (result.empty() == true) {
                    result = '{' + entry;
                }
                else {
                    result += ',' + entry;
                }
            }

            _adminLock.Unlock();

            if (result.empty() == false) {
                result += '}';
            }

            return (result);
        }
        void Announce(Exchange::IKeyProducer& remoteControl)
        {
            _adminLock.Lock();

            std::list<Exchange::IKeyProducer*>::iterator index(std::find(_remotes.begin(), _remotes.end(), &remoteControl));

            // Announce a remote only once.
            ASSERT(index == _remotes.end());

            if (index == _remotes.end()) {
                _remotes.push_back(&remoteControl);

                if (_callback != nullptr) {
                    remoteControl.Callback(_callback);
                }
            }

            _adminLock.Unlock();
        }
        void Revoke(Exchange::IKeyProducer& remoteControl)
        {
            _adminLock.Lock();

            std::list<Exchange::IKeyProducer*>::iterator index(std::find(_remotes.begin(), _remotes.end(), &remoteControl));

            // Only revoke remotes you subscribed !!!!
            ASSERT(index != _remotes.end());

            if (index != _remotes.end()) {
                _remotes.erase(index);

                if (_callback != nullptr) {
                    remoteControl.Callback(nullptr);
                }
            }

            _adminLock.Unlock();
        }

        void RevokeAll()
        {
            _adminLock.Lock();

            std::list<Exchange::IKeyProducer*>::iterator index(_remotes.begin());

            while (index != _remotes.end()) {

                if (_callback != nullptr) {
                    (*index)->Callback(nullptr);
                }

                index++;
            }
            _remotes.clear();

            _adminLock.Unlock();
        }

        void Callback(Exchange::IKeyHandler* callback)
        {
            _adminLock.Lock();

            ASSERT((_callback == nullptr) ^ (callback == nullptr));

            std::list<Exchange::IKeyProducer*>::iterator index(_remotes.begin());

            _callback = callback;

            while (index != _remotes.end()) {
                uint32 result = (*index)->Callback(callback);

                if (result != Core::ERROR_NONE) {
                    if (callback == nullptr) {
                        SYSLOG(PluginHost::Startup, (_T("Failed to initialize %s, error [%d]"), (*index)->Name(), result));
                    }
                    else {
                        SYSLOG(PluginHost::Shutdown, (_T("Failed to deinitialize %s, error [%d]"), (*index)->Name(), result));
                    }
                }

                index++;
            }

            _adminLock.Unlock();
        }

    private:
        Core::CriticalSection _adminLock;
        Exchange::IKeyHandler* _callback;
        std::list<Exchange::IKeyProducer*> _remotes;
    };
}
}

#endif // __REMOTEADMINISTRATOR_H
