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
            if (lst_->size_ == 0)
                return *this;
            if (lst_->size_ == 1 && item_.lock() == lst_->tmp_->next) {
                item_ = lst_->tmp_;
                return *this;
            }
            item_ = item_.lock()->next;
            return *this;
        }
        const iterator operator++ (int) {
            if (lst_->size_ == 0)
                return *this;
            if (lst_->size_ == 1 && item_.lock() == lst_->tmp_->next) {
                item_ = lst_->tmp_;
                return *this;
            }
            std::shared_ptr<lst_node> res = item_.lock();
            item_ = item_.lock()->next;
            return iterator(res);
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
        queue const *lst_;
        friend class queue;
    };
    std::shared_ptr<lst_node> tmp_;
    ull size_;
    std::shared_ptr<lst_node> new_node(const value_type& value) {
        return std::make_shared<lst_node>(value);
    }
    void empty_insert(const value_type& value) {
        tmp_->next = new_node(value);
        tmp_->next->prev = tmp_;
        tmp_->prev = tmp_->next;
    }
    void it_insert(std::shared_ptr<lst_node> item, const value_type& value) {
        if (size_ == 0) {
            empty_insert(value);
            size_++;
            return ;
        }
        std::shared_ptr<lst_node> new_elem = new_node(value);
        if (item == (tmp_->next)) {
            new_elem->next = tmp_->next;
            new_elem->prev = tmp_;
            tmp_->next = new_elem;
            item->prev = new_elem;
            item->next = tmp_;
            size_++;
            return;
        }
        new_elem->next = item;
        new_elem->prev = item->prev;
        item->prev.lock()->next = new_elem;
        item->prev = new_elem;
        size_++;
    }
    void it_rmv(std::shared_ptr<lst_node> item) {
        if (size_ == 0) {
            std::cout << "nothing to remove\n";
        } else if (size_ == 1) {
            tmp_->next = nullptr;
        } else {
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
                item->prev.lock()->next = item->next;
                size_--;
                return ;
            }
            item->next->prev = item->prev;
            item->prev.lock()->next = item->next;
        }
        size_--;
    }
};

#endif