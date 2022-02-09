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

#ifndef NET_CONFIGLOCAL_HPP
#define NET_CONFIGLOCAL_HPP

#include "ConfigLocal.h" // IWYU pragma: export

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include "utils/CLI11.hpp"
#include "utils/Config.h"

#endif // DOXYGEN_SHOULD_SKIP_THIS

namespace net::config {

    template <typename SocketAddress>
    ConfigLocal<SocketAddress>::ConfigLocal(CLI::App* baseSc) {
        addressSc = baseSc->add_subcommand("local");
        addressSc->description("Bind options");
        addressSc->configurable();
    }

    template <typename SocketAddress>
    const SocketAddress& ConfigLocal<SocketAddress>::getLocalAddress() {
        if (!initialized) {
            utils::Config::instance().parse(true); // Try command line parsing in case Address is not initialized using setLocalAddress

            address = getAddress();
            initialized = true;
        } else if (!updated) {
            updateFromCommandLine();
            updated = true;
        }

        return address;
    }

    template <typename SocketAddress>
    void ConfigLocal<SocketAddress>::setLocalAddress(const SocketAddress& localAddress) {
        this->address = localAddress;
        this->initialized = true;
    }

    template <typename SocketAddress>
    void ConfigLocal<SocketAddress>::require(CLI::Option* opt) {
        addressSc->required();
        opt->required();
    }

    template <typename SocketAddress>
    void ConfigLocal<SocketAddress>::require(CLI::Option* opt1, CLI::Option* opt2) {
        addressSc->required();
        opt1->required();
        opt2->required();
    }

} // namespace net::config

#endif // NET_CONFIGLOCAL_HPP
