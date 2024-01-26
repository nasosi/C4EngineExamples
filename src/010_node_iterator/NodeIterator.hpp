#ifndef NODE_ITERATOR_HPP
#define NODE_ITERATOR_HPP

#include <type_traits>
#include <iterator>

#include "C4Node.h"
#include "C4Cameras.h"
#include "C4Instances.h"
#include "C4Models.h"
#include "C4Sources.h"
#include "C4Triggers.h"
#include "C4Effects.h"
#include "C4Emitters.h"
#include "C4Skybox.h"
#include "C4Impostors.h"
#include "C4Physics.h"
#include "C4Blockers.h"

namespace C4 {

namespace detail
{

template <class NType> struct nodeEnumTraits    {  }; // Not allowing generic. Maybe we should?

template <> struct nodeEnumTraits<Source>       { typedef std::integral_constant<NodeType,  kNodeSource>    id ; };
template <> struct nodeEnumTraits<Geometry>     { typedef std::integral_constant<NodeType,  kNodeGeometry>  id ; };
template <> struct nodeEnumTraits<Camera>       { typedef std::integral_constant<NodeType,  kNodeCamera>    id ; };
template <> struct nodeEnumTraits<Light>        { typedef std::integral_constant<NodeType,  kNodeLight>     id ; };
template <> struct nodeEnumTraits<Instance>     { typedef std::integral_constant<NodeType,  kNodeInstance>  id ; };
template <> struct nodeEnumTraits<Model>        { typedef std::integral_constant<NodeType,  kNodeModel>     id ; };
template <> struct nodeEnumTraits<Bone>         { typedef std::integral_constant<NodeType,  kNodeBone>      id ; };
template <> struct nodeEnumTraits<Marker>       { typedef std::integral_constant<NodeType,  kNodeMarker>    id ; };
template <> struct nodeEnumTraits<Trigger>      { typedef std::integral_constant<NodeType,  kNodeTrigger>   id ; };
template <> struct nodeEnumTraits<Effect>       { typedef std::integral_constant<NodeType,  kNodeEffect>    id ; };
template <> struct nodeEnumTraits<Emitter>      { typedef std::integral_constant<NodeType,  kNodeEmitter>   id ; };
template <> struct nodeEnumTraits<Space>        { typedef std::integral_constant<NodeType,  kNodeSpace>     id ; };
template <> struct nodeEnumTraits<Portal>       { typedef std::integral_constant<NodeType,  kNodePortal>    id ; };
template <> struct nodeEnumTraits<Zone>         { typedef std::integral_constant<NodeType,  kNodeZone>      id ; };
template <> struct nodeEnumTraits<Skybox>       { typedef std::integral_constant<NodeType,  kNodeSkybox>    id ; };
template <> struct nodeEnumTraits<Impostor>     { typedef std::integral_constant<NodeType,  kNodeImpostor>  id ; };
template <> struct nodeEnumTraits<PhysicsNode>  { typedef std::integral_constant<NodeType,  kNodePhysics>   id ; };
template <> struct nodeEnumTraits<Shape>        { typedef std::integral_constant<NodeType,  kNodeShape>     id ; };
template <> struct nodeEnumTraits<Joint>        { typedef std::integral_constant<NodeType,  kNodeJoint>     id ; };
template <> struct nodeEnumTraits<Field>        { typedef std::integral_constant<NodeType,  kNodeField>     id ; };
template <> struct nodeEnumTraits<Blocker>      { typedef std::integral_constant<NodeType,  kNodeBlocker>   id ; };
}

template <class CastType>
class NodeIterator
{
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = CastType;
    using difference_type = ptrdiff_t;
    using pointer = CastType*;
    using reference = CastType&;

    NodeIterator( Node *startingNode, Node *node ):
        startingNode_ ( startingNode ),
        node_( node )
    {

    }

    NodeIterator &operator++( )
    {
        if ( node_ != nullptr )
        {
            node_ = startingNode_->GetNextTreeNode( node_ );
        }

        while ( node_ )
        {
            if ( node_->GetNodeType() == detail::nodeEnumTraits<CastType>::id::value )
            {
                return *this;
            }
            node_ = startingNode_->GetNextTreeNode( node_ );
        }
        return *this;
    }

    CastType& operator*( )
    {
        return *static_cast<CastType *>( node_ );
    }

    const CastType& operator*( ) const
    {
        return *static_cast<const CastType *>( node_ );
    }

    bool operator==(const NodeIterator& other) const
    {
        return node_ == other.node_;
    }

    bool operator!=(const NodeIterator& other) const
    {
        return !( *this == other ) ;
    }

private:
    Node *startingNode_ = nullptr;
    Node *node_ = nullptr;
};

template <class CastType>
class IteratableNodeListOfType
{
public:
    IteratableNodeListOfType( Node *startingNode ):
        startingNode_ ( startingNode )
    {

    }

    NodeIterator<CastType> begin( ) const
    {
        Node *node = startingNode_;

        while( node )
        {
            if ( node->GetNodeType() == detail::nodeEnumTraits<CastType>::id::value )
            {
                return NodeIterator<CastType>(startingNode_, node);
            }
            node = startingNode_->GetNextTreeNode( node );
        }

        return NodeIterator<CastType>(startingNode_, node);
    }

    NodeIterator<CastType> end( ) const
    {
        return NodeIterator<CastType>( startingNode_, nullptr );
    }

private:
    Node *startingNode_ = nullptr;
};

}
#endif // NODE_ITERATOR_HPP
