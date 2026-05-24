#include "idef0parser.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

// ─── Вспомогательные функции ──────────────────────────────────────────────

Idef0Arrow Idef0Parser::parseArrow(const QJsonObject &obj)
{
    Idef0Arrow arrow;
    arrow.label = obj.value("label").toString("Без названия");
    arrow.type  = obj.value("type").toString("input");
    return arrow;
}

Idef0Block Idef0Parser::parseBlock(const QJsonObject &obj)
{
    Idef0Block block;
    block.id       = obj.value("id").toString();
    block.name     = obj.value("name").toString();
    block.parentId = obj.value("parentId").toString();

    auto parseArrows = [&](const QString &key) -> QList<Idef0Arrow> {
        QList<Idef0Arrow> list;
        for (const QJsonValue &v : obj.value(key).toArray())
            list.append(parseArrow(v.toObject()));
        return list;
    };

    block.inputs     = parseArrows("inputs");
    block.outputs    = parseArrows("outputs");
    block.controls   = parseArrows("controls");
    block.mechanisms = parseArrows("mechanisms");
    return block;
}

// ─── Публичный метод ──────────────────────────────────────────────────────

Idef0Diagram Idef0Parser::fromJson(const QString &jsonText)
{
    Idef0Diagram diagram;

    // Убираем возможные markdown-обёртки (```json ... ```)
    QString clean = jsonText.trimmed();
    if (clean.startsWith("```")) {
        int start = clean.indexOf('\n') + 1;
        int end   = clean.lastIndexOf("```");
        if (end > start)
            clean = clean.mid(start, end - start).trimmed();
    }

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(clean.toUtf8(), &err);

    if (err.error != QJsonParseError::NoError || !doc.isObject())
        return diagram;   // Вернём пустую диаграмму при ошибке

    QJsonObject root = doc.object();
    diagram.title     = root.value("title").toString("Система");
    diagram.purpose   = root.value("purpose").toString();
    diagram.viewpoint = root.value("viewpoint").toString();

    for (const QJsonValue &v : root.value("blocks").toArray())
        diagram.blocks.append(parseBlock(v.toObject()));

    return diagram;
}
