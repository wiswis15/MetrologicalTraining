#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "../../generics/generics.h"
#include "../../devices/devices.h"

ENUM_CONVERSION_BEGIN(WPEFramework::RaspberryPi::Board::model)

    { WPEFramework::RaspberryPi::Board::MODEL_A,       _TXT(_T("Pi 1 A"))        },
    { WPEFramework::RaspberryPi::Board::MODEL_B,       _TXT(_T("Pi 1 B"))        },
    { WPEFramework::RaspberryPi::Board::MODEL_A_PLUS,  _TXT(_T("Pi 1 A+"))       },
    { WPEFramework::RaspberryPi::Board::MODEL_B_PLUS,  _TXT(_T("Pi 1 B+"))       },
    { WPEFramework::RaspberryPi::Board::MODEL_PI_2,    _TXT(_T("Pi 2"))          },
    { WPEFramework::RaspberryPi::Board::MODEL_ALPHA,   _TXT(_T("Pi Alpha"))      },
    { WPEFramework::RaspberryPi::Board::MODEL_CM,      _TXT(_T("Pi CM"))         },
    { WPEFramework::RaspberryPi::Board::MODEL_PI_3,    _TXT(_T("Pi 3"))          },
    { WPEFramework::RaspberryPi::Board::MODEL_PI_ZERO, _TXT(_T("Pi Zero"))       },
    { WPEFramework::RaspberryPi::Board::MODEL_UNKNOWN, _TXT(_T("Model Unknown")) },

ENUM_CONVERSION_END(WPEFramework::RaspberryPi::Board::model)

ENUM_CONVERSION_BEGIN(WPEFramework::RaspberryPi::Board::manufacturer)

    { WPEFramework::RaspberryPi::Board::SONY,    _TXT(_T("Sony"))    },
    { WPEFramework::RaspberryPi::Board::EGOMAN,  _TXT(_T("Egoman"))  },
    { WPEFramework::RaspberryPi::Board::EMBEST,  _TXT(_T("Embest"))  },
    { WPEFramework::RaspberryPi::Board::UNKNOWN, _TXT(_T("Unknown")) },

ENUM_CONVERSION_END(WPEFramework::RaspberryPi::Board::manufacturer)

ENUM_CONVERSION_BEGIN(WPEFramework::RaspberryPi::Board::revision)

    { WPEFramework::RaspberryPi::Board::REVISION_0,   _TXT(_T("0"))   },
    { WPEFramework::RaspberryPi::Board::REVISION_1,   _TXT(_T("1"))   },
    { WPEFramework::RaspberryPi::Board::REVISION_1_1, _TXT(_T("1.1")) },
    { WPEFramework::RaspberryPi::Board::REVISION_1_2, _TXT(_T("1.2")) },
    { WPEFramework::RaspberryPi::Board::REVISION_2,   _TXT(_T("2"))   },

ENUM_CONVERSION_END(WPEFramework::RaspberryPi::Board::revision)

ENUM_CONVERSION_BEGIN(WPEFramework::RaspberryPi::Board::processor)

    { WPEFramework::RaspberryPi::Board::BCM2835, _TXT(_T("BCM2835")) },
    { WPEFramework::RaspberryPi::Board::BCM2836, _TXT(_T("BCM2836")) },

ENUM_CONVERSION_END(WPEFramework::RaspberryPi::Board::processor)

ENUM_CONVERSION_BEGIN(WPEFramework::RaspberryPi::Board::memory)

    { WPEFramework::RaspberryPi::Board::MEM_256M,  _TXT(_T("256MB"))  },
    { WPEFramework::RaspberryPi::Board::MEM_512M,  _TXT(_T("512MB"))  },
    { WPEFramework::RaspberryPi::Board::MEM_1024M, _TXT(_T("1024MB")) },

ENUM_CONVERSION_END(WPEFramework::RaspberryPi::Board::memory)

ENUM_CONVERSION_BEGIN(WPEFramework::RaspberryPi::Pin::trigger_mode)

    { WPEFramework::RaspberryPi::Pin::RISING,  _TXT(_T("rising"))  },
    { WPEFramework::RaspberryPi::Pin::FALLING, _TXT(_T("falling")) },
    { WPEFramework::RaspberryPi::Pin::BOTH,    _TXT(_T("both"))    },
    { WPEFramework::RaspberryPi::Pin::HIGH,    _TXT(_T("high"))    },
    { WPEFramework::RaspberryPi::Pin::LOW,     _TXT(_T("low"))     },

ENUM_CONVERSION_END(WPEFramework::RaspberryPi::Pin::trigger_mode)

using namespace WPEFramework;

bool ParseOptions(int argc, char** argv, uint8& pin, uint8& value)
{
    int index = 1;
    bool showHelp = false;

    while ((index < argc) && (showHelp == false)) {

        if (strncmp(argv[index], "-pin=", 5) == 0) {
            pin = (argv[index][5] - '0');
            if (argv[index][6] != '\0') {
                pin = (pin * 10) + (argv[index][6] - '0');
            }
        }
        else if (strncmp(argv[index], "-set=", 5) == 0) {
            value = (((argv[index][5] - '0') & 0x01) << 4) | 0x20;
        }
        else if (strncmp(argv[index], "-input=", 7) == 0) {
	    value = (Core::EnumerateType<RaspberryPi::Pin::trigger_mode>(&(argv[index][7])).Value());
            printf("Mode set to value: %s -> %d \n", &(argv[index][7]), value);
        }
        else {
            showHelp = true;
        }
        index++;
    }

    return (showHelp);
}

void ShowDetails (RaspberryPi::Board& board) {
    printf("RaspberryPi details:\n");
    printf("   Model:        %s\n", Core::EnumerateType<RaspberryPi::Board::model>(board.Model()).Data());
    printf("   Revision:     %s\n", Core::EnumerateType<RaspberryPi::Board::revision>(board.Revision()).Data());
    printf("   Manufacturer: %s\n", Core::EnumerateType<RaspberryPi::Board::manufacturer>(board.Manufacturer()).Data());
    printf("   Processor:    %s\n", Core::EnumerateType<RaspberryPi::Board::processor>(board.Processor()).Data());
    printf("   Memory:       %s\n", Core::EnumerateType<RaspberryPi::Board::memory>(board.Memory()).Data());
    printf("   Warranty:     %s\n", (board.Warranty() ? "true" : "void") );
}

class PinObserver : public RaspberryPi::IObserver {
  
  virtual void Activity (RaspberryPi::Pin&  pin) {
    printf("Pin: %d, now has value: %s\n", pin.Id(), (pin.Get() ? "high" : "low"));
  }
};


int main(int argc, char** argv)
{
    // Get access to the physical board.
    RaspberryPi::Board& board (RaspberryPi::Board::Instance());

    if (board.Error() != 0) {
        printf ("Error constructing board. Error %d\n", board.Error());
        printf("  0: Everything Oke.\n");
        printf("  1: /proc/cpuinfo could not be opened.\n");
        printf("  2: Does not seem to be valid hardware platform.\n");
        printf("  3: Hardware (BCM2708/BCM2709) found but not a proper revision.\n");
        printf("  4: Hardware keyword found but not proper hardware (only BCM2708/BCM2709 supported)\n");
        printf("  5: Invalid formatted version\n");
        printf("  6: Could not open memory space GPIO\n");
        printf("  7: Could not open memory space (root priviliges)\n");
        printf("  9: Could not map base IO pointer\n");
        printf(" 10: Could not map PWM IO pointer\n");
        printf(" 11: Could not map clock IO pointer\n");
        printf(" 12: Could not map pads IO pointer\n");
        printf(" 13: Could not map timer IO pointer\n");
    }
    else {
        uint8 physical = 0;
        uint8 value = 0;

        if (ParseOptions(argc, argv, physical, value) == true) {
            printf("\nraspio [-set=<0|1>] [-pin=<0..32>] [-input=<falling|rising|both|high|low>]\n");
            ShowDetails(board);
            Core::Singleton::Dispose();
            exit(0);
        }

        Core::ProxyType<RaspberryPi::Pin> pin (board.BoardPin(physical));

        if (pin.IsValid() == false) {
            printf ("This is not an IO pin [%d]. Could not act on this pin.\n", physical);
            ShowDetails(board);
        }
        else {
	    PinObserver callback;

            if ((value & 0x20) != 0) {
                // We got an explicit set, set the value and bail out. 
                pin->Mode(RaspberryPi::Pin::OUTPUT);
                pin->Set((value & 0x10) != 0);
            } 
            else {
                int element;
                RaspberryPi::Pin::trigger_mode triggerMode(static_cast<RaspberryPi::Pin::trigger_mode>(value & 0x0F));

                do {
                    element = toupper(getchar());

                    switch (element) {
                        case 'O': {
                            //  Observe the pin to (Input)
                            pin->Mode(RaspberryPi::Pin::INPUT);
			    pin->Register(&callback);
                            if ((value & 0x0F) == 0) {
                                printf ("Observing pin [%d] in mode [default]. Current value: %s\n", physical, (pin->Get() ? "High" : "Low"));
                            }
                            else {
                                pin->Trigger(triggerMode);
                                printf ("Observing pin [%d] in mode [%s]. Current value: %s\n", 
                                    physical, 
                                    Core::EnumerateType<RaspberryPi::Pin::trigger_mode>(triggerMode).Data(), 
                                    (pin->Get() ? "High" : "Low"));
                            }
                            break;
                        }
                        case 'S': {
                            // Set the pin to 1 (Output)
                            pin->Mode(RaspberryPi::Pin::OUTPUT);
                            pin->Set(true);
                            printf ("Set pin [%d]. Current value: %s\n", physical, (pin->Get() ? "High" : "Low"));
                            break;
                        }
                        case 'C': {
                            // Clear the pin to 0 (Output)
                            pin->Mode(RaspberryPi::Pin::OUTPUT);
                            pin->Set(false);
                            printf ("Clear pin [%d]. Current value: %s\n", physical, (pin->Get() ? "High" : "Low"));
                            break;
                        }
                        case 'D': {
                            // Print the diaganostics information
                            ShowDetails(board);
                            break;
                        }
                        case 'Q': break;
                        default: {
                        }
                    }
                } while (element != 'Q');
            }
        }
    }

    Core::Singleton::Dispose();

    return (0);
}
