#pragma once
#include <stack>

template <typename T>
class binary_search_tree;

template <typename T>
class bst_node
{
    // allowing private access to the binary_search_tree
    friend class binary_search_tree<T>;

public:
    T get() const
    {
        return value_;
    }

    bst_node* left() const
    {
        return left_;
    }

    bst_node* right() const
    {
        return right_;
    }

private:
    // don't allow to create a node outside of the binary_search_tree
    bst_node(const T& value) : value_(value), left_(nullptr), right_(nullptr)
    {
    }

    T value_;
    bst_node* left_;
    bst_node* right_;
};

template <typename T>
class binary_search_tree
{
public:
    binary_search_tree() : root_(nullptr), size_(0)
    {
    }

    binary_search_tree(const binary_search_tree& other) : root_(nullptr), size_(0)
    {
        for (auto elem = other.begin_preorder(); elem != other.end_preorder(); ++elem)
        {
            add(*elem);
        }
    }

    binary_search_tree(const binary_search_tree&& other) noexcept : root_(other.root_), size_(other.size_)
    {
        other.root_ = nullptr;
        other.size_ = 0;
    }

    bst_node<T>* root() const
    {
        return root_;
    }

    void add(const T& value)
    {
        // double pointer for delayed access to the pointer for creation in the end of the method
        // covering special case for root creation
        bst_node<T>** current = &root_;
        // iterate through the tree until empty pointer in the correct place is found
        while (*current)
        {
            // ignore duplicate values
            if (value == (*current)->get())
            {
                return;
            }

            // right - greater values, left - lower values
            if (value > (*current)->get())
            {
                current = &(*current)->right_;
            }
            else
            {
                current = &(*current)->left_;
            }
        }
        *current = new bst_node<T>(value);
        size_++;
    }

    bst_node<T>* find(const T& value) const
    {
        return find_with_parent_(value).target;
    }

    bool contains(const T& value) const
    {
        return find(value) != nullptr;
    }

    void clear()
    {
        delete root_;
        root_ = nullptr;
        size_ = 0;
    }

    bool remove(const T& value)
    {
        search_result delete_result = find_with_parent(value);

        // nothing found
        if (!delete_result.target)
        {
            return false;
        }

        // identify type of node by children count to determine the right delete method to use
        switch (check_node_type(delete_result))
        {
        case node_type::no_children:
            delete_node_no_children(delete_result);
            break;
        case node_type::one_child:
            delete_node_one_child(delete_result);
            break;
        case node_type::two_children:
            delete_node_two_children(delete_result);
            break;
        case node_type::invalid:
            throw std::runtime_error("Couldn't delete the value from the tree: invalid node");
        default:
            throw std::runtime_error("Unsupported node type");
        }

        size_--;
        return true;
    }

    size_t size() const
    {
        return size_;
    }

    ~binary_search_tree()
    {
        // deleting root node triggers recursive tree delete
        for (auto elem = begin_postorder(); elem != end_postorder(); ++elem)
        {
            clear_children_and_delete_node(elem.get_node());
        }
    }

    // ITERATORS
    // generic iterator
    template <typename Derived>
    class base_iterator
    {
    protected:
        bst_node<T>* current_ = nullptr;
        std::stack<bst_node<T>*> stack_;

        // add node only if it's not a nullptr
        void add_node_to_stack(bst_node<T>* node)
        {
            if (node)
            {
                stack_.push(node);
            }
        }

    public:
        virtual base_iterator& operator++() = 0;
        virtual ~base_iterator() = default;
        base_iterator() = default;
        base_iterator(const base_iterator& other) = default;
        base_iterator(base_iterator&& other) = default;
        
        T& operator*()
        {
            if (!current_)
            {
                throw std::runtime_error("Attempt to access nullptr value");
            }
            return current_->value_;
        }

        bst_node<T>* get_node()
        {
            return current_;
        }

        bool operator!=(const base_iterator& other) const
        {
            return current_ != other.current_;
        }

        bool operator==(const base_iterator& other) const
        {
            return current_ == other.current_;
        }
        
        Derived operator++(int)
        {
            Derived tmp = static_cast<Derived>(*this);
            ++(*this);
            return tmp;
        }
    };
    
    // inorder iterator - ordered iteration from min to max
    class inorder_iterator final : public base_iterator<inorder_iterator>
    {
        using base_iterator<inorder_iterator>::current_;
        using base_iterator<inorder_iterator>::stack_;
        
    public:
        inorder_iterator(): base_iterator<inorder_iterator>()
        {
            current_ = nullptr;
        }

        explicit inorder_iterator(bst_node<T>* root): base_iterator<inorder_iterator>()
        {
            // find the most left element (min) of the tree
            current_ = root;
            while (current_)
            {
                stack_.push(current_);
                current_ = current_->left_;
            }
            // after the last iteration current_ will be nullptr
            if (!stack_.empty())
            {
                current_ = stack_.top();
            }
        }

        inorder_iterator& operator++() override
        {
            // if top stack element is already used and not needed anymore
            if (!stack_.empty() && current_ == stack_.top())
            {
                stack_.pop();
            }
            // need to work further if stack is not empty
            if (!stack_.empty())
            {
                // take the next element
                current_ = stack_.top();
                // if need to process right part of the subtree
                if (current_->right_)
                {
                    // remove current element since it's saved in current_ field
                    stack_.pop();
                    // add right element and all it's left descendants to the stack
                    stack_.push(current_->right_);
                    while (stack_.top()->left_)
                    {
                        stack_.push(stack_.top()->left_);
                    }
                }
            }
            // end of the iteration cycle
            else
            {
                current_ = nullptr;
            }
            return *this;
        }
    };

    // preorder iterator (root - left - right order)
    class preorder_iterator final : public base_iterator<preorder_iterator>
    {
        using base_iterator<preorder_iterator>::current_;
        using base_iterator<preorder_iterator>::stack_;
        using base_iterator<preorder_iterator>::add_node_to_stack;
        
    public:
        preorder_iterator(): base_iterator<preorder_iterator>()
        {
            current_ = nullptr;
        }

        explicit preorder_iterator(bst_node<T>* root): base_iterator<preorder_iterator>()
        {
            // begin with root
            current_ = root;
            if (root)
            {
                // add right first so that left node would be top in the stack
                add_node_to_stack(root->right_);
                add_node_to_stack(root->left_);
            }
        }

        preorder_iterator& operator++() override
        {
            // if there are nodes to iterate left in the stack
            if (!stack_.empty())
            {
                // get the first priority node
                current_ = stack_.top();
                stack_.pop();
                // add its child nodes to the stack
                add_node_to_stack(current_->right_);
                add_node_to_stack(current_->left_);
            }
            // end of the iteration cycle
            else
            {
                current_ = nullptr;
            }

            return *this;
        }
    };

    // post-order iterator
    class postorder_iterator final : public base_iterator<postorder_iterator>
    {
        using base_iterator<postorder_iterator>::current_;
        using base_iterator<postorder_iterator>::stack_;
        using base_iterator<postorder_iterator>::add_node_to_stack;
        
    public:
        postorder_iterator(): base_iterator<postorder_iterator>()
        {
            current_ = nullptr;
        }

        explicit postorder_iterator(bst_node<T>* root): base_iterator<postorder_iterator>()
        {
            if (root)
            {
                add_node_to_stack(root);
                current_ = root;
                postorder_advance();
            }
            else
            {
                current_ = nullptr;
            }
        }

        postorder_iterator& operator++() override
        {
            // remove current node from the top of the stack
            stack_.pop();
            // if there are nodes to iterate left in the stack
            if (!stack_.empty())
            {
                current_ = stack_.top();

                // if children of current node are already passed (means we need to take step back to the root)
                // no need to iterate through children of the node (it would lead to infinite loop otherwise)
                if (return_stack_.top() == current_)
                {
                    // remove that node from return stack
                    return_stack_.pop();
                }
                // add children of the node to the stack
                else
                {
                    postorder_advance();
                }
            }
            else
            {
                current_ = nullptr;
            }

            return *this;
        }
        
    private:
        void postorder_advance()
        {
            if (!current_)
            {
                return;
            }
            // while node has at least one child
            while (current_->left_ || current_->right_)
            {
                // add this node to return stack (to avoid infinite loop)
                return_stack_.push(current_);
                // add children to the stack so that left child would be on top of stack
                add_node_to_stack(current_->right_);
                add_node_to_stack(current_->left_);
                // left -> right
                current_ = stack_.top();
            }
        }
        
        std::stack<bst_node<T>*> return_stack_;
    };

    inorder_iterator begin_inorder() { return inorder_iterator(root_); }
    inorder_iterator end_inorder() { return inorder_iterator(nullptr); }

    preorder_iterator begin_preorder() { return preorder_iterator(root_); }
    preorder_iterator end_preorder() { return preorder_iterator(nullptr); }

    postorder_iterator begin_postorder() { return postorder_iterator(root_); }
    postorder_iterator end_postorder() { return postorder_iterator(nullptr); }

private:
    // possible directions of the child nodes in binary tree
    enum class child_direction { left, right, none };

    // node types based on child elements for deleting methods
    enum class node_type { no_children, one_child, two_children, invalid };

    // used in find_extreme_in_subtree to determine the direction for search 
    enum class search_extreme { min_value, max_value };

    // struct that contains target for search, its parent, and the the direction from the parent node to target
    // (to avoid values check)
    struct search_result
    {
        bst_node<T>* parent;
        bst_node<T>* target;
        child_direction direction;
    };

    void delete_node_no_children(search_result delete_result, const bool delete_ptr = true)
    {
        update_parent_link(delete_result, nullptr);

        // update root if it was deleted
        if (root_ == delete_result.target)
        {
            root_ = nullptr;
        }

        if (delete_ptr)
        {
            clear_children_and_delete_node(delete_result.target);
        }
    }

    void delete_node_one_child(search_result delete_result, const bool delete_ptr = true)
    {
        bst_node<T>* child = delete_result.target->left_
                                 ? delete_result.target->left_
                                 : delete_result.target->right_;

        // replace node to delete with its child node
        update_parent_link(delete_result, child);

        // update root if it was deleted
        if (root_ == delete_result.target)
        {
            root_ = child;
        }

        if (delete_ptr)
        {
            clear_children_and_delete_node(delete_result.target);
        }
    }

    void delete_node_two_children(search_result delete_result, const bool delete_ptr = true)
    {
        // find inorder predecessor of the deleting node (max in left part of the tree)
        search_result replace_result = find_extreme_in_subtree(delete_result.target->left_,
                                                               delete_result.target, child_direction::left, search_extreme::max_value);

        // delete inorder predecessor from the tree without deleting node itself
        switch (check_node_type(replace_result))
        {
        case node_type::no_children:
            delete_node_no_children(replace_result, false);
            break;
        case node_type::one_child:
            delete_node_one_child(replace_result, false);
            break;
        // max or min element cannot have two children
        case node_type::two_children:
            throw std::runtime_error("Max or min element has two children: unexpected behavior");
        case node_type::invalid:
            throw std::runtime_error("Couldn't delete the value from the tree: invalid node");
        default:
            throw std::runtime_error("Unsupported node type");
        }

        // set replacing node's children from node to delete
        replace_result.target->left_ = delete_result.target->left_;
        replace_result.target->right_ = delete_result.target->right_;

        // update deleting node's parent link
        update_parent_link(delete_result, replace_result.target);

        // remove replacing node's parent link
        update_parent_link(replace_result, nullptr);

        // update root if it was deleted
        if (root_ == delete_result.target)
        {
            root_ = replace_result.target;
        }

        if (delete_ptr)
        {
            clear_children_and_delete_node(delete_result.target);
        }
    }

    // this method prevents spoiling the descendants of the deleting node, since it might have children still assigned
    static void clear_children_and_delete_node(bst_node<T>* to_delete)
    {
        to_delete->left_ = nullptr;
        to_delete->right_ = nullptr;
        delete to_delete;
    }

    node_type check_node_type(const search_result& result) const
    {
        // node to delete has no children
        if (!result.target->left_ && !result.target->right_)
        {
            return node_type::no_children;
        }

        // node to delete has two children
        if (result.target->left_ && result.target->right_)
        {
            return node_type::two_children;
        }

        // node to delete has one child
        if (result.target->left_ || result.target->right_)
        {
            return node_type::one_child;
        }

        return node_type::invalid;
    }

    void update_parent_link(const search_result& from_result, bst_node<T>* to)
    {
        if (!from_result.parent)
        {
            return;
        }
        // check which child of parent is the target node
        switch (from_result.direction)
        {
        case child_direction::left:
            from_result.parent->left_ = to;
            break;
        case child_direction::right:
            from_result.parent->right_ = to;
            break;
        case child_direction::none:
            throw std::runtime_error("Couldn't update parent link: invalid direction");
        default:
            throw std::runtime_error("Unsupported type of node");
        }
    }

    search_result find_with_parent(const T& value) const
    {
        bst_node<T>* parent = nullptr;
        bst_node<T>* current = root_;
        child_direction direction = child_direction::none;

        // while haven't faced nullptr
        while (current)
        {
            // return the node in case of value match
            if (value == current->get())
            {
                return {parent, current, direction};
            }

            parent = current;

            // right - greater values, left - lower values
            if (value > current->get())
            {
                direction = child_direction::right;
                current = current->right_;
            }
            else
            {
                direction = child_direction::left;
                current = current->left_;
            }
        }
        // if there was no return in the while loop - haven't found anything
        return search_result{nullptr, nullptr, child_direction::none};
    }

    // find extreme (min or max value) among node and its descendants
    search_result find_extreme_in_subtree(bst_node<T>* subtree_root, bst_node<T>* subtree_parent,
                                          const child_direction& initial_direction, search_extreme extreme) const
    {
        bst_node<T>* current = subtree_root;
        bst_node<T>* parent = subtree_parent;
        child_direction direction = initial_direction;

        // determine the direction of passing based on min or max search
        auto next_child = extreme == search_extreme::min_value
                              ? [](bst_node<T>* node) -> bst_node<T>* { return node->left_; }
                              : [](bst_node<T>* node) -> bst_node<T>* { return node->right_; };

        // go deeper and update values until it's the last possible node in this direction
        while (next_child(current))
        {
            parent = current;
            current = next_child(current);
            direction = extreme == search_extreme::min_value ? child_direction::left : child_direction::right;
        }
        return {parent, current, direction};
    }

    bst_node<T>* root_;
    size_t size_;
};
