/**
 * @file Turniej.h
 * @brief Deklaracja klasy Turniej – organizator Euro 2012.
 *
 * Zarzadza druzynami, grupami i faza pucharowa. Udostepnia
 * czyste API ktore GUI moze wywolywac bez lamani separacji.
 *
 * UWAGA: Brak zaleznosci od Qt.
 *
 * @author Projekt zaliczeniowy - Programowanie 2
 */
#pragma once
#include "Druzyna.h"
#include "Mecz.h"
#include "Exceptions.h"
#include <array>
#include <memory>
#include <string>
#include <vector>

/** @brief Liczba druzyn w jednej grupie. */
constexpr int DRUZYNY_W_GRUPIE = 4;

/**
 * @brief Statystyki druzyny w tabeli grupowej.
 */
struct StatystykiGrupowe {
    std::string nazwaDruzyny; ///< Nazwa druzyny.
    int punkty = 0;           ///< Liczba punktow.
    int bramkiStrzelone = 0;  ///< Gole strzelone.
    int bramkiStracone  = 0;  ///< Gole stracone.

    int roznicaBramek() const { return bramkiStrzelone - bramkiStracone; }
};

/**
 * @brief Klasa reprezentujaca grupe turniejowa (4 druzyny).
 *
 * Przechowuje wskazniki do druzyn, rozgrywa mecze systemem
 * kazdy-z-kazdym i wyznacza dwie awansujace druzyny.
 */
class Grupa {
public:
    /**
     * @brief Konstruktor grupy.
     * @param litera Litera grupy (np. 'A').
     */
    explicit Grupa(char litera);
    ~Grupa() = default;

    /**
     * @brief Dodaje druzyne do grupy (wskaznik nie-owning).
     * @throws TurniejWyjatek Jesli grupa ma juz 4 druzyny.
     */
    void dodajDruzyne(Druzyna* druzyna);

    /**
     * @brief Rozgrywa wszystkie mecze grupowe (6 meczy).
     * @throws TurniejWyjatek Jesli faza juz rozegrana lub za malo druzyn.
     */
    void symuluj();

    /**
     * @brief Zwraca posortowana tabele grupy.
     */
    std::vector<StatystykiGrupowe> getTabela() const;

    /**
     * @brief Zwraca dwie druzyny, ktore awansuja do fazy pucharowej.
     * @throws TurniejWyjatek Jesli faza nie zostala rozegrana.
     */
    std::vector<Druzyna*> getAwansujace() const;

    char   getLitera()     const { return m_litera; }
    bool   czySymulowana() const { return m_symulowana; }

    /** @brief Wyswietla wyniki meczy i tabele. */
    void wyswietl() const;

    const std::vector<std::unique_ptr<Mecz>>& getMecze() const { return m_mecze; }

private:
    char                                   m_litera;     ///< Litera grupy.
    std::vector<Druzyna*>                  m_druzyny;    ///< Nie-owning (wlasciciel: Turniej).
    std::vector<std::unique_ptr<Mecz>>     m_mecze;      ///< Rozegrane mecze (wlasciciel).
    bool                                   m_symulowana; ///< Flaga wykonania.
};

// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Klasa zarzadzajaca calym turniejem Euro 2012.
 *
 * Tworzy druzyny, grupy, przeprowadza faze grupowa i pucharowa.
 * Udostepnia czyste API uzywane zarowno przez ConsoleApp jak i QtGuiApp.
 */
class Turniej {
public:
    Turniej();
    ~Turniej() = default;

    /**
     * @brief Inicjalizuje turniej: tworzy druzyny, sklady i grupy Euro 2012.
     */
    void inicjalizuj();

    /** @brief Rozgrywa cala faze grupowa. */
    void rozegrajFazeGrupowa();

    /** @brief Rozgrywa faze pucharowa (cwiercf., polfinaly, final). */
    void rozegrajFazePucharowa();

    bool czyFazaGrupowaRozegrana()   const { return m_fazowaRozegrana; }
    bool czyFazaPucharowaRozegrana() const { return m_pucharowaRozegrana; }

    // ── API dla GUI i konsoli ────────────────────────────────────────────

    /**
     * @brief Zwraca tabele grupy (0=A, 1=B, 2=C, 3=D).
     * @throws TurniejWyjatek Jesli faza grupowa nie rozegrana.
     */
    std::vector<StatystykiGrupowe> getTabelaGrupy(int indeksGrupy) const;

    /**
     * @brief Zwraca liste nazw druzyn.
     */
    std::vector<std::string> getNazdruzyni() const;

    /**
     * @brief Zwraca top N strzelcow (wszystkie druzyny).
     */
    std::vector<std::pair<std::string, int>> getTopStrzelcy(int n) const;

    /**
     * @brief Zwraca nazwe mistrza (lub "" jesli nie wyznaczono).
     */
    std::string getMistrz() const;

    /** @brief Wyswietla raport z calego turnieju. */
    void wyswietlRaport() const;

private:
    std::vector<std::unique_ptr<Druzyna>> m_druzyny;  ///< Wlasciciel druzyn.
    std::array<Grupa, 4>                  m_grupy;    ///< Grupy A-D.
    std::vector<std::unique_ptr<Mecz>>    m_mecze_pf; ///< Mecze fazy pucharowej.

    bool        m_fazowaRozegrana;    ///< Flaga fazy grupowej.
    bool        m_pucharowaRozegrana; ///< Flaga fazy pucharowej.
    std::string m_mistrz;             ///< Nazwa mistrza.

    /**
     * @brief Tworzy druzyne z podanymi danymi i dodaje do turnieju.
     * @return Wskaznik do stworzonej druzyny.
     */
    Druzyna* stworzDruzyne(const std::string& nazwa, const std::string& kraj);

    /**
     * @brief Rozgrywa jeden mecz fazy pucharowej.
     * @param dom  Druzyna domowa.
     * @param gosc Druzyna gostki.
     * @return Zwyciezca (50/50 przy remisie – rzuty karne).
     */
    Druzyna& rozegrajMeczPucharowy(Druzyna& dom, Druzyna& gosc, const std::string& runda);

    /** @brief Buduje sklady druzyn Euro 2012. */
    void budujSklady();
};