#pragma once

namespace WPEFramework
{
    typedef Core::IPCMessageType<99,
            Core::IPC::ScalarType<uint32_t>,
            Core::IPC::ScalarType<uint32_t> > RPCTestMessage;
}
