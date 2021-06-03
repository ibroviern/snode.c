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

#include "SocketContext.h"

#include "log/Logger.h"
#include "web/ws/SubProtocol.h" // for SubProtocol, SubProtocol::Role, WSProto...
#include "web/ws/server/SubProtocolPluginInterface.h"
#include "web/ws/server/SubProtocolSelector.h"
#include "web/ws/ws_utils.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#define CLOSE_SOCKET_TIMEOUT 10

namespace web::ws::server {

    SocketContext::SocketContext(web::ws::server::SubProtocol* subProtocol, web::ws::SubProtocol::Role role)
        : web::ws::SocketContext(subProtocol, role) {
    }

    SocketContext::SocketContext::~SocketContext() {
        web::ws::SubProtocolPluginInterface* subProtocolPluginInterface =
            web::ws::server::SubProtocolSelector::instance().select(subProtocol->getName());

        if (subProtocolPluginInterface != nullptr) {
            subProtocolPluginInterface->destroy(dynamic_cast<web::ws::server::SubProtocol*>(subProtocol));
        }
    }

    SocketContext* SocketContext::create(web::http::server::Request& req, web::http::server::Response& res) {
        std::string subProtocolName = req.header("sec-websocket-protocol");

        web::ws::server::SubProtocolPluginInterface* subProtocolPluginInterface =
            web::ws::server::SubProtocolSelector::instance().select(subProtocolName);

        web::ws::server::SocketContext* Context = nullptr;

        if (subProtocolPluginInterface != nullptr) {
            web::ws::server::SubProtocol* subProtocol =
                static_cast<web::ws::server::SubProtocol*>(subProtocolPluginInterface->create(req, res));

            if (subProtocol != nullptr) {
                subProtocol->setClients(subProtocolPluginInterface->getClients());

                Context = new web::ws::server::SocketContext(subProtocol, web::ws::SubProtocol::Role::SERVER);

                if (Context != nullptr) {
                    res.set("Upgrade", "websocket");
                    res.set("Connection", "Upgrade");
                    res.set("Sec-WebSocket-Protocol", subProtocolName);

                    web::ws::serverWebSocketKey(req.header("sec-websocket-key"), [&res](char* key) -> void {
                        res.set("Sec-WebSocket-Accept", key);
                    });

                    res.status(101); // Switch Protocol
                } else {
                    res.status(500);
                }
            } else {
                res.status(500); // Internal Server Error
            }
        } else {
            res.status(404); // Not found
        }

        return Context;
    }

} // namespace web::ws::server