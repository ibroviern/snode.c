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

#ifndef LEGACY_SOCKETLISTENER_H
#define LEGACY_SOCKETLISTENER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "SocketConnection.h"
#include "socket/sock_stream/SocketListener.h"

namespace net::socket::stream::legacy {

    template <typename SocketT>
    class SocketListener : public net::socket::stream::SocketListener<net::socket::stream::legacy::SocketConnection<SocketT>> {
    public:
        using Socket = SocketT;
        using SocketConnection =
            typename net::socket::stream::SocketListener<net::socket::stream::legacy::SocketConnection<Socket>>::SocketConnection;

        using net::socket::stream::SocketListener<net::socket::stream::legacy::SocketConnection<Socket>>::SocketListener;
    };

} // namespace net::socket::stream::legacy

#endif // LEGACY_SOCKETLISTENER_H