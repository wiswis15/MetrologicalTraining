#include "WhiteListedOriginDomainsList.h"

#include "Utils.h"

using std::vector;
using std::unique_ptr;

// For now we report errors to stderr.
// TODO: does the injected bundle need a more formal way of dealing with errors?
#include <iostream>
using std::cerr;
using std::endl;

namespace WPEFramework {
namespace WebKit {

    // Constructor via list origin-domain pair.
    WhiteListedOriginDomainsList::OriginAccessWhitelistEntry::OriginAccessWhitelistEntry(
        const Core::JSON::String& origin, const Core::JSON::String& domain)
        : Core::JSON::Container()
        , Origin(origin)
        , Domain(domain)
    {
        Add(_T("origin"), &Origin);
        Add(_T("domain"), &Domain);
    }

    // Default constructor.
    WhiteListedOriginDomainsList::OriginAccessWhitelistEntry::OriginAccessWhitelistEntry()
        : Core::JSON::Container()
        , Origin()
        , Domain()
    {
        Add(_T("origin"), &Origin);
        Add(_T("domain"), &Domain);
    }

    // Copy-constructor.
    WhiteListedOriginDomainsList::OriginAccessWhitelistEntry::OriginAccessWhitelistEntry(const OriginAccessWhitelistEntry& rhs)
        : Core::JSON::Container()
        , Origin(rhs.Origin)
        , Domain(rhs.Domain)
    {
        Add(_T("origin"), &Origin);
        Add(_T("domain"), &Domain);
    }

    // Returns instance wrapped in unique_ptr.
    /* static */ unique_ptr<WhiteListedOriginDomainsList> WhiteListedOriginDomainsList::MakeUnique()
    {
        return unique_ptr<WhiteListedOriginDomainsList>(new WhiteListedOriginDomainsList());
    }

    // Parses JSON containing white listed CORS origin-domain pairs.
    /* static */ unique_ptr<WhiteListedOriginDomainsList> WhiteListedOriginDomainsList::ParseWhiteListedOriginDomainPairs(const string& jsonString)
    {
        unique_ptr<WhiteListedOriginDomainsList> whiteList = MakeUnique();

        Core::JSON::ArrayType<OriginAccessWhitelistEntry> entries;
        entries.FromString(jsonString);
        Core::JSON::ArrayType<OriginAccessWhitelistEntry>::Iterator originIndex(entries.Elements());

        while (originIndex.Next() == true) {
            const Core::JSON::String& origin = originIndex.Current().Origin;
            const Core::JSON::String& domain = originIndex.Current().Domain;

            if ((origin.IsSet() == true) && (domain.IsSet() == true)) {
                Core::JSON::ArrayType<Core::JSON::String> domains;
                domains.FromString(domain);

                Core::JSON::ArrayType<Core::JSON::String>::Iterator domainIndex(domains.Elements());
                while (domainIndex.Next()) {
                    whiteList->AddPair(OriginAccessWhitelistEntry(origin, domainIndex.Current()));
                }
            }
        }

        return whiteList;
    }

    // Gets white list from WPEFramework via synchronous message.
    /* static */ unique_ptr<WhiteListedOriginDomainsList> WhiteListedOriginDomainsList::RequestFromWPEFramework(WKBundleRef bundle)
    {
        string messageName = GetMessageName();
        std::string utf8MessageName = Core::ToString(messageName.c_str());

        WKStringRef jsMessageName = WKStringCreateWithUTF8CString(utf8MessageName.c_str());
        WKMutableArrayRef messageBody = WKMutableArrayCreate();
        WKTypeRef returnData;

        WKBundlePostSynchronousMessage(bundle, jsMessageName, messageBody, &returnData);

        WKStringRef returnedString = static_cast<WKStringRef>(returnData);

        string jsonString = WebKit::Utils::WKStringToString(returnedString);

        unique_ptr<WhiteListedOriginDomainsList> whiteList = ParseWhiteListedOriginDomainPairs(jsonString);

        WKRelease(returnData);
        WKRelease(messageBody);
        WKRelease(jsMessageName);

        return whiteList;
    }

    // Stores origin/domain pair.
    void WhiteListedOriginDomainsList::AddPair(const OriginAccessWhitelistEntry& addedPair)
    {
        WhiteListedPairs.push_back(addedPair);
    }

    // Adds stored entries to WebKit.
    void WhiteListedOriginDomainsList::AddWhiteListToWebKit(WKBundleRef bundle)
    {
        const bool allowDestinationSubdomains = true;

        for (const OriginAccessWhitelistEntry& pair : WhiteListedPairs) {
            const string& origin = pair.Origin.Value();
            const string& domain = pair.Domain.Value();

            WKStringRef protocol, host;
            ExtractProtocolAndHost(domain, protocol, host);
            WKStringRef wkOrigin = WKStringCreateWithUTF8CString(origin.c_str());

            WKBundleAddOriginAccessWhitelistEntry(bundle, wkOrigin, protocol, host, allowDestinationSubdomains);

            WKRelease(host);
            WKRelease(protocol);
            WKRelease(wkOrigin);

            cerr << "Added origin->domain pair to WebKit white list: " << origin << " -> " << domain << endl;
        }
    }

    // Gets protocol and host from url string. "protocol" and "host" should be released by caller.
    /*virtual */ void WhiteListedOriginDomainsList::ExtractProtocolAndHost(const string& domain, WKStringRef& protocol, WKStringRef& host)
    {
        std::string utf8Domain = Core::ToString(domain.c_str());
        WKURLRef url = WKURLCreateWithUTF8CString(utf8Domain.c_str());

        protocol = WKURLCopyScheme(url);
        host = WKURLCopyHostName(url);

        WKRelease(url);
    }
}
}
