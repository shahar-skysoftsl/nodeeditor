#pragma once

#include <QtGui/QPainter>

#include "AbstractConnectionPainter.hpp"
#include "Definitions.hpp"

namespace QtNodes {

class BasicGraphicsScene;
class GraphModel;
class NodeGeometry;
class NodeGraphicsObject;
class ConnectionGraphicsObject;
class NodeState;

/// @ Lightweight class incapsulating paint code.
class NODE_EDITOR_PUBLIC DefaultConnectionPainter : public AbstractConnectionPainter
{
public:
    void paint(QPainter *painter, ConnectionGraphicsObject &ngo) const override;
        
    QPainterPath getPainterStroke(ConnectionGraphicsObject const &cgo) override;
};
} // namespace QtNodes
