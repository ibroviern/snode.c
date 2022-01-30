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

#include "net/un/stream/ServerConfig.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include "utils/CLI11.hpp"
#include "utils/Config.h"

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace net::un::stream {
    ServerConfig::ServerConfig(const std::string& name)
        : net::ConfigBase(name) {
        net::ConfigBacklog::populate(serverSc);
        net::un::stream::ConfigBind::populate(serverSc);
        net::ConfigConn::populate(serverSc);
    }

    void ServerConfig::required(bool req) const {
        utils::Config::instance().required(serverSc, req);
        utils::Config::instance().required(serverBindSc, req);
        utils::Config::instance().required(bindServerSunPathOpt, req);
    }

} // namespace net::un::stream