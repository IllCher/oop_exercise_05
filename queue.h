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
        tail_ = std::make_shared<lst_node>();
        head_ = tail_;
    }
    queue(const queue& q) = delete;
    queue& operator = (const queue&) = delete;
    void push(const value_type& value) {
        std::shared_ptr<lst_node> new_elem = new_node(value);
        if (empty()) {
            head_ = new_elem;
            head_->next = tail_;
            tail_->prev = head_;
        } else {
            tail_->prev.lock()->next = new_elem;
            new_elem->prev = tail_->prev;
            new_elem->next = tail_;
            tail_->prev = new_elem;
        }
    }

    void pop() {
        if (empty())
            throw std::logic_error("empty");
        head_ = head_->next;
    }

    reference top() {
        if (empty()) {
            throw std::logic_error("empty");
        }
        return head_->value;
    }

    size_type size() {
        return size_;
    }

    bool empty() {
        return head_ == tail_;
    }

    iterator begin() {
        return iterator(head_, this);
    }

    iterator end() {
        return iterator(tail_, this);
    }

    void it_insert(iterator it, const value_type& value) {
        if (it == end()) {
            push(value);
            size_++;
            return;
        }
        std::shared_ptr<lst_node> new_elem = new_node(value);
        if (it == begin()) {
            new_elem->next = head_;
            head_->prev = new_elem;
            head_ = new_elem;
            size_++;
            return ;
        }
        new_elem->prev = it.item_.lock()->prev;
        it.item_.lock()->prev.lock()->next = new_elem;
        new_elem->next = it.item_.lock();
        it.item_.lock()->prev = new_elem;
        size_++;
    }
    void it_rmv(iterator it) {
        std::shared_ptr<lst_node> tmp = it.item_.lock();
        if (it == end()) {
            throw std::logic_error("can't remove end iterator");
        }
        if (it == begin()) {
            pop();
            size_--;
            return ;
        }
        std::shared_ptr<lst_node> next_tmp = tmp->next;
        std::weak_ptr<lst_node> prev_tmp = tmp->prev;
        prev_tmp.lock()->next = next_tmp;
        next_tmp->prev = prev_tmp;
        size_--;
    }

private:
    struct lst_node {
        lst_node() = default;
        std::shared_ptr<lst_node> next;
        std::weak_ptr<lst_node> prev;
        value_type value;

        lst_node(const value_type& val):
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

        iterator(std::shared_ptr<lst_node> item, queue const * lst): item_(item), lst_(lst)
        {}

        ~iterator() = default;

        iterator(const iterator& it) {
            item_ = it.item_;
            lst_ = it.lst_;
        }

        iterator& operator= (const iterator& it) {
            item_ = it.item_;
            return *this;
        }

        iterator& operator++ () {
            std::shared_ptr<lst_node> tmp = item_.lock();
            if (tmp) {
                if (tmp->next == nullptr) {
                    throw std::logic_error("out of bounds");
                } else {
                    tmp = tmp->next;
                    item_ = tmp;
                    return  *this;
                }
            }
            throw std::logic_error("smt strange");
        }
        iterator operator++ (int) {
            iterator res(*this);
            ++(*this);
            return res;
        }
        reference operator*() {
            return item_.lock()->value;
        }

        pointer operator->() {
            return &item_->value;
        }

        bool operator!= (const iterator& example) {
            return !(*this == example);
        }

        bool operator== (const iterator& example) {
            return item_.lock() == example.item_.lock();
        }
    private:
        std::weak_ptr<lst_node> item_;
        queue const *lst_;
        friend class queue;
    };
    std::shared_ptr<lst_node> head_;
    std::shared_ptr<lst_node> tail_;
    ull size_;
    std::shared_ptr<lst_node> new_node(const value_type& value) {
        return std::make_shared<lst_node>(value);
    }
};

#endif