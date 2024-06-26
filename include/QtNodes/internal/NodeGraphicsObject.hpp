#pragma once

#include <QtCore/QUuid>
#include <QtWidgets/QGraphicsObject>


#include "AbstractNodeGraphicsObject.hpp"

class QGraphicsProxyWidget;

namespace QtNodes {

class BasicGraphicsScene;
class AbstractGraphModel;

class NODE_EDITOR_PUBLIC NodeGraphicsObject : public AbstractNodeGraphicsObject
{
    Q_OBJECT
public:
    // Needed for qgraphicsitem_cast
    /*enum { Type = UserType + 1 };

    int type() const override { return Type; }*/

public:
    NodeGraphicsObject(AbstractQGraphicsScene &scene, NodeId node);

    ~NodeGraphicsObject() override = default;

public:

    QRectF boundingRect() const override;

    void setGeometryChanged() override;

    /// Visits all attached connections and corrects
    /// their corresponding end points.
    virtual void moveConnections() const override;

    /// Repaints the node once with reacting ports.
    void reactToConnection(AbstractConnectionGraphicsObject const *cgo);

    void move() override;

    //void moveConnections() override;

protected:
    void paint(QPainter *painter,
               QStyleOptionGraphicsItem const *option,
               QWidget *widget = 0) override;

    /*QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    void hoverMoveEvent(QGraphicsSceneHoverEvent *) override;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;*/

private:
    void embedQWidget();

    void setLockedState();

private:
    //NodeId _nodeId;

    //AbstractGraphModel &_graphModel;

    //NodeState _nodeState;

    // either nullptr or owned by parent QGraphicsItem
    QGraphicsProxyWidget *_proxyWidget;

    // AbstractGraphicsObject interface

};
} // namespace QtNodes
