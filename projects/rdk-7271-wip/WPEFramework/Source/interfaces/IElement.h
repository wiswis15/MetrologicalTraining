#ifndef __IELEMENT_OBJECT_H
#define __IELEMENT_OBJECT_H

// ---- Include system wide include files ----
#include "Module.h"

// ---- Include local include files ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Referenced classes and types ----

// ---- Class Definition ----

namespace WPEFramework {
	namespace Exchange {
		struct IElement : virtual public Core::IUnknown {
			virtual ~IElement() {}

			enum { ID = 0x0000004C };


			struct INotification : virtual public Core::IUnknown {

				virtual ~INotification() {}

				enum { ID = 0x0000004D };

				// Push changes. If the Current value changes, the next method is called.
				virtual void Update() = 0;
			};

			struct IFactory : virtual public Core::IUnknown {
				virtual ~IFactory() {}

				enum { ID = 0x0000004E };

				struct INotification : virtual public Core::IUnknown {

					virtual ~INotification() {}

					enum { ID = 0x0000004F };

					virtual void Activated(IElement* source) = 0;
					virtual void Deactivated(IElement* source) = 0;
				};

				// Pushing notifications to interested sinks
				virtual void Register(INotification* sink) = 0;
				virtual void Unregister(INotification* sink) = 0;
			};

			enum identification {
				ZWAVE = 0x01000000
			};

			//  Basic/specific and dimension together define the Type.
			// 32     13    | 3 |  4  |     12     |
			//  +---------------+------------------+
			//  | dimension |FLT|basic|  specific  |
			//  +---------------+------------------+
			//  FLT = Floating point. The number of decimals thats 
			//        should be considerd to be the remainder.
			//        3 bits (0..7)
			//
			enum basic {          /* 4 bits (16)*/
				regulator       = 0x0,
				measurement     = 0x1
			};

			enum specific {       /* 12 bits (4096) */
				general         = 0x000,
				electricity     = 0x001,
				water           = 0x002,
				gas             = 0x003,
				air             = 0x004,
				smoke           = 0x005,
				carbonMonoxide  = 0x006,
				carbonDioxide   = 0x007,
				temperature     = 0x008,
				accessControl   = 0x009,
				burglar         = 0x00A,
				powerManagement = 0x00B,
				system          = 0x00C,
				emergency       = 0x00D,
				clock           = 0x00E
			};

			enum dimension {      /* 13 bits (8192) */
				logic           = 0x0000,        /* values 0 or 1  */
				percentage      = 0x0001,        /* values 0 - 100 */
				kwh             = 0x0002,        /* kilo Watt hours  */
				kvah            = 0x0003,        /* kilo Volt Ampere hours */
				pulses          = 0x0004,        /* counter */
				degrees         = 0x0005
			};

			enum condition {
				constructing    = 0x0000,
				activated       = 0x0001,
				deactivated     = 0x0002
			};

			// ------------------------------------------------------------------------
			// Convenience methods to extract interesting information from the type
			// ------------------------------------------------------------------------
			inline basic Basic() const {
				return (static_cast<basic>((Type() >> 12) & 0xF));
			}
			inline dimension Dimension() const {
				return (static_cast<IElement::dimension>((Type() >> 19) & 0x1FFF));
			}
			inline specific Specific() const {
				return (static_cast<IElement::specific>(Type() & 0xFFF));
			}
			inline uint8 Decimals() const {
				return ((Type() >> 16) & 0x07);
			}

			// Pushing notifications to interested sinks
			virtual void Register(INotification* sink) = 0;
			virtual void Unregister(INotification* sink) = 0;

			// Element require communication, so might fail, report our condition
			virtual condition Condition() const = 0;

			// Identification of this element.
			virtual uint32 Identification() const = 0;

			// Characteristics of this element
			virtual uint32 Type() const = 0;

			// Value determination of this element
			virtual sint32 Minimum() const = 0;
			virtual sint32 Maximum() const = 0;
			virtual sint32 Current() const = 0;
			virtual uint32 Current(const sint32 value) = 0;

			// Periodically we might like to be triggered, call this method at a set time.
			virtual void Trigger() = 0;
		};
	}
} // Namespace Exchange

#endif // __IELEMENT_OBJECT_H