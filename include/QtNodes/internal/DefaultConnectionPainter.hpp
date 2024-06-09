#pragma once

#include <QtGui/QPainter>

#include "AbstractConnectionPainter.hpp"
#include "Definitions.hpp"

namespace QtNodes {

class AbstractQGraphicsScene;
class AbstractGraphModel;
class NodeGeometry;
class AbstractNodeGraphicsObject;
class AbstractConnectionGraphicsObject;
class NodeState;

/// @ Lightweight class incapsulating paint code.
class NODE_EDITOR_PUBLIC DefaultConnectionPainter : public AbstractConnectionPainter
{
public:
    void paint(QPainter *painter, AbstractConnectionGraphicsObject &ngo) const override;
        
    QPainterPath getPainterStroke(AbstractConnectionGraphicsObject const &cgo) override;
};
} // namespace QtNodes
