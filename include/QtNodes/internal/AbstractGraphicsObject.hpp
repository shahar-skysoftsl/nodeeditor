#pragma once

#include <utility>

#include <QtCore/QUuid>
#include <QtWidgets/QGraphicsObject>

#include "Definitions.hpp"

class QGraphicsSceneMouseEvent;

namespace QtNodes {

class AbstractGraphModel;
class AbstractQGraphicsScene;

class NODE_EDITOR_PUBLIC AbstractGraphicsObject : public QGraphicsObject
{
    Q_OBJECT
public:
    AbstractGraphicsObject(AbstractQGraphicsScene &scene, QObject *parent = nullptr);
    ~AbstractGraphicsObject();

    virtual AbstractGraphModel &graphModel() const final;

    virtual AbstractQGraphicsScene *nodeScene() const final;

    virtual void move() = 0;

protected:
    AbstractGraphModel &_graphModel;
};

}; // namespace QtNodes
