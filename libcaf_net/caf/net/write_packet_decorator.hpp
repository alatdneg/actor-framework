/******************************************************************************
 *                       ____    _    _____                                   *
 *                      / ___|  / \  |  ___|    C++                           *
 *                     | |     / _ \ | |_       Actor                         *
 *                     | |___ / ___ \|  _|      Framework                     *
 *                      \____/_/   \_|_|                                      *
 *                                                                            *
 * Copyright 2011-2019 Dominik Charousset                                     *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License or  *
 * (at your option) under the terms and conditions of the Boost Software      *
 * License 1.0. See accompanying files LICENSE and LICENSE_ALTERNATIVE.       *
 *                                                                            *
 * If you did not receive a copy of the license files, see                    *
 * http://opensource.org/licenses/BSD-3-Clause and                            *
 * http://www.boost.org/LICENSE_1_0.txt.                                      *
 ******************************************************************************/

#pragma once

#include "caf/byte.hpp"
#include "caf/span.hpp"

namespace caf {
namespace net {

/// Implements the interface for transport and application policies and
/// dispatches member functions either to `decorator` or `parent`.
template <class Object, class Parent>
class write_packet_decorator {
public:
  using transport_type = typename Parent::transport_type;

  using application_type = typename Parent::application_type;

  write_packet_decorator(Object& object, Parent& parent)
    : object_(object), parent_(parent) {
    // nop
  }

  template <class Header, class... Ts>
  void write_packet(const Header& header, span<const byte> payload,
                    Ts&&... xs) {
    object_.write_packet(parent_, header, payload, std::forward<Ts>(xs)...);
  }

  actor_system& system() {
    return parent_.system();
  }

  void cancel_timeout(atom_value type, uint64_t id) {
    parent_.cancel_timeout(type, id);
  }

  template <class... Ts>
  uint64_t set_timeout(timestamp tout, atom_value type, Ts&&... xs) {
    return parent_.set_timeout(tout, type, std::forward<Ts>(xs)...);
  }

  transport_type& transport() {
    return parent_.transport();
  }

private:
  Object& object_;
  Parent& parent_;
};

template <class Object, class Parent>
write_packet_decorator<Object, Parent>
make_write_packet_decorator(Object& object, Parent& parent) {
  return {object, parent};
}

} // namespace net
} // namespace caf
