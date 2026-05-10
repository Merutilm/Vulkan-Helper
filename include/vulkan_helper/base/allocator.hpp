//
// Created by Merutilm on 2025-09-09.
//

#pragma once
#include <mutex>

namespace merutilm::vkh {


    template<typename VkAlloc, typename... Args>
    concept ArgsMatcher = requires(VkAlloc alloc, Args... args)
    {
        { alloc(args...)};
    };

    struct allocator {
        inline static std::mutex mutex;

        explicit allocator() = delete;

        template<typename VkAlloc, typename... Args> requires ArgsMatcher<VkAlloc, Args...>
        static auto invoke(VkAlloc func, Args&&... args) -> decltype(func(std::forward<Args>(args)...)) {
            std::scoped_lock lock(mutex);
            return func(std::forward<Args>(args)...);
        }
    };
}
