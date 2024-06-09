#pragma once

#include "NodeState.hpp"
#include "AbstractGraphicsObject.hpp"


namespace QtNodes {

class NODE_EDITOR_PUBLIC AbstractNodeGraphicsObject : public AbstractGraphicsObject
{
    Q_OBJECT
public:
    AbstractNodeGraphicsObject(AbstractQGraphicsScene &scene, NodeId node, QObject *parent = nullptr);
    
    enum { Type = UserType + 1 };

    int type() const override { return Type; }

    virtual NodeId nodeId() final { return _nodeId; }
    virtual NodeId nodeId() const final { return _nodeId; }

    virtual NodeState &nodeState() { return _nodeState; }
    
    virtual void reactToConnection(AbstractConnectionGraphicsObject const *cgo);

    virtual void setGeometryChanged();
    virtual void moveConnections() const;

    virtual QRectF boundingRect() const override;

protected:

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *) override;

    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;


    NodeId _nodeId;
    NodeState _nodeState;
};

}; // namespace QtNodes
