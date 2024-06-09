#include "locateNode.hpp"

#include <vector>

#include <QtCore/QList>
#include <QtWidgets/QGraphicsScene>

#include "NodeGraphicsObject.hpp"
#include "AbstractNodeGraphicsObject.hpp"

namespace QtNodes {

AbstractNodeGraphicsObject *locateNodeAt(QPointF scenePoint,
                                 QGraphicsScene &scene,
                                 QTransform const &viewTransform)
{
    // items under cursor
    QList<QGraphicsItem *> items = scene.items(scenePoint,
                                               Qt::IntersectsItemShape,
                                               Qt::DescendingOrder,
                                               viewTransform);

    // items convertable to NodeGraphicsObject
    std::vector<QGraphicsItem *> filteredItems;

    std::copy_if(items.begin(),
                 items.end(),
                 std::back_inserter(filteredItems),
                 [](QGraphicsItem *item) {
                     return (qgraphicsitem_cast<AbstractNodeGraphicsObject *>(item) != nullptr);
                 });

    AbstractNodeGraphicsObject *node = nullptr;

    if (!filteredItems.empty()) {
        QGraphicsItem *graphicsItem = filteredItems.front();
        node = dynamic_cast<AbstractNodeGraphicsObject *>(graphicsItem);
    }

    return node;
}

} // namespace QtNodes
