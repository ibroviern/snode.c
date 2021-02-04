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

#ifndef LEGACY_CLIENT_H
#define LEGACY_CLIENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "client/Client.h"
#include "socket/ip/tcp/ipv4/legacy/SocketClient.h"
#include "socket/ip/tcp/ipv6/legacy/SocketClient.h"

namespace http::client::legacy {

    class Client : public http::client::Client<net::socket::ip::tcp::ipv4::legacy::SocketClient> {
    public:
        using http::client::Client<net::socket::ip::tcp::ipv4::legacy::SocketClient>::Client;
    };

    class Client6 : public http::client::Client<net::socket::ip::tcp::ipv6::legacy::SocketClient> {
    public:
        using http::client::Client<net::socket::ip::tcp::ipv6::legacy::SocketClient>::Client;
    };

} // namespace http::client::legacy

#endif // LEGACY_CLIENT_H