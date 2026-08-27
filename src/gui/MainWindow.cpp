/**
 * @file MainWindow.cpp
 * @brief Implementacja glownego okna Qt GUI powiazana z plikiem .ui.
 *
 * Plik zawiera implementacje interfejsu graficznego. Klasa dba o rygorystyczne
 * oddzielenie warstwy wizualnej od logiki biznesowej (Turniej), komunikujac sie
 * z nia wylacznie poprzez publiczne API i przechwytujac strumien konsoli do wyswietlania logow.
 *
 * @author Projekt zaliczeniowy - Programowanie 2
 */
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <iostream>
#include <QMessageBox>
#include <QColor>
#include <QFont>
#include <sstream>
#include <cstdlib>
#include <ctime>

// ─────────────────────────────────────────────────────────────────────────────
// Konstruktor / Destruktor
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Konstruktor glownego okna aplikacji.
 *
 * Inicjalizuje ziarno losowosci, laduje interfejs z pliku XML (.ui),
 * tworzy obiekt Turnieju, aplikuje motyw graficzny i laczy sygnaly z przyciskow do slotow.
 *
 * @param parent Wskaznik na widzet nadrzedny (domyslnie nullptr).
 */
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Inicjalizacja z pliku XML (.ui)
    ui->setupUi(this);

    // Inicjalizacja logiki biznesowej
    try {
        m_turniej.inicjalizuj();
    } catch (const EuroWyjatek& e) {
        QMessageBox::critical(this, "Blad inicjalizacji", QString::fromStdString(e.what()));
    }

    applyDarkTheme();

    // ── Połączenia sygnałów (Signals & Slots) ───────────────────────────
    connect(ui->m_btnGrupa,  &QPushButton::clicked,
            this, &MainWindow::onRozegrajFazeGrupowa);
    connect(ui->m_btnPuchar, &QPushButton::clicked,
            this, &MainWindow::onRozegrajFazePucharowa);
    connect(ui->m_grupaCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onGrupaZmieniona);

    // Poprawne rozciaganie naglowkow w tabelach (zdefiniowane w kodzie dla bezpieczenstwa)
    ui->m_tabelaGrupy->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->m_strzelcyTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
}

/**
 * @brief Destruktor glownego okna.
 * Zwalnia pamiec zajmowana przez obiekt interfejsu (Ui::MainWindow).
 */
MainWindow::~MainWindow() {
    delete ui;
}

// ─────────────────────────────────────────────────────────────────────────────
// Sloty
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Slot obslugujacy klikniecie przycisku "Rozegraj Faze Grupowa".
 *
 * Wykonuje symulacje fazy grupowej w logice biznesowej. Przechwytuje standardowe
 * wyjscie (std::cout), formatuje je uzywajac tagow HTML (kolorowanie skladni)
 * i wyswietla wewnatrz komponentu QTextBrowser. Aktualizuje tabele.
 */
void MainWindow::onRozegrajFazeGrupowa() {
    try {
        // Przekierowanie strumienia z logiki (nie łamiemy separacji)
        std::ostringstream buf;
        std::streambuf* old = std::cout.rdbuf(buf.rdbuf());
        m_turniej.rozegrajFazeGrupowa();
        std::cout.rdbuf(old);

        // Tworzenie "eleganckiego" logu w formacie HTML
        QString htmlLog = "<html><body style='font-family: Segoe UI; color: #cbd5e1;'>";
        htmlLog += "<h2 style='color: #3b82f6; border-bottom: 1px solid #334155; padding-bottom: 5px;'>RELACJA Z FAZY GRUPOWEJ</h2>";

        QString rawText = QString::fromStdString(buf.str());

        // Zastępowanie kluczowych słów kolorowym HTML-em
        rawText.replace("GOL!", "<b style='color: #22c55e; font-size: 14px;'>⚽ GOL!</b>");
        rawText.replace("BRAMKA", "<b style='color: #22c55e; font-size: 14px;'>⚽ BRAMKA</b>");
        rawText.replace("CZERWONA", "<b style='color: #ef4444;'>🟥 CZERWONA</b>");
        rawText.replace("zolta", "<b style='color: #facc15;'>🟨 zolta</b>");
        rawText.replace("Zmiana:", "<span style='color: #8b5cf6;'>🔄 Zmiana:</span>");
        rawText.replace("=== Grupa", "<br><h3 style='color: #fbbf24; margin-top: 20px;'>=== Grupa");
        rawText.replace("Tabela:", "</h3>");
        rawText.replace("\n", "<br>");

        ui->m_logMeczy->setHtml(htmlLog + rawText + "</body></html>");

    } catch (const EuroWyjatek& e) {
        QMessageBox::warning(this, "Blad", QString::fromStdString(e.what()));
        return;
    }

    // Aktualizacja stanu przycisków
    ui->m_btnGrupa->setEnabled(false);
    ui->m_btnGrupa->setText("✔  FAZA GRUPOWA ZAKOŃCZONA");
    ui->m_btnPuchar->setEnabled(true);

    // Odświeżenie widoków tabel
    odswiezTabelaGrupy(ui->m_grupaCombo->currentIndex());
    odswiezStrzelcy();
}

/**
 * @brief Slot obslugujacy klikniecie przycisku "Rozegraj Faze Pucharowa".
 *
 * Symuluje mecze drabinki pucharowej. Podobnie jak faza grupowa, przechwytuje
 * std::cout i dokleja pokolorowany raport na gore istniejacego logu meczy.
 * Na koniec pobiera i wyswietla zwyciezce turnieju.
 */
void MainWindow::onRozegrajFazePucharowa() {
    try {
        std::ostringstream buf;
        std::streambuf* old = std::cout.rdbuf(buf.rdbuf());
        m_turniej.rozegrajFazePucharowa();
        std::cout.rdbuf(old);

        std::string mistrz = m_turniej.getMistrz();

        // Elegancki napis wygranej
        ui->m_mistrzLabel->setStyleSheet("color: #fbbf24; background-color: #1e293b; border-radius: 10px; padding: 15px; border: 2px solid #fbbf24;");
        ui->m_mistrzLabel->setText("🏆 MISTRZ EURO 2012: " + QString::fromStdString(mistrz).toUpper() + " 🏆");

        // Dodanie logów z fazy pucharowej na początek okna logów
        QString htmlLog = "<br><h2 style='color: #a855f7; border-bottom: 1px solid #334155; padding-bottom: 5px;'>FAZA PUCHAROWA</h2>";
        QString rawText = QString::fromStdString(buf.str());
        rawText.replace("GOL!", "<b style='color: #22c55e;'>⚽ GOL!</b>");
        rawText.replace("CZERWONA", "<b style='color: #ef4444;'>🟥 CZERWONA</b>");
        rawText.replace("\n", "<br>");

        QString currentHtml = ui->m_logMeczy->toHtml();
        ui->m_logMeczy->setHtml(currentHtml + htmlLog + rawText);

    } catch (const EuroWyjatek& e) {
        QMessageBox::warning(this, "Blad", QString::fromStdString(e.what()));
        return;
    }

    ui->m_btnPuchar->setEnabled(false);
    ui->m_btnPuchar->setText("✔  TURNIEJ ZAKOŃCZONY");
    odswiezStrzelcy();
}

/**
 * @brief Slot wywolywany przy zmianie wybranej grupy w QComboBox.
 * @param index Indeks wybranej grupy (0 dla A, 1 dla B, itd.).
 */
void MainWindow::onGrupaZmieniona(int index) {
    if (m_turniej.czyFazaGrupowaRozegrana())
        odswiezTabelaGrupy(index);
}

// ─────────────────────────────────────────────────────────────────────────────
// Odswiezanie widoków
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Pobiera dane z logiki i rysuje tabele dla wybranej grupy.
 *
 * Formatuje wiersze w komponencie QTableWidget. Dwie pierwsze druzyny
 * (awansujace) sa wyrozniane kolorem zlotym i pogrubieniem, a bilans bramkowy
 * kolorowany jest na zielono (dodatni) lub czerwono (ujemny).
 *
 * @param indeks Indeks grupy (0-3).
 */
void MainWindow::odswiezTabelaGrupy(int indeks) {
    std::vector<StatystykiGrupowe> tabela;
    try {
        tabela = m_turniej.getTabelaGrupy(indeks);
    } catch (const EuroWyjatek& e) {
        return;
    }

    ui->m_tabelaGrupy->setRowCount(static_cast<int>(tabela.size()));
    for (int r = 0; r < static_cast<int>(tabela.size()); r++) {
        const auto& s = tabela[r];

        // Wyroznienie druzyn awansujacych (miejsca 1 i 2)
        QString nazwaPre = (r < 2) ? "⭐ " : "   ";
        auto* nazwaItem = new QTableWidgetItem(nazwaPre + QString::fromStdString(s.nazwaDruzyny));

        if (r < 2) {
            nazwaItem->setForeground(QColor("#fbbf24")); // Złoty
            nazwaItem->setFont(QFont("Segoe UI", 10, QFont::Bold));
        }

        ui->m_tabelaGrupy->setItem(r, 0, nazwaItem);

        auto* pktItem = new QTableWidgetItem(QString::number(s.punkty));
        pktItem->setFont(QFont("Segoe UI", 10, QFont::Bold));
        ui->m_tabelaGrupy->setItem(r, 1, pktItem);

        ui->m_tabelaGrupy->setItem(r, 2, new QTableWidgetItem(QString::number(s.bramkiStrzelone)));
        ui->m_tabelaGrupy->setItem(r, 3, new QTableWidgetItem(QString::number(s.bramkiStracone)));

        // Kolorowanie bilansu bramek
        int bilans = s.roznicaBramek();
        QString bilansStr = (bilans > 0 ? "+" : "") + QString::number(bilans);
        auto* bilansItem = new QTableWidgetItem(bilansStr);
        if (bilans > 0) bilansItem->setForeground(QColor("#22c55e")); // Zielony
        else if (bilans < 0) bilansItem->setForeground(QColor("#ef4444")); // Czerwony

        ui->m_tabelaGrupy->setItem(r, 4, bilansItem);

        // Wyrównanie do środka dla cyferek
        for(int c=1; c<=4; ++c) {
            ui->m_tabelaGrupy->item(r, c)->setTextAlignment(Qt::AlignCenter);
        }
    }
}

/**
 * @brief Rysuje ranking najlepszych strzelcow w komponencie QTableWidget.
 *
 * Pobiera top 10 strzelcow turnieju z logiki. Top 3 otrzymuje odpowiadajace im
 * medale (ikony emoji) i formatowanie tekstu.
 */
void MainWindow::odswiezStrzelcy() {
    auto strzelcy = m_turniej.getTopStrzelcy(10); // Pobieramy top 10
    ui->m_strzelcyTable->setRowCount(static_cast<int>(strzelcy.size()));

    for (int r = 0; r < static_cast<int>(strzelcy.size()); r++) {
        auto* pozItem = new QTableWidgetItem(QString::number(r + 1));
        pozItem->setTextAlignment(Qt::AlignCenter);
        ui->m_strzelcyTable->setItem(r, 0, pozItem);

        auto* nazwItem = new QTableWidgetItem(QString::fromStdString(strzelcy[r].first));
        ui->m_strzelcyTable->setItem(r, 1, nazwItem);

        auto* golItem = new QTableWidgetItem(QString::number(strzelcy[r].second));
        golItem->setTextAlignment(Qt::AlignCenter);
        golItem->setFont(QFont("Segoe UI", 10, QFont::Bold));

        // Złoty but dla lidera
        if (r == 0) {
            pozItem->setText("🥇");
            nazwItem->setForeground(QColor("#fbbf24"));
            nazwItem->setFont(QFont("Segoe UI", 10, QFont::Bold));
            golItem->setForeground(QColor("#fbbf24"));
        } else if (r == 1) {
            pozItem->setText("🥈");
        } else if (r == 2) {
            pozItem->setText("🥉");
        }

        ui->m_strzelcyTable->setItem(r, 2, golItem);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Styl (CSS)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Aplikuje globalny arkusz stylow (Qt Style Sheets).
 * Nadaje calej aplikacji nowoczesny, ciemny wyglad uzywajac skladni wzorowanej na CSS.
 */
void MainWindow::applyDarkTheme() {
    setStyleSheet(R"(
        QMainWindow { background-color: #0f172a; }

        QTabWidget::pane { border: 1px solid #1e293b; background: #1e293b; border-radius: 8px; }
        QTabBar::tab {
            background: #334155; color: #94a3b8; padding: 12px 30px;
            border-top-left-radius: 8px; border-top-right-radius: 8px; margin-right: 2px;
            font-size: 13px; font-weight: bold;
        }
        QTabBar::tab:selected { background: #3b82f6; color: white; }

        QTableWidget {
            background-color: #1e293b; color: #f1f5f9; gridline-color: #334155;
            border: 1px solid #334155; selection-background-color: #3b82f6; border-radius: 8px;
            font-size: 13px;
        }
        QHeaderView::section {
            background-color: #0f172a; color: #60a5fa; font-weight: bold;
            padding: 8px; border: none; border-bottom: 2px solid #3b82f6;
        }

        QGroupBox {
            color: #60a5fa; font-weight: bold; font-size: 14px;
            border: 1px solid #334155; border-radius: 8px;
            margin-top: 15px; padding-top: 15px;
        }
        QGroupBox::title { subcontrol-origin: margin; left: 15px; padding: 0 5px; }

        QPushButton {
            background-color: #2563eb; color: white; border: none;
            border-radius: 6px; padding: 10px;
        }
        QPushButton:hover { background-color: #1d4ed8; }
        QPushButton:disabled { background-color: #1e293b; color: #475569; border: 1px solid #334155;}

        QComboBox {
            background-color: #0f172a; color: white; border: 1px solid #3b82f6;
            border-radius: 4px; padding: 6px 12px; font-weight: bold;
        }

        QTextBrowser {
            background-color: #020617; border: 1px solid #334155; border-radius: 8px;
            padding: 10px; selection-background-color: #3b82f6;
        }

        QLabel { color: #f1f5f9; }
    )");
}