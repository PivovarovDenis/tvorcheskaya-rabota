#include "drawioexporter.h"
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QList>
#include <QPair>
#include <QDebug>

// ═══════════════════════════════════════════════════════════════════════════
//  КОНСТАНТЫ КОМПОНОВКИ  (соответствуют reference XML: 1169×827)
// ═══════════════════════════════════════════════════════════════════════════

// Размер страницы — как в эталонном XML (draw.io A4 landscape)
static const int PW = 1169;   // pageWidth
static const int PH = 827;    // pageHeight

// Отступы от краёв страницы до диаграммной области
static const int HDR_H   = 80;    // высота шапки
static const int FTR_H   = 40;    // высота подвала
static const int MARGIN_L = 20;   // левое поле внутри контейнера
static const int MARGIN_R = 20;   // правое поле

// Размеры функционального блока (Марка: 3:2 или квадрат)
static const int BLK_W = 150;
static const int BLK_H = 78;

// Диагональный шаг между блоками (SADT: каскад сверху-слева вниз-вправо)
static const int STEP_X = 190;   // горизонтальный шаг центров блоков
static const int STEP_Y = 100;   // вертикальный шаг центров блоков

// Длины «усов» внешних стрелок (до границы страницы)
static const int EXT_LEFT   = MARGIN_L;        // откуда начинается левый внешний вход
static const int EXT_RIGHT  = PW - MARGIN_R;   // куда уходит правый выход
static const int EXT_TOP    = HDR_H + 2;       // граница шапки
static const int EXT_BOT    = PH - FTR_H - 2; // граница подвала

// Минимальный отступ стрелки от блока
static const int STUB = 30;

// ═══════════════════════════════════════════════════════════════════════════
//  УТИЛИТЫ
// ═══════════════════════════════════════════════════════════════════════════

QString DrawioExporter::escapeXml(const QString &s) {
    QString r = s;
    r.replace("&","&amp;").replace("<","&lt;").replace(">","&gt;").replace("\"","&quot;");
    return r;
}
static QString xe(const QString &s) { return DrawioExporter::escapeXml(s); }

// Сгенерировать уникальный строковый id
static QString uid(int &counter, const QString &prefix = "c") {
    return prefix + QString::number(counter++);
}

// ═══════════════════════════════════════════════════════════════════════════
//  ШАПКА  (верхний штамп, точная копия reference XML)
// ═══════════════════════════════════════════════════════════════════════════

QString DrawioExporter::generateHeaderTable(const Idef0Diagram &diagram)
{
    // Все ячейки — дети контейнера hdr (id задаётся снаружи через подстановку)
    // Используем те же размеры и стили, что в reference XML (Page-2, Page-3)
    QString out;

    // ── Контейнер шапки ──────────────────────────────────────────────────
    out +=
        "<mxCell id=\"hdr\" value=\"\" "
        "style=\"rounded=0;whiteSpace=wrap;html=1;container=1;resizable=0;movable=0;editable=0;\" "
        "vertex=\"1\" parent=\"page_root\">\n"
        "  <mxGeometry width=\"1150\" height=\"80\" as=\"geometry\"/>\n"
        "</mxCell>\n";

    // Используется в:
    out +=
        "<mxCell id=\"hdr_used\" value=\"Используется в:\" "
        "style=\"rounded=0;whiteSpace=wrap;html=1;noLabel=0;verticalAlign=top;align=center;\" "
        "vertex=\"1\" parent=\"hdr\">\n"
        "  <mxGeometry width=\"120\" height=\"80\" as=\"geometry\"/>\n"
        "</mxCell>\n";

    // Автор / Проект / Замечания
    QString authorCell = QString(
                             "<mxCell id=\"hdr_author\" "
                             "value=\"&lt;div&gt;Автор:&lt;/div&gt;&lt;div&gt;&lt;br&gt;&lt;/div&gt;"
                             "&lt;div&gt;Проект: %1&lt;/div&gt;&lt;div&gt;&lt;br&gt;&lt;/div&gt;"
                             "&lt;div&gt;Замечания:&lt;/div&gt;\" "
                             "style=\"rounded=0;whiteSpace=wrap;html=1;align=left;verticalAlign=top;\" "
                             "vertex=\"1\" parent=\"hdr\">\n"
                             "  <mxGeometry x=\"120\" width=\"450\" height=\"80\" as=\"geometry\"/>\n"
                             "</mxCell>\n"
                             ).arg(xe(diagram.title));
    out += authorCell;

    // Блок статусов (пустой контейнер)
    out +=
        "<mxCell id=\"hdr_stat\" value=\"\" "
        "style=\"rounded=0;whiteSpace=wrap;html=1;\" "
        "vertex=\"1\" parent=\"hdr\">\n"
        "  <mxGeometry x=\"570\" width=\"230\" height=\"80\" as=\"geometry\"/>\n"
        "</mxCell>\n";

    // Чёрный маркер (активная версия — первая строка закрашена)
    out +=
        "<mxCell id=\"hdr_dot\" value=\"\" "
        "style=\"rounded=0;whiteSpace=wrap;html=1;fillColor=#000000;\" "
        "vertex=\"1\" parent=\"hdr\">\n"
        "  <mxGeometry x=\"570\" width=\"20\" height=\"20\" as=\"geometry\"/>\n"
        "</mxCell>\n";

    // Строки статуса: Рабочая версия / Проект / Рекомендовано / Публикация
    struct StatusRow { int y; QString label; };
    QList<StatusRow> rows = {
        {0,  "Рабочая версия"},
        {20, "Проект"},
        {40, "Рекомендовано"},
        {60, "Публикация"}
    };
    for (int i = 0; i < rows.size(); ++i) {
        // пустые маркеры (y > 0)
        if (i > 0) {
            out += QString(
                       "<mxCell id=\"hdr_dot%1\" value=\"\" "
                       "style=\"rounded=0;whiteSpace=wrap;html=1;\" "
                       "vertex=\"1\" parent=\"hdr\">\n"
                       "  <mxGeometry x=\"570\" y=\"%2\" width=\"20\" height=\"20\" as=\"geometry\"/>\n"
                       "</mxCell>\n"
                       ).arg(i).arg(rows[i].y);
        }
        out += QString(
                   "<mxCell id=\"hdr_sl%1\" value=\"&lt;div align=&quot;left&quot;&gt;%2&lt;/div&gt;\" "
                   "style=\"rounded=0;whiteSpace=wrap;html=1;align=left;\" "
                   "vertex=\"1\" parent=\"hdr\">\n"
                   "  <mxGeometry x=\"590\" y=\"%3\" width=\"210\" height=\"20\" as=\"geometry\"/>\n"
                   "</mxCell>\n"
                   ).arg(i).arg(rows[i].label).arg(rows[i].y);
    }

    // Читатель (правая колонка)
    out +=
        "<mxCell id=\"hdr_reader\" value=\"Читатель\" "
        "style=\"rounded=0;whiteSpace=wrap;html=1;\" "
        "vertex=\"1\" parent=\"hdr\">\n"
        "  <mxGeometry x=\"800\" width=\"160\" height=\"20\" as=\"geometry\"/>\n"
        "</mxCell>\n";
    for (int r = 1; r <= 3; ++r) {
        out += QString(
                   "<mxCell id=\"hdr_rr%1\" value=\"\" "
                   "style=\"rounded=0;whiteSpace=wrap;html=1;\" "
                   "vertex=\"1\" parent=\"hdr\">\n"
                   "  <mxGeometry x=\"800\" y=\"%2\" width=\"160\" height=\"20\" as=\"geometry\"/>\n"
                   "</mxCell>\n"
                   ).arg(r).arg(r * 20);
    }

    // Дата / Пересмотр (текст)
    out +=
        "<mxCell id=\"hdr_dateLbl\" "
        "value=\"&lt;div align=&quot;left&quot;&gt;Дата:&lt;/div&gt;"
        "&lt;div align=&quot;left&quot;&gt;&lt;br&gt;&lt;/div&gt;"
        "&lt;div align=&quot;left&quot;&gt;Пересмотр:&lt;/div&gt;\" "
        "style=\"text;html=1;whiteSpace=wrap;strokeColor=none;fillColor=none;"
        "align=left;verticalAlign=top;rounded=0;\" "
        "vertex=\"1\" parent=\"hdr\">\n"
        "  <mxGeometry x=\"370\" width=\"200\" height=\"80\" as=\"geometry\"/>\n"
        "</mxCell>\n";

    // Дата / ячейки справа от «Читатель»
    out +=
        "<mxCell id=\"hdr_date0\" value=\"Дата\" "
        "style=\"rounded=0;whiteSpace=wrap;html=1;\" "
        "vertex=\"1\" parent=\"hdr\">\n"
        "  <mxGeometry x=\"960\" width=\"40\" height=\"20\" as=\"geometry\"/>\n"
        "</mxCell>\n";
    for (int r = 1; r <= 3; ++r) {
        out += QString(
                   "<mxCell id=\"hdr_date%1\" value=\"\" "
                   "style=\"rounded=0;whiteSpace=wrap;html=1;\" "
                   "vertex=\"1\" parent=\"hdr\">\n"
                   "  <mxGeometry x=\"960\" y=\"%2\" width=\"40\" height=\"20\" as=\"geometry\"/>\n"
                   "</mxCell>\n"
                   ).arg(r).arg(r * 20);
    }

    // Контекст:
    out +=
        "<mxCell id=\"hdr_ctx\" value=\"Контекст:\" "
        "style=\"rounded=0;whiteSpace=wrap;html=1;align=left;verticalAlign=top;\" "
        "vertex=\"1\" parent=\"hdr\">\n"
        "  <mxGeometry x=\"1000\" width=\"150\" height=\"80\" as=\"geometry\"/>\n"
        "</mxCell>\n";

    return out;
}

// ═══════════════════════════════════════════════════════════════════════════
//  ПОДВАЛ  (NODE / TITLE / NUMBER — точно как в reference XML)
// ═══════════════════════════════════════════════════════════════════════════

QString DrawioExporter::generateFooterTable(const Idef0Diagram &diagram)
{
    QString nodeLabel = xe(diagram.title.isEmpty() ? "A0" : diagram.title);
    QString titleLabel = xe(diagram.purpose.isEmpty()
                                ? diagram.title
                                : diagram.purpose);

    QString out;

    // Контейнер подвала
    out +=
        "<mxCell id=\"ftr\" value=\"\" "
        "style=\"rounded=0;whiteSpace=wrap;html=1;container=1;resizable=0;movable=0;editable=0;\" "
        "vertex=\"1\" parent=\"page_root\">\n"
        "  <mxGeometry y=\"770\" width=\"1150\" height=\"40\" as=\"geometry\"/>\n"
        "</mxCell>\n";

    // Вершина (NODE)
    out += QString(
               "<mxCell id=\"ftr_node\" value=\"&lt;div&gt;Вершина: %1&lt;/div&gt;\" "
               "style=\"rounded=0;whiteSpace=wrap;html=1;align=left;fontSize=16;\" "
               "vertex=\"1\" parent=\"ftr\">\n"
               "  <mxGeometry width=\"220\" height=\"40\" as=\"geometry\"/>\n"
               "</mxCell>\n"
               ).arg(nodeLabel);

    // Заголовок (TITLE)
    out += QString(
               "<mxCell id=\"ftr_title\" value=\"Заголовок: %1\" "
               "style=\"rounded=0;whiteSpace=wrap;html=1;align=left;fontSize=16;\" "
               "vertex=\"1\" parent=\"ftr\">\n"
               "  <mxGeometry x=\"220\" width=\"650\" height=\"40\" as=\"geometry\"/>\n"
               "</mxCell>\n"
               ).arg(titleLabel);

    // Номер (NUMBER)
    out +=
        "<mxCell id=\"ftr_num\" value=\"Номер:\" "
        "style=\"rounded=0;whiteSpace=wrap;html=1;align=left;fontSize=16;\" "
        "vertex=\"1\" parent=\"ftr\">\n"
        "  <mxGeometry x=\"870\" width=\"280\" height=\"40\" as=\"geometry\"/>\n"
        "</mxCell>\n";

    return out;
}

// ═══════════════════════════════════════════════════════════════════════════
//  АВТОРАССТАНОВКА  (SADT-диагональ Марка гл.11)
// ═══════════════════════════════════════════════════════════════════════════
// Контейнер внутри страницы занимает (x=10,y=8,w=1150,h=810).
// Шапка: y=0..80, подвал: y=770..810  => рабочая область: y=[100..760]
// Слева остаём ~120px под входные внешние стрелки,
// справа ~120px под выходные.

void DrawioExporter::autoLayout(Idef0Diagram &diagram)
{
    // Рабочая зона внутри контейнера (координаты относительно контейнера)
    // Шапка: 0..80, подвал: 770..810
    const int WORK_X1 = 130;   // резерв для входных стрелок слева
    const int WORK_Y1 = 100;   // под шапкой
    const int WORK_X2 = 1020;  // резерв для выходных стрелок справа
    const int WORK_Y2 = 760;   // над подвалом

    // Собираем только дочерние блоки (parentId не пустой — это A1,A2,...)
    QList<Idef0Block *> kids;
    for (Idef0Block &b : diagram.blocks) {
        if (!b.parentId.isEmpty())
            kids.append(&b);
    }
    int n = kids.size();
    if (n == 0) return;

    // Доступная область
    int availW = WORK_X2 - WORK_X1;
    int availH = WORK_Y2 - WORK_Y1;

    // Рассчитываем шаги так, чтобы цепочка вписалась
    // Последний блок: startX + (n-1)*stepX + BLK_W <= WORK_X2
    //                  startY + (n-1)*stepY + BLK_H <= WORK_Y2
    int stepX, stepY;
    if (n > 1) {
        stepX = qMin(STEP_X, (availW - BLK_W) / (n - 1));
        stepY = qMin(STEP_Y, (availH - BLK_H) / (n - 1));
        // Но не меньше минимума (иначе блоки налезут друг на друга);
        // при этом нельзя превышать уже вычисленный лимит (вписываемость в зону)
        stepX = qMax(stepX, BLK_W + 10);
        stepY = qMax(stepY, BLK_H + 10);
    } else {
        stepX = 0;
        stepY = 0;
    }

    // Центрируем цепочку в рабочей зоне
    int chainW = (n - 1) * stepX + BLK_W;
    int chainH = (n - 1) * stepY + BLK_H;
    int startX = WORK_X1 + (availW - chainW) / 2;
    int startY = WORK_Y1 + (availH - chainH) / 2;

    for (int i = 0; i < n; ++i) {
        kids[i]->x = startX + i * stepX;
        kids[i]->y = startY + i * stepY;
        kids[i]->w = BLK_W;
        kids[i]->h = BLK_H;
    }
}

// ═══════════════════════════════════════════════════════════════════════════
//  ВСПОМОГАТЕЛЬНЫЕ СТРУКТУРЫ ДЛЯ СТРЕЛОК
// ═══════════════════════════════════════════════════════════════════════════

// Описывает одну «точку подключения» стрелки к блоку
struct ArrowPoint {
    enum Side { Left, Right, Top, Bottom };
    Side side;
    int  idx;   // порядковый номер среди стрелок данного типа на этом блоке
    int  total; // всего стрелок данного типа
};

// Возвращает абсолютные координаты точки подключения
static QPair<int,int> connPoint(const Idef0Block &b, ArrowPoint::Side side, int idx, int total)
{
    // Равномерное распределение fraction/(total+1) по грани
    int fraction = idx + 1;
    switch (side) {
    case ArrowPoint::Left:
    case ArrowPoint::Right: {
        int y = b.y + b.h * fraction / (total + 1);
        return { side == ArrowPoint::Left ? b.x : b.x + b.w, y };
    }
    case ArrowPoint::Top:
    case ArrowPoint::Bottom: {
        int x = b.x + b.w * fraction / (total + 1);
        return { x, side == ArrowPoint::Top ? b.y : b.y + b.h };
    }
    }
    return {0,0};
}

// ═══════════════════════════════════════════════════════════════════════════
//  ГЕНЕРАЦИЯ БЛОКОВ И СТРЕЛОК
// ═══════════════════════════════════════════════════════════════════════════

// Стиль блока — как в reference XML
static const QString BLOCK_STYLE =
    "rounded=0;whiteSpace=wrap;html=1;fontSize=14;";

// Стиль edge (ортогональный) — как в reference XML
static const QString EDGE_STYLE =
    "edgeStyle=orthogonalEdgeStyle;rounded=0;orthogonalLoop=1;jettySize=auto;html=1;";

// Стиль метки-узла (label)
static const QString LABEL_STYLE =
    "text;html=1;whiteSpace=wrap;strokeColor=none;fillColor=none;"
    "align=center;verticalAlign=middle;rounded=0;fontSize=14;";

// ── Стили стрелок ───────────────────────────────────────────────────────────

// Внешний вход / управление / механизм — стрелка ВХОДИТ в блок (→ блок):
// startArrow=none (хвост у края страницы), endArrow=classic (наконечник у блока)
static const QString EXT_IN_STYLE =
    "edgeStyle=orthogonalEdgeStyle;rounded=0;orthogonalLoop=1;jettySize=auto;html=1;"
    "startArrow=none;startFill=0;endArrow=classic;endFill=1;";

// Внешний выход — стрелка УХОДИТ из блока (блок →):
// startArrow=none (у блока хвост), endArrow=classic (наконечник у края страницы)
static const QString EXT_OUT_STYLE =
    "edgeStyle=orthogonalEdgeStyle;rounded=0;orthogonalLoop=1;jettySize=auto;html=1;"
    "startArrow=none;startFill=0;endArrow=classic;endFill=1;";

// Прямая межблочная связь Ai→Aj (j>i): выход правой грани → вход левой грани
static const QString INTER_STYLE =
    "edgeStyle=orthogonalEdgeStyle;rounded=0;orthogonalLoop=1;jettySize=auto;html=1;";

// Обратная связь Ai→Aj (j<i): выход правой грани → обход сверху → вход верхней грани
// Соответствует паттерну WhAdMxAdYetdzibp1JNq-93 из эталонного XML:
//   exitX=1 (правая грань src) → Array[вправо от src, выше всех блоков, влево к tgt] → entryY=0 (верх tgt)
static const QString FEEDBACK_STYLE =
    "edgeStyle=orthogonalEdgeStyle;rounded=0;orthogonalLoop=1;jettySize=auto;html=1;"
    "exitX=1;exitY=0.25;exitDx=0;exitDy=0;";

QString DrawioExporter::generateBlocksAndArrows(const Idef0Diagram &diagram, int &id)
{
    QString cells;
    QString pageRoot = "page_root";

    // ── Карта: blockId → cellId (mxCell id прямоугольника блока)
    QMap<QString,QString> cellIdMap;

    // ── 1. Прямоугольники функциональных блоков ─────────────────────────
    for (const Idef0Block &b : diagram.blocks) {
        if (b.parentId.isEmpty()) continue;   // A0 не рисуем

        QString cid = uid(id, "blk_");
        cellIdMap[b.id] = cid;

        cells += QString(
                     "<mxCell id=\"%1\" value=\"%2\" style=\"%3\" "
                     "vertex=\"1\" parent=\"%4\">\n"
                     "  <mxGeometry x=\"%5\" y=\"%6\" width=\"%7\" height=\"%8\" as=\"geometry\"/>\n"
                     "</mxCell>\n"
                     ).arg(cid, xe(b.name), BLOCK_STYLE, pageRoot)
                     .arg(b.x).arg(b.y).arg(b.w).arg(b.h);

        // Номер блока в нижнем правом углу
        QString numId = uid(id, "num_");
        cells += QString(
                     "<mxCell id=\"%1\" value=\"%2\" style=\"%3\" "
                     "vertex=\"1\" parent=\"%4\">\n"
                     "  <mxGeometry x=\"%5\" y=\"%6\" width=\"20\" height=\"20\" as=\"geometry\"/>\n"
                     "</mxCell>\n"
                     ).arg(numId, xe(b.id), LABEL_STYLE, pageRoot)
                     .arg(b.x + b.w - 20)
                     .arg(b.y + b.h - 20);
    }

    // ── 2. Межблочные соединения (прямые и обратные связи) ───────────────
    //
    // ПРЯМАЯ связь Ai→Aj (j > i по индексу в списке):
    //   Выходит с правой грани Ai (exitX=1), входит в левую грань Aj (entryX=0).
    //   draw.io строит ортогональный маршрут автоматически.
    //
    // ОБРАТНАЯ связь Ai→Aj (j < i) — паттерн из эталонного XML (WhAdMxAdYetdzibp1JNq-93):
    //   1. Выходит с правой грани Ai (exitX=1, exitY=0.25 — верх правой грани)
    //   2. Идёт вправо до точки правее Ai
    //   3. Поднимается выше всех блоков (в зону y = HDR_H + 20..60)
    //   4. Идёт влево до x над целевым блоком Aj
    //   5. Входит в верхнюю грань Aj (entryY=0)
    //   Промежуточные точки задаём через <Array as="points">.

    QSet<QString> connectedInputs;   // "blockId:arrowLabel"
    QSet<QString> connectedOutputs;

    // Порядок дочерних блоков в списке (A1, A2, ...)
    QList<const Idef0Block*> kids;
    for (const Idef0Block &b : diagram.blocks)
        if (!b.parentId.isEmpty())
            kids.append(&b);

    // Высота «коридора» для обратных связей: несколько горизонтальных дорожек
    // над блоками, между шапкой (HDR_H=80) и верхом самого высокого блока.
    // Каждой обратной связи — своя дорожка (смещение по y).
    int feedbackLaneCount = 0;

    for (int si = 0; si < kids.size(); ++si) {
        const Idef0Block &src = *kids[si];
        if (!cellIdMap.contains(src.id)) continue;
        const QString srcCid = cellIdMap[src.id];

        for (int oi = 0; oi < src.outputs.size(); ++oi) {
            const Idef0Arrow &out = src.outputs[oi];

            // Ищем целевой блок с совпадающим входом
            for (int ti = 0; ti < kids.size(); ++ti) {
                if (ti == si) continue;
                const Idef0Block &tgt = *kids[ti];
                if (!cellIdMap.contains(tgt.id)) continue;
                const QString tgtCid = cellIdMap[tgt.id];

                for (int ii = 0; ii < tgt.inputs.size(); ++ii) {
                    if (out.label != tgt.inputs[ii].label) continue;

                    QString eid = uid(id, "e_");

                    if (ti > si) {
                        // ── ПРЯМАЯ СВЯЗЬ: Ai → Aj (j > i) ────────────────
                        // Выход правой грани src → вход левой грани tgt
                        double exitY  = (double)(oi + 1) / (src.outputs.size() + 1);
                        double entryY = (double)(ii + 1) / (tgt.inputs.size() + 1);

                        cells += QString(
                                     "<mxCell id=\"%1\" value=\"%2\" "
                                     "style=\"%3"
                                     "exitX=1;exitY=%4;exitDx=0;exitDy=0;"
                                     "entryX=0;entryY=%5;entryDx=0;entryDy=0;"
                                     "align=left;verticalAlign=bottom;fontSize=13;\" "
                                     "edge=\"1\" source=\"%6\" target=\"%7\" parent=\"%8\">\n"
                                     "  <mxGeometry relative=\"1\" as=\"geometry\"/>\n"
                                     "</mxCell>\n"
                                     ).arg(eid, xe(out.label), INTER_STYLE)
                                     .arg(exitY, 0, 'f', 4)
                                     .arg(entryY, 0, 'f', 4)
                                     .arg(srcCid, tgtCid, pageRoot);

                    } else {
                        // ── ОБРАТНАЯ СВЯЗЬ: Ai → Aj (j < i) ──────────────
                        // Паттерн из эталона (WhAdMxAdYetdzibp1JNq-93):
                        //   src правая грань (exitX=1, exitY≈0.25)
                        //   → вправо до точки за src
                        //   → вверх в «коридор» над блоками
                        //   → влево до точки над tgt
                        //   → вниз в верхнюю грань tgt (entryY=0)
                        //
                        // «Коридор» — горизонтальная дорожка:
                        //   y = HDR_H + 20 + feedbackLaneCount * 15
                        // Это гарантирует, что несколько обратных связей не накладываются.

                        int laneY = HDR_H + 20 + feedbackLaneCount * 15;
                        ++feedbackLaneCount;

                        // x-координата точки «вправо от src» — правый край src + зазор
                        int rightOfSrc = src.x + src.w + 20;
                        // x-координата точки «над tgt» — центр верхней грани tgt
                        int aboveTgt   = tgt.x + tgt.w / 2;

                        // Точка входа в верхнюю грань tgt: центр (entryX=0.5, entryY=0)
                        // Можно параметризовать по ii, но в эталоне используется ~0.78
                        double entryX = (double)(ii + 1) / (tgt.inputs.size() + 1);

                        cells += QString(
                                     "<mxCell id=\"%1\" value=\"%2\" "
                                     "style=\"%3"
                                     "exitX=1;exitY=0.25;exitDx=0;exitDy=0;"
                                     "entryX=%4;entryY=0;entryDx=0;entryDy=0;"
                                     "align=left;verticalAlign=bottom;fontSize=13;\" "
                                     "edge=\"1\" source=\"%5\" target=\"%6\" parent=\"%7\">\n"
                                     "  <mxGeometry relative=\"1\" as=\"geometry\">\n"
                                     "    <Array as=\"points\">\n"
                                     "      <mxPoint x=\"%8\" y=\"%9\"/>\n"
                                     "      <mxPoint x=\"%8\" y=\"%10\"/>\n"
                                     "      <mxPoint x=\"%11\" y=\"%10\"/>\n"
                                     "    </Array>\n"
                                     "  </mxGeometry>\n"
                                     "</mxCell>\n"
                                     ).arg(eid, xe(out.label), INTER_STYLE)
                                     .arg(entryX, 0, 'f', 4)
                                     .arg(srcCid, tgtCid, pageRoot)
                                     .arg(rightOfSrc)          // %8  x точки справа от src
                                     .arg(src.y + src.h / 4)   // %9  y у правой грани src (выход)
                                     .arg(laneY)               // %10 y горизонтального коридора
                                     .arg(aboveTgt);           // %11 x над tgt
                    }

                    connectedOutputs.insert(src.id + ":" + out.label);
                    connectedInputs.insert(tgt.id + ":" + out.label);
                    break;
                }
            }
        }
    }

    // ── 3. Внешние стрелки ───────────────────────────────────────────────
    // Input   — от левого края страницы → в левую грань блока (→)
    // Output  — из правой грани блока → к правому краю страницы (→)
    // Control — от верхнего края страницы ↓ в верхнюю грань блока
    // Mechanism — от нижнего края страницы ↑ в нижнюю грань блока

    for (const Idef0Block &b : diagram.blocks) {
        if (b.parentId.isEmpty() || !cellIdMap.contains(b.id)) continue;
        const QString &cid = cellIdMap[b.id];

        // ── INPUT: левый край → левая грань блока ────────────────────────
        int nIn = b.inputs.size();
        for (int i = 0; i < nIn; ++i) {
            const Idef0Arrow &arr = b.inputs[i];
            if (connectedInputs.contains(b.id + ":" + arr.label)) continue;

            double entryY = (double)(i + 1) / (nIn + 1);
            auto [bx, by] = connPoint(b, ArrowPoint::Left, i, nIn);

            QString eid = uid(id, "e_");
            cells += QString(
                         "<mxCell id=\"%1\" value=\"%2\" "
                         "style=\"%3"
                         "entryX=0;entryY=%4;entryDx=0;entryDy=0;"
                         "fontSize=14;verticalAlign=bottom;\" "
                         "edge=\"1\" target=\"%5\" parent=\"%6\">\n"
                         "  <mxGeometry relative=\"1\" as=\"geometry\">\n"
                         "    <mxPoint x=\"%7\" y=\"%8\" as=\"sourcePoint\"/>\n"
                         "  </mxGeometry>\n"
                         "</mxCell>\n"
                         ).arg(eid, xe(arr.label), EXT_IN_STYLE)
                         .arg(entryY, 0, 'f', 4)
                         .arg(cid, pageRoot)
                         .arg(EXT_LEFT + 10).arg(by);
        }

        // ── OUTPUT: правая грань блока → правый край страницы ────────────
        int nOut = b.outputs.size();
        for (int i = 0; i < nOut; ++i) {
            const Idef0Arrow &arr = b.outputs[i];
            if (connectedOutputs.contains(b.id + ":" + arr.label)) continue;

            double exitY = (double)(i + 1) / (nOut + 1);
            auto [bx, by] = connPoint(b, ArrowPoint::Right, i, nOut);

            QString eid = uid(id, "e_");
            cells += QString(
                         "<mxCell id=\"%1\" value=\"%2\" "
                         "style=\"%3"
                         "exitX=1;exitY=%4;exitDx=0;exitDy=0;"
                         "fontSize=14;verticalAlign=bottom;\" "
                         "edge=\"1\" source=\"%5\" parent=\"%6\">\n"
                         "  <mxGeometry relative=\"1\" as=\"geometry\">\n"
                         "    <mxPoint x=\"%7\" y=\"%8\" as=\"targetPoint\"/>\n"
                         "  </mxGeometry>\n"
                         "</mxCell>\n"
                         ).arg(eid, xe(arr.label), EXT_OUT_STYLE)
                         .arg(exitY, 0, 'f', 4)
                         .arg(cid, pageRoot)
                         .arg(EXT_RIGHT - 10).arg(by);
        }

        // ── CONTROL: верхний край страницы ↓ → верхняя грань блока ───────
        int nCtl = b.controls.size();
        for (int i = 0; i < nCtl; ++i) {
            const Idef0Arrow &arr = b.controls[i];
            double entryX = (double)(i + 1) / (nCtl + 1);
            auto [bx, by] = connPoint(b, ArrowPoint::Top, i, nCtl);

            QString eid = uid(id, "e_");
            cells += QString(
                         "<mxCell id=\"%1\" value=\"%2\" "
                         "style=\"%3"
                         "entryX=%4;entryY=0;entryDx=0;entryDy=0;"
                         "fontSize=14;align=left;verticalAlign=bottom;\" "
                         "edge=\"1\" target=\"%5\" parent=\"%6\">\n"
                         "  <mxGeometry relative=\"1\" as=\"geometry\">\n"
                         "    <mxPoint x=\"%7\" y=\"%8\" as=\"sourcePoint\"/>\n"
                         "  </mxGeometry>\n"
                         "</mxCell>\n"
                         ).arg(eid, xe(arr.label), EXT_IN_STYLE)
                         .arg(entryX, 0, 'f', 4)
                         .arg(cid, pageRoot)
                         .arg(bx).arg(EXT_TOP + 5);
        }

        // ── MECHANISM: нижний край страницы ↑ → нижняя грань блока ───────
        int nMec = b.mechanisms.size();
        for (int i = 0; i < nMec; ++i) {
            const Idef0Arrow &arr = b.mechanisms[i];
            double entryX = (double)(i + 1) / (nMec + 1);
            auto [bx, by] = connPoint(b, ArrowPoint::Bottom, i, nMec);

            QString eid = uid(id, "e_");
            cells += QString(
                         "<mxCell id=\"%1\" value=\"%2\" "
                         "style=\"%3"
                         "entryX=%4;entryY=1;entryDx=0;entryDy=0;"
                         "align=right;verticalAlign=bottom;fontSize=14;\" "
                         "edge=\"1\" target=\"%5\" parent=\"%6\">\n"
                         "  <mxGeometry relative=\"1\" as=\"geometry\">\n"
                         "    <mxPoint x=\"%7\" y=\"%8\" as=\"sourcePoint\"/>\n"
                         "  </mxGeometry>\n"
                         "</mxCell>\n"
                         ).arg(eid, xe(arr.label), EXT_IN_STYLE)
                         .arg(entryX, 0, 'f', 4)
                         .arg(cid, pageRoot)
                         .arg(bx).arg(EXT_BOT - 5);
        }
    }

    return cells;
}

// ═══════════════════════════════════════════════════════════════════════════
//  СБОРКА СТРАНИЦЫ  (точно по структуре reference XML: Page-2, Page-3)
// ═══════════════════════════════════════════════════════════════════════════

static QString buildPage(const QString &pageId,
                         const QString &pageName,
                         const Idef0Diagram &diagram,
                         int &cellCounter)
{
    // Генерируем три части содержимого
    QString header = DrawioExporter::generateHeaderTable(diagram);
    QString footer = DrawioExporter::generateFooterTable(diagram);
    QString body   = DrawioExporter::generateBlocksAndArrows(diagram, cellCounter);

    // Структура: <diagram> → <mxGraphModel> → <root>
    //   mxCell id=0, mxCell id=1 parent=0  (стандарт)
    //   контейнер страницы (page_root)
    //   шапка, подвал, блоки/стрелки — всё внутри page_root

    // Контейнер страницы — точно как в ref XML (movable=0, allowArrows=0)
    QString pageContainer = QString(
        "<mxCell id=\"page_root\" value=\"\" "
        "style=\"rounded=0;whiteSpace=wrap;html=1;container=1;noLabel=0;"
        "editable=1;movable=0;resizable=1;rotatable=1;deletable=1;"
        "locked=0;connectable=0;allowArrows=0;\" "
        "vertex=\"1\" parent=\"1\">\n"
        "  <mxGeometry x=\"10\" y=\"8\" width=\"1150\" height=\"810\" as=\"geometry\"/>\n"
        "</mxCell>\n"
        );

    return QString(
               "  <diagram name=\"%1\" id=\"%2\">\n"
               "    <mxGraphModel dx=\"1270\" dy=\"811\" grid=\"1\" gridSize=\"10\""
               " guides=\"1\" tooltips=\"1\" connect=\"1\" arrows=\"1\" fold=\"1\""
               " page=\"1\" pageScale=\"1\" pageWidth=\"%3\" pageHeight=\"%4\""
               " math=\"0\" shadow=\"0\">\n"
               "      <root>\n"
               "        <mxCell id=\"0\"/>\n"
               "        <mxCell id=\"1\" parent=\"0\"/>\n"
               "%5"        // page container
               "%6"        // header
               "%7"        // footer
               "%8"        // blocks & arrows
               "      </root>\n"
               "    </mxGraphModel>\n"
               "  </diagram>\n"
               ).arg(xe(pageName))
        .arg(pageId)
        .arg(PW).arg(PH)
        .arg(pageContainer)
        .arg(header)
        .arg(footer)
        .arg(body);
}

// ═══════════════════════════════════════════════════════════════════════════
//  ПУБЛИЧНЫЕ МЕТОДЫ
// ═══════════════════════════════════════════════════════════════════════════

QString DrawioExporter::toDrawio(const Idef0Diagram &diagram)
{
    Idef0Diagram d = diagram;

    // Нужна ли расстановка?
    bool needLayout = true;
    for (const Idef0Block &b : d.blocks) {
        if (b.x != 0 || b.y != 0) { needLayout = false; break; }
    }
    if (needLayout) autoLayout(d);

    int cellCounter = 100;

    // Подвал для главной страницы должен показывать корневой узел (A0)
    // Строим одну страницу: декомпозиция A0 (блоки A1..An)
    Idef0Diagram mainPage = d;

    // purpose → заголовок подвала
    if (mainPage.purpose.isEmpty())
        mainPage.purpose = mainPage.title;

    QString page = buildPage("main_page", d.title, mainPage, cellCounter);

    return QString(
               "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
               "<mxfile host=\"IDEF0Generator\" agent=\"IDEF0Generator/1.0\" version=\"28.2.8\" pages=\"1\">\n"
               "%1"
               "</mxfile>\n"
               ).arg(page);
}

bool DrawioExporter::saveToFile(const Idef0Diagram &diagram, const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    out << toDrawio(diagram);
    file.close();
    return true;
}