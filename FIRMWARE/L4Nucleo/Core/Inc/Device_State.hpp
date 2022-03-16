/*
 * Device_State.hpp
 *
 *  Created on: 16 mar 2022
 *      Author: wasyl
 */

#ifndef INC_DEVICE_STATE_HPP_
#define INC_DEVICE_STATE_HPP_

#include <etl/circular_buffer.h>

#include <type_traits>

const size_t buffer_size = 10;

template <typename State>
class Device_State {
    static_assert(std::is_enum<State>(), "Device state must be enum struct");
    static_assert(sizeof(State) == sizeof(uint8_t),
                  "Device state must by represented be underlying 8bit type");

    etl::circular_buffer<State, buffer_size> buffer_of_states;

public:
    Device_State() = default;

    void set_state(State state) { buffer_of_states.push(state); }

    State get_state() {
        if (is_empty()) {
            return static_cast<State>(0);
        }
        auto temp_state = buffer_of_states.front();
        buffer_of_states.pop();
        return temp_state;
    }

    [[nodiscard]] bool is_empty() const { return buffer_of_states.empty(); }

    void assert_state(bool expr, State state) {
        if (not(expr)) {
            set_state(state);
        }
    }
};



#endif /* INC_DEVICE_STATE_HPP_ */
