/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020, 2021, 2022 Volker Christian <me@vchrist.at>
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

#ifndef NET_L2_STREAM_SERVERCONFIG_H
#define NET_L2_STREAM_SERVERCONFIG_H

#include "net/ConfigBacklog.h"
#include "net/ConfigBase.h"
#include "net/ConfigConn.h"
#include "net/l2/ConfigLocal.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <string> // for string

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace net::l2::stream {

    class ServerConfig
        : public net::ConfigBase
        , public net::ConfigBacklog
        , public net::l2::ConfigLocal
        , public net::ConfigConn {
    public:
        explicit ServerConfig(const std::string& name);
    };

} // namespace net::l2::stream

#endif
