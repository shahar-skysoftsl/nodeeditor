#pragma once

#include <utility>

#include <QtCore/QUuid>
#include <QtWidgets/QGraphicsObject>

#include "ConnectionState.hpp"
#include "Definitions.hpp"

#include "AbstractConnectionGraphicsObject.hpp"


class QGraphicsSceneMouseEvent;

namespace QtNodes {

class AbstractGraphModel;
class AbstractQGraphicsScene;
class AbstractNodeGraphicsObject;
class BasicGraphicsScene;

/// Graphic Object for connection. Adds itself to scene
class NODE_EDITOR_PUBLIC ConnectionGraphicsObject : public AbstractConnectionGraphicsObject
{
    Q_OBJECT
public:
    // Needed for qgraphicsitem_cast
    /*enum { Type = UserType + 2 };

    int type() const override { return Type; }*/

public:
    ConnectionGraphicsObject() = default;
    ConnectionGraphicsObject(AbstractQGraphicsScene &scene, ConnectionId const connectionId);

    ~ConnectionGraphicsObject() override  = default;


public:
    /*AbstractGraphModel &graphModel() const;

    BasicGraphicsScene *nodeScene() const;*/

    ConnectionId const &connectionId() const override;

    QRectF boundingRect() const override;

    QPainterPath shape() const override;

    const QPointF &endPoint(PortType portType) const override;
    void setEndPoint(PortType portType, const QPointF &point) override;

   /* QPointF const &endPoint(PortType portType) const;

    QPointF out() const { return _out; }

    QPointF in() const { return _in; }*/

    std::pair<QPointF, QPointF> pointsC1C2() const override;

    void move() override;


protected:
    /*
    void paint(QPainter *painter,
               QStyleOptionGraphicsItem const *option,
               QWidget *widget = 0) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;*/

private:
    void initializePosition() override;

    void addGraphicsEffect();

    std::pair<QPointF, QPointF> pointsC1C2Horizontal() const override;

    std::pair<QPointF, QPointF> pointsC1C2Vertical() const override;

private:
    /*ConnectionId _connectionId;

    AbstractGraphModel &_graphModel;

    ConnectionState _connectionState;

    mutable QPointF _out;
    mutable QPointF _in;*/

};

} // namespace QtNodes
