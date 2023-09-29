#pragma once
#include <ostream>

template <typename T>
class singly_linked_list;

template <typename T>
class sll_node
{
    friend class singly_linked_list<T>;

public:
    const T& get() const
    {
        return value_;
    }

    sll_node* next() const
    {
        return next_;
    }

private:
    explicit sll_node(const T& value, sll_node* next = nullptr) : value_(value), next_(next)
    {
    }

    void set_next(sll_node* next)
    {
        next_ = next;
    }

    T value_;
    sll_node* next_;
};

template <typename T>
class singly_linked_list
{
public:
    singly_linked_list() : head_(nullptr), tail_(nullptr), length_(0)
    {
    }

    singly_linked_list(const singly_linked_list& other) : head_(nullptr), tail_(nullptr), length_(0)
    {
        auto* current = other.head_;
        while (current)
        {
            this->push_back(current->get());
            current = current->next();
        }
    }

    singly_linked_list(singly_linked_list&& other) noexcept
        : head_(other.head_), tail_(other.tail_), length_(other.length_)
    {
        other.head_ = other.tail_ = nullptr;
        other.length_ = 0;
    }

    class iterator
    {
    public:
        iterator(sll_node<T>* node) : node_(node)
        {
        }

        const T& operator*() { return node_->get(); }

        iterator& operator++()
        {
            node_ = node_->next();
            return *this;
        }

        bool operator!=(const iterator& other) { return node_ != other.node_; }

    private:
        sll_node<T>* node_;
    };

    iterator begin() { return iterator(head_); }
    iterator end() { return iterator(nullptr); }

    void push_back(const T& value)
    {
        if (length_ == 0)
        {
            push_forward(value);
            return;
        }
        tail_->set_next(new sll_node<T>(value));
        tail_ = tail_->next();

        length_++;
    }

    void push_forward(const T& value)
    {
        head_ = new sll_node<T>(value, head_);

        if (length_ == 0)
        {
            tail_ = head_;
        }

        length_++;
    }

    void remove(const T& value)
    {
        if (!head_) return;

        sll_node<T> dummy_start(T(), head_);
        auto* current = &dummy_start;

        while (current->next())
        {
            if (current->next()->get() == value)
            {
                auto* to_delete = current->next();
                if (tail_ == to_delete)
                {
                    tail_ = current;
                }
                current->set_next(to_delete->next());
                length_--;
                delete to_delete;
                break;
            }
            current = current->next();
        }

        if (dummy_start.next() != head_)
        {
            head_ = dummy_start.next();
        }

        if (length_ == 0)
        {
            tail_ = head_;
        }
    }

    bool contains(const T& value) const
    {
        auto* current = head_;

        while (current)
        {
            if (current->get() == value)
            {
                return true;
            }
            current = current->next();
        }

        return false;
    }

    void clear()
    {
        auto* current = head_;
        while (current)
        {
            auto* to_delete = current;
            current = current->next();
            delete to_delete;
        }
        head_ = tail_ = nullptr;
        length_ = 0;
    }

    size_t length() const
    {
        return length_;
    }

    singly_linked_list& operator=(singly_linked_list&& other) noexcept
    {
        if (this != &other)
        {
            clear();
            head_ = other.head_;
            tail_ = other.tail_;
            length_ = other.length_;
            other.head_ = other.tail_ = nullptr;
            other.length_ = 0;
        }
        return *this;
    }

    singly_linked_list& operator=(const singly_linked_list& other)
    {
        if (this != &other)
        {
            clear();
            
            auto* current = other.head_;
            while (current)
            {
                this->push_back(current->get());
                current = current->next();
            }
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const singly_linked_list<T>& list)
    {
        sll_node<T>* current = list.head_;
        os << "[";

        while (current)
        {
            os << current->get();

            current = current->next();
            if (current)
            {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }

    ~singly_linked_list()
    {
        auto* current = head_;

        while (current)
        {
            auto* to_delete = current;
            current = current->next();
            delete to_delete;
        }
    }

private:
    sll_node<T>* head_;
    sll_node<T>* tail_;
    size_t length_;
};
