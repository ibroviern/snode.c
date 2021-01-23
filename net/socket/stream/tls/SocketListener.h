/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020 Volker Christian <me@vchrist.at>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NET_SOCKET_stream_TLS_SOCKETLISTENER_H
#define NET_SOCKET_stream_TLS_SOCKETLISTENER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <cstddef>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "Logger.h"
#include "SocketConnection.h"
#include "TLSHandshake.h"
#include "socket/stream/SocketListener.h"
#include "ssl_utils.h"

namespace net::socket::stream {

    template <typename SocketListener>
    class SocketServer;

    namespace tls {

        template <typename SocketT>
        class SocketListener : public stream::SocketListener<stream::tls::SocketConnection<SocketT>> {
        public:
            using SocketConnection = stream::tls::SocketConnection<SocketT>;
            using Socket = typename SocketConnection::Socket;
            using SocketAddress = typename Socket::SocketAddress;

            SocketListener(const std::function<void(SocketConnection* socketConnection)>& onConstruct,
                           const std::function<void(SocketConnection* socketConnection)>& onDestruct,
                           const std::function<void(SocketConnection* socketConnection)>& onConnect,
                           const std::function<void(SocketConnection* socketConnection)>& onDisconnect,
                           const std::function<void(SocketConnection* socketConnection, const char* junk, std::size_t junkLen)>& onRead,
                           const std::function<void(SocketConnection* socketConnection, int errnum)>& onReadError,
                           const std::function<void(SocketConnection* socketConnection, int errnum)>& onWriteError,
                           const std::map<std::string, std::any>& options)
                : stream::SocketListener<SocketConnection>(
                      onConstruct,
                      onDestruct,
                      [onConnect, &ctx = this->ctx](SocketConnection* socketConnection) -> void {
                          SSL* ssl = socketConnection->startSSL(ctx);

                          if (ssl != nullptr) {
                              socketConnection->SocketConnection::SocketReader::suspend();

                              SSL_set_accept_state(ssl);

                              TLSHandshake::doHandshake(
                                  ssl,
                                  [&onConnect, socketConnection](void) -> void { // onSuccess
                                      socketConnection->ReadEventReceiver::resume();
                                      onConnect(socketConnection);
                                  },
                                  [socketConnection](void) -> void { // onTimeout
                                      PLOG(ERROR) << "SSL/TLS handshake timeout";
                                      socketConnection->SocketConnection::SocketReader::disable();
                                  },
                                  [socketConnection](int sslErr) -> void { // onError
                                      ssl_log("SSL/TLS handshake failed", -sslErr);
                                      socketConnection->setSSLError(-sslErr);
                                      socketConnection->SocketConnection::SocketReader::disable();
                                  });
                          } else {
                              socketConnection->SocketConnection::SocketReader::disable();
                              ssl_log_error("SSL/TLS initialization failed");
                          }
                      },
                      [onDisconnect](SocketConnection* socketConnection) -> void { // onDisconnect
                          socketConnection->stopSSL();
                          onDisconnect(socketConnection);
                      },
                      onRead,
                      onReadError,
                      onWriteError,
                      options) {
                ctx = ssl_ctx_new(options, true);
                if (ctx != nullptr) {
                    SSL_CTX_set_session_id_context(ctx, reinterpret_cast<unsigned char*>(&sslSessionCtxId), sizeof(sslSessionCtxId));
                    sslSessionCtxId++;
                }
            }

            ~SocketListener() override {
                ssl_ctx_free(ctx);
            }

            void listen(const SocketAddress& localAddress, int backlog, const std::function<void(int err)>& onError) override {
                if (ctx == nullptr) {
                    errno = EINVAL;
                    onError(EINVAL);
                } else {
                    stream::SocketListener<SocketConnection>::listen(localAddress, backlog, onError);
                }
            }

        private:
            SSL_CTX* ctx = nullptr;

            static int sslSessionCtxId;
        };

        template <typename Socket>
        int SocketListener<Socket>::sslSessionCtxId = 1;

    } // namespace tls

} // namespace net::socket::stream

#endif // NET_SOCKET_stream_TLS_SOCKETLISTENER_H