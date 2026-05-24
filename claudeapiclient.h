#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>

class ClaudeApiClient : public QObject
{
    Q_OBJECT

public:
    explicit ClaudeApiClient(QObject *parent = nullptr);

    void generateIdef0(const QString &technicalSpec);

signals:
    void finished(const QString &jsonResponse);
    void errorOccurred(const QString &message);
    void progressUpdate(const QString &message);

private slots:
    void onReplyFinished();

private:
    QNetworkAccessManager *m_manager;

    static QString buildSystemPrompt();
    static QString buildUserPrompt(const QString &spec);
};
