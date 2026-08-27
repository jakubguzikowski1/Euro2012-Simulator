/**
 * @file Mecz.h
 * @brief Deklaracja klasy Mecz realizujacej symulacje meczowa.
 *
 * Implementuje "petle meczowa" iterujaca przez 90 minut, generujaca
 * obiekty ZdarzenieMeczowe i przetwarzajaca je polimorficznie.
 *
 * UWAGA: Brak zaleznosci od Qt.
 *
 * @author Projekt zaliczeniowy - Programowanie 2
 */
#pragma once
#include "Druzyna.h"
#include "ZdarzenieMeczowe.h"
#include <memory>
#include <string>
#include <vector>

/**
 * @brief Klasa reprezentujaca pojedynczy mecz pilkarski.
 *
 * Przechowuje referencje do dwoch druzyn, wynik oraz liste
 * polimorficznych zdarzen meczowych. Glowna logika symulacji
 * zawarta jest w metodzie symuluj().
 */
class Mecz {
public:
    /**
     * @brief Konstruktor meczu.
     * @param gospodarz Referencja do druzyny domowej.
     * @param gosc      Referencja do druzyny gostki.
     * @throws MeczWyjatek Jesli druzyna nie przejdzie walidacji.
     */
    Mecz(Druzyna& gospodarz, Druzyna& gosc);
    ~Mecz() = default;

    Mecz(const Mecz&)            = delete;
    Mecz& operator=(const Mecz&) = delete;

    /**
     * @brief Symuluje mecz – glowna petla meczowa (90 minut).
     *
     * Dla kazdej minuty losuje zdarzenie (Strzal, Faul, Kartonik,
     * RzutRozny, Zmiana), tworzy odpowiedni obiekt pochodny
     * ZdarzenieMeczowe, wywoluje wykonaj() (polimorficznie)
     * i aktualizuje wynik meczu na podstawie zwroconego EfektZdarzenia.
     *
     * @throws MeczWyjatek Jesli mecz zostal juz rozegrany.
     */
    void symuluj();

    // ── Gettery wynikow ──────────────────────────────────────────────────
    int         getBramkiGospodarza() const;
    int         getBramkiGoscia()     const;
    bool        czyRozegrany()        const;
    bool        czyRemis()            const;

    /**
     * @brief Zwraca zwyciezce meczu lub nullptr przy remisie.
     */
    Druzyna*    getZwyciezca();

    const Druzyna& getGospodarz() const;
    const Druzyna& getGosc()      const;

    /**
     * @brief Zwraca liste polimorficznych zdarzen meczowych.
     */
    const std::vector<std::unique_ptr<ZdarzenieMeczowe>>& getZdarzenia() const;

    /** @brief Wyswietla raport z meczu (zdarzenia chronologicznie). */
    void wyswietlRaport() const;

private:
    Druzyna& m_gospodarz;  ///< Druzyna domowa.
    Druzyna& m_gosc;       ///< Druzyna gostki.

    int  m_bramkiGospodarza; ///< Gole druzyny domowej.
    int  m_bramkiGoscia;     ///< Gole druzyny gostki.
    bool m_rozegrany;        ///< Flaga: czy mecz zostal juz rozegrany.

    /** @brief Polimorficzna lista zdarzen meczowych (wlasciciel). */
    std::vector<std::unique_ptr<ZdarzenieMeczowe>> m_zdarzenia;

    // ── Metody pomocnicze generujace zdarzenia ───────────────────────────

    /**
     * @brief Generuje i przetwarza zdarzenie Strzal.
     * @param minuta         Minuta meczu.
     * @param czyGospodarz   Czy atakuje druzyna domowa.
     */
    void generujStrzal(int minuta, bool czyGospodarz);

    /**
     * @brief Generuje i przetwarza zdarzenie Faul.
     * @param minuta        Minuta meczu.
     * @param czyGospodarz  Czy faul popelnia druzyna domowa.
     */
    void generujFaul(int minuta, bool czyGospodarz);

    /**
     * @brief Generuje i przetwarza zdarzenie Kartonik.
     * @param minuta        Minuta meczu.
     * @param czyGospodarz  Czy kartke dostaje zawodnik druzyny domowej.
     */
    void generujKartonik(int minuta, bool czyGospodarz);

    /**
     * @brief Generuje i przetwarza zdarzenie RzutRozny.
     * @param minuta        Minuta meczu.
     * @param czyGospodarz  Czy rzut wykonuje druzyna domowa.
     */
    void generujRzutRozny(int minuta, bool czyGospodarz);

    /**
     * @brief Generuje i przetwarza zdarzenie Zmiana.
     * @param minuta        Minuta meczu.
     * @param czyGospodarz  Czy zmiane robi druzyna domowa.
     * @param zmGospodarz   Licznik zmian gospdoarza (modyfikowany).
     * @param zmGosc        Licznik zmian goscia (modyfikowany).
     */
    void generujZmiane(int minuta, bool czyGospodarz, int& zmGospodarz, int& zmGosc);

    /**
     * @brief Przetwarza efekt zdarzenia i aktualizuje wynik.
     * @param efekt Efekt zwrocony przez wykonaj().
     */
    void przetworzEfekt(EfektZdarzenia efekt);

    /**
     * @brief Zwraca losowego zawodnika z druzyny (bez zawieszonego).
     * @param druzyna  Druzyna.
     * @return Imie i nazwisko lub "Nieznany".
     */
    static std::string losujZawodnika(const Druzyna& druzyna);
};
