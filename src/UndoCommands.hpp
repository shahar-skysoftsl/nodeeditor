#pragma once

#include "Definitions.hpp"

#include <QUndoCommand>
#include <QtCore/QJsonObject>
#include <QtCore/QPointF>

#include <unordered_set>

namespace QtNodes {

class AbstractQGraphicsScene;

class CreateCommand : public QUndoCommand
{
public:
    CreateCommand(AbstractQGraphicsScene *scene, QString const name, QPointF const &mouseScenePos);

    void undo() override;
    void redo() override;

private:
    AbstractQGraphicsScene *_scene;
    NodeId _nodeId;
    QJsonObject _sceneJson;
};

/**
 * Selected scene objects are serialized and then removed from the scene.
 * The deleted elements could be restored in `undo`.
 */
class DeleteCommand : public QUndoCommand
{
public:
    DeleteCommand(AbstractQGraphicsScene *scene);

    void undo() override;
    void redo() override;

private:
    AbstractQGraphicsScene *_scene;
    QJsonObject _sceneJson;
};

class CopyCommand : public QUndoCommand
{
public:
    CopyCommand(AbstractQGraphicsScene *scene);
};

class PasteCommand : public QUndoCommand
{
public:
    PasteCommand(AbstractQGraphicsScene *scene, QPointF const &mouseScenePos);

    void undo() override;
    void redo() override;

private:
    QJsonObject takeSceneJsonFromClipboard();
    QJsonObject makeNewNodeIdsInScene(QJsonObject const &sceneJson);

private:
    AbstractQGraphicsScene *_scene;
    QPointF const &_mouseScenePos;
    QJsonObject _newSceneJson;
};

class DisconnectCommand : public QUndoCommand
{
public:
    DisconnectCommand(AbstractQGraphicsScene *scene, ConnectionId const);

    void undo() override;
    void redo() override;

private:
    AbstractQGraphicsScene *_scene;

    ConnectionId _connId;
};

class ConnectCommand : public QUndoCommand
{
public:
    ConnectCommand(AbstractQGraphicsScene *scene, ConnectionId const);

    void undo() override;
    void redo() override;

private:
    AbstractQGraphicsScene *_scene;

    ConnectionId _connId;
};

class MoveNodeCommand : public QUndoCommand
{
public:
    MoveNodeCommand(AbstractQGraphicsScene *scene, QPointF const &diff);

    void undo() override;
    void redo() override;

    /**
   * A command ID is used in command compression. It must be an integer unique to
   * this command's class, or -1 if the command doesn't support compression.
   */
    int id() const override;

    /**
   * Several sequential movements could be merged into one command.
   */
    bool mergeWith(QUndoCommand const *c) override;

private:
    AbstractQGraphicsScene *_scene;
    std::unordered_set<NodeId> _selectedNodes;
    QPointF _diff;
};

} // namespace QtNodes
