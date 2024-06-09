#ifndef DEFAULTNODEGRAPHICSOBJECT_HPP
#define DEFAULTNODEGRAPHICSOBJECT_HPP

#include "AbstractNodeGraphicsObject.hpp"


namespace QtNodes {

class NODE_EDITOR_PUBLIC DefaultNodeGraphicsObject : public AbstractNodeGraphicsObject
{
public:
    DefaultNodeGraphicsObject(AbstractQGraphicsScene &scene, NodeId node, QObject *parent = nullptr)
        : AbstractNodeGraphicsObject(scene, node, parent)
    {}
};
}
#endif // DEFAULTNODEGRAPHICSOBJECT_HPP
