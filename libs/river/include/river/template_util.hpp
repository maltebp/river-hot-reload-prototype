#pragma once

#include <tuple>

namespace rv::template_util { 
    
    template<typename T>
    struct False : std::false_type { };

    template<typename ... Ts>
    static std::tuple<std::tuple<Ts, std::size_t>...> to_tuples_with_indices(Ts... ts) {
        
        auto tuple_creator = [&]<std::size_t ... TIndices>(std::index_sequence<TIndices...>) {
            return std::make_tuple(std::make_tuple(ts, TIndices)...);
        };

        return tuple_creator(std::make_index_sequence<sizeof...(Ts)>{});
    }

}