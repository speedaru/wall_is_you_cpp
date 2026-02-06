#pragma once
#include "pch.h"

template<typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() = default;

	// Disable copying to prevent mutex issues
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    void Push(T value) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(std::move(value));
    }

    bool TryPop(T& outValue) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_queue.empty()) return false;
        
        outValue = std::move(m_queue.front());
        m_queue.pop();
        return true;
    }

    bool Empty() {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
};