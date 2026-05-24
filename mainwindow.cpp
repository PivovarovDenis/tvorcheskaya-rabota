#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "drawioexporter.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_client(new ClaudeApiClient(this))
{
    ui->setupUi(this);
    setWindowTitle("IDEF0 Generator");
    setMinimumSize(960, 660);

    connect(m_client, &ClaudeApiClient::finished,
            this,     &MainWindow::onApiFinished);
    connect(m_client, &ClaudeApiClient::errorOccurred,
            this,     &MainWindow::onApiError);
    connect(m_client, &ClaudeApiClient::progressUpdate,
            this,     &MainWindow::onApiProgress);

    connect(ui->btnGenerate, &QPushButton::clicked, this, &MainWindow::onGenerateClicked);
    connect(ui->btnSave,     &QPushButton::clicked, this, &MainWindow::onSaveClicked);
    connect(ui->btnClear,    &QPushButton::clicked, this, &MainWindow::onClearClicked);

    ui->btnSave->setEnabled(false);
    statusBar()->showMessage(
        QString::fromUtf8("Введите техническое задание и нажмите «Сгенерировать IDEF0».")
    );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onGenerateClicked()
{
    QString spec = ui->teTechnicalSpec->toPlainText().trimmed();
    if (spec.isEmpty()) {
        QMessageBox::warning(this,
            QString::fromUtf8("Пустое ТЗ"),
            QString::fromUtf8("Введите техническое задание."));
        return;
    }

    setUiBusy(true);
    ui->teJsonPreview->clear();
    ui->teStructureView->clear();
    m_client->generateIdef0(spec);
}

void MainWindow::onSaveClicked()
{
    if (m_diagram.blocks.isEmpty()) return;

    QString path = QFileDialog::getSaveFileName(
        this,
        QString::fromUtf8("Сохранить диаграмму"),
        m_diagram.title + ".drawio",
        "draw.io (*.drawio);;All (*)"
    );
    if (path.isEmpty()) return;
    if (!path.endsWith(".drawio", Qt::CaseInsensitive))
        path += ".drawio";

    if (DrawioExporter::saveToFile(m_diagram, path)) {
        statusBar()->showMessage(QString::fromUtf8("✅ Сохранено: ") + path);
        QMessageBox::information(this,
            QString::fromUtf8("Готово"),
            QString::fromUtf8("Файл сохранён:\n") + path +
            QString::fromUtf8("\n\nОткройте в draw.io или diagrams.net"));
    } else {
        QMessageBox::critical(this,
            QString::fromUtf8("Ошибка"),
            QString::fromUtf8("Не удалось записать файл."));
    }
}

void MainWindow::onClearClicked()
{
    ui->teTechnicalSpec->clear();
    ui->teJsonPreview->clear();
    ui->teStructureView->clear();
    m_diagram = Idef0Diagram();
    ui->btnSave->setEnabled(false);
    statusBar()->showMessage(QString::fromUtf8("Поля очищены."));
}

void MainWindow::onApiFinished(const QString &jsonResponse)
{
    setUiBusy(false);
    ui->teJsonPreview->setPlainText(jsonResponse);

    m_diagram = Idef0Parser::fromJson(jsonResponse);

    if (m_diagram.blocks.isEmpty()) {
        statusBar()->showMessage(QString::fromUtf8("⚠️ Не удалось разобрать JSON."));
        QMessageBox::warning(this,
            QString::fromUtf8("Ошибка разбора"),
            QString::fromUtf8("Не удалось извлечь блоки IDEF0.\n"
                              "Проверьте вкладку JSON ответ."));
        return;
    }

    updatePreview();
    ui->btnSave->setEnabled(true);
    statusBar()->showMessage(
        QString::fromUtf8("✅ Диаграмма построена: ")
        + QString::number(m_diagram.blocks.size())
        + QString::fromUtf8(" блоков.")
    );
}

void MainWindow::onApiError(const QString &message)
{
    setUiBusy(false);
    statusBar()->showMessage(QString::fromUtf8("❌ ") + message);
    QMessageBox::critical(this, QString::fromUtf8("Ошибка API"), message);
}

void MainWindow::onApiProgress(const QString &message)
{
    statusBar()->showMessage(message);
    QApplication::processEvents();
}

void MainWindow::setUiBusy(bool busy)
{
    ui->btnGenerate->setEnabled(!busy);
    ui->btnSave->setEnabled(!busy && !m_diagram.blocks.isEmpty());
    ui->btnClear->setEnabled(!busy);
    ui->teTechnicalSpec->setReadOnly(busy);
    ui->btnGenerate->setText(busy
        ? QString::fromUtf8("⏳ Генерация...")
        : QString::fromUtf8("Сгенерировать IDEF0"));
}

void MainWindow::updatePreview()
{
    QString text;
    text += "========================================\n";
    text += QString("  %1\n").arg(m_diagram.title);
    text += QString::fromUtf8("  Цель:          %1\n").arg(m_diagram.purpose);
    text += QString::fromUtf8("  Точка зрения:  %1\n").arg(m_diagram.viewpoint);
    text += "========================================\n\n";

    for (const Idef0Block &b : m_diagram.blocks) {
        bool isTop = b.parentId.isEmpty();
        text += (isTop ? "+----- " : "  +--- ");
        text += QString("[%1] %2\n").arg(b.id, b.name);

        auto section = [&](const QString &icon, const QList<Idef0Arrow> &arrows) {
            for (const Idef0Arrow &a : arrows)
                text += QString("    %1 %2\n").arg(icon, a.label);
        };
        section(QString::fromUtf8("→  [Вход]:       "), b.inputs);
        section(QString::fromUtf8("→  [Выход]:      "), b.outputs);
        section(QString::fromUtf8("↓  [Управление]: "), b.controls);
        section(QString::fromUtf8("↑  [Механизм]:   "), b.mechanisms);
        text += "\n";
    }

    ui->teStructureView->setPlainText(text);
}
