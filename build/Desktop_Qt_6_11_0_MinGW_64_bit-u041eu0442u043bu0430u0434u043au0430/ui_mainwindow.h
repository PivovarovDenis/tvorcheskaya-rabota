/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *lbHeader;
    QSplitter *splitterMain;
    QGroupBox *gbSpec;
    QVBoxLayout *vlSpec;
    QLabel *lbSpecHint;
    QPlainTextEdit *teTechnicalSpec;
    QHBoxLayout *hlButtons;
    QPushButton *btnGenerate;
    QPushButton *btnClear;
    QGroupBox *gbResult;
    QVBoxLayout *vlResult;
    QTabWidget *tabWidget;
    QWidget *tabStructure;
    QVBoxLayout *vlStructure;
    QPlainTextEdit *teStructureView;
    QWidget *tabJson;
    QVBoxLayout *vlJson;
    QPlainTextEdit *teJsonPreview;
    QPushButton *btnSave;
    QLabel *lbLegend;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1080, 740);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName("verticalLayout");
        lbHeader = new QLabel(centralwidget);
        lbHeader->setObjectName("lbHeader");
        lbHeader->setAlignment(Qt::AlignCenter);
        lbHeader->setMinimumHeight(56);

        verticalLayout->addWidget(lbHeader);

        splitterMain = new QSplitter(centralwidget);
        splitterMain->setObjectName("splitterMain");
        splitterMain->setOrientation(Qt::Horizontal);
        splitterMain->setChildrenCollapsible(false);
        gbSpec = new QGroupBox(splitterMain);
        gbSpec->setObjectName("gbSpec");
        vlSpec = new QVBoxLayout(gbSpec);
        vlSpec->setSpacing(8);
        vlSpec->setObjectName("vlSpec");
        lbSpecHint = new QLabel(gbSpec);
        lbSpecHint->setObjectName("lbSpecHint");
        lbSpecHint->setWordWrap(true);

        vlSpec->addWidget(lbSpecHint);

        teTechnicalSpec = new QPlainTextEdit(gbSpec);
        teTechnicalSpec->setObjectName("teTechnicalSpec");

        vlSpec->addWidget(teTechnicalSpec);

        hlButtons = new QHBoxLayout();
        hlButtons->setObjectName("hlButtons");
        btnGenerate = new QPushButton(gbSpec);
        btnGenerate->setObjectName("btnGenerate");
        btnGenerate->setMinimumHeight(42);

        hlButtons->addWidget(btnGenerate);

        btnClear = new QPushButton(gbSpec);
        btnClear->setObjectName("btnClear");
        btnClear->setMinimumHeight(42);

        hlButtons->addWidget(btnClear);


        vlSpec->addLayout(hlButtons);

        splitterMain->addWidget(gbSpec);
        gbResult = new QGroupBox(splitterMain);
        gbResult->setObjectName("gbResult");
        vlResult = new QVBoxLayout(gbResult);
        vlResult->setSpacing(8);
        vlResult->setObjectName("vlResult");
        tabWidget = new QTabWidget(gbResult);
        tabWidget->setObjectName("tabWidget");
        tabStructure = new QWidget();
        tabStructure->setObjectName("tabStructure");
        vlStructure = new QVBoxLayout(tabStructure);
        vlStructure->setObjectName("vlStructure");
        teStructureView = new QPlainTextEdit(tabStructure);
        teStructureView->setObjectName("teStructureView");
        teStructureView->setReadOnly(true);
        QFont font;
        font.setFamilies({QString::fromUtf8("Courier New")});
        font.setPointSize(10);
        teStructureView->setFont(font);

        vlStructure->addWidget(teStructureView);

        tabWidget->addTab(tabStructure, QString());
        tabJson = new QWidget();
        tabJson->setObjectName("tabJson");
        vlJson = new QVBoxLayout(tabJson);
        vlJson->setObjectName("vlJson");
        teJsonPreview = new QPlainTextEdit(tabJson);
        teJsonPreview->setObjectName("teJsonPreview");
        teJsonPreview->setReadOnly(true);
        teJsonPreview->setFont(font);

        vlJson->addWidget(teJsonPreview);

        tabWidget->addTab(tabJson, QString());

        vlResult->addWidget(tabWidget);

        btnSave = new QPushButton(gbResult);
        btnSave->setObjectName("btnSave");
        btnSave->setMinimumHeight(42);
        btnSave->setEnabled(false);

        vlResult->addWidget(btnSave);

        splitterMain->addWidget(gbResult);

        verticalLayout->addWidget(splitterMain);

        lbLegend = new QLabel(centralwidget);
        lbLegend->setObjectName("lbLegend");
        lbLegend->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lbLegend);

        MainWindow->setCentralWidget(centralwidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "IDEF0 Generator", nullptr));
        MainWindow->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"    QMainWindow {\n"
"      background-color: #0d9488;\n"
"    }\n"
"    QWidget#centralwidget {\n"
"      background-color: #0d9488;\n"
"    }\n"
"    QGroupBox {\n"
"      background-color: #1e293b;\n"
"      border: 1px solid #0f766e;\n"
"      border-radius: 10px;\n"
"      color: #e2e8f0;\n"
"      font-size: 12px;\n"
"      font-weight: bold;\n"
"      margin-top: 22px;\n"
"      padding-top: 4px;\n"
"    }\n"
"    QGroupBox::title {\n"
"      subcontrol-origin: margin;\n"
"      subcontrol-position: top center;\n"
"      padding: 2px 14px;\n"
"      color: #000000;\n"
"      font-size: 13px;\n"
"      font-weight: bold;\n"
"      background-color: #5eead4;\n"
"      border-radius: 4px;\n"
"    }\n"
"    QPlainTextEdit {\n"
"      background-color: #0f172a;\n"
"      color: #e2e8f0;\n"
"      border: 1px solid #334155;\n"
"      border-radius: 6px;\n"
"      font-size: 12px;\n"
"      padding: 4px;\n"
"    }\n"
"    QPlainTextEdit:focus {\n"
"      border: 1px solid #14b8a6;\n"
"    }\n"
"    QTabWi"
                        "dget::pane {\n"
"      background-color: #0f172a;\n"
"      border: 1px solid #334155;\n"
"      border-radius: 6px;\n"
"    }\n"
"    QTabBar::tab {\n"
"      background-color: #1e293b;\n"
"      color: #94a3b8;\n"
"      border: 1px solid #334155;\n"
"      border-bottom: none;\n"
"      border-radius: 4px 4px 0 0;\n"
"      padding: 6px 18px;\n"
"      font-size: 11px;\n"
"    }\n"
"    QTabBar::tab:selected {\n"
"      background-color: #0f172a;\n"
"      color: #5eead4;\n"
"      font-weight: bold;\n"
"    }\n"
"    QTabBar::tab:hover {\n"
"      color: #e2e8f0;\n"
"    }\n"
"    QStatusBar {\n"
"      background-color: #134e4a;\n"
"      color: #99f6e4;\n"
"      font-size: 11px;\n"
"    }\n"
"    QLabel#lbLegend {\n"
"      color: #ffffff;\n"
"      font-size: 13px;\n"
"      font-weight: bold;\n"
"    }\n"
"    QLabel#lbSpecHint {\n"
"      color: #94a3b8;\n"
"      font-size: 11px;\n"
"    }\n"
"   ", nullptr));
        lbHeader->setText(QCoreApplication::translate("MainWindow", "<div style=\"text-align:center;\">\n"
"         <span style=\"font-size:22px; font-weight:bold; color:#ffffff; letter-spacing:1px;\">\n"
"           IDEF0 Generator\n"
"         </span>\n"
"         <br/>\n"
"         <span style=\"font-size:11px; color:#99f6e4;\">\n"
"           \320\223\320\265\320\275\320\265\321\200\320\260\321\202\320\276\321\200 SADT-\320\264\320\270\320\260\320\263\321\200\320\260\320\274\320\274 \320\275\320\260 \320\261\320\260\320\267\320\265 DeepSeek AI\n"
"         </span>\n"
"       </div>", nullptr));
        gbSpec->setTitle(QCoreApplication::translate("MainWindow", "\320\242\320\265\321\205\320\275\320\270\321\207\320\265\321\201\320\272\320\276\320\265 \320\267\320\260\320\264\320\260\320\275\320\270\320\265", nullptr));
        lbSpecHint->setText(QCoreApplication::translate("MainWindow", "\320\236\320\277\320\270\321\210\320\270\321\202\320\265 \321\201\320\270\321\201\321\202\320\265\320\274\321\203: \320\275\320\260\320\267\320\275\320\260\321\207\320\265\320\275\320\270\320\265, \320\262\321\205\320\276\320\264\320\275\321\213\320\265 \320\264\320\260\320\275\320\275\321\213\320\265, \321\200\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202\321\213, \320\276\320\263\321\200\320\260\320\275\320\270\321\207\320\265\320\275\320\270\321\217, \320\270\321\201\320\277\320\276\320\273\320\275\320\270\321\202\320\265\320\273\320\270.", nullptr));
        teTechnicalSpec->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\270\320\274\320\265\321\200: \320\240\320\260\320\267\321\200\320\260\320\261\320\276\321\202\320\260\321\202\321\214 \321\201\320\270\321\201\321\202\320\265\320\274\321\203 \321\203\321\207\321\221\321\202\320\260 \321\201\320\272\320\273\320\260\320\264\321\201\320\272\320\270\321\205 \320\267\320\260\320\277\320\260\321\201\320\276\320\262. \320\241\320\270\321\201\321\202\320\265\320\274\320\260 \320\277\321\200\320\270\320\275\320\270\320\274\320\260\320\265\321\202 \320\275\320\260\320\272\320\273\320\260\320\264\320\275\321\213\320\265 \320\276\321\202 \320\277\320\276\321\201\321\202\320\260\320\262\321\211\320\270\320\272\320\276\320\262, \320\276\320\261\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\320\265\321\202 \320\264\320\260\320\275\320\275\321\213\320\265 \320\276 \321\202\320\276\320\262\320\260\321\200\320\260\321\205, \321\204\320\276\321\200\320\274\320\270\321\200\321\203\320\265\321\202 \320\276\321\202\321\207\321\221\321\202\321\213 \320\270 \321"
                        "\203\320\262\320\265\320\264\320\276\320\274\320\273\321\217\320\265\321\202 \320\274\320\265\320\275\320\265\320\264\320\266\320\265\321\200\320\276\320\262. \320\230\321\201\320\277\320\276\320\273\320\275\320\270\321\202\320\265\320\273\320\270: \320\272\320\273\320\260\320\264\320\276\320\262\321\211\320\270\320\272\320\270, \320\261\321\203\321\205\320\263\320\260\320\273\321\202\320\265\321\200\321\213, \321\201\320\270\321\201\321\202\320\265\320\274\320\260 1\320\241.", nullptr));
        btnGenerate->setText(QCoreApplication::translate("MainWindow", "\320\241\320\263\320\265\320\275\320\265\321\200\320\270\321\200\320\276\320\262\320\260\321\202\321\214 IDEF0", nullptr));
        btnGenerate->setStyleSheet(QCoreApplication::translate("MainWindow", "QPushButton { background-color: #0f766e; color: #ffffff; border-radius: 8px; font-size: 13px; font-weight: bold; padding: 4px 20px; border: none; }\n"
"QPushButton:hover { background-color: #14b8a6; }\n"
"QPushButton:pressed { background-color: #0d9488; }\n"
"QPushButton:disabled { background-color: #1e3a38; color: #4b7a76; }", nullptr));
        btnClear->setText(QCoreApplication::translate("MainWindow", "\320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214", nullptr));
        btnClear->setStyleSheet(QCoreApplication::translate("MainWindow", "QPushButton { background-color: #1e293b; color: #94a3b8; border: 1px solid #334155; border-radius: 8px; font-size: 12px; padding: 4px 14px; }\n"
"QPushButton:hover { background-color: #334155; color: #e2e8f0; }\n"
"QPushButton:pressed { background-color: #0f172a; }", nullptr));
        gbResult->setTitle(QCoreApplication::translate("MainWindow", "\320\240\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202", nullptr));
        tabWidget->setStyleSheet(QString());
        teStructureView->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\227\320\264\320\265\321\201\321\214 \320\277\320\276\321\217\320\262\320\270\321\202\321\201\321\217 \321\201\321\202\321\200\321\203\320\272\321\202\321\203\321\200\320\260 IDEF0-\320\261\320\273\320\276\320\272\320\276\320\262 \320\277\320\276\321\201\320\273\320\265 \320\263\320\265\320\275\320\265\321\200\320\260\321\206\320\270\320\270...", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabStructure), QCoreApplication::translate("MainWindow", "\320\241\321\202\321\200\321\203\320\272\321\202\321\203\321\200\320\260 \320\261\320\273\320\276\320\272\320\276\320\262", nullptr));
        teJsonPreview->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\241\321\213\321\200\320\276\320\271 JSON-\320\276\321\202\320\262\320\265\321\202 \320\276\321\202 \320\275\320\265\320\271\321\200\320\276\321\201\320\265\321\202\320\270 \320\277\320\276\321\217\320\262\320\270\321\202\321\201\321\217 \320\267\320\264\320\265\321\201\321\214...", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabJson), QCoreApplication::translate("MainWindow", "JSON \320\276\321\202\320\262\320\265\321\202", nullptr));
        btnSave->setText(QCoreApplication::translate("MainWindow", "\360\237\222\276  \320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\272\320\260\320\272 .drawio", nullptr));
        btnSave->setStyleSheet(QCoreApplication::translate("MainWindow", "QPushButton { background-color: #15803d; color: white; border-radius: 8px; font-size: 13px; font-weight: bold; padding: 4px 20px; border: none; }\n"
"QPushButton:hover { background-color: #16a34a; }\n"
"QPushButton:pressed { background-color: #14532d; }\n"
"QPushButton:disabled { background-color: #1a3626; color: #4a7a5a; }", nullptr));
        lbLegend->setText(QCoreApplication::translate("MainWindow", "\342\206\222 \320\222\321\205\320\276\320\264 (Input) \342\200\224 \321\201\320\273\320\265\320\262\320\260   |   \342\206\222 \320\222\321\213\321\205\320\276\320\264 (Output) \342\200\224 \321\201\320\277\321\200\320\260\320\262\320\260   |   \342\206\223 \320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265 (Control) \342\200\224 \321\201\320\262\320\265\321\200\321\205\321\203   |   \342\206\221 \320\234\320\265\321\205\320\260\320\275\320\270\320\267\320\274 (Mechanism) \342\200\224 \321\201\320\275\320\270\320\267\321\203", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
