/**
 * @file Zawodnik.h
 * @brief Drzewo dziedziczenia nr 1 – Zawodnicy.
 *
 * Abstrakcyjna klasa bazowa Zawodnik oraz 5 klas pochodnych:
 * Bramkarz, SrodkowyObronca, BocznyObronca, Pomocnik, Napastnik.
 * Demonstruje: hermetyzacje, abstrakcje, dziedziczenie, polimorfizm.
 *
 * UWAGA: Brak jakichkolwiek zaleznosci od Qt.
 *
 * @author Projekt zaliczeniowy - Programowanie 2
 */
#pragma once
#include "Exceptions.h"
#include <string>

// ─────────────────────────────────────────────────────────────────────────────
// Klasa bazowa (abstrakcyjna)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Abstrakcyjna klasa bazowa reprezentujaca zawodnika pilkarskiego.
 *
 * Hermetyzuje dane osobowe i wspolne statystyki. Definiuje
 * czyste metody wirtualne, ktore kazda klasa pochodna musi zaimplementowac.
 */
class Zawodnik {
public:
    /**
     * @brief Konstruktor zawodnika.
     * @param imie      Imie zawodnika.
     * @param nazwisko  Nazwisko zawodnika.
     * @param numer     Numer koszulki (1-99).
     * @param wiek      Wiek zawodnika (15-45).
     * @param szybkosc  Wspolna statystyka szybkosci (1-99).
     * @param sila      Wspolna statystyka sily fizycznej (1-99).
     * @throws ZawodnikWyjatek Jesli wartosc statystyki jest poza zakresem.
     */
    Zawodnik(const std::string& imie, const std::string& nazwisko,
             int numer, int wiek, int szybkosc, int sila);

    /** @brief Wirtualny destruktor – konieczny w hierarchii polimorficznej. */
    virtual ~Zawodnik() = default;

    // ── Gettery danych osobowych ─────────────────────────────────────────
    std::string getImie()     const;
    std::string getNazwisko() const;
    std::string getPelneNazwisko() const;
    int         getNumer()    const;
    int         getWiek()     const;
    int         getSzybkosc() const;
    int         getSila()     const;
    int         getKondycja() const;

    // ── Statystyki turniejowe ────────────────────────────────────────────
    int  getGole()        const;
    int  getZolteKartki() const;
    bool isSuspended()    const;

    void dodajGola();
    void dodajZoltaKartke();
    void dodajCzerwonaKartke();
    /** @brief Zmniejsza kondycje zawodnika (wywolywane co minute meczu). */
    void zmniejszKondycje(int o = 1);

    // ── Czyste metody wirtualne (polimorfizm) ───────────────────────────

    /**
     * @brief Wykonuje akcje charakterystyczna dla danej pozycji.
     * @return Tekstowy opis wykonanej akcji.
     */
    virtual std::string wykonajAkcje() const = 0;

    /**
     * @brief Zwraca typ zawodnika jako tekst (np. "Bramkarz").
     * @return Nazwa pozycji.
     */
    virtual std::string getTyp() const = 0;

    /**
     * @brief Zwraca ogolna ocene zawodnika (1-99), liczona z wag pozycji.
     * @return Wartosc oceny.
     */
    virtual int getOgolnaOcena() const = 0;

    /** @brief Wyswietla krotkie info o zawodniku. */
    void wyswietl() const;

protected:
    /**
     * @brief Sprawdza czy wartosc statystyki jest w dozwolonym zakresie.
     * @param wartosc Wartosc do sprawdzenia.
     * @param nazwa   Nazwa statystyki (do komunikatu bledu).
     * @throws ZawodnikWyjatek Jesli wartosc poza zakresem 1-99.
     */
    static void walidujStatystyke(int wartosc, const std::string& nazwa);

    std::string m_imie;      ///< Imie zawodnika.
    std::string m_nazwisko;  ///< Nazwisko zawodnika.
    int m_numer;             ///< Numer koszulki.
    int m_wiek;              ///< Wiek zawodnika.
    int m_szybkosc;          ///< Szybkosc (1-99).
    int m_sila;              ///< Sila fizyczna (1-99).
    int m_kondycja;          ///< Biezaca kondycja (100 na poczatku meczu).

    int  m_gole;             ///< Gole zdobyte w turnieju.
    int  m_zolteKartki;      ///< Liczba zoltych kartek.
    bool m_zawieszony;       ///< Flaga zawieszenia.
};

// ─────────────────────────────────────────────────────────────────────────────
// Klasy pochodne (5 klas)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Bramkarz – pochodna klasy Zawodnik.
 *
 * Specyficzne statystyki: refleks i gra_na_wyjsciach.
 */
class Bramkarz : public Zawodnik {
public:
    /**
     * @brief Konstruktor bramkarza.
     * @param imie             Imie.
     * @param nazwisko         Nazwisko.
     * @param numer            Numer koszulki.
     * @param wiek             Wiek.
     * @param szybkosc         Szybkosc (1-99).
     * @param sila             Sila (1-99).
     * @param refleks          Refleks bramkarski (1-99).
     * @param graNaWyjsciach   Gra na wyjsciach (1-99).
     */
    Bramkarz(const std::string& imie, const std::string& nazwisko,
             int numer, int wiek, int szybkosc, int sila,
             int refleks, int graNaWyjsciach);

    std::string wykonajAkcje() const override;
    std::string getTyp()       const override { return "Bramkarz"; }
    int         getOgolnaOcena() const override;

    int getRefleks()          const { return m_refleks; }
    int getGraNaWyjsciach()   const { return m_graNaWyjsciach; }

private:
    int m_refleks;          ///< Refleks (1-99).
    int m_graNaWyjsciach;   ///< Gra na wyjsciach (1-99).
};

// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Srodkowy obronca – pochodna klasy Zawodnik.
 *
 * Specyficzne statystyki: odbior i gra_glowa.
 */
class SrodkowyObronca : public Zawodnik {
public:
    SrodkowyObronca(const std::string& imie, const std::string& nazwisko,
                    int numer, int wiek, int szybkosc, int sila,
                    int odbior, int graGlowa);

    std::string wykonajAkcje() const override;
    std::string getTyp()       const override { return "Srodkowy Obronca"; }
    int         getOgolnaOcena() const override;

    int getOdbior()  const { return m_odbior; }
    int getGraGlowa()const { return m_graGlowa; }

private:
    int m_odbior;    ///< Skutecznosc odbioru pilki (1-99).
    int m_graGlowa;  ///< Gra glowa (1-99).
};

// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Boczny obronca – pochodna klasy Zawodnik.
 *
 * Specyficzne statystyki: drybling i wytrzymalosc.
 */
class BocznyObronca : public Zawodnik {
public:
    BocznyObronca(const std::string& imie, const std::string& nazwisko,
                  int numer, int wiek, int szybkosc, int sila,
                  int drybling, int wytrzymalosc);

    std::string wykonajAkcje() const override;
    std::string getTyp()       const override { return "Boczny Obronca"; }
    int         getOgolnaOcena() const override;

    int getDrybling()      const { return m_drybling; }
    int getWytrzymalosc()  const { return m_wytrzymalosc; }

private:
    int m_drybling;       ///< Umiejetnosc dryblowania (1-99).
    int m_wytrzymalosc;   ///< Wytrzymalosc biegowa (1-99).
};

// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Pomocnik – pochodna klasy Zawodnik.
 *
 * Specyficzne statystyki: podanie i wizja.
 */
class Pomocnik : public Zawodnik {
public:
    Pomocnik(const std::string& imie, const std::string& nazwisko,
             int numer, int wiek, int szybkosc, int sila,
             int podanie, int wizja);

    std::string wykonajAkcje() const override;
    std::string getTyp()       const override { return "Pomocnik"; }
    int         getOgolnaOcena() const override;

    int getPodanie() const { return m_podanie; }
    int getWizja()   const { return m_wizja; }

private:
    int m_podanie;  ///< Dokladnosc podania (1-99).
    int m_wizja;    ///< Wizja boiska (1-99).
};

// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Napastnik – pochodna klasy Zawodnik.
 *
 * Specyficzne statystyki: strzelanie i ustawienie.
 */
class Napastnik : public Zawodnik {
public:
    Napastnik(const std::string& imie, const std::string& nazwisko,
              int numer, int wiek, int szybkosc, int sila,
              int strzelanie, int ustawienie);

    std::string wykonajAkcje() const override;
    std::string getTyp()       const override { return "Napastnik"; }
    int         getOgolnaOcena() const override;

    int getStrzelanie()  const { return m_strzelanie; }
    int getUstawienie()  const { return m_ustawienie; }

private:
    int m_strzelanie;   ///< Skutecznosc strzelecka (1-99).
    int m_ustawienie;   ///< Ustawienie sie na boisku (1-99).
};
