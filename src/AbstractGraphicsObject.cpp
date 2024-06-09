#include "AbstractGraphicsObject.hpp"

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsBlurEffect>
#include <QtWidgets/QGraphicsDropShadowEffect>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QStyleOptionGraphicsItem>

#include <QtCore/QDebug>

#include <stdexcept>

#include "AbstractGraphModel.hpp"
#include "AbstractNodeGeometry.hpp"
#include "AbstractPainter.hpp"
#include "AbstractQGraphicsScene.hpp"

#include "locateNode.hpp"

namespace QtNodes {
AbstractGraphicsObject::AbstractGraphicsObject(AbstractQGraphicsScene &scene, QObject *parent)
    : QGraphicsObject(nullptr)
    , _graphModel(scene.graphModel())
{
    scene.addItem(this);
}

AbstractGraphicsObject ::~AbstractGraphicsObject()
{
}


AbstractGraphModel &AbstractGraphicsObject::graphModel() const
{
    return _graphModel;
}

AbstractQGraphicsScene *AbstractGraphicsObject::nodeScene() const
{
    return dynamic_cast<AbstractQGraphicsScene *>(scene());
}

}; // namespace QtNodes
