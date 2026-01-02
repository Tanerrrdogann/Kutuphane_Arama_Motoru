#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QFrame>
#include <QMenu>
#include <QDialog>
#include <QFormLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QMessageBox>
#include <QCloseEvent>
#include <QCompleter>
#include <QStringListModel>
#include <QGraphicsDropShadowEffect>
#include <QResizeEvent>
#include <QPropertyAnimation>
#include <QAbstractItemView>
#include <QLocale>
#include "backend.h"

const QString STIL_MAIN_WINDOW = "QMainWindow { background-color: #C0C0C0; }";

const QString STIL_CONTENT_CARD = R"(
    QWidget#ContentCard {
        background-color: white;
        border-radius: 25px;
    }
)";

const QString STIL_ARAMA_KAPSULU_NORMAL = R"(
    QFrame#SearchPill {
        background-color: white;
        border: 2px solid #E0E0E0;
        border-radius: 35px;
    }
)";

const QString STIL_ARAMA_KAPSULU_BFS = R"(
    QFrame#SearchPill {
        background-color: transparent;
        border: none;
    }
)";

const QString STIL_BFS_INPUT = R"(
    QLineEdit {
        background-color: white;
        border: 2px solid #E0E0E0;
        border-radius: 25px;
        padding-left: 20px;
        font-size: 15px;
        color: #333;
        min-height: 50px;
        max-height: 50px;
    }
)";

const QString STIL_COMBO_ROUND = R"(
    QComboBox {
        background-color: white;
        border: 2px solid #E0E0E0;
        border-radius: 25px;
        padding-left: 20px;
        padding-right: 50px;
        min-height: 50px;
        max-height: 50px;
        font-size: 15px;
        color: #333;
    }
    QComboBox::drop-down {
        subcontrol-origin: padding;
        subcontrol-position: center right;
        width: 50px;
        border-left: 1px solid #E0E0E0;
        background: transparent;
        margin-right: 5px;
    }
    QComboBox::down-arrow {
        image: url(/Users/macbook/Documents/asagi.png);
        width: 45px;
        height: 45px;
        margin-right: 5px;
    }
    QComboBox QAbstractItemView {
        background-color: white;
        border: 1px solid #D0D0D0;
        border-radius: 15px;
        padding: 5px;
        outline: 0;
        margin-top: 5px;
    }
    QComboBox QAbstractItemView::item {
        min-height: 30px;
        padding: 2px 10px;
        border-radius: 8px;
        margin: 2px 0px;
        color: #333;
    }
    QComboBox QAbstractItemView::item:selected {
        background-color: #F0F0F0;
        color: black;
        font-weight: bold;
    }
)";

const QString STIL_DAIRE_BTN = R"(
    QPushButton {
        background-color: #C0C0C0;
        color: #444;
        border-radius: 25px;
        min-width: 50px;
        max-width: 50px;
        min-height: 50px;
        max-height: 50px;
        border: none;
        padding: 0px;
        qproperty-iconSize: 100px 100px;
    }
    QPushButton:hover { background-color: #A0A0A0; }
    QPushButton:disabled {
        background-color: #F8F8F8;
        border: 1px solid #F0F0F0;
    })";

const QString STIL_DAIRE_BTN_SEARCH = R"(
    QPushButton {
        background-color: #C0C0C0;
        color: #444;
        border-radius: 25px;
        min-width: 50px;
        max-width: 50px;
        min-height: 46px;
        max-height: 46px;
        border: none;
        padding-bottom: 4px;
        qproperty-iconSize: 100px 100px;
    }
    QPushButton:hover { background-color: #A0A0A0; }
    QPushButton:disabled {
        background-color: #F8F8F8;
        border: 1px solid #F0F0F0;
    })";

const QString STIL_DAIRE_BTN_UNDO = R"(
    QPushButton {
        background-color: #C0C0C0;
        border-radius: 25px;
        min-width: 45px;
        max-width: 45px;
        min-height: 50px;
        max-height: 50px;
        border: none;
        padding-right: 5px;
        qproperty-iconSize: 75px 75px;
    }
    QPushButton:hover { background-color: #A0A0A0; }

    QPushButton:disabled {
        background-color: #E0E0E0;
    }
)";

const QString STIL_DAIRE_BTN_REDO = R"(
    QPushButton {
        background-color: #C0C0C0;
        border-radius: 25px;
        min-width: 46px;
        max-width: 46px;
        min-height: 50px;
        max-height: 50px;
        border: none;
        padding-left: 4px;

        qproperty-iconSize: 75px 75px;
    }
    QPushButton:hover { background-color: #A0A0A0; }
    QPushButton:disabled {
        background-color: #E0E0E0;
    }
)";

const QString STIL_FAB_BTN = R"(
    QPushButton {
        background-color: #333;
        color: white;
        border-radius: 30px;
        min-width: 60px;
        max-width: 60px;
        min-height: 60px;
        max-height: 60px;
        font-size: 40px;
        font-weight: bold;
        border: none;
        padding: 0px;
        padding-bottom: 0px;
        text-align: center;
    }
    QPushButton:hover { background-color: #555; }
)";

const QString STIL_FILTRE_BTN = R"(
    QPushButton {
        background-color: #C0C0C0;
        color: white;
        border-radius: 25px;
        min-height: 50px;
        max-height: 50px;
        padding-left: 35px;
        padding-right: 35px;
        font-weight: bold;
        font-size: 25px;
        border: none;
    }
    QPushButton:hover { background-color: #A0A0A0; }
)";

const QString STIL_ARAMA_INPUT = R"(
    QLineEdit {
        border: none;
        background: transparent;
        font-size: 16px;
        color: #333;
        margin-left: 10px;
        margin-right: 10px;
        padding: 0px;
    }
)";

const QString STIL_LABEL_FILTRE = R"(
    QLabel {
        color: #333;
        font-weight: bold;
        font-size: 14px;

        min-height: 45px;
        max-height: 45px;

        qproperty-alignment: 'AlignVCenter | AlignLeft';

        border: none;
        padding-right: 10px;
    }
)";

const QString STIL_INPUT_FILTRE = R"(
    QLineEdit {
        background-color: white;
        border: 1px solid #DDD;
        border-radius: 25px;
        padding: 0px 15px;
        color: #333;
        selection-background-color: #CCC;
        min-height: 50px;
        max-height: 50px;
        font-size: 15px;
    }
)";

const QString STIL_COMBO_MODERN = R"(
    QComboBox {
        background-color: white;
        border: 1px solid #DDD;
        border-radius: 25px;
        padding-left: 25px;
        padding-right: 50px;
        min-height: 50px;
        max-height: 50px;
        font-size: 15px;
        color: #333;
    }
    QComboBox QLineEdit {
        background: transparent;
        border: none;
        color: #333;
        margin-left: 10px;
        margin-right: 10px;
    }
    QComboBox::drop-down {
        subcontrol-origin: padding;
        subcontrol-position: center right;
        width: 50px;
        border-left: 1px solid #E0E0E0;
        background: transparent;
    }
    QComboBox::down-arrow {
        image: url(/Users/macbook/Documents/asagi.png);
        width: 45px;
        height: 45px;

        margin-right: 5px;
    }
    QComboBox QAbstractItemView {
        background-color: white;
        border: 1px solid #D0D0D0;
        border-radius: 15px;
        padding: 5px;
        outline: 0;
        margin-top: 5px;
    }
    QComboBox QAbstractItemView::item {
        min-height: 30px;
        padding: 2px 10px;
        border-radius: 8px;
        margin: 2px 0px;
        color: #333;
    }
    QComboBox QAbstractItemView::item:selected {
        background-color: #F0F0F0;
        color: black;
        font-weight: bold;
    }
)";

const QString STIL_SPINBOX_MODERN = R"(
    QSpinBox, QDoubleSpinBox {
        background-color: white;
        border: 1px solid #E0E0E0;
        border-radius: 25px;
        padding-left: 15px;
        padding-right: 15px;
        font-size: 15px;
        color: #757575;
        min-height: 50px;
        max-height: 50px;
        min-width: 80px;
    }
    QSpinBox::up-button, QDoubleSpinBox::up-button {
        background: transparent;
        width: 25px;
    }
    QSpinBox::down-button, QDoubleSpinBox::down-button {
        background: transparent;
        width: 25px;
    }
)";

const QString STIL_OVERLAY_BTN = R"(
    QPushButton {
        background-color: white;
        color: black;
        border-radius: 20px;
        font-weight: bold;
        font-size: 14px;
        border: 1px solid #CCC;
        min-height: 40px;
        max-height: 40px;
    }
    QPushButton:hover { background-color: #F0F0F0; }
)";

const QString STIL_KART = R"(
    QFrame {
        background-color: #F9F9F9;
        border-radius: 15px;
        border: 1px solid #EEEEEE;
    }
)";

const QString STIL_MENU = R"(
    QMenu {
        background-color: white;
        border: 1px solid #D0D0D0;
        border-radius: 15px;
        padding: 5px;
        font-size: 15px;
        color: #333;
    }
    QMenu::item {
        padding: 8px 20px;
        border-radius: 10px;

        margin: 2px 5px;
    }
    QMenu::item:selected {
        background-color: #F0F0F0;
        color: black;
        font-weight: bold;
    }
    QMenu::separator {
        height: 1px;
        background: #E0E0E0;
        margin: 5px 15px;
    }
)";

const QString STIL_COMPLETER = R"(
    QListView {
        background-color: white;
        border: 1px solid #CCC;
        border-radius: 10px;
        font-size: 16px;
        outline: 0;
        color: #333;
    }
    QListView::item {
        padding: 10px;
    }
    QListView::item:selected, QListView::item:hover {
        background-color: #E0E0E0;
        color: black;
        border-radius: 5px;
        border-bottom: none;
    }
)";

QString tr_duzelt(const char* veri) {
    if (!veri) return "";
    return QString::fromUtf8(veri);
}

QString trNormalize(const QString& s) {
    QString r = s;
    r = r.replace("İ", "i");
    r = r.replace("I", "ı");
    return r.toLower(); 
}

class KitapKarti : public QFrame {
    Q_OBJECT
public:
    int kitapID;

    KitapKarti(int id, QString baslik, QString yazar, int yil, double puan, char** etiketler, int etiket_sayisi) {
        QLocale tr(QLocale::Turkish, QLocale::Turkey);

        this->kitapID = id;

        this->setFixedHeight(140);
        this->setStyleSheet(STIL_KART);
        this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(25, 20, 25, 20);
        mainLayout->setSpacing(5);

        if(baslik.isEmpty()) baslik = "İsimsiz Kitap";
        if(yazar.isEmpty()) yazar = "Yazar Bilinmiyor";

        QLabel* lblBaslik = new QLabel(baslik);
        lblBaslik->setStyleSheet("font-family: Arial; font-size: 20px; font-weight: bold; color: #222; background: transparent; border: none;");
        lblBaslik->setWordWrap(false);

        QLabel* lblDetay = new QLabel(yazar + QString("  •  %1").arg(yil));
        lblDetay->setStyleSheet("font-family: Arial; font-size: 15px; color: #666; font-weight: 500; background: transparent; border: none;");

        mainLayout->addWidget(lblBaslik);
        mainLayout->addSpacing(8);
        mainLayout->addWidget(lblDetay);

        mainLayout->addStretch();

        QHBoxLayout* bottomLayout = new QHBoxLayout();

        QString tagStr = "";
        if(etiketler && etiket_sayisi > 0) {
            QStringList tl;
            for(int i=0; i<etiket_sayisi; i++) {
                if(etiketler[i]) {
                    QString e = tr_duzelt(etiketler[i]);
                    tl << e;
                }
            }
            tagStr = tl.join(", ");
        } else {
            tagStr = "Etiket Yok";
        }

        QLabel* lblEtiket = new QLabel(tagStr);
        lblEtiket->setStyleSheet("color: #1976D2; font-style: italic; font-weight: 500; font-size: 13px; background: transparent; border: none;");

        QLabel* lblPuan = new QLabel(QString("★ %1").arg(puan, 0, 'f', 1));
        lblPuan->setStyleSheet("color: #FBC02D; font-weight: bold; font-size: 18px; background: transparent; border: none;");

        bottomLayout->addWidget(lblEtiket);
        bottomLayout->addStretch();
        bottomLayout->addWidget(lblPuan);

        mainLayout->addLayout(bottomLayout);

        this->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(this, &QFrame::customContextMenuRequested, [=](const QPoint &pos){
            QMenu contextMenu;
            contextMenu.setStyleSheet(STIL_MENU);
            contextMenu.addAction("Güncelle", [=](){ emit kitapGuncelleIstegi(kitapID); });
            contextMenu.addAction("Sil", [=](){ emit kitapSilIstegi(kitapID); });
            contextMenu.exec(this->mapToGlobal(pos));
        });
    }
signals:
    void kitapSilIstegi(int id);
    void kitapGuncelleIstegi(int id);
};

class FilterOverlay : public QWidget {
    Q_OBJECT
public:
    QLineEdit *leYazar, *leEtiket;
    QSpinBox *sbMinYil, *sbMaxYil;
    QDoubleSpinBox *sbMinPuan, *sbMaxPuan;
    QComboBox *cbAlgo, *cbSort, *cbDirection;
    QPushButton *btnApply, *btnReset, *btnClose;

    FilterOverlay(QWidget* parent = nullptr) : QWidget(parent) {
        this->hide();
        QHBoxLayout* mainLayout = new QHBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);

        QPushButton* dummySpace = new QPushButton();
        dummySpace->setStyleSheet("background: transparent; border: none;");
        dummySpace->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        connect(dummySpace, &QPushButton::clicked, this, &FilterOverlay::hide);
        mainLayout->addWidget(dummySpace, 1);

        QWidget* contentPanel = new QWidget();
        contentPanel->setObjectName("OverlayContent");
        contentPanel->setStyleSheet("QWidget#OverlayContent { background-color: #E0E0E0; border-radius: 0px; }");

        contentPanel->setFixedWidth(420);

        QVBoxLayout* panelLayout = new QVBoxLayout(contentPanel);
        panelLayout->setContentsMargins(40, 50, 40, 40);
        panelLayout->setSpacing(15);

        QHBoxLayout* headerLayout = new QHBoxLayout();
        QLabel* lblTitle = new QLabel("Filtrele");
        lblTitle->setAlignment(Qt::AlignCenter);
        lblTitle->setStyleSheet("font-size: 28px; font-weight: bold; color: #000; border: none;");

        btnClose = new QPushButton("✖");
        btnClose->setCursor(Qt::PointingHandCursor);
        btnClose->setStyleSheet("background: white; border-radius: 20px; color: #555; font-weight: bold; font-size: 16px; min-width: 40px; min-height: 40px; border: none;");
        connect(btnClose, &QPushButton::clicked, this, &FilterOverlay::hide);

        headerLayout->addWidget(lblTitle, 1);
        headerLayout->addWidget(btnClose);
        panelLayout->addLayout(headerLayout);
        panelLayout->addSpacing(25);

        QFormLayout* form = new QFormLayout();
        form->setVerticalSpacing(20);

        form->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        form->setHorizontalSpacing(20);

        leYazar = new QLineEdit(); leYazar->setPlaceholderText("Yazar ara..."); leYazar->setStyleSheet(STIL_INPUT_FILTRE);
        leEtiket = new QLineEdit(); leEtiket->setPlaceholderText("Etiket ara..."); leEtiket->setStyleSheet(STIL_INPUT_FILTRE);

        QHBoxLayout* yilLay = new QHBoxLayout();
        sbMinYil = new QSpinBox();
        sbMinYil->setRange(0, 2030);
        sbMinYil->setSpecialValueText("Min");
        sbMinYil->setStyleSheet(STIL_SPINBOX_MODERN);
        sbMinYil->setButtonSymbols(QAbstractSpinBox::PlusMinus);

        sbMaxYil = new QSpinBox();
        sbMaxYil->setRange(0, 2030);
        sbMaxYil->setSpecialValueText("Max");
        sbMaxYil->setStyleSheet(STIL_SPINBOX_MODERN);
        sbMaxYil->setButtonSymbols(QAbstractSpinBox::PlusMinus);

        yilLay->addWidget(sbMinYil); yilLay->addWidget(sbMaxYil);

        QHBoxLayout* puanLay = new QHBoxLayout();
        sbMinPuan = new QDoubleSpinBox();
        sbMinPuan->setRange(0, 10);
        sbMinPuan->setStyleSheet(STIL_SPINBOX_MODERN);
        sbMinPuan->setButtonSymbols(QAbstractSpinBox::PlusMinus);

        sbMaxPuan = new QDoubleSpinBox();
        sbMaxPuan->setRange(0, 10);
        sbMaxPuan->setValue(10);
        sbMaxPuan->setStyleSheet(STIL_SPINBOX_MODERN);
        sbMaxPuan->setButtonSymbols(QAbstractSpinBox::PlusMinus);

        puanLay->addWidget(sbMinPuan); puanLay->addWidget(sbMaxPuan);

        cbAlgo = new QComboBox(); cbAlgo->setStyleSheet(STIL_COMBO_MODERN);
        cbAlgo->setEditable(true); cbAlgo->lineEdit()->setReadOnly(true); cbAlgo->lineEdit()->setAlignment(Qt::AlignCenter);
        cbAlgo->addItem(" Merge Sort"); cbAlgo->addItem(" Quick Sort");

        cbSort = new QComboBox(); cbSort->setStyleSheet(STIL_COMBO_MODERN);
        cbSort->setEditable(true); cbSort->lineEdit()->setReadOnly(true); cbSort->lineEdit()->setAlignment(Qt::AlignCenter);
        cbSort->addItem(" Akıllı Sıralama", KRITER_TF_IDF);
        cbSort->addItem(" Puana Göre", KRITER_POPULERLIK);
        cbSort->addItem(" Yıla Göre", KRITER_YIL);
        cbSort->addItem(" Yazara Göre", KRITER_YAZAR);
        cbSort->addItem(" Başlığa Göre", KRITER_BASLIK);

        cbDirection = new QComboBox(); cbDirection->setStyleSheet(STIL_COMBO_MODERN);
        cbDirection->setEditable(true); cbDirection->lineEdit()->setReadOnly(true); cbDirection->lineEdit()->setAlignment(Qt::AlignCenter);
        cbDirection->addItem(" Azalan"); cbDirection->addItem(" Artan ");

        QLabel *l1=new QLabel("Yazar:"); l1->setStyleSheet(STIL_LABEL_FILTRE); form->addRow(l1, leYazar);
        QLabel *l2=new QLabel("Etiket:"); l2->setStyleSheet(STIL_LABEL_FILTRE); form->addRow(l2, leEtiket);
        QLabel *l3=new QLabel("Yıl:"); l3->setStyleSheet(STIL_LABEL_FILTRE); form->addRow(l3, yilLay);
        QLabel *l4=new QLabel("Puan:"); l4->setStyleSheet(STIL_LABEL_FILTRE); form->addRow(l4, puanLay);
        QLabel *l5=new QLabel("Algoritma:"); l5->setStyleSheet(STIL_LABEL_FILTRE); form->addRow(l5, cbAlgo);
        QLabel *l6=new QLabel("Sıralama:"); l6->setStyleSheet(STIL_LABEL_FILTRE); form->addRow(l6, cbSort);
        QLabel *l7=new QLabel("Yön:"); l7->setStyleSheet(STIL_LABEL_FILTRE); form->addRow(l7, cbDirection);

        panelLayout->addLayout(form);
        panelLayout->addStretch();

        QHBoxLayout* btnLayout = new QHBoxLayout();
        btnApply = new QPushButton("UYGULA");
        btnApply->setCursor(Qt::PointingHandCursor);
        btnApply->setStyleSheet(STIL_OVERLAY_BTN);

        btnReset = new QPushButton("SIFIRLA");
        btnReset->setCursor(Qt::PointingHandCursor);
        btnReset->setStyleSheet(STIL_OVERLAY_BTN);

        btnLayout->addWidget(btnApply);
        btnLayout->addWidget(btnReset);
        panelLayout->addLayout(btnLayout);

        mainLayout->addWidget(contentPanel);
    }
};

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    Kutuphane* library;
    FilterOverlay* overlayPanel;

    QPushButton *btnUndo, *btnRedo, *btnSettingsIcon, *btnFilterText, *btnSearchIcon, *btnAddFab;
    QLineEdit *searchInputNormal, *inputStartBook, *inputTargetBook;
    QFrame *searchPill;
    QWidget *listContainer;
    QVBoxLayout *listLayout;
    QLabel *bfsArrowLabel;
    QCompleter *completer;
    QComboBox *bfsCriteriaCombo;

    bool isBFSMode = false;
    bool filtreAktif = false;
    QString f_yazar, f_etiket;
    int f_minYil = -1, f_maxYil = -1;
    double f_minPuan = 0, f_maxPuan = 10;
    int f_algo = 0, f_kriter = KRITER_TF_IDF, f_yon = 0;
    bool useSmartSort = false;

public:
    MainWindow() {
        library = kutuphane_olustur();
        veri_yukle_csv(library, "kitaplar.csv");

        this->setStyleSheet(STIL_MAIN_WINDOW);
        setupUI();

        overlayPanel = new FilterOverlay(this);
        setupOverlayConnections();

        updateBookList();

        setupCompleter();

        updateUndoRedoState();
    }

    void resizeEvent(QResizeEvent* event) override {
        if(overlayPanel) overlayPanel->resize(this->size());
        if(btnAddFab) {
            int m = 60;
            btnAddFab->setFixedSize(60, 60);
            btnAddFab->move(this->width() - btnAddFab->width() - m,
                            this->height() - btnAddFab->height() - m);
            btnAddFab->raise();
        }
        QMainWindow::resizeEvent(event);
    }

    void setupUI() {
        QWidget* centralWidget = new QWidget();
        centralWidget->setStyleSheet("background-color: transparent;");
        setCentralWidget(centralWidget);
        this->resize(1200, 800);
        this->setWindowTitle("Kütüphane GUI");

        QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
        mainLayout->setContentsMargins(40, 40, 40, 40);

        QWidget* contentCard = new QWidget();
        contentCard->setObjectName("ContentCard");
        contentCard->setStyleSheet(STIL_CONTENT_CARD);

        QVBoxLayout* cardLayout = new QVBoxLayout(contentCard);
        cardLayout->setContentsMargins(40, 40, 40, 0);
        cardLayout->setSpacing(25);

        QHBoxLayout* navLayout = new QHBoxLayout();
        btnUndo = new QPushButton();
        btnUndo->setCursor(Qt::PointingHandCursor);
        btnUndo->setStyleSheet(STIL_DAIRE_BTN_UNDO);
        btnUndo->setIcon(QIcon("/Users/macbook/Documents/geri.png"));
        connect(btnUndo, &QPushButton::clicked, this, &MainWindow::onUndo);

        btnRedo = new QPushButton();
        btnRedo->setCursor(Qt::PointingHandCursor);
        btnRedo->setStyleSheet(STIL_DAIRE_BTN_REDO);
        btnRedo->setIcon(QIcon("/Users/macbook/Documents/ileri.png"));
        connect(btnRedo, &QPushButton::clicked, this, &MainWindow::onRedo);

        navLayout->addWidget(btnUndo);
        navLayout->addWidget(btnRedo);
        navLayout->addStretch();
        cardLayout->addLayout(navLayout);

        searchPill = new QFrame();
        searchPill->setObjectName("SearchPill");
        searchPill->setStyleSheet(STIL_ARAMA_KAPSULU_NORMAL);
        searchPill->setFixedHeight(70);

        QHBoxLayout* pillLayout = new QHBoxLayout(searchPill);
        pillLayout->setContentsMargins(10, 0, 10, 0);
        pillLayout->setSpacing(10);

        btnSettingsIcon = new QPushButton();
        btnSettingsIcon->setIcon(QIcon("/Users/macbook/Documents/ayarlar.png"));
        btnSettingsIcon->setCursor(Qt::PointingHandCursor);
        btnSettingsIcon->setStyleSheet(STIL_DAIRE_BTN);
        connect(btnSettingsIcon, &QPushButton::clicked, this, &MainWindow::showModeMenu);

        searchInputNormal = new QLineEdit();
        searchInputNormal->setPlaceholderText("Kitap, Yazar, Etiket ara...");
        searchInputNormal->setStyleSheet(STIL_ARAMA_INPUT);
        connect(searchInputNormal, &QLineEdit::returnPressed, this, &MainWindow::onSearch);

        btnFilterText = new QPushButton("Filtrele");
        btnFilterText->setCursor(Qt::PointingHandCursor);
        btnFilterText->setStyleSheet(STIL_FILTRE_BTN);
        connect(btnFilterText, &QPushButton::clicked, this, &MainWindow::openFilterOverlay);

        inputStartBook = new QLineEdit();
        inputStartBook->setPlaceholderText("Başlangıç Kitabı");
        inputStartBook->setStyleSheet(STIL_BFS_INPUT);
        inputStartBook->setVisible(false);

        bfsArrowLabel = new QLabel();
        QPixmap pixArrow("/Users/macbook/Documents/ileri.png");
        bfsArrowLabel->setPixmap(pixArrow.scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        bfsArrowLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        bfsArrowLabel->setStyleSheet("border: none; margin: 0px;");
        bfsArrowLabel->setVisible(false);

        inputTargetBook = new QLineEdit();
        inputTargetBook->setPlaceholderText("Hedef Kitap");
        inputTargetBook->setStyleSheet(STIL_BFS_INPUT);
        inputTargetBook->setVisible(false);

        bfsCriteriaCombo = new QComboBox();
        bfsCriteriaCombo->setStyleSheet(STIL_COMBO_ROUND);
        bfsCriteriaCombo->setEditable(true);
        bfsCriteriaCombo->lineEdit()->setReadOnly(true);
        bfsCriteriaCombo->lineEdit()->setAlignment(Qt::AlignCenter);
        bfsCriteriaCombo->lineEdit()->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
        bfsCriteriaCombo->addItem("Yazar", KRITER_YAZAR);
        bfsCriteriaCombo->addItem("Yıl", KRITER_YIL);
        bfsCriteriaCombo->addItem("Etiket", KRITER_ETIKET);
        bfsCriteriaCombo->addItem("Puan", KRITER_POPULERLIK);
        bfsCriteriaCombo->addItem("Başlık", KRITER_BASLIK);
        bfsCriteriaCombo->setVisible(false);

        btnSearchIcon = new QPushButton();
        btnSearchIcon->setIcon(QIcon("/Users/macbook/Documents/arama.png"));
        btnSearchIcon->setCursor(Qt::PointingHandCursor);
        btnSearchIcon->setStyleSheet(STIL_DAIRE_BTN_SEARCH);
        connect(btnSearchIcon, &QPushButton::clicked, this, &MainWindow::onSearch);

        pillLayout->addWidget(btnSettingsIcon);
        pillLayout->addWidget(searchInputNormal, 1);

        pillLayout->addWidget(inputStartBook, 1);
        pillLayout->addWidget(bfsArrowLabel);
        pillLayout->addWidget(inputTargetBook, 1);
        pillLayout->addWidget(bfsCriteriaCombo);

        pillLayout->addWidget(btnFilterText);
        pillLayout->addWidget(btnSearchIcon);

        cardLayout->addWidget(searchPill);

        QScrollArea* scroll = new QScrollArea();
        scroll->setWidgetResizable(true);
        scroll->setStyleSheet("QScrollArea { border: none; background: transparent; } QScrollBar:vertical { width: 0px; }");

        listContainer = new QWidget();
        listContainer->setStyleSheet("background-color: white;");
        listLayout = new QVBoxLayout(listContainer);
        listLayout->setSpacing(15);
        listLayout->setAlignment(Qt::AlignTop);
        listLayout->setContentsMargins(0,0,0,100);

        scroll->setWidget(listContainer);
        cardLayout->addWidget(scroll);

        mainLayout->addWidget(contentCard);

        btnAddFab = new QPushButton("+", this);
        btnAddFab->setCursor(Qt::PointingHandCursor);
        btnAddFab->setStyleSheet(STIL_FAB_BTN);
        connect(btnAddFab, &QPushButton::clicked, this, &MainWindow::showAddBookDialog);
    }

    void setupOverlayConnections() {
        connect(overlayPanel->btnApply, &QPushButton::clicked, this, &MainWindow::applyFilters);
        connect(overlayPanel->btnReset, &QPushButton::clicked, this, &MainWindow::resetFilters);
    }

    void openFilterOverlay() {
        overlayPanel->raise();
        overlayPanel->show();
    }

    void updateBookList() {
        if(isBFSMode) return;

        DiziListe* islenecekListe = NULL;
        bool aramaListesiGecici = false;

        QString aramaMetni = searchInputNormal->text();
        QByteArray ba_arama = aramaMetni.toUtf8();

        // 1. Arama Mantığı
        if (aramaMetni.isEmpty()) {
            islenecekListe = library->tum_kitaplar;
        } else {
            if (useSmartSort) {
                islenecekListe = tf_idf_arama_listesi(library, ba_arama.constData());
            } else {
                islenecekListe = trie_ara(library->trie, ba_arama.constData());
                if (!islenecekListe || islenecekListe->adet == 0) {
                    if(islenecekListe) diziliste_sil(islenecekListe);
                    islenecekListe = tf_idf_arama_listesi(library, ba_arama.constData());
                }
            }
            aramaListesiGecici = true;
        }

        DiziListe* filtrelenmisListe = NULL;
        bool filtreListesiGecici = false;

        QByteArray ba_f_yazar = f_yazar.toUtf8();
        QByteArray ba_f_etiket = f_etiket.toUtf8();

        if (filtreAktif) {
            if (islenecekListe != NULL) {
                filtrelenmisListe = detayli_filtrele(
                    islenecekListe,
                    f_minYil,
                    f_maxYil,
                    f_yazar.isEmpty() ? NULL : ba_f_yazar.constData(),
                    NULL,
                    f_etiket.isEmpty() ? NULL : ba_f_etiket.constData(),
                    (float)f_minPuan,
                    (float)f_maxPuan
                    );
                filtreListesiGecici = true;
            }
        } else {
            filtrelenmisListe = islenecekListe;
        }
        if (filtrelenmisListe && filtrelenmisListe->veri && filtrelenmisListe->adet > 1 && f_kriter != KRITER_TF_IDF) {
            if (f_algo == 1) quicksort_kitap(filtrelenmisListe->veri, 0, filtrelenmisListe->adet - 1, f_kriter, f_yon);
            else mergesort_kitap(filtrelenmisListe->veri, 0, filtrelenmisListe->adet - 1, f_kriter, f_yon);
        }

        QLayoutItem* item;
        while ((item = listLayout->takeAt(0)) != nullptr) {
            if(item->widget()) delete item->widget();
            delete item;
        }

        if (filtrelenmisListe && filtrelenmisListe->adet > 0) {
            if(filtrelenmisListe->veri != NULL) {
                for (int i = 0; i < filtrelenmisListe->adet; i++) {
                    Kitap* k = filtrelenmisListe->veri[i];
                    if (k != NULL && k->id >= 0) {
                        addBookCard(k);
                    }
                }
            }
        }
        else if (filtreAktif || !aramaMetni.isEmpty()) {
            QLabel* info = new QLabel("Sonuç bulunamadı.");
            info->setAlignment(Qt::AlignCenter);
            info->setStyleSheet("color: #999; font-size: 16px; margin-top: 20px; font-weight: bold;");
            listLayout->addWidget(info);
        }

        if (filtreListesiGecici && filtrelenmisListe != islenecekListe && filtrelenmisListe != NULL) {
            diziliste_sil(filtrelenmisListe);
        }
        if (aramaListesiGecici && islenecekListe != library->tum_kitaplar && islenecekListe != NULL) {
            diziliste_sil(islenecekListe);
        }

        updateUndoRedoState();
    }

    void applyFilters() {
        filtreAktif = true;
        f_yazar = overlayPanel->leYazar->text();
        f_etiket = overlayPanel->leEtiket->text();
        f_minYil = (overlayPanel->sbMinYil->value() == 0) ? -1 : overlayPanel->sbMinYil->value();
        f_maxYil = (overlayPanel->sbMaxYil->value() == 0) ? -1 : overlayPanel->sbMaxYil->value();
        f_minPuan = overlayPanel->sbMinPuan->value();
        f_maxPuan = overlayPanel->sbMaxPuan->value();
        f_algo = overlayPanel->cbAlgo->currentIndex();
        f_kriter = overlayPanel->cbSort->currentData().toInt();
        f_yon = overlayPanel->cbDirection->currentIndex();

        if(f_kriter == KRITER_TF_IDF) useSmartSort = true; else useSmartSort = false;

        updateBookList();
    }

    void resetFilters() {
        filtreAktif = false;
        overlayPanel->leYazar->clear();
        overlayPanel->leEtiket->clear();
        overlayPanel->sbMinYil->setValue(0);
        overlayPanel->sbMaxYil->setValue(0);
        overlayPanel->sbMinPuan->setValue(0);
        overlayPanel->sbMaxPuan->setValue(10);

        f_kriter = KRITER_TF_IDF; useSmartSort = false;
        overlayPanel->cbSort->setCurrentIndex(0);

        updateBookList();
    }

    void showModeMenu() {
        QMenu menu;
        menu.setStyleSheet(STIL_MENU);
        QAction* actNormal = menu.addAction("Normal Arama / Listeleme");
        QAction* actBFS = menu.addAction("BFS (Bağlantı Bulma)");

        actNormal->setCheckable(true); actNormal->setChecked(!isBFSMode);
        actBFS->setCheckable(true); actBFS->setChecked(isBFSMode);

        connect(actNormal, &QAction::triggered, [=](){ isBFSMode = false; updateModeUI(); });
        connect(actBFS, &QAction::triggered, [=](){ isBFSMode = true; updateModeUI(); });
        menu.exec(QCursor::pos());
    }

    void updateModeUI() {
        searchInputNormal->setVisible(!isBFSMode);
        btnFilterText->setVisible(!isBFSMode);

        inputStartBook->setVisible(isBFSMode);
        bfsArrowLabel->setVisible(isBFSMode);
        inputTargetBook->setVisible(isBFSMode);
        bfsCriteriaCombo->setVisible(isBFSMode);

        if(isBFSMode) {
            searchPill->setStyleSheet(STIL_ARAMA_KAPSULU_BFS);
            QLayoutItem* item; while ((item = listLayout->takeAt(0)) != nullptr) { delete item->widget(); delete item; }
        } else {
            searchPill->setStyleSheet(STIL_ARAMA_KAPSULU_NORMAL);
            updateBookList();
        }
    }

    void onSearch() {
        if(isBFSMode) {
            QString startTxt = inputStartBook->text();
            QString targetTxt = inputTargetBook->text();

            QByteArray ba_start = startTxt.toUtf8();
            QByteArray ba_target = targetTxt.toUtf8();

            Kitap* k1 = kitap_bul_isimle(library, ba_start.constData());
            Kitap* k2 = kitap_bul_isimle(library, ba_target.constData());

            QLayoutItem* item;
            while ((item = listLayout->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }

            if(!k1 || !k2) {
                QLabel* err = new QLabel("Kitaplar bulunamadı! Tam isimleri kontrol edin.");
                err->setStyleSheet("color: #D32F2F; font-size: 16px; font-weight: bold; margin-top: 20px;");
                err->setAlignment(Qt::AlignCenter);
                listLayout->addWidget(err);
                return;
            }

            int kriter = bfsCriteriaCombo->currentData().toInt();
            char* sonuc = en_kisa_yol_bfs_str(library, k1->id, k2->id, kriter);
            QString res = QString::fromUtf8(sonuc);
            free(sonuc);

            if(res.contains("BAĞLANTI YOLU BULUNDU")) {
                QStringList lines = res.split("\n", Qt::SkipEmptyParts);
                for(int i = 0; i < lines.size(); i++) {
                    QString line = lines[i].trimmed();
                    if(line.contains("BAĞLANTI") || line.contains("⬇")) continue;

                    QByteArray ba_line = line.toUtf8();
                    Kitap* k = kitap_bul_isimle(library, ba_line.constData());

                    if(k) {
                        addBookCard(k);
                        if(i < lines.size() - 1) {
                            QLabel* arrow = new QLabel();
                            QPixmap pixArrow("/Users/macbook/Documents/asagi.png");
                            arrow->setPixmap(pixArrow.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                            arrow->setAlignment(Qt::AlignCenter);
                            arrow->setStyleSheet("border: none; background: transparent;");
                            listLayout->addWidget(arrow);
                        }
                    }
                }
            } else {
                QLabel* info = new QLabel(res);
                info->setWordWrap(true);
                info->setAlignment(Qt::AlignCenter);
                info->setStyleSheet("font-size: 16px; padding: 20px; color: #555;");
                listLayout->addWidget(info);
            }
        } else {
            updateBookList();
        }
    }

    void setupCompleter() {
        QStringList normList;

        for(int i=0; i<library->tum_kitaplar->adet; i++) {
            if(library->tum_kitaplar->veri[i]->baslik) {
                QString ham = tr_duzelt(library->tum_kitaplar->veri[i]->baslik);
                normList << ham;  
            }
        }

        completer = new QCompleter(normList, this);

        completer->setCaseSensitivity(Qt::CaseInsensitive);

        QAbstractItemView* popup = completer->popup();
        popup->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
        if(QFrame* frame = qobject_cast<QFrame*>(popup)) {
            frame->setFrameShape(QFrame::NoFrame);
        }
        popup->setStyleSheet(STIL_COMPLETER);

        searchInputNormal->setCompleter(completer);
        inputStartBook->setCompleter(completer);
        inputTargetBook->setCompleter(completer);

        connect(searchInputNormal, &QLineEdit::textEdited, this, [=](const QString& text){
            completer->setCompletionPrefix(trNormalize(text));
        });

        connect(inputStartBook, &QLineEdit::textEdited, this, [=](const QString& text){
            completer->setCompletionPrefix(trNormalize(text));
        });

        connect(inputTargetBook, &QLineEdit::textEdited, this, [=](const QString& text){
            completer->setCompletionPrefix(trNormalize(text));
        });
    }

    void updateUndoRedoState() {
        btnUndo->setEnabled(library->undo_stack->ust != NULL);
        btnRedo->setEnabled(library->redo_stack->ust != NULL);
    }

    void onUndo() { islemi_geri_al(library); updateBookList(); updateUndoRedoState(); setupCompleter(); }
    void onRedo() { islemi_ileri_al(library); updateBookList(); updateUndoRedoState(); setupCompleter(); }

    void deleteBook(int id) {
        if(QMessageBox::question(this,"Sil","Emin misin?") == QMessageBox::Yes) {
            kutuphane_kitap_sil(library, id);
            updateBookList();
            updateUndoRedoState();
            setupCompleter();
        }
    }

    void showAddBookDialog() {
        QDialog d(this);
        d.setWindowTitle("Yeni Kitap Ekle");
        d.setStyleSheet("background-color: white;");

        QFormLayout* f = new QFormLayout(&d);
        f->setVerticalSpacing(15);
        f->setHorizontalSpacing(20);

        QString labelStyle = "font-size: 16px; font-weight: bold; color: #333;"
                             "min-height: 50px; max-height: 50px;"
                             "qproperty-alignment: 'AlignVCenter | AlignLeft';";

        QLineEdit* l1 = new QLineEdit(); l1->setStyleSheet(STIL_INPUT_FILTRE);
        l1->setPlaceholderText("Kitap adı giriniz...");

        QLineEdit* l2 = new QLineEdit(); l2->setStyleSheet(STIL_INPUT_FILTRE);
        l2->setPlaceholderText("Yazar adı giriniz...");

        QSpinBox* l3 = new QSpinBox(); l3->setRange(0,2030); l3->setValue(2023); l3->setStyleSheet(STIL_SPINBOX_MODERN);
        l3->setButtonSymbols(QAbstractSpinBox::PlusMinus);

        QLineEdit* l_tags = new QLineEdit();
        l_tags->setPlaceholderText("Virgülle ayırın (Örn: Roman, Bilim Kurgu)");
        l_tags->setStyleSheet(STIL_INPUT_FILTRE);

        QDoubleSpinBox* l4 = new QDoubleSpinBox(); l4->setMaximum(10); l4->setValue(5); l4->setStyleSheet(STIL_SPINBOX_MODERN);
        l4->setButtonSymbols(QAbstractSpinBox::PlusMinus);

        QLabel* lab1 = new QLabel("Başlık:"); lab1->setStyleSheet(labelStyle);
        f->addRow(lab1, l1);

        QLabel* lab2 = new QLabel("Yazar:"); lab2->setStyleSheet(labelStyle);
        f->addRow(lab2, l2);

        QLabel* lab3 = new QLabel("Yıl:"); lab3->setStyleSheet(labelStyle);
        f->addRow(lab3, l3);

        QLabel* labTags = new QLabel("Etiketler:"); labTags->setStyleSheet(labelStyle);
        f->addRow(labTags, l_tags);

        QLabel* lab4 = new QLabel("Puan:"); lab4->setStyleSheet(labelStyle);
        f->addRow(lab4, l4);

        QPushButton* b = new QPushButton("KAYDET");
        b->setCursor(Qt::PointingHandCursor);
        b->setStyleSheet("background-color: #333; "
                         "color: white; "
                         "border-radius: 25px; "
                         "min-height: 50px; "
                         "font-weight: bold; "
                         "font-size: 16px; "
                         "border: none; "
                         "margin-top: 20px;");
        f->addRow(b);

        connect(b, &QPushButton::clicked, [&](){
            if(l1->text().trimmed().isEmpty() || l2->text().trimmed().isEmpty()) {
                QMessageBox::warning(&d, "Eksik Bilgi", "Lütfen Kitap Başlığı ve Yazar adını giriniz.");
                return;
            }

            QByteArray ba_baslik = l1->text().toUtf8();
            QByteArray ba_yazar = l2->text().toUtf8();

            char* etiketler[20]; int etiket_sayisi = 0;
            QStringList parts = l_tags->text().split(',', Qt::SkipEmptyParts);
            QByteArrayList bytes;
            for(const QString& s : parts) bytes.append(s.trimmed().toUtf8());
            for(int i=0; i<bytes.size() && i<20; i++) {
                etiketler[i] = (char*)bytes[i].constData();
                etiket_sayisi++;
            }
            kutuphane_yeni_kitap_ekle(library, ba_baslik.constData(), ba_yazar.constData(), "-", l3->value(), etiketler, etiket_sayisi, (float)l4->value());
            updateBookList(); updateUndoRedoState(); setupCompleter(); d.accept();
        });
        d.exec();
    }

    void showUpdateBookDialog(int id) {
        Kitap* k = id_ile_kitap_getir(library, id);
        if(!k) return;

        QDialog d(this);
        d.setWindowTitle("Kitap Güncelle");
        d.setStyleSheet("background-color: white;");

        QFormLayout* f = new QFormLayout(&d);
        f->setVerticalSpacing(15);
        f->setHorizontalSpacing(20);

        QString labelStyle = "font-size: 16px; font-weight: bold; color: #333;"
                             "min-height: 50px; max-height: 50px;"
                             "qproperty-alignment: 'AlignVCenter | AlignLeft';";

        QLineEdit* l1 = new QLineEdit(tr_duzelt(k->baslik));
        l1->setStyleSheet(STIL_INPUT_FILTRE);

        QLineEdit* l2 = new QLineEdit(tr_duzelt(k->yazar));
        l2->setStyleSheet(STIL_INPUT_FILTRE);

        QSpinBox* l3 = new QSpinBox();
        l3->setRange(0, 2030);
        l3->setValue(k->yil);
        l3->setStyleSheet(STIL_SPINBOX_MODERN);
        l3->setButtonSymbols(QAbstractSpinBox::PlusMinus);

        QString tagsStr;
        if(k->etiketler) {
            QStringList tl;
            for(int i=0; i<k->etiket_sayisi; i++) {
                if(k->etiketler[i]) tl << tr_duzelt(k->etiketler[i]);
            }
            tagsStr = tl.join(", ");
        }
        QLineEdit* l_tags = new QLineEdit(tagsStr);
        l_tags->setStyleSheet(STIL_INPUT_FILTRE);

        QDoubleSpinBox* l4 = new QDoubleSpinBox();
        l4->setMaximum(10);
        l4->setValue(k->pop);
        l4->setStyleSheet(STIL_SPINBOX_MODERN);
        l4->setButtonSymbols(QAbstractSpinBox::PlusMinus);

        QLabel* lab1 = new QLabel("Başlık:"); lab1->setStyleSheet(labelStyle);
        f->addRow(lab1, l1);
        QLabel* lab2 = new QLabel("Yazar:"); lab2->setStyleSheet(labelStyle);
        f->addRow(lab2, l2);
        QLabel* lab3 = new QLabel("Yıl:"); lab3->setStyleSheet(labelStyle);
        f->addRow(lab3, l3);
        QLabel* labTags = new QLabel("Etiketler:"); labTags->setStyleSheet(labelStyle);
        f->addRow(labTags, l_tags);
        QLabel* lab4 = new QLabel("Puan:"); lab4->setStyleSheet(labelStyle);
        f->addRow(lab4, l4);

        QPushButton* b = new QPushButton("GÜNCELLE");
        b->setCursor(Qt::PointingHandCursor);
        b->setStyleSheet("background-color: #1976D2; color: white; border-radius: 25px; min-height: 50px; font-weight: bold; font-size: 16px; border: none; margin-top: 20px;");
        f->addRow(b);

        connect(b, &QPushButton::clicked, [&](){
            if(l1->text().trimmed().isEmpty() || l2->text().trimmed().isEmpty()) {
                QMessageBox::warning(&d, "Eksik Bilgi", "Lütfen Kitap Başlığı ve Yazar adını giriniz.");
                return;
            }

            QByteArray ba_baslik = l1->text().toUtf8();
            QByteArray ba_yazar = l2->text().toUtf8();

            char* etiketler[20];
            int etiket_sayisi = 0;
            QStringList parts = l_tags->text().split(',', Qt::SkipEmptyParts);

            QByteArrayList bytes;
            for(const QString& s : parts) {
                bytes.append(s.trimmed().toUtf8());
            }

            for(int i=0; i<bytes.size() && i<20; i++) {
                etiketler[i] = (char*)bytes[i].constData();
                etiket_sayisi++;
            }

            Kitap* yeni_veri = kitap_olustur(0, ba_baslik.constData(), ba_yazar.constData(), "-", l3->value(), etiketler, etiket_sayisi, (float)l4->value());

            kutuphane_kitap_guncelle(library, id, yeni_veri);

            updateBookList();
            updateUndoRedoState();
            setupCompleter();
            d.accept();
        });

        d.exec();
    }

    void addBookCard(Kitap* k) {
        if(!k) return;
        QString t = tr_duzelt(k->baslik);
        QString y = tr_duzelt(k->yazar);

        if (t.isEmpty()) t = "İsimsiz Kitap";
        if (y.isEmpty()) y = "Yazar Bilinmiyor";

        KitapKarti* kart = new KitapKarti(k->id, t, y, k->yil, k->pop, k->etiketler, k->etiket_sayisi);
        connect(kart, &KitapKarti::kitapSilIstegi, this, &MainWindow::deleteBook);
        connect(kart, &KitapKarti::kitapGuncelleIstegi, this, &MainWindow::showUpdateBookDialog);
        listLayout->addWidget(kart);
    }

    void closeEvent(QCloseEvent *event) override {
        veri_kaydet_csv(library, "kitaplar.csv");
        event->accept();
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QFont font("Arial", 10);
    QLocale::setDefault(QLocale(QLocale::Turkish, QLocale::Turkey));
    app.setFont(font);
    MainWindow w;
    w.show();
    return app.exec();
}

#include "main.moc"
