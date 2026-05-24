#pragma once

#include "idef0parser.h"
#include <QString>

class DrawioExporter
{
public:
    static QString toDrawio(const Idef0Diagram &diagram);

    static bool saveToFile(
        const Idef0Diagram &diagram,
        const QString &filePath
        );

    static QString generateHeaderTable(
        const Idef0Diagram &diagram
        );

    static QString generateFooterTable(
        const Idef0Diagram &diagram
        );

    static QString generateBlocksAndArrows(
        const Idef0Diagram &diagram,
        int &idCounter
        );

    static QString escapeXml(const QString &s);

private:
    static void autoLayout(Idef0Diagram &diagram);
};