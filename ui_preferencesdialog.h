/********************************************************************************
** Form generated from reading UI file 'preferencesdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFERENCESDIALOG_H
#define UI_PREFERENCESDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFontComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PreferencesDialog
{
public:
    QVBoxLayout *verticalLayout_6;
    QTabWidget *tabWidget;
    QWidget *tab_General;
    QVBoxLayout *verticalLayout_General;
    QVBoxLayout *verticalLayout_5;
    QGroupBox *groupBox_Java;
    QVBoxLayout *verticalLayout;
    QRadioButton *defaultJavaRadio;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *customJavaRadio;
    QLineEdit *customJavaPathEdit;
    QToolButton *customJavaPathButton;
    QGroupBox *groupBox_PlantUML;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *defaultPlatUmlRadio;
    QHBoxLayout *horizontalLayout_4;
    QRadioButton *customPlantUmlRadio;
    QLineEdit *customPlantUmlEdit;
    QToolButton *customPlantUmlButton;
    QGroupBox *groupBox_Graphviz;
    QVBoxLayout *verticalLayout_3;
    QRadioButton *defaultGraphizRadio;
    QHBoxLayout *horizontalLayout_5;
    QRadioButton *customGraphizRadio;
    QLineEdit *customGraphizEdit;
    QToolButton *customGraphizButton;
    QSpacerItem *verticalSpacer_General;
    QWidget *tab_Assistant;
    QVBoxLayout *verticalLayout_Assistant;
    QGroupBox *groupBox_Assistant_File;
    QVBoxLayout *verticalLayout_11;
    QHBoxLayout *horizontalLayout_16;
    QLabel *label_6;
    QLineEdit *assistantXmlEdit;
    QToolButton *assistantXmlButton;
    QGroupBox *groupBox_Assistant_Font;
    QHBoxLayout *horizontalLayout_14;
    QHBoxLayout *horizontalLayout_9;
    QFontComboBox *assistantInfoFontComboBox;
    QSpinBox *assistantInfoFontSizeSpinBox;
    QSpacerItem *verticalSpacer_Assistant;
    QWidget *tab_Preview;
    QVBoxLayout *verticalLayout_Preview;
    QGroupBox *groupBox_ImageCache;
    QVBoxLayout *verticalLayout_4;
    QRadioButton *defaultCacheRadio;
    QHBoxLayout *horizontalLayout_6;
    QRadioButton *customCacheRadio;
    QLineEdit *customCacheEdit;
    QToolButton *customCacheButton;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_9;
    QSpinBox *cacheMaxSize;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_7;
    QLabel *cacheCurrentSizeLabel;
    QPushButton *clearCacheButton;
    QGroupBox *groupBox_Preview_Other;
    QVBoxLayout *verticalLayout_12;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_3;
    QSpinBox *autoRefreshSpin;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer;
    QCheckBox *refreshOnSaveCheckBox;
    QSpacerItem *verticalSpacer_Preview;
    QWidget *tab_Editor;
    QVBoxLayout *verticalLayout_Editor;
    QGroupBox *groupBox_Font;
    QHBoxLayout *horizontalLayout_11;
    QHBoxLayout *horizontalLayout_2;
    QFontComboBox *editorFontComboBox;
    QSpinBox *editorFontSizeSpinBox;
    QGroupBox *groupBox_Indention;
    QVBoxLayout *verticalLayout_9;
    QCheckBox *autoIndentCheckBox;
    QCheckBox *useSpacesInsteadTabsCheckBox;
    QHBoxLayout *horizontalLayout_13;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label;
    QSpinBox *indentSizeSpinBox;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox_Editor_Other;
    QVBoxLayout *verticalLayout_8;
    QCheckBox *useLastDocumentCheckBox;
    QSpacerItem *verticalSpacer_Editor;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_4;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *PreferencesDialog)
    {
        if (PreferencesDialog->objectName().isEmpty())
            PreferencesDialog->setObjectName(QStringLiteral("PreferencesDialog"));
        PreferencesDialog->resize(712, 445);
        PreferencesDialog->setModal(true);
        verticalLayout_6 = new QVBoxLayout(PreferencesDialog);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        tabWidget = new QTabWidget(PreferencesDialog);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab_General = new QWidget();
        tab_General->setObjectName(QStringLiteral("tab_General"));
        verticalLayout_General = new QVBoxLayout(tab_General);
        verticalLayout_General->setObjectName(QStringLiteral("verticalLayout_General"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        groupBox_Java = new QGroupBox(tab_General);
        groupBox_Java->setObjectName(QStringLiteral("groupBox_Java"));
        verticalLayout = new QVBoxLayout(groupBox_Java);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        defaultJavaRadio = new QRadioButton(groupBox_Java);
        defaultJavaRadio->setObjectName(QStringLiteral("defaultJavaRadio"));

        verticalLayout->addWidget(defaultJavaRadio);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        customJavaRadio = new QRadioButton(groupBox_Java);
        customJavaRadio->setObjectName(QStringLiteral("customJavaRadio"));

        horizontalLayout_3->addWidget(customJavaRadio);

        customJavaPathEdit = new QLineEdit(groupBox_Java);
        customJavaPathEdit->setObjectName(QStringLiteral("customJavaPathEdit"));

        horizontalLayout_3->addWidget(customJavaPathEdit);

        customJavaPathButton = new QToolButton(groupBox_Java);
        customJavaPathButton->setObjectName(QStringLiteral("customJavaPathButton"));

        horizontalLayout_3->addWidget(customJavaPathButton);


        verticalLayout->addLayout(horizontalLayout_3);


        verticalLayout_5->addWidget(groupBox_Java);

        groupBox_PlantUML = new QGroupBox(tab_General);
        groupBox_PlantUML->setObjectName(QStringLiteral("groupBox_PlantUML"));
        verticalLayout_2 = new QVBoxLayout(groupBox_PlantUML);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        defaultPlatUmlRadio = new QRadioButton(groupBox_PlantUML);
        defaultPlatUmlRadio->setObjectName(QStringLiteral("defaultPlatUmlRadio"));

        verticalLayout_2->addWidget(defaultPlatUmlRadio);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        customPlantUmlRadio = new QRadioButton(groupBox_PlantUML);
        customPlantUmlRadio->setObjectName(QStringLiteral("customPlantUmlRadio"));

        horizontalLayout_4->addWidget(customPlantUmlRadio);

        customPlantUmlEdit = new QLineEdit(groupBox_PlantUML);
        customPlantUmlEdit->setObjectName(QStringLiteral("customPlantUmlEdit"));

        horizontalLayout_4->addWidget(customPlantUmlEdit);

        customPlantUmlButton = new QToolButton(groupBox_PlantUML);
        customPlantUmlButton->setObjectName(QStringLiteral("customPlantUmlButton"));

        horizontalLayout_4->addWidget(customPlantUmlButton);


        verticalLayout_2->addLayout(horizontalLayout_4);


        verticalLayout_5->addWidget(groupBox_PlantUML);

        groupBox_Graphviz = new QGroupBox(tab_General);
        groupBox_Graphviz->setObjectName(QStringLiteral("groupBox_Graphviz"));
        verticalLayout_3 = new QVBoxLayout(groupBox_Graphviz);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        defaultGraphizRadio = new QRadioButton(groupBox_Graphviz);
        defaultGraphizRadio->setObjectName(QStringLiteral("defaultGraphizRadio"));

        verticalLayout_3->addWidget(defaultGraphizRadio);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        customGraphizRadio = new QRadioButton(groupBox_Graphviz);
        customGraphizRadio->setObjectName(QStringLiteral("customGraphizRadio"));

        horizontalLayout_5->addWidget(customGraphizRadio);

        customGraphizEdit = new QLineEdit(groupBox_Graphviz);
        customGraphizEdit->setObjectName(QStringLiteral("customGraphizEdit"));

        horizontalLayout_5->addWidget(customGraphizEdit);

        customGraphizButton = new QToolButton(groupBox_Graphviz);
        customGraphizButton->setObjectName(QStringLiteral("customGraphizButton"));

        horizontalLayout_5->addWidget(customGraphizButton);


        verticalLayout_3->addLayout(horizontalLayout_5);


        verticalLayout_5->addWidget(groupBox_Graphviz);

        verticalSpacer_General = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_General);


        verticalLayout_General->addLayout(verticalLayout_5);

        tabWidget->addTab(tab_General, QString());
        tab_Assistant = new QWidget();
        tab_Assistant->setObjectName(QStringLiteral("tab_Assistant"));
        verticalLayout_Assistant = new QVBoxLayout(tab_Assistant);
        verticalLayout_Assistant->setObjectName(QStringLiteral("verticalLayout_Assistant"));
        groupBox_Assistant_File = new QGroupBox(tab_Assistant);
        groupBox_Assistant_File->setObjectName(QStringLiteral("groupBox_Assistant_File"));
        verticalLayout_11 = new QVBoxLayout(groupBox_Assistant_File);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));
        label_6 = new QLabel(groupBox_Assistant_File);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_16->addWidget(label_6);

        assistantXmlEdit = new QLineEdit(groupBox_Assistant_File);
        assistantXmlEdit->setObjectName(QStringLiteral("assistantXmlEdit"));

        horizontalLayout_16->addWidget(assistantXmlEdit);

        assistantXmlButton = new QToolButton(groupBox_Assistant_File);
        assistantXmlButton->setObjectName(QStringLiteral("assistantXmlButton"));

        horizontalLayout_16->addWidget(assistantXmlButton);


        verticalLayout_11->addLayout(horizontalLayout_16);


        verticalLayout_Assistant->addWidget(groupBox_Assistant_File);

        groupBox_Assistant_Font = new QGroupBox(tab_Assistant);
        groupBox_Assistant_Font->setObjectName(QStringLiteral("groupBox_Assistant_Font"));
        horizontalLayout_14 = new QHBoxLayout(groupBox_Assistant_Font);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        assistantInfoFontComboBox = new QFontComboBox(groupBox_Assistant_Font);
        assistantInfoFontComboBox->setObjectName(QStringLiteral("assistantInfoFontComboBox"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(assistantInfoFontComboBox->sizePolicy().hasHeightForWidth());
        assistantInfoFontComboBox->setSizePolicy(sizePolicy);

        horizontalLayout_9->addWidget(assistantInfoFontComboBox);

        assistantInfoFontSizeSpinBox = new QSpinBox(groupBox_Assistant_Font);
        assistantInfoFontSizeSpinBox->setObjectName(QStringLiteral("assistantInfoFontSizeSpinBox"));
        assistantInfoFontSizeSpinBox->setMinimum(5);
        assistantInfoFontSizeSpinBox->setValue(10);

        horizontalLayout_9->addWidget(assistantInfoFontSizeSpinBox);


        horizontalLayout_14->addLayout(horizontalLayout_9);


        verticalLayout_Assistant->addWidget(groupBox_Assistant_Font);

        verticalSpacer_Assistant = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_Assistant->addItem(verticalSpacer_Assistant);

        tabWidget->addTab(tab_Assistant, QString());
        tab_Preview = new QWidget();
        tab_Preview->setObjectName(QStringLiteral("tab_Preview"));
        verticalLayout_Preview = new QVBoxLayout(tab_Preview);
        verticalLayout_Preview->setObjectName(QStringLiteral("verticalLayout_Preview"));
        groupBox_ImageCache = new QGroupBox(tab_Preview);
        groupBox_ImageCache->setObjectName(QStringLiteral("groupBox_ImageCache"));
        groupBox_ImageCache->setCheckable(true);
        verticalLayout_4 = new QVBoxLayout(groupBox_ImageCache);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        defaultCacheRadio = new QRadioButton(groupBox_ImageCache);
        defaultCacheRadio->setObjectName(QStringLiteral("defaultCacheRadio"));

        verticalLayout_4->addWidget(defaultCacheRadio);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        customCacheRadio = new QRadioButton(groupBox_ImageCache);
        customCacheRadio->setObjectName(QStringLiteral("customCacheRadio"));

        horizontalLayout_6->addWidget(customCacheRadio);

        customCacheEdit = new QLineEdit(groupBox_ImageCache);
        customCacheEdit->setObjectName(QStringLiteral("customCacheEdit"));

        horizontalLayout_6->addWidget(customCacheEdit);

        customCacheButton = new QToolButton(groupBox_ImageCache);
        customCacheButton->setObjectName(QStringLiteral("customCacheButton"));

        horizontalLayout_6->addWidget(customCacheButton);


        verticalLayout_4->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_9 = new QLabel(groupBox_ImageCache);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_7->addWidget(label_9);

        cacheMaxSize = new QSpinBox(groupBox_ImageCache);
        cacheMaxSize->setObjectName(QStringLiteral("cacheMaxSize"));
        cacheMaxSize->setMaximum(1000);

        horizontalLayout_7->addWidget(cacheMaxSize);

        label_2 = new QLabel(groupBox_ImageCache);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_7->addWidget(label_2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_3);


        verticalLayout_4->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_7 = new QLabel(groupBox_ImageCache);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_8->addWidget(label_7);

        cacheCurrentSizeLabel = new QLabel(groupBox_ImageCache);
        cacheCurrentSizeLabel->setObjectName(QStringLiteral("cacheCurrentSizeLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cacheCurrentSizeLabel->sizePolicy().hasHeightForWidth());
        cacheCurrentSizeLabel->setSizePolicy(sizePolicy1);
        cacheCurrentSizeLabel->setFrameShape(QFrame::StyledPanel);
        cacheCurrentSizeLabel->setFrameShadow(QFrame::Sunken);

        horizontalLayout_8->addWidget(cacheCurrentSizeLabel);

        clearCacheButton = new QPushButton(groupBox_ImageCache);
        clearCacheButton->setObjectName(QStringLiteral("clearCacheButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(clearCacheButton->sizePolicy().hasHeightForWidth());
        clearCacheButton->setSizePolicy(sizePolicy2);

        horizontalLayout_8->addWidget(clearCacheButton);


        verticalLayout_4->addLayout(horizontalLayout_8);


        verticalLayout_Preview->addWidget(groupBox_ImageCache);

        groupBox_Preview_Other = new QGroupBox(tab_Preview);
        groupBox_Preview_Other->setObjectName(QStringLiteral("groupBox_Preview_Other"));
        verticalLayout_12 = new QVBoxLayout(groupBox_Preview_Other);
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        label_3 = new QLabel(groupBox_Preview_Other);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_10->addWidget(label_3);

        autoRefreshSpin = new QSpinBox(groupBox_Preview_Other);
        autoRefreshSpin->setObjectName(QStringLiteral("autoRefreshSpin"));

        horizontalLayout_10->addWidget(autoRefreshSpin);

        label_4 = new QLabel(groupBox_Preview_Other);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_10->addWidget(label_4);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer);


        verticalLayout_12->addLayout(horizontalLayout_10);

        refreshOnSaveCheckBox = new QCheckBox(groupBox_Preview_Other);
        refreshOnSaveCheckBox->setObjectName(QStringLiteral("refreshOnSaveCheckBox"));

        verticalLayout_12->addWidget(refreshOnSaveCheckBox);


        verticalLayout_Preview->addWidget(groupBox_Preview_Other, 0, Qt::AlignTop);

        verticalSpacer_Preview = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_Preview->addItem(verticalSpacer_Preview);

        tabWidget->addTab(tab_Preview, QString());
        tab_Editor = new QWidget();
        tab_Editor->setObjectName(QStringLiteral("tab_Editor"));
        verticalLayout_Editor = new QVBoxLayout(tab_Editor);
        verticalLayout_Editor->setObjectName(QStringLiteral("verticalLayout_Editor"));
        groupBox_Font = new QGroupBox(tab_Editor);
        groupBox_Font->setObjectName(QStringLiteral("groupBox_Font"));
        horizontalLayout_11 = new QHBoxLayout(groupBox_Font);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        editorFontComboBox = new QFontComboBox(groupBox_Font);
        editorFontComboBox->setObjectName(QStringLiteral("editorFontComboBox"));
        sizePolicy.setHeightForWidth(editorFontComboBox->sizePolicy().hasHeightForWidth());
        editorFontComboBox->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(editorFontComboBox);

        editorFontSizeSpinBox = new QSpinBox(groupBox_Font);
        editorFontSizeSpinBox->setObjectName(QStringLiteral("editorFontSizeSpinBox"));
        editorFontSizeSpinBox->setMinimum(5);
        editorFontSizeSpinBox->setValue(10);

        horizontalLayout_2->addWidget(editorFontSizeSpinBox);


        horizontalLayout_11->addLayout(horizontalLayout_2);


        verticalLayout_Editor->addWidget(groupBox_Font);

        groupBox_Indention = new QGroupBox(tab_Editor);
        groupBox_Indention->setObjectName(QStringLiteral("groupBox_Indention"));
        verticalLayout_9 = new QVBoxLayout(groupBox_Indention);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        autoIndentCheckBox = new QCheckBox(groupBox_Indention);
        autoIndentCheckBox->setObjectName(QStringLiteral("autoIndentCheckBox"));
        autoIndentCheckBox->setChecked(true);

        verticalLayout_9->addWidget(autoIndentCheckBox);

        useSpacesInsteadTabsCheckBox = new QCheckBox(groupBox_Indention);
        useSpacesInsteadTabsCheckBox->setObjectName(QStringLiteral("useSpacesInsteadTabsCheckBox"));

        verticalLayout_9->addWidget(useSpacesInsteadTabsCheckBox);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        label = new QLabel(groupBox_Indention);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_12->addWidget(label);

        indentSizeSpinBox = new QSpinBox(groupBox_Indention);
        indentSizeSpinBox->setObjectName(QStringLiteral("indentSizeSpinBox"));
        indentSizeSpinBox->setMinimum(1);
        indentSizeSpinBox->setValue(4);

        horizontalLayout_12->addWidget(indentSizeSpinBox);


        horizontalLayout_13->addLayout(horizontalLayout_12);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_2);


        verticalLayout_9->addLayout(horizontalLayout_13);


        verticalLayout_Editor->addWidget(groupBox_Indention);

        groupBox_Editor_Other = new QGroupBox(tab_Editor);
        groupBox_Editor_Other->setObjectName(QStringLiteral("groupBox_Editor_Other"));
        verticalLayout_8 = new QVBoxLayout(groupBox_Editor_Other);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        useLastDocumentCheckBox = new QCheckBox(groupBox_Editor_Other);
        useLastDocumentCheckBox->setObjectName(QStringLiteral("useLastDocumentCheckBox"));

        verticalLayout_8->addWidget(useLastDocumentCheckBox);


        verticalLayout_Editor->addWidget(groupBox_Editor_Other);

        verticalSpacer_Editor = new QSpacerItem(20, 308, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_Editor->addItem(verticalSpacer_Editor);

        tabWidget->addTab(tab_Editor, QString());

        verticalLayout_6->addWidget(tabWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        buttonBox = new QDialogButtonBox(PreferencesDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        sizePolicy2.setHeightForWidth(buttonBox->sizePolicy().hasHeightForWidth());
        buttonBox->setSizePolicy(sizePolicy2);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout->addWidget(buttonBox);


        verticalLayout_6->addLayout(horizontalLayout);


        retranslateUi(PreferencesDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), PreferencesDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), PreferencesDialog, SLOT(reject()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(PreferencesDialog);
    } // setupUi

    void retranslateUi(QDialog *PreferencesDialog)
    {
        PreferencesDialog->setWindowTitle(QApplication::translate("PreferencesDialog", "Preferences", 0));
        groupBox_Java->setTitle(QApplication::translate("PreferencesDialog", "Java executable", 0));
        defaultJavaRadio->setText(QApplication::translate("PreferencesDialog", "Default", 0));
        customJavaRadio->setText(QApplication::translate("PreferencesDialog", "Custom", 0));
        customJavaPathButton->setText(QApplication::translate("PreferencesDialog", "...", 0));
        groupBox_PlantUML->setTitle(QApplication::translate("PreferencesDialog", "PlantUML jar", 0));
        defaultPlatUmlRadio->setText(QApplication::translate("PreferencesDialog", "Default", 0));
        customPlantUmlRadio->setText(QApplication::translate("PreferencesDialog", "Custom", 0));
        customPlantUmlButton->setText(QApplication::translate("PreferencesDialog", "...", 0));
        groupBox_Graphviz->setTitle(QApplication::translate("PreferencesDialog", "Graphviz / Dot", 0));
        defaultGraphizRadio->setText(QApplication::translate("PreferencesDialog", "Default", 0));
        customGraphizRadio->setText(QApplication::translate("PreferencesDialog", "Custom", 0));
        customGraphizButton->setText(QApplication::translate("PreferencesDialog", "...", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_General), QApplication::translate("PreferencesDialog", "General", 0));
        groupBox_Assistant_File->setTitle(QApplication::translate("PreferencesDialog", "Assistant", 0));
        label_6->setText(QApplication::translate("PreferencesDialog", "XML File", 0));
        assistantXmlButton->setText(QApplication::translate("PreferencesDialog", "...", 0));
        groupBox_Assistant_Font->setTitle(QApplication::translate("PreferencesDialog", "Font", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_Assistant), QApplication::translate("PreferencesDialog", "Assistant", 0));
        groupBox_ImageCache->setTitle(QApplication::translate("PreferencesDialog", "Image Cache", 0));
        defaultCacheRadio->setText(QApplication::translate("PreferencesDialog", "Default", 0));
        customCacheRadio->setText(QApplication::translate("PreferencesDialog", "Custom", 0));
        customCacheButton->setText(QApplication::translate("PreferencesDialog", "...", 0));
        label_9->setText(QApplication::translate("PreferencesDialog", "Max size:", 0));
        label_2->setText(QApplication::translate("PreferencesDialog", "MB", 0));
        label_7->setText(QApplication::translate("PreferencesDialog", "Current size:", 0));
        cacheCurrentSizeLabel->setText(QString());
        clearCacheButton->setText(QApplication::translate("PreferencesDialog", "Clear cache", 0));
        groupBox_Preview_Other->setTitle(QApplication::translate("PreferencesDialog", "Other", 0));
        label_3->setText(QApplication::translate("PreferencesDialog", "Auto-refresh interval:", 0));
        label_4->setText(QApplication::translate("PreferencesDialog", "Seconds", 0));
        refreshOnSaveCheckBox->setText(QApplication::translate("PreferencesDialog", "Refresh on save", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_Preview), QApplication::translate("PreferencesDialog", "Preview", 0));
        groupBox_Font->setTitle(QApplication::translate("PreferencesDialog", "Font", 0));
        groupBox_Indention->setTitle(QApplication::translate("PreferencesDialog", "Indention", 0));
        autoIndentCheckBox->setText(QApplication::translate("PreferencesDialog", "Autoindent", 0));
        useSpacesInsteadTabsCheckBox->setText(QApplication::translate("PreferencesDialog", "Use Spaces instead Tabs", 0));
        label->setText(QApplication::translate("PreferencesDialog", "Indent size:", 0));
        groupBox_Editor_Other->setTitle(QApplication::translate("PreferencesDialog", "Other", 0));
        useLastDocumentCheckBox->setText(QApplication::translate("PreferencesDialog", "Load last file on startup", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_Editor), QApplication::translate("PreferencesDialog", "Editor", 0));
    } // retranslateUi

};

namespace Ui {
    class PreferencesDialog: public Ui_PreferencesDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFERENCESDIALOG_H
