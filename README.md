# 🧩 IDEF0 Generator

> **Генератор SADT/IDEF0 диаграмм на базе AI**  
> Приложение на **C++ / Qt 6** — введи ТЗ, получи готовую диаграмму

---

## ✨ Возможности

| Функция | Описание |
|--------|----------|
| 📝 Ввод ТЗ | Принимает техническое задание в текстовом виде |
| 🤖 AI-генерация | Отправляет ТЗ в **DeepSeek V3.2** через OpenRouter API |
| 🏗️ IDEF0 структура | Блок A0 + декомпозиция A1…An с полным набором стрелок |
| 👁️ Предпросмотр | Дерево блоков и сырой JSON прямо в интерфейсе |
| 💾 Экспорт | Сохранение в файл **`.drawio`** (XML формат draw.io) |

---

## 🗂️ Структура проекта

```
IDEF0Generator/
├── 📄 IDEF0Generator.pro     — файл проекта Qt
├── 📄 main.cpp               — точка входа
├── 📄 mainwindow.h/.cpp      — главное окно
├── 📄 mainwindow.ui          — форма Qt Designer
├── 📄 claudeapiclient.h/.cpp — клиент AI API
├── 📄 idef0parser.h/.cpp     — парсер JSON → Idef0Diagram
└── 📄 drawioexporter.h/.cpp  — генерация .drawio XML
```

---

## 🏛️ Архитектура

```
MainWindow
  ├── ClaudeApiClient   — асинхронный HTTP-запрос к OpenRouter API
  │     └── QNetworkAccessManager (Qt Network)
  ├── Idef0Parser       — разбор JSON → Idef0Diagram
  └── DrawioExporter    — генерация mxGraphModel XML → .drawio
```

---

## 🔧 Сборка

### Требования

| Компонент | Версия |
|-----------|--------|
| Qt | 5.15+ или 6.x |
| Qt Creator | 8+ (рекомендуется) |
| Компилятор | MSVC 2019+ / MinGW 8+ / GCC / Clang |
| Модули Qt | `core` `gui` `widgets` `network` |

### Команды

```bash
# Через Qt Creator:
# Открыть IDEF0Generator.pro → Configure → Build → Run

# Через командную строку:
qmake IDEF0Generator.pro

make             # Linux / macOS
nmake            # Windows (MSVC)
mingw32-make     # Windows (MinGW)
```

---

## 🚀 Быстрый старт

1. **Получите API-ключ** на [openrouter.ai](https://openrouter.ai) → Keys
2. Вставьте ключ в поле `API Key` в приложении
3. Введите **техническое задание** в левую панель
4. Нажмите **«Сгенерировать IDEF0»**
5. Посмотрите результат во вкладке **«Структура блоков»**
6. Нажмите **«Сохранить как .drawio»** и откройте в [diagrams.net](https://app.diagrams.net/)

---

## 🎨 Цветовая схема стрелок

| Тип стрелки | Сторона блока | Цвет |
|-------------|---------------|------|
| Вход (Input) | ▶ Левая | 🟠 Оранжевый |
| Выход (Output) | ▶ Правая | 🔵 Синий |
| Управление (Control) |  ▼Верхняя | 🔴 Красный |
| Механизм (Mechanism) | ▲ Нижняя | 🟢 Зелёный |

---

## 📂 Открытие `.drawio` файла

- 🌐 [diagrams.net](https://app.diagrams.net/) — бесплатно, онлайн и desktop
- 💡 **JetBrains IDEs** — плагин `drawio`
- 💻 **VS Code** — расширение `Draw.io Integration`

---

## 📜 Лицензия

**MIT** — используйте свободно, форкайте, улучшайте.
