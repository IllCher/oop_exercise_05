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

    queue() {
        head = std::make_shared<lst_node>();
        tail = head;
    }
    void push(const value_type& value) {
        std::shared_ptr<lst_node> new_elem = new_node(value);
        if (empty()) {
            head = new_elem;
            head->next = tail;
            tail->prev = head;
        } else {
            tail->prev.lock()->next = new_elem;
            new_elem->prev = tail->prev;
            new_elem->next = tail;
            tail->prev = new_elem;
        }
    }

    void pop() {
        if (size_ == 0)
            throw std::logic_error("empty");
        head = head->next;
    }

    reference top() {
        if (size_ == 0) {
            throw std::logic_error("empty");
        }
        return head->value;
    }

    size_type size() {
        return size_;
    }

    bool empty() {
        return head == tail;
    }

    iterator begin() {
        return iterator(head, this);
    }

    iterator end() {
        return iterator(tail, this);
    }

   iterator insert(iterator pos, const value_type& value) {
        it_insert(pos.item_.lock(), value);
        return iterator(pos.item_.lock()->prev, this);
    }

    iterator erase(iterator pos) {
        iterator res = pos;
        ++res;
        it_rmv(pos.item_.lock());
        return res;
    }

private:
    struct lst_node {
        std::shared_ptr<lst_node> next;
        std::weak_ptr<lst_node> prev;
        value_type value;
        lst_node() = default;
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

        iterator(std::weak_ptr<lst_node> item, queue const * lst): item_(item), lst_(lst)
        {}

        ~iterator() = default;

        iterator(const iterator& it) {
            item_ = it.item_;
            lst_ = it.lst_;
        }

        iterator& operator= (const iterator& it) {
            item_ = it.item_;
            lst_ = it.lst_;
            return *this;
        }

        iterator& operator++ () {
            std::shared_ptr<lst_node> temp = item_;
            if (temp) {
                if (temp->next == nullptr) {
                    throw std::out_of_range("Going out of container boundaries");
                }
                temp = temp->next;
                item_ = temp;
                return *this;
            } else {
                throw std::runtime_error("Element pointed by this iterator doesnt exist anymore");
            }
        }
        const iterator operator++ (int) {
            iterator result(*this);
            (*this)++;
            return result;
        }
        iterator& operator-- () {
            if (lst_->size_ == 0)
                return *this;
            item_ = item_->prev;
            return *this;
        }
        reference operator*() {
            return item_.lock()->value;
        }

        pointer operator->() {
            return &item_->value;
        }

        bool operator!= (const iterator& example) {
            return item_.lock() != example.item_.lock();
        }

        bool operator== (const iterator& example) {
            return item_.lock() == example.item_.lock();
        }
    private:
        std::weak_ptr<lst_node> item_;
        queue const* lst_;
        friend class queue;
    };
    std::shared_ptr<lst_node> head;
    std::shared_ptr<lst_node> tail;
    ull size_;
    std::shared_ptr<lst_node> new_node(const value_type& value) {
        return std::make_shared<lst_node>(value);
    }
    void empty_insert(const value_type& value) {
        push(value);
    }
    void it_insert(std::shared_ptr<lst_node> item, const value_type& value) {
        if (size() == 0) {
            empty_insert(value);
            size_++;
            return;
        }
        std::shared_ptr<lst_node> new_elem = new_node(value);
        if (item == head) {
            new_elem->next = head;
            head->prev = new_elem;
            head = new_elem;
        } else {
            new_elem->next = item;
            new_elem->prev = item->prev;
            item->prev.lock()->next = new_elem;
            item->prev = new_elem;
            size_++;
        }
    }
    void it_rmv(std::shared_ptr<lst_node> item) {
        if (size_ == 0) {
            std::cout << "nothing to remove\n";
        } else if (item == head) {
            pop();
        } else {
            item->prev.lock()->next = item->next;
            item->next->prev = item->prev;
        }
    }
};

#endif