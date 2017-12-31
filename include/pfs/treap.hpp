#ifndef __PFS_TREAP_HPP__
#define __PFS_TREAP_HPP__

#include <pfs/utility.hpp>

namespace pfs {

//struct priority_traits
//{
//    typedef T value_type;
//    static T generate ();
//};

template <typename KeyT
        , typename T
        , typename PriorityTraits
        , template <typename> class ContainerT>
class treap
{
    struct node
    {
        KeyT          _key;
        priority_type _priority;
        
        node * _left;
        node * _right;
        
        node()
        {}
        
        node (KeyT * key, priority_type priority, node * left, node * right)
            : _key(key)
            , _priority(priority)
            , _left(left)
            , _right(right)
        {}

        void split (key_type const & key, node ** pleft, node ** pright, ContainerT<node> & container)
        {
            node * new_node = 0;

            if (_key <= key) {
                if (_right == 0)
                    right = 0;
                else
                    _right.split(key, & new_node, pright);

                container.push_back(node(_key, _priority, _left, new_node));
                *pleft = & container.back();
            } else {
                if (_left == 0)
                    *pleft = 0;
                else
                    _left.split(key, pleft, & new_node);

                container.push_back(node(_key, _priority, new_node, _right));
                *pright = & container.back();
            }
        }
        
    }; // node
    
    typedef KeyT                                      key_type;
    typedef PriorityTraits                            priority_traits_type;
    typedef typename priority_traits_type::value_type priority_type;
    typedef ContainerT<node>                          container_type;
    
private:    
    container_type _d;
    
private:
    node * create_node (key_type const & key, priority_type priority, node * left, node * right)
    {
        _d.push_back(node(key, priority, left, right));
        return & _d.back();
    }
    
    node * merge (node * left, node * right)
    {
        if (left == 0)
            return right;

        if (right == 0)
            return left;

        if (left->_priority > right->_priority) {
            node * new_right = merge(left->_right, right);
            return create_node(left->_key, left->_priority, left->_left, new_right);
        } else {
            node * new_left = merge(left, right->_left);
            return create_node(right->_key, right->_priority, new_left, right->_right);
        }
    }

public:    
    void insert (key_type const & key)
    {
        node * left, * right;
        
        split(key, & left, & right);
        
        node * m = create_node(key, priority_traits_type::generate(), 0, 0);
        merge(merge(left, m), right);
    }
    
    public Treap Remove(int x)
    {
        Treap l, m, r;
        Split(x - 1, out l, out r);
        r.Split(x, out m, out r);
        return Merge(l, r);
    }
};

} // pfs

#endif /* __PFS_TREAP_HPP__ */

