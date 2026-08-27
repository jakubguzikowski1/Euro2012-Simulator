/**
 * @file MainWindow.h
 * @brief Glowne okno Qt GUI aplikacji Euro 2012.
 *
 * KRYTYCZNE – separacja warstw:
 *   - MainWindow UZYWA klasy Turniej przez jej publiczne API.
 *   - MainWindow NIE wnika w implementacje logiki.
 *   - Logika NIE zna Qt – komunikacja odbywa sie przez czyste API
 *     (std::string, std::vector, struktury POD).
 *
 * @author Projekt zaliczeniowy - Programowanie 2
 */
#pragma once

// ── Qt (tylko w warstwie GUI) ─────────────────────────────────────────────────
#include <QMainWindow>
#include <QTabWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QTextBrowser>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QSplitter>

// ── Logika (brak Qt w tych naglowkach) ───────────────────────────────────────
#include "Turniej.h"

// Forward declaration wygenerowanego przez uic
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief Glowne okno graficzne aplikacji Euro 2012.
 *
 * Komunikuje sie z Turniej wylacznie przez jego publiczne API:
 *  - getTabelaGrupy()    → dane do QTableWidget
 *  - getTopStrzelcy()    → dane do QTableWidget
 *  - getNazdruzyni()     → dane do QComboBox
 *  - getMistrz()         → tekst do QLabel
 *  Nie korzysta z zadnych wewnetrznych szczegolów implementacji logiki.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    /** @brief Rozgrywa faze grupowa i odswieza widoki. */
    void onRozegrajFazeGrupowa();

    /** @brief Rozgrywa faze pucharowa i odswieza widoki. */
    void onRozegrajFazePucharowa();

    /** @brief Odswieza tabele po zmianie wybranej grupy. */
    void onGrupaZmieniona(int index);

private:
    Ui::MainWindow* ui;        ///< Wskaznik do wygenerowanego UI.
    Turniej         m_turniej; ///< Obiekt logiki (brak Qt w jego kodzie).

    /** @brief Odswieza tabele grupowa dla wybranej grupy. */
    void odswiezTabelaGrupy(int indeks);

    /** @brief Odswieza tabele top strzelcow. */
    void odswiezStrzelcy();

    /** @brief Stosuje ciemny motyw graficzny. */
    void applyDarkTheme();
};
