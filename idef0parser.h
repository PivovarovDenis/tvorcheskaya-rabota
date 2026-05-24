#pragma once

#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>

struct Idef0Arrow {
    QString label;
    QString type;  // "input" | "output" | "control" | "mechanism"
};

struct Idef0Block {
    QString id;
    QString name;
    QList<Idef0Arrow> inputs;
    QList<Idef0Arrow> outputs;
    QList<Idef0Arrow> controls;
    QList<Idef0Arrow> mechanisms;
    QString parentId;

    // Required for QList::indexOf
    bool operator==(const Idef0Block &other) const {
        return id == other.id;
    }
    int x = 0, y = 0, w = 200, h = 80;
};

struct Idef0Diagram {
    QString title;
    QString purpose;
    QString viewpoint;
    QList<Idef0Block> blocks;
};

class Idef0Parser {
public:
    static Idef0Diagram fromJson(const QString &jsonText);
private:
    static Idef0Arrow parseArrow(const QJsonObject &obj);
    static Idef0Block parseBlock(const QJsonObject &obj);
};
