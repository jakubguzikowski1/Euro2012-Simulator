/**
 * @file Druzyna.h
 * @brief Deklaracja klasy Druzyna reprezentujacej druzyne pilkarska.
 *
 * Druzyna jest wlascicielem swoich zawodnikow i sztabu szkoleniowego
 * (przechowywanych jako std::unique_ptr do abstrakcyjnych klas bazowych).
 *
 * UWAGA: Brak zaleznosci od Qt.
 *
 * @author Projekt zaliczeniowy - Programowanie 2
 */
#pragma once
#include "Zawodnik.h"
#include "CzlonekSztabu.h"
#include "Exceptions.h"
#include <memory>
#include <vector>
#include <string>

/** @brief Minimalna liczba zawodnikow wymagana do rozegrania meczu. */
constexpr int MIN_ZAWODNIKOW = 5;

/**
 * @brief Klasa reprezentujaca druzyne pilkarska.
 *
 * Hermetyzuje nazwe, kraj, zawodnikow i sztab. Udostepnia metody
 * obliczajace sile ataku i obrony na podstawie statystyk zawodnikow.
 */
class Druzyna {
public:
    /**
     * @brief Konstruktor druzyny.
     * @param nazwa  Pelna nazwa druzyny (np. "Polska").
     * @param kraj   Skrot ISO panstwa (np. "POL").
     * @throws DrużynaWyjatek Jesli nazwa jest pusta.
     */
    Druzyna(const std::string& nazwa, const std::string& kraj);

    Druzyna(const Druzyna&)            = delete;
    Druzyna& operator=(const Druzyna&) = delete;
    Druzyna(Druzyna&&)                 = default;
    Druzyna& operator=(Druzyna&&)      = default;
    ~Druzyna() = default;

    std::string getNazwa() const;
    std::string getKraj()  const;

    /**
     * @brief Dodaje zawodnika do skladu.
     * @param zawodnik Wlascicielstwo przekazywane do druzyny.
     */
    void dodajZawodnika(std::unique_ptr<Zawodnik> zawodnik);

    /**
     * @brief Dodaje czlonka sztabu.
     * @param czlonek Wlascicielstwo przekazywane do druzyny.
     */
    void dodajCzlonkaSztabu(std::unique_ptr<CzlonekSztabu> czlonek);

    /**
     * @brief Sprawdza czy druzyna ma wystarczajaca liczbe zawodnikow.
     * @throws DrużynaWyjatek Jesli mniej niz MIN_ZAWODNIKOW.
     */
    void waliduj() const;

    int getLiczbaZawodnikow() const;

    /**
     * @brief Zwraca wskaznik do zawodnika po indeksie (nie-owning).
     * @throws DrużynaWyjatek Jesli indeks nieprawidlowy.
     */
    Zawodnik*       getZawodnik(int index);
    const Zawodnik* getZawodnik(int index) const;

    /**
     * @brief Oblicza srednia sile ataku (napastnicy + pomocnicy).
     * @return Wartosc 1-99.
     */
    int getSilaAtaku()  const;

    /**
     * @brief Oblicza srednia sile obrony (obroncy + bramkarz).
     * @return Wartosc 1-99.
     */
    int getSilaObrony() const;

    /**
     * @brief Zwraca zawodnika o najwyzszej ocenie ofensywnej.
     * @return Wskaznik lub nullptr jesli brak.
     */
    Zawodnik* getNajlepszyAtakujacy();

    /**
     * @brief Zwraca bramkarza druzyny (lub nullptr).
     */
    Zawodnik* getBramkarz();

    /** @brief Wyswietla sklad druzyny. */
    void wyswietlSklad() const;

    /** @brief Wyswietla sztab szkoleniowy (polimorficznie). */
    void wyswietlSztab() const;

private:
    std::string m_nazwa;  ///< Nazwa druzyny.
    std::string m_kraj;   ///< Skrot kraju.

    std::vector<std::unique_ptr<Zawodnik>>      m_zawodnicy; ///< Sklad (wlasciciel).
    std::vector<std::unique_ptr<CzlonekSztabu>> m_sztab;     ///< Sztab (wlasciciel).
};
