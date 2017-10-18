#ifndef __IACTION_H
#define __IACTION_H

#include "Module.h"

namespace WPEFramework {
namespace Core {
    template <typename ELEMENT>
    struct IDispatchType {
        virtual ~IDispatchType(){};
        virtual void Dispatch(ELEMENT& element) = 0;
    };

    template <>
    struct IDispatchType<void> {
        virtual ~IDispatchType(){};
        virtual void Dispatch() = 0;
    };
}
} // namespace Core

#endif // __IACTION_H
