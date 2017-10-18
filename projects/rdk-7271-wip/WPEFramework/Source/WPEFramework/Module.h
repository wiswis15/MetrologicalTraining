#ifndef __MODULE_FRAMEWORK_APPLICATION_H
#define __MODULE_FRAMEWORK_APPLICATION_H

#ifndef MODULE_NAME
#define MODULE_NAME Application
#endif

#include "../core/core.h"
#include "../tracing/tracing.h"
#include "../cryptalgo/cryptalgo.h"
#include "../websocket/websocket.h"
#include "../plugins/plugins.h"

#undef EXTERNAL
#define EXTERNAL

#endif // __MODULE_FRAMEWORK_APPLICATION_H
