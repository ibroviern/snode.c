#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "WebApp.h"

#include "HTTPContext.h"
#include "socket/legacy/SocketServer.h"


namespace legacy {

    void WebApp::listen(int port, const std::function<void(int err)>& onError) {
        errno = 0;

        legacy::SocketServer::instance(
            [this](legacy::SocketConnection* connectedSocket) -> void { // onConnect
                connectedSocket->setAttribute<HTTPContext*>(new HTTPContext(*this, connectedSocket));
            },
            [](legacy::SocketConnection* connectedSocket) -> void { // onDisconnect
                connectedSocket->getAttribute<HTTPContext*>([](HTTPContext*& context) -> void {
                    delete context;
                });
            },
            [](legacy::SocketConnection* connectedSocket, const char* junk, ssize_t n) -> void { // readProcessor
                connectedSocket->getAttribute<HTTPContext*>([&junk, &n](HTTPContext*& context) -> void {
                    context->receiveData(junk, n);
                });
            },
            [](legacy::SocketConnection* connectedSocket, int errnum) -> void { // onReadError
                connectedSocket->getAttribute<HTTPContext*>([&errnum](HTTPContext*& context) -> void {
                    context->onReadError(errnum);
                });
            },
            [](legacy::SocketConnection* connectedSocket, int errnum) -> void { // onWriteError
                connectedSocket->getAttribute<HTTPContext*>([&errnum](HTTPContext*& context) -> void {
                    context->onReadError(errnum);
                });
            })
            ->listen(port, 5, [&](int err) -> void {
                if (onError) {
                    onError(err);
                }
            });
    }

} // namespace legacy
