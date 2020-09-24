#ifndef SERVERT_H
#define SERVERT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <any>
#include <easylogging++.h>
#include <functional>
#include <map>
#include <netinet/in.h>
#include <string>

#endif // DOXYGEN_SHOULD_SKIP_THIS

#include "ServerContext.h"

namespace http {

    class Request;
    class Response;

    template <typename SocketServerT>
    class Server {
    public:
        using SocketServer = SocketServerT;
        using SocketConnection = typename SocketServer::SocketListener::SocketConnection;

        Server(const std::function<void(SocketConnection*)>& onConnect,
               const std::function<void(Request& req, Response& res)>& onRequestReady,
               const std::function<void(Request& req, Response& res)>& onRequestCompleted,
               const std::function<void(SocketConnection*)>& onDisconnect,
               const std::map<std::string, std::any>& options = {{}})
            : socketServer(
                  [onConnect, onRequestReady, onRequestCompleted](SocketConnection* socketConnection) -> void { // onConnect
                      socketConnection->template setContext<ServerContext*>(
                          new ServerContext(socketConnection, onRequestReady, onRequestCompleted));
                      onConnect(socketConnection);
                  },
                  [onDisconnect](SocketConnection* socketConnection) -> void { // onDisconnect
                      onDisconnect(socketConnection);
                      socketConnection->template getContext<ServerContext*>([](ServerContext*& serverContext) -> void {
                          delete serverContext;
                      });
                  },
                  [](SocketConnection* socketConnection, const char* junk, ssize_t junkSize) -> void { // onRead
                      socketConnection->template getContext<ServerContext*>([&junk, &junkSize](ServerContext*& serverContext) -> void {
                          serverContext->receiveRequestData(junk, junkSize);
                      });
                  },
                  [](SocketConnection* socketConnection, int errnum) -> void { // onReadError
                      socketConnection->template getContext<ServerContext*>([&errnum](ServerContext*& serverContext) -> void {
                          serverContext->onReadError(errnum);
                      });
                  },
                  [](SocketConnection* socketConnection, int errnum) -> void { // onWriteError
                      socketConnection->template getContext<ServerContext*>([&errnum](ServerContext*& serverContext) -> void {
                          serverContext->onWriteError(errnum);
                      });
                  },
                  options) {
        }

    public:
        void listen(unsigned short port, const std::function<void(int err)>& onError) {
            errno = 0;

            socketServer.listen(port, 5, onError);
        }

        void listen(const std::string host, unsigned short port, const std::function<void(int err)>& onError) {
            errno = 0;

            socketServer.listen(host, port, 5, onError);
        }

    protected:
        SocketServer socketServer;
    };

} // namespace http

#endif // SERVERT_H