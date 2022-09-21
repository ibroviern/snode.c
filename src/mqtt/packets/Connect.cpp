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

#include "mqtt/packets/Connect.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif // DOXYGEN_SHOUÖD_SKIP_THIS

namespace mqtt::packets {

    Connect::Connect(ControlPacketFactory& controlPacketFactory)
        : mqtt::ControlPacket(controlPacketFactory) {
    }

    Connect::~Connect() {
    }

    std::string Connect::protocol() const {
        uint16_t protocolLength = be16toh(*reinterpret_cast<uint16_t*>(const_cast<char*>(data.data() + 0)));

        return std::string(data.data() + 2, protocolLength);
    }

    uint8_t Connect::version() const {
        return static_cast<uint8_t>(*(data.data() + 6));
    }

    uint8_t Connect::flags() const {
        return static_cast<uint8_t>(*(data.data() + 7));
    }

    uint16_t Connect::keepAlive() const {
        return be16toh(*reinterpret_cast<uint16_t*>(const_cast<char*>(data.data() + 8)));
    }
    /*
        uint8_t Connect::propertyLength() const {
        }

        std::vector<char> Connect::properties() const {
        }

        uint8_t Connect::payloadLength() const {
        }

        std::vector<char> Connect::payload() const {
        }
    */

} // namespace mqtt::packets
