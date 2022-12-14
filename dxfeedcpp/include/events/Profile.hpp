#pragma once

#ifndef DXFEED_HPP_INCLUDED
#    error Please include only the DXFeed.hpp header
#endif

extern "C" {
#include <EventData.h>
}

#include <string>
#include <utility>

#include "common/DXFCppConfig.hpp"

#include "converters/DateTimeConverter.hpp"

#include "Event.hpp"
#include "EventFlags.hpp"
#include "EventTraits.hpp"

namespace dxfcpp {

struct Profile final : public MarketEvent, public Lasting {
    /// The alias to a type of shared pointer to the Profile object
    using Ptr = std::shared_ptr<Profile>;

    std::string toString() const override { return std::string(); }
};

} // namespace dxfcpp