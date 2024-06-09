#pragma once

#include "AbstractGraphicsObject.hpp"
#include "ConnectionState.hpp"

namespace QtNodes {

class AbstractQGraphicsScene;

class NODE_EDITOR_PUBLIC AbstractConnectionGraphicsObject : public AbstractGraphicsObject
{
    Q_OBJECT
public:
    AbstractConnectionGraphicsObject(AbstractQGraphicsScene &scene,
                                     ConnectionId const connectionId,
                                     QObject *parent = nullptr);

    ~AbstractConnectionGraphicsObject(){};

    // Needed for qgraphicsitem_cast
    enum { Type = UserType + 2 };

    int type() const override { return Type; }

    virtual ConnectionId const &connectionId() const { return _connectionId; };
    //setEndPoint
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    virtual QPointF const &endPoint(PortType portType) const;
    virtual void setEndPoint(PortType portType, QPointF const &point);

    virtual QPointF out() const { return _out; }

    virtual QPointF in() const { return _in; }

    virtual ConnectionState const &connectionState() const { return _connectionState; }

    virtual ConnectionState &connectionState() { return _connectionState; }

    virtual void move() override;

    virtual std::pair<QPointF, QPointF> pointsC1C2() const;

    //pointsC1C2

protected:
    virtual void paint(QPainter *painter,
               QStyleOptionGraphicsItem const *option,
               QWidget *widget = 0) override;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    virtual void initializePosition();

    virtual std::pair<QPointF, QPointF> pointsC1C2Horizontal() const;

    virtual std::pair<QPointF, QPointF> pointsC1C2Vertical() const;

    ConnectionId _connectionId;

    ConnectionState _connectionState;

    mutable QPointF _out;
    mutable QPointF _in;
};

}; // namespace QtNodes