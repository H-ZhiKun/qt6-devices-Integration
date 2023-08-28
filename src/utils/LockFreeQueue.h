#pragma once
#include "NonCopyable.h"
#include <atomic>
#include <cassert>
#include <memory>
#include <type_traits>

template <typename T> class LockFreeQueue : public AppFrame::NonCopyable
{
  public:
    LockFreeQueue() : head_(new BufferNode), tail_(head_.load(std::memory_order_relaxed))
    {
    }
    ~LockFreeQueue()
    {
        T output;
        while (this->dequeue(output))
        {
        }
        BufferNode *front = head_.load(std::memory_order_relaxed);
        delete front;
    }

    void enqueue(T &&input)
    {
        BufferNode *node{new BufferNode(std::move(input))};
        BufferNode *prevhead{head_.exchange(node, std::memory_order_acq_rel)};
        prevhead->next_.store(node, std::memory_order_release);
        size_++;
    }
    void enqueue(const T &input)
    {
        BufferNode *node{new BufferNode(input)};
        BufferNode *prevhead{head_.exchange(node, std::memory_order_acq_rel)};
        prevhead->next_.store(node, std::memory_order_release);
        size_++;
    }

    bool dequeue(T &output)
    {
        BufferNode *tail = tail_.load(std::memory_order_relaxed);
        BufferNode *next = tail->next_.load(std::memory_order_acquire);

        if (next == nullptr)
        {
            return false;
        }
        output = std::move(*(next->dataPtr_));
        delete next->dataPtr_;
        tail_.store(next, std::memory_order_release);
        delete tail;
        size_--;
        return true;
    }

    bool empty()
    {
        BufferNode *tail = tail_.load(std::memory_order_relaxed);
        BufferNode *next = tail->next_.load(std::memory_order_acquire);
        return next == nullptr;
    }
    uint16_t getSize()
    {
        return size_;
    }

  private:
    struct BufferNode
    {
        BufferNode() = default;
        BufferNode(const T &data) : dataPtr_(new T(data))
        {
        }
        BufferNode(T &&data) : dataPtr_(new T(std::move(data)))
        {
        }
        T *dataPtr_;
        std::atomic<BufferNode *> next_{nullptr};
    };

    std::atomic<BufferNode *> head_{nullptr};
    std::atomic<BufferNode *> tail_{nullptr};
    std::atomic<uint16_t> size_{0};
};
