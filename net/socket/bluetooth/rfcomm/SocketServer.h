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

#ifndef NET_SOCKET_BLUETOOTH_RFCOMM_SOCKETSERVER_H
#define NET_SOCKET_BLUETOOTH_RFCOMM_SOCKETSERVER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <functional>
#include <string>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace net::socket::bluetooth::rfcomm {

    template <typename SocketServerSuper>
    class SocketServer : public SocketServerSuper {
    public:
        using SocketServerSuper::SocketServerSuper;

        using SocketConnection = typename SocketServerSuper::SocketConnection;
        using Socket = typename SocketConnection::Socket;
        using SocketAddress = typename Socket::SocketAddress;

        void listen(uint8_t port, int backlog, const std::function<void(int err)>& onError) const {
            SocketServerSuper::listen(SocketAddress(port), backlog, onError);
        }

        void listen(const std::string& btAddress, uint8_t port, int backlog, const std::function<void(int err)>& onError) const {
            SocketServerSuper::listen(SocketAddress(btAddress, port), backlog, onError);
        }
    };

} // namespace net::socket::bluetooth::rfcomm

#endif // NET_SOCKET_BLUETOOTH_RFCOMM_SOCKETSERVER_H