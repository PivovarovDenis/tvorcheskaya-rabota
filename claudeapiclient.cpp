#include "claudeapiclient.h"

#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>


static const char* DEEPSEEK_API_KEY = "key";
static const char* DEEPSEEK_MODEL   = "deepseek/deepseek-v3.2";   // модель на OpenRouter
static const char* DEEPSEEK_URL     = "https://openrouter.ai/api/v1/chat/completions";


// ---------------------------------------------------------------------

ClaudeApiClient::ClaudeApiClient(QObject *parent)
    : QObject(parent)
    , m_manager(new QNetworkAccessManager(this))
{}

// ---- System prompt  --------------------------
QString ClaudeApiClient::buildSystemPrompt()
{
    return QString(
        "You are an expert in SADT/IDEF0 methodology. "
        "Analyze the given technical specification and build an IDEF0 functional block hierarchy.\n\n"
        "STRICT RULES:\n"
        "1. Return ONLY valid JSON. No explanations, no markdown fences, no extra text.\n"
        "2. Response format:\n"
        "{\n"
        "  \"title\": \"System name\",\n"
        "  \"purpose\": \"Diagram purpose\",\n"
        "  \"viewpoint\": \"Observer role\",\n"
        "  \"blocks\": [\n"
        "    {\n"
        "      \"id\": \"A0\",\n"
        "      \"name\": \"Main function name\",\n"
        "      \"parentId\": \"\",\n"
        "      \"inputs\":     [{\"label\": \"...\", \"type\": \"input\"}],\n"
        "      \"outputs\":    [{\"label\": \"...\", \"type\": \"output\"}],\n"
        "      \"controls\":   [{\"label\": \"...\", \"type\": \"control\"}],\n"
        "      \"mechanisms\": [{\"label\": \"...\", \"type\": \"mechanism\"}]\n"
        "    },\n"
        "    {\n"
        "      \"id\": \"A1\",\n"
        "      \"name\": \"Sub-function 1\",\n"
        "      \"parentId\": \"A0\",\n"
        "      \"inputs\":     [{\"label\": \"...\", \"type\": \"input\"}],\n"
        "      \"outputs\":    [{\"label\": \"...\", \"type\": \"output\"}],\n"
        "      \"controls\":   [{\"label\": \"...\", \"type\": \"control\"}],\n"
        "      \"mechanisms\": [{\"label\": \"...\", \"type\": \"mechanism\"}]\n"
        "    }\n"
        "  ]\n"
        "}\n\n"
        "IDEF0 RULES:\n"
        "- A0 is the single top-level block (parentId = \"\")\n"
        "- Decompose A0 into blocks A1, A2, A3, ... (parentId = \"A0\")\n"
        "- Each block must have 1-4 arrows of each type\n"
        "- Arrow types: input (data entering from left), output (results leaving right),\n"
        "  control (rules/constraints from top), mechanism (resources/actors from bottom)\n"
        "- If the output label of one block matches the input label of another, they are connected\n"
        "- Generate 3-6 decomposition blocks (A1...A6)\n"
        "- Use the SAME LANGUAGE as the technical specification for all labels and names\n"
        "- OUTPUT ONLY JSON, absolutely nothing else"
        );
}

// ---- User prompt ------------------------------------------------------------
QString ClaudeApiClient::buildUserPrompt(const QString &spec)
{
    return QString("Technical specification:\n\n%1\n\n"
                   "Build an IDEF0 diagram. Return only JSON, no other text.").arg(spec);
}

// ---- Send request to DeepSeek (OpenRouter) ---------------------------------
void ClaudeApiClient::generateIdef0(const QString &technicalSpec)
{
    emit progressUpdate(QString::fromUtf8("Отправка запроса к DeepSeek V3.2 (OpenRouter)..."));

    QUrl url(DEEPSEEK_URL);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization",
                         QByteArray("Bearer ") + QByteArray(DEEPSEEK_API_KEY));
    // OpenRouter рекомендует указывать Referer и Title (опционально)
    request.setRawHeader("HTTP-Referer", "https://github.com/your-app/IDEF0Generator");
    request.setRawHeader("X-Title", "IDEF0 Generator for Qt");

    // Формируем тело запроса (OpenRouter-совместимый JSON)
    QJsonObject body;
    body["model"]       = QString(DEEPSEEK_MODEL);
    body["max_tokens"]  = 4096;
    body["temperature"] = 0.2;   // стабильный JSON

    QJsonArray messages;
    QJsonObject sysMsg;
    sysMsg["role"]    = "system";
    sysMsg["content"] = buildSystemPrompt();
    messages.append(sysMsg);

    QJsonObject userMsg;
    userMsg["role"]    = "user";
    userMsg["content"] = buildUserPrompt(technicalSpec);
    messages.append(userMsg);

    body["messages"] = messages;

    QNetworkReply *reply = m_manager->post(request, QJsonDocument(body).toJson());
    connect(reply, &QNetworkReply::finished, this, &ClaudeApiClient::onReplyFinished);
}

// ---- Handle response --------------------------------------------------------
void ClaudeApiClient::onReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) return;
    reply->deleteLater();

    QByteArray rawData = reply->readAll();

    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred(reply->errorString() + "\n" + QString::fromUtf8(rawData));
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(rawData);
    if (doc.isNull() || !doc.isObject()) {
        emit errorOccurred(QString::fromUtf8("Некорректный ответ от DeepSeek (не JSON)."));
        return;
    }

    QJsonArray choices = doc.object().value("choices").toArray();
    if (choices.isEmpty()) {
        emit errorOccurred(QString::fromUtf8("Пустой ответ от DeepSeek."));
        return;
    }

    QString text = choices[0].toObject()
                       .value("message").toObject()
                       .value("content").toString();

    emit progressUpdate(QString::fromUtf8("Ответ получен. Разбор IDEF0..."));
    emit finished(text);
}