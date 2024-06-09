 #include "NodeState.hpp"

// // #include "ConnectionGraphicsObject.hpp"
// // #include "NodeGraphicsObject.hpp"

#include "AbstractConnectionGraphicsObject.hpp"
#include "AbstractNodeGraphicsObject.hpp"

 namespace QtNodes {

  NodeState::NodeState(AbstractNodeGraphicsObject &ngo)
      : _ngo(ngo)
      , _hovered(false)
      , _resizing(false)
      , _connectionForReaction{nullptr}
{
      Q_UNUSED(_ngo);
}

AbstractConnectionGraphicsObject const *NodeState::connectionForReaction() const
 {
    return _connectionForReaction.data();
}

void NodeState::storeConnectionForReaction(AbstractConnectionGraphicsObject const *cgo)
{
    _connectionForReaction = cgo;
}

void NodeState::resetConnectionForReaction()
{
    _connectionForReaction.clear();
}
 

 } // namespace QtNodes
