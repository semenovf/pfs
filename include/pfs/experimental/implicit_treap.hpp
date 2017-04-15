/* 
 * File:   implicit_treap.hpp
 * Author: wladt
 *
 * Created on April 9, 2017, 7:48 PM
 */

#ifndef __PFS_IMPLICIT_TREAP_HPP__
#define __PFS_IMPLICIT_TREAP_HPP__

#include <pfs/utility.hpp>

namespace pfs {

//struct priority_traits
//{
//    typedef T value_type;
//    static T generate ();
//};

/**
 */
template <typename T
        , typename PriorityTraits
        , template <typename> class ContainerT>
class implicit_treap
{
    typedef T      value_type;
    typedef size_t size_type;
    typedef PriorityTraits                            priority_traits_type;
    typedef typename priority_traits_type::value_type priority_value_type;
    
    struct node
    {
        value_type          _value;
        priority_value_type _priority;
        size_type           _size;
        
        node * _left;
        node * _right;
        
        node()
        {}
        
        node (value_type const & value, priority_value_type priority, node * left, node * right)
            : _value(value)
            , _priority(priority)
            , _left(left)
            , _right(right)
            , _size(0)
        {}
        
        static size_type size_of (node * p)
        {
            return p == 0 ? 0 : p->_size;
        }
        
        void recalc ()
        {
            _size = size_of(_left) + size_of(_right) + 1;
        }
    }; // node
    
    typedef ContainerT<node> container_type;

private:    
    container_type _d;
    node *         _root;
    
private:
    node * create_node (value_type const & value
            , priority_value_type priority
            , node * left
            , node * right)
    {
        _d.push_back(node(value, priority, left, right));
        return & _d.back();
    }

    void split (size_type x, node * cur_node, node ** pleft, node ** pright);
    node * merge (node * left, node * right);
    
public:
    implicit_treap ()
        : _root(0)
    {}
        
    void insert (size_type pos, value_type const & value);
    void remove (size_type pos);
};

template <typename T
        , typename PriorityTraits
        , template <typename> class ContainerT>
void implicit_treap<T, PriorityTraits, ContainerT>::split (
          size_type x
        , node * cur_node
        , node ** pleft
        , node ** pright)
{
    //Push(this); // проталкивание!

    node * new_node = 0;
    int cur_index = node::size_of(cur_node->_left) + 1;

    if (cur_index <= x) {
        if (cur_node->_right == 0)
            *pright = 0;
        else
            split(x - cur_index, cur_node->_right, & new_node, pright);

        *pleft = create_node(cur_node->_value
                , cur_node->_priority
                , cur_node->_left
                , new_node);
        (*pleft)->recalc();
    } else {
        if (cur_node->_left == 0)
            pleft = 0;
        else
            split(x, cur_node->_left, pleft, & new_node);

        *pright = create_node(cur_node->_value
                , cur_node->_priority
                , new_node
                , cur_node->_right);
        (*pright)->recalc();
    }
}

template <typename T
        , typename PriorityTraits
        , template <typename> class ContainerT>
typename implicit_treap<T, PriorityTraits, ContainerT>::node * 
implicit_treap<T, PriorityTraits, ContainerT>::merge (node * left, node * right)
{
    //        Push( L );
    //        Push( R );

    if (left == 0)
        return right;

    if (right == 0)
        return left;

    node * result;

    if (left->_priority > right->_priority) {
        node * new_node = merge(left->_right, right);
        result = create_node(left->_value, left->_priority, left->_left, new_node);
    } else {
        node * new_node = merge(left, right->_left);
        result = create_node(right->_value, right->_priority, new_node, right->_right);
    }

    result->recalc();
    return result;
}

template <typename T
        , typename PriorityTraits
        , template <typename> class ContainerT>
void implicit_treap<T, PriorityTraits, ContainerT>::insert (
          size_type pos
        , value_type const & value)
{
    if (!_root) {
        _root = create_node(value, priority_traits_type::generate(), 0, 0);
    } else {
        node * left = 0, * right = 0;
        split(pos, _root, & left, & right);
        node * m = create_node(value, priority_traits_type::generate(), 0, 0);
        merge(merge(left, m), right);
    }
}

template <typename T
        , typename PriorityTraits
        , template <typename> class ContainerT>
void implicit_treap<T, PriorityTraits, ContainerT>::remove (size_type pos)
{
    if (_root) {
        node * left, * m, * right;

        split(pos, _root, & left, & right);
        split(1, right, & m, & right);

        merge(left, right);
    }
}    

} // pfs

#endif /* __PFS_IMPLICIT_TREAP_HPP__ */

