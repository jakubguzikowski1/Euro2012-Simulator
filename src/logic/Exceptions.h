/**
 * @file Exceptions.h
 * @brief Hierarchia wlasnych wyjatkow projektu Euro 2012.
 *
 * Wszystkie wyjatki dziedzicza po std::exception i dostarczaja
 * czytelny komunikat przez metode what(). Brak zaleznosci od Qt.
 *
 * @author Projekt zaliczeniowy - Programowanie 2
 */
#pragma once
#include <exception>
#include <string>

/**
 * @brief Bazowy wyjatek calego projektu Euro 2012.
 *
 * Wszystkie pozostale wyjatki dziedzicza po tej klasie,
 * co pozwala na przechwycenie dowolnego bledu jednym catch.
 */
class EuroWyjatek : public std::exception {
public:
    /**
     * @brief Konstruktor z komunikatem bledu.
     * @param komunikat Opis bledu.
     */
    explicit EuroWyjatek(const std::string& komunikat)
        : m_komunikat("EuroWyjatek: " + komunikat) {}

    /** @brief Zwraca opis bledu (override z std::exception). */
    const char* what() const noexcept override {
        return m_komunikat.c_str();
    }

protected:
    std::string m_komunikat; ///< Tresc komunikatu bledu.
};

// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Wyjatek zwiazany z nieprawidlowymi danymi druzyny.
 *
 * Rzucany np. gdy druzyna ma mniej zawodnikow niz wymagane minimum.
 */
class DrużynaWyjatek : public EuroWyjatek {
public:
    explicit DrużynaWyjatek(const std::string& komunikat)
        : EuroWyjatek("Druzyna - " + komunikat) {}
};

/**
 * @brief Wyjatek zwiazany z przebiegiem meczu.
 *
 * Rzucany np. przy probie ponownego symulowania juz rozegranego meczu.
 */
class MeczWyjatek : public EuroWyjatek {
public:
    explicit MeczWyjatek(const std::string& komunikat)
        : EuroWyjatek("Mecz - " + komunikat) {}
};

/**
 * @brief Wyjatek zwiazany z nieprawidlowymi danymi zawodnika.
 *
 * Rzucany np. gdy wartosc statystyki jest poza zakresem 1-99.
 */
class ZawodnikWyjatek : public EuroWyjatek {
public:
    explicit ZawodnikWyjatek(const std::string& komunikat)
        : EuroWyjatek("Zawodnik - " + komunikat) {}
};

/**
 * @brief Wyjatek zwiazany z organizacja turnieju.
 *
 * Rzucany np. przy probie rozegrania fazy pucharowej
 * przed zakonczeniem fazy grupowej.
 */
class TurniejWyjatek : public EuroWyjatek {
public:
    explicit TurniejWyjatek(const std::string& komunikat)
        : EuroWyjatek("Turniej - " + komunikat) {}
};

/**
 * @brief Wyjatek zwiazany z nieprawidlowym zdarzeniem meczowym.
 *
 * Rzucany np. gdy zdarzenie odwoluje sie do zawodnika nullptr.
 */
class ZdarzenieWyjatek : public EuroWyjatek {
public:
    explicit ZdarzenieWyjatek(const std::string& komunikat)
        : EuroWyjatek("Zdarzenie - " + komunikat) {}
};
