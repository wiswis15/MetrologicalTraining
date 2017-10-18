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

bool ParseOptions(int argc, char** argv, uint8& ce, uint8& cs, uint8& port)
{
    int index = 1;
    bool showHelp = false;

    while ((index < argc) && (showHelp == false)) {

        if (strncmp(argv[index], "-ce=", 5) == 0) {
            ce = (argv[index][4] - '0');
            if (argv[index][5] != '\0') {
                ce = (ce * 10) + (argv[index][5] - '0');
            }
        }
        else if (strncmp(argv[index], "-cs=", 5) == 0) {
            cs = (argv[index][4] - '0');
            if (argv[index][5] != '\0') {
                cs = (cs * 10) + (argv[index][5] - '0');
            }
        }
        else if (strncmp(argv[index], "-port=", 6) == 0) {
            port = (argv[index][7] - '0');
        }
        else {
            showHelp = true;
        }
        index++;
    }

    return (showHelp);
}

int main(int argc, char** argv)
{
    uint8 ce = 0;
    uint8 cs = 0;
    uint8 port = 0;

    if ( (ParseOptions(argc, argv, ce, cs, port) == true) && (cs != 0) && (ce != 0) ) {
        printf("\nnrf24l01 [-ce=<0..32>] [-cs=<0..23>] [-port=[0..1]\n");
        Core::Singleton::Dispose();
        exit(0);
    }

    TCHAR portName[32];
    sprintf(portName, "/dev/spi%d", port);

    Devices::RaspNRF24L01 device (cs, ce, string(portName), 5);
    char element;

    do {
        element = toupper(getchar());

        switch (element) {
            case 'O': {
                //  Observe the pin to (Input)
                break;
            }
            case 'S': {
                // Set the pin to 1 (Output)
                break;
            }
            case 'C': {
                // Clear the pin to 0 (Output)
                break;
            }
            case 'Q': break;
            default: {
            }
        }
    } while (element != 'Q');

    Core::Singleton::Dispose();

    return (0);
}
