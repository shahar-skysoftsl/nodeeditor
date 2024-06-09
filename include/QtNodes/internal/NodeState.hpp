#pragma once

#include <unordered_map>
#include <vector>

#include <QtCore/QPointF>
#include <QtCore/QPointer>
#include <QtCore/QUuid>

#include "Export.hpp"

#include "Definitions.hpp"
#include "NodeData.hpp"

namespace QtNodes {

class AbstractConnectionGraphicsObject;
class AbstractNodeGraphicsObject;

class NODE_EDITOR_PUBLIC NodeState
{
public:
    NodeState(AbstractNodeGraphicsObject &ngo);

    bool hovered() const { return _hovered; }

    void setHovered(bool hovered = true) { _hovered = hovered; }

    void setResizing(bool resizing) {  _resizing = resizing; }

    bool resizing() const { return _resizing; }

    AbstractConnectionGraphicsObject const *connectionForReaction() const;

    void storeConnectionForReaction(AbstractConnectionGraphicsObject const *cgo);

    void resetConnectionForReaction();

private:
    AbstractNodeGraphicsObject &_ngo;

    bool _hovered;

    bool _resizing;

    //AbstractConnectionGraphicsObject const *_connectionForReaction = nullptr;
    
    //std::shared_ptr<>

    // QPointer tracks the QObject inside and is automatically cleared
    // when the object is destroyed.
    QPointer<AbstractConnectionGraphicsObject const> _connectionForReaction = nullptr;
};







/// Stores bool for hovering connections and resizing flag.
//class NODE_EDITOR_PUBLIC NodeState: public AbstractNodeState<>
//{
//public:
//    NodeState(NodeGraphicsObject &ngo): AbstractNodeState<>(ngo) {};
//
//public:
//    // bool hovered() const { return _hovered; }
//
//    // void setHovered(bool hovered = true) { _hovered = hovered; }
//
//    // void setResizing(bool resizing);
//
//    // bool resizing() const;
//
//    // ConnectionGraphicsObject const *connectionForReaction() const;
//
//    // void storeConnectionForReaction(ConnectionGraphicsObject const *cgo);
//
//    // void resetConnectionForReaction();
//
//private:
//    // NodeGraphicsObject &_ngo;
//
//    // bool _hovered;
//
//    // bool _resizing;
//
//    // // QPointer tracks the QObject inside and is automatically cleared
//    // // when the object is destroyed.
//    // QPointer<ConnectionGraphicsObject const> _connectionForReaction;
//};










} // namespace QtNodes
