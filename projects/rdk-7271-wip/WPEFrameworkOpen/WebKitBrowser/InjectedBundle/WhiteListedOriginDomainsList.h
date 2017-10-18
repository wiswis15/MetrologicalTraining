#ifndef __WHITELISTEDORIGINDOMAINSLIST_H
#define __WHITELISTEDORIGINDOMAINSLIST_H

#include "Module.h"

#include <WPE/WebKit.h>

#include <vector>
#include <memory>

namespace WPEFramework {
namespace WebKit {

    class WhiteListedOriginDomainsList {
    public:
        // Origin/Domain pair stored in JSON string.
        class OriginAccessWhitelistEntry : public Core::JSON::Container {
        private:
            OriginAccessWhitelistEntry& operator=(const OriginAccessWhitelistEntry&) = delete;

        public:
            OriginAccessWhitelistEntry(const Core::JSON::String& origin, const Core::JSON::String& domain);
            OriginAccessWhitelistEntry();
            OriginAccessWhitelistEntry(const OriginAccessWhitelistEntry& rhs);

            Core::JSON::String Origin;
            Core::JSON::String Domain;
        };

    public:
        void AddWhiteListToWebKit(WKBundleRef bundle);

        static std::unique_ptr<WhiteListedOriginDomainsList> MakeUnique();
        static std::unique_ptr<WhiteListedOriginDomainsList> RequestFromWPEFramework(WKBundleRef bundle);

        // Name of message posted between WPEFramework and WPEWebProcess.
        static inline string GetMessageName()
        {
            return Core::ClassNameOnly(typeid(WhiteListedOriginDomainsList).name()).Data();
        }

    private:
        WhiteListedOriginDomainsList() {}

        void AddPair(const OriginAccessWhitelistEntry& addedPair);

        static std::unique_ptr<WhiteListedOriginDomainsList> ParseWhiteListedOriginDomainPairs(const string& jsonString);

        static void ExtractProtocolAndHost(const std::string& domain, WKStringRef& protocol, WKStringRef& host);

        std::vector<OriginAccessWhitelistEntry> WhiteListedPairs;
    };
}
}

#endif // __WHITELISTEDORIGINDOMAINSLIST_H
