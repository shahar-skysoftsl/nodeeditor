#include "AbstractConnectionGraphicsObject.hpp"
#include <stdexcept>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QGraphicsView>

#include "AbstractQGraphicsScene.hpp"
#include "NodeConnectionInteraction.hpp"
#include "locateNode.hpp"
#include "ConnectionIdUtils.hpp"
#include "StyleCollection.hpp"

namespace QtNodes {


    AbstractConnectionGraphicsObject::AbstractConnectionGraphicsObject(AbstractQGraphicsScene &scene,
                                 ConnectionId const connectionId,
                                 QObject *parent)
    : AbstractGraphicsObject(scene, parent)
    , _connectionId(connectionId)
    , _connectionState(*this)
    , _out{0, 0}
    , _in{0, 0}
{
};

void AbstractConnectionGraphicsObject::paint(QPainter *painter,
                                     QStyleOptionGraphicsItem const *option,
                                     QWidget *)
{
    if (!scene())
        return;

    nodeScene()->connectionPainter().paint(painter, *this);
}


QRectF AbstractConnectionGraphicsObject::boundingRect() const
{
    auto points = pointsC1C2();

    // `normalized()` fixes inverted rects.
    QRectF basicRect = QRectF(_out, _in).normalized();

    QRectF c1c2Rect = QRectF(points.first, points.second).normalized();

    QRectF commonRect = basicRect.united(c1c2Rect);

    auto const &connectionStyle = StyleCollection::connectionStyle();
    float const diam = connectionStyle.pointDiameter();
    QPointF const cornerOffset(diam, diam);

    // Expand rect by port circle diameter
    commonRect.setTopLeft(commonRect.topLeft() - cornerOffset);
    commonRect.setBottomRight(commonRect.bottomRight() + 2 * cornerOffset);

    return commonRect;
}

QPainterPath AbstractConnectionGraphicsObject::shape() const
{
#ifdef DEBUG_DRAWING

    //QPainterPath path;

    //path.addRect(boundingRect());
    //return path;

#else
    return nodeScene()->connectionPainter().getPainterStroke(*this);
#endif
}


QPointF const &AbstractConnectionGraphicsObject::endPoint(PortType portType) const
{
    Q_ASSERT(portType != PortType::None);

    return (portType == PortType::Out ? _out : _in);
}

void AbstractConnectionGraphicsObject::setEndPoint(PortType portType, QPointF const &point)
{
    if (portType == PortType::In)
        _in = point;
    else
        _out = point;
}

void AbstractConnectionGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}



void AbstractConnectionGraphicsObject::move()
{
    auto moveEnd = [this](ConnectionId cId, PortType portType) {
        NodeId nodeId = getNodeId(portType, cId);

        if (nodeId == InvalidNodeId)
            return;

        AbstractNodeGraphicsObject *ngo = nodeScene()->nodeGraphicsObject(nodeId);

        if (ngo) {
            AbstractNodeGeometry &geometry = nodeScene()->nodeGeometry();

            QPointF scenePos = geometry.portScenePosition(nodeId,
                                                          portType,
                                                          getPortIndex(portType, cId),
                                                          ngo->sceneTransform());

            QPointF connectionPos = sceneTransform().inverted().map(scenePos);

            setEndPoint(portType, connectionPos);
        }
    };

    moveEnd(_connectionId, PortType::Out);
    moveEnd(_connectionId, PortType::In);

    prepareGeometryChange();

    update();
}


void AbstractConnectionGraphicsObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    prepareGeometryChange();

    auto view = static_cast<QGraphicsView *>(event->widget());
    auto ngo = locateNodeAt(event->scenePos(), *nodeScene(), view->transform());
    if (ngo) {
        ngo->reactToConnection(this);

        _connectionState.setLastHoveredNode(ngo->nodeId());
    } else {
        _connectionState.resetLastHoveredNode();
    }

    //-------------------

    auto requiredPort = _connectionState.requiredPort();

    if (requiredPort != PortType::None) {
        setEndPoint(requiredPort, event->pos());
    }

    //-------------------

    update();

    event->accept();
}

void AbstractConnectionGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);

    ungrabMouse();
    event->accept();

    auto view = static_cast<QGraphicsView *>(event->widget());

    Q_ASSERT(view);

    auto ngo = locateNodeAt(event->scenePos(), *nodeScene(), view->transform());

    bool wasConnected = false;

    if (ngo) {
        NodeConnectionInteraction interaction(*ngo, *this, *nodeScene());

        wasConnected = interaction.tryConnect();
    }

    // If connection attempt was unsuccessful
    if (!wasConnected) {
        // Resulting unique_ptr is not used and automatically deleted.
        nodeScene()->resetDraftConnection();
    }
}

void AbstractConnectionGraphicsObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    _connectionState.setHovered(true);

    update();

    // Signal
    nodeScene()->connectionHovered(connectionId(), event->screenPos());

    event->accept();
}

void AbstractConnectionGraphicsObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    _connectionState.setHovered(false);

    update();

    // Signal
    nodeScene()->connectionHoverLeft(connectionId());

    event->accept();
}





std::pair<QPointF, QPointF> AbstractConnectionGraphicsObject::pointsC1C2() const
{
    switch (nodeScene()->orientation()) {
    case Qt::Horizontal:
        return pointsC1C2Horizontal();
        break;

    case Qt::Vertical:
        return pointsC1C2Vertical();
        break;
    }

    throw std::logic_error("Unreachable code after switch statement");
}

std::pair<QPointF, QPointF> AbstractConnectionGraphicsObject::pointsC1C2Horizontal() const
{
    double const defaultOffset = 200;

    double xDistance = _in.x() - _out.x();

    double horizontalOffset = qMin(defaultOffset, std::abs(xDistance));

    double verticalOffset = 0;

    double ratioX = 0.5;

    if (xDistance <= 0) {
        double yDistance = _in.y() - _out.y() + 20;

        double vector = yDistance < 0 ? -1.0 : 1.0;

        verticalOffset = qMin(defaultOffset, std::abs(yDistance)) * vector;

        ratioX = 1.0;
    }

    horizontalOffset *= ratioX;

    QPointF c1(_out.x() + horizontalOffset, _out.y() + verticalOffset);

    QPointF c2(_in.x() - horizontalOffset, _in.y() - verticalOffset);

    return std::make_pair(c1, c2);
}

std::pair<QPointF, QPointF> AbstractConnectionGraphicsObject::pointsC1C2Vertical() const
{
    double const defaultOffset = 200;

    double yDistance = _in.y() - _out.y();

    double verticalOffset = qMin(defaultOffset, std::abs(yDistance));

    double horizontalOffset = 0;

    double ratioY = 0.5;

    if (yDistance <= 0) {
        double xDistance = _in.x() - _out.x() + 20;

        double vector = xDistance < 0 ? -1.0 : 1.0;

        horizontalOffset = qMin(defaultOffset, std::abs(xDistance)) * vector;

        ratioY = 1.0;
    }

    verticalOffset *= ratioY;

    QPointF c1(_out.x() + horizontalOffset, _out.y() + verticalOffset);

    QPointF c2(_in.x() - horizontalOffset, _in.y() - verticalOffset);

    return std::make_pair(c1, c2);
}




void AbstractConnectionGraphicsObject::initializePosition()
{
    // This function is only called when the ConnectionGraphicsObject
    // is newly created. At this moment both end coordinates are (0, 0)
    // in Connection G.O. coordinates. The position of the whole
    // Connection G. O. in scene coordinate system is also (0, 0).
    // By moving the whole object to the Node Port position
    // we position both connection ends correctly.

    if (_connectionState.requiredPort() != PortType::None) {
        PortType attachedPort = oppositePort(_connectionState.requiredPort());

        PortIndex portIndex = getPortIndex(attachedPort, _connectionId);
        NodeId nodeId = getNodeId(attachedPort, _connectionId);

        AbstractNodeGraphicsObject *ngo = nodeScene()->nodeGraphicsObject(nodeId);

        if (ngo) {
            QTransform nodeSceneTransform = ngo->sceneTransform();

            AbstractNodeGeometry &geometry = nodeScene()->nodeGeometry();

            QPointF pos = geometry.portScenePosition(nodeId,
                                                     attachedPort,
                                                     portIndex,
                                                     nodeSceneTransform);

            this->setPos(pos);
        }
    }

    move();
}

}