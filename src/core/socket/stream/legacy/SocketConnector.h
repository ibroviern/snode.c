/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020, 2021 Volker Christian <me@vchrist.at>
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

#ifndef NET_SOCKET_STREAM_LEGACY_SOCKETCONNECTOR_H
#define NET_SOCKET_STREAM_LEGACY_SOCKETCONNECTOR_H

#include "core/socket/stream/SocketConnector.h"
#include "core/socket/stream/legacy/SocketConnection.h" // IWYU pragma: export

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace core::socket::stream::legacy {

    template <typename SocketT>
    class SocketConnector : public core::socket::stream::SocketConnector<legacy::SocketConnection<SocketT>> {
    private:
        using Socket = SocketT;

    public:
        using SocketConnection = legacy::SocketConnection<Socket>;
        using SocketAddress = typename Socket::SocketAddress;

        SocketConnector(const std::shared_ptr<SocketContextFactory>& socketContextFactory,
                        const std::function<void(const SocketAddress&, const SocketAddress&)>& onConnect,
                        const std::function<void(SocketConnection*)>& onConnected,
                        const std::function<void(SocketConnection*)>& onDisconnect,
                        const std::map<std::string, std::any>& options)
            : core::socket::stream::SocketConnector<SocketConnection>(
                  socketContextFactory,
                  onConnect,
                  [onConnected](SocketConnection* socketConnection) -> void {
                      socketConnection->SocketConnection::SocketReader::resume();
                      onConnected(socketConnection);
                  },
                  onDisconnect,
                  options) {
        }
    };

} // namespace core::socket::stream::legacy

#endif // NET_SOCKET_STREAM_LEGACY_SOCKETCONNECTOR_H