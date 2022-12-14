#pragma once

#ifndef DXFEED_HPP_INCLUDED
#    error Please include only the DXFeed.hpp header
#endif

#include <memory>
#include <string>
#include <utility>

#include "EventFlags.hpp"
#include "EventSource.hpp"

namespace dxfcpp {

/// Base abstract class for all dxFeed C++ API entities
struct Entity {
    /// The default virtual d-tor
    virtual ~Entity() = default;

    /// Returns a string representation of the entity
    virtual std::string toString() const = 0;
};

/// Base abstract "shared entity" class. Has some helpers for dynamic polymorphism
struct SharedEntity : public Entity, std::enable_shared_from_this<SharedEntity> {
    /// The simple type synonym for the SharedEntity type
    using Ptr = std::shared_ptr<SharedEntity>;

    /**
     * Checks that pointer to the current type could be converted to type T*
     * In other words: whether type T belongs to the type hierarchy in which the current type resides.
     * @tparam T The type to check
     * @return true if type belongs to the type hierarchy in which the current type resides.
     */
    template <typename T> bool is() const noexcept {
        try {
            auto p = dynamic_cast<const T *>(this);
            (void)(p);

            return true;
        } catch (const std::bad_cast &) {
            return false;
        }
    }

    /**
     *
     * @tparam T
     * @return
     */
    template <typename T> std::shared_ptr<T> sharedAs() noexcept {
        return std::dynamic_pointer_cast<T>(shared_from_this());
    }

    /**
     *
     * @tparam T
     * @return
     */
    template <typename T> std::shared_ptr<T> sharedAs() const {
        return std::dynamic_pointer_cast<T>(shared_from_this());
    }
};

/// Marks all event types that can be received via dxFeed API.
struct Event : public SharedEntity {
    ///
    using Ptr = std::shared_ptr<Event>;

    /// Returns event symbol that identifies this event type
    virtual const std::string &getEventSymbol() const = 0;

    /// Changes event symbol that identifies this event type
    virtual void setEventSymbol(const std::string &) = 0;

    /// Returns time when event was created or zero when time is not available.
    /// Events that are coming from a network connections do not have an embedded event time information and
    /// this method will return zero for them, meaning that event was received just now.
    virtual std::uint64_t getEventTime() const = 0;

    /// Changes event creation time.
    virtual void setEventTime(std::uint64_t) = 0;
};

/**
 * Base class for all market events. All market events are plain java objects that
 * extend this class. Market event classes are simple beans with setter and getter methods for their
 * properties and minimal business logic. All market events have #eventSymbol_ property that is
 * defined by this class.
 */
struct MarketEvent : public Event {
    /// The alias to a type of shared pointer to the MarketEvent object
    using Ptr = std::shared_ptr<MarketEvent>;

  private:
    std::string eventSymbol_{};
    std::uint64_t eventTime_{};

  protected:
    /**
     *
     * @param eventSymbol
     */
    explicit MarketEvent(std::string eventSymbol) : eventSymbol_{std::move(eventSymbol)} {}

  public:
    /// Returns symbol of this event.
    const std::string &getEventSymbol() const override { return eventSymbol_; }
    /**
     *
     * @param eventSymbol
     */
    void setEventSymbol(const std::string &eventSymbol) override { eventSymbol_ = eventSymbol; }
    ///
    std::uint64_t getEventTime() const override { return eventTime_; }
    /**
     *
     * @param eventTime
     */
    void setEventTime(std::uint64_t eventTime) override { eventTime_ = eventTime; }
};

///
struct Indexed {
    /// The alias to a type of shared pointer to the Indexed object
    using Ptr = std::shared_ptr<Indexed>;

    ///
    virtual const IndexedEventSource &getSource() const = 0;
    ///
    virtual const EventFlagsMask &getEventFlags() const = 0;
    ///
    virtual void setEventFlags(const EventFlagsMask &) = 0;
    ///
    virtual std::uint64_t getIndex() const = 0;
    ///
    virtual void setIndex(std::uint64_t) = 0;
};

///
struct Lasting {
    /// The alias to a type of shared pointer to the Lasting object
    using Ptr = std::shared_ptr<Lasting>;
};

///
struct TimeSeries : public Indexed {
    /// The alias to a type of shared pointer to the TimeSeries object
    using Ptr = std::shared_ptr<TimeSeries>;

    ///
    virtual std::uint64_t getEventId() const { return getIndex(); }
    ///
    virtual uint64_t getTime() const = 0;
};

} // namespace dxfcpp