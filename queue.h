#ifndef QUEUE_H
#define QUEUE_H
#include <memory>
#include <cstdint>
typedef unsigned long long ull;
template <class T>
class queue {
private:
    class iterator;
public:
    using value_type = T;
    using size_type = ull;
    using reference = value_type&;

    queue(): size_(0) {
        tmp_ = new_node(value_type());
    }
    ~queue() {
        tmp_->next = nullptr;
    }

    void push(const value_type& value) {
        it_insert(tmp_, value);
    }

    void pop() {
        if (size_ == 0)
            throw std::logic_error("empty");
        it_rmv(tmp_->next);
    }

    reference top() {
        if (size_ == 0) {
            throw std::logic_error("empty");
        }
        return tmp_->next->value;
    }

    size_type size() {
        return size_;
    }

    bool empty() {
        return size_ == 0;
    }

    iterator begin() {
        if (size_ == 0)
            return iterator(tmp_, this);
        return iterator(tmp_->next, this);
    }

    iterator end() {
        return iterator(tmp_, this);
    }

   iterator insert(iterator pos, const value_type& value) {
        it_insert(pos.item_, value);
        return iterator(pos.item_->prev, this);
    }

    iterator erase(iterator pos) {
        iterator res = pos;
        ++res;
        it_rmv(pos.item_);
        return res;
    }

private:
    struct queue_node {
        std::shared_ptr<queue_node> next;
        std::shared_ptr<queue_node> prev;
        value_type value;

        queue_node(const value_type& val):
                value(val), next(nullptr)
        {}
    };

    class iterator {
    public:

        using difference_type = ull;
        using value_type = queue::value_type;
        using reference = queue::value_type&;
        using pointer = queue::value_type*;
        using iterator_category = std::forward_iterator_tag;

        iterator(std::shared_ptr<queue_node> item, queue const * lst): item_(item), queue_(lst)
        {}

        ~iterator() = default;

        iterator(const iterator& it) {
            item_ = it.item_;
            queue_ = it.queue_;
        }

        iterator& operator= (const iterator& it) {
            item_ = it.item_;
            queue_ = it.queue_;
            return *this;
        }

        iterator& operator++ () {
            if (queue_->size_ == 0)
                return *this;
            if (queue_->size_ == 1 && item_ == queue_->tmp_->next) {
                item_ = queue_->tmp_;
                return *this;
            }
            item_ = item_->next;
            return *this;
        }
        iterator& operator-- () {
            if (queue_->size_ == 0)
                return *this;
            item_ = item_->prev;
            return *this;
        }
        reference operator*() {
            return item_->value;
        }

        pointer operator->() {
            return &item_->value;
        }

        bool operator!= (const iterator& example) {
            return item_ != example.item_;
        }
    private:
        std::shared_ptr<queue_node> item_;
        queue const *queue_;
        friend class queue;
    };
    std::shared_ptr<queue_node> tmp_;
    ull size_;
    std::shared_ptr<queue_node> new_node(const value_type& value) {
        return std::make_shared<queue_node>(value);
    }
    void empty_insert(const value_type& value) {
        tmp_->next = new_node(value);
        tmp_->next->prev = tmp_;
        tmp_->prev = tmp_->next;
    }
    void it_insert(std::shared_ptr<queue_node> item, const value_type& value) {
        if (size_ == 0) {
            empty_insert(value);
            size_++;
            return ;
        }
        std::shared_ptr<queue_node> new_elem = new_node(value);
        new_elem->next = item;
        new_elem->prev = item->prev;
        item->prev->next = new_elem;
        item->prev = new_elem;
        size_++;
    }
    void it_rmv(std::shared_ptr<queue_node> item) {
        if (size_ == 0) {
            std::cout << "nothing to remove\n";
        } else if (size_ == 1) {
            tmp_->next = nullptr;
        } else {
            item->next->prev = item->prev;
            item->prev->next = item->next;
            if (size_ == 2) {
                if (item->next == tmp_) {
                    tmp_->next->next = nullptr;
                    tmp_->prev = tmp_->next;
                } else {
                    item->next->next = nullptr;
                    item->next->prev = tmp_;
                    tmp_->next = item->next;
                }
                item->next->prev = item->prev;
                item->prev->next = item->next;
                item->next = nullptr;
                item->prev = nullptr;
                return ;
            }
        }
        size_--;
    }
};

#endif