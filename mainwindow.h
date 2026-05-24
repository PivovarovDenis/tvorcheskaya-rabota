#pragma once

#include <QMainWindow>
#include "claudeapiclient.h"
#include "idef0parser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onGenerateClicked();
    void onSaveClicked();
    void onClearClicked();
    void onApiFinished(const QString &jsonResponse);
    void onApiError(const QString &message);
    void onApiProgress(const QString &message);

private:
    Ui::MainWindow  *ui;
    ClaudeApiClient *m_client;
    Idef0Diagram     m_diagram;

    void setUiBusy(bool busy);
    void updatePreview();
};
