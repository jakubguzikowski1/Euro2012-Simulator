/**
 * @file Turniej.cpp
 * @brief Implementacja klas Grupa i Turniej.
 *
 * Plik zawiera logike zarzadzania rozgrywkami: faza grupowa (system kazdy z kazdym)
 * oraz faza pucharowa (drabinka). Odpowiada takze za tworzenie historycznych skladow
 * oraz zwalnianie pamieci po obiektach Druzyna i Mecz.
 *
 * @author Projekt zaliczeniowy - Programowanie 2
 */
#include "Turniej.h"
#include "Exceptions.h"
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <memory>

// ── Grupa ────────────────────────────────────────────────────────────────

/**
 * @brief Konstruktor inicjalizujacy pusta grupe.
 * @param litera Litera identyfikujaca grupe (np. 'A', 'B').
 */
Grupa::Grupa(char litera) : m_litera(litera), m_symulowana(false) {}

/**
 * @brief Dodaje wskaznik na druzyne do grupy.
 * @param druzyna Surowy wskaznik na istniejaca druzyne (nie-owning).
 * @throws TurniejWyjatek Jesli probujemy dodac wiecej druzyn niz wynosi limit (DRUZYNY_W_GRUPIE).
 */
void Grupa::dodajDruzyne(Druzyna* druzyna) {
    if (m_druzyny.size() >= DRUZYNY_W_GRUPIE)
        throw TurniejWyjatek("Grupa " + std::string(1, m_litera) + " jest juz pelna.");
    m_druzyny.push_back(druzyna);
}

/**
 * @brief Rozgrywa wszystkie mecze w grupie systemem "kazdy z kazdym".
 * Dla 4 druzyn generuje dokladnie 6 spotkan.
 * @throws TurniejWyjatek Jesli grupa zostala juz zasymulowana lub brakuje w niej druzyn.
 */
void Grupa::symuluj() {
    if (m_symulowana)
        throw TurniejWyjatek("Grupa " + std::string(1, m_litera) + " zostala juz rozegrana.");
    if (m_druzyny.size() != DRUZYNY_W_GRUPIE)
        throw TurniejWyjatek("Grupa nie ma wystarczajacej liczby druzyn do startu.");

    std::cout << "\n=== Grupa " << m_litera << " - Wyniki Meczy ===\n";
    for (size_t i = 0; i < m_druzyny.size(); ++i) {
        for (size_t j = i + 1; j < m_druzyny.size(); ++j) {
            auto mecz = std::make_unique<Mecz>(*m_druzyny[i], *m_druzyny[j]);
            mecz->symuluj();
            mecz->wyswietlRaport();
            m_mecze.push_back(std::move(mecz));
        }
    }
    m_symulowana = true;
}

/**
 * @brief Generuje, podlicza i sortuje tabele grupy na podstawie rozegranych spotkan.
 * Zlicza punkty (3 za wygrana, 1 za remis) oraz bilans bramkowy.
 * @return Wektor struktur StatystykiGrupowe posortowany od 1 do 4 miejsca.
 */
std::vector<StatystykiGrupowe> Grupa::getTabela() const {
    std::vector<StatystykiGrupowe> tab;
    for (const auto* d : m_druzyny) {
        tab.push_back({d->getNazwa(), 0, 0, 0});
    }

    for (const auto& mecz : m_mecze) {
        if (!mecz->czyRozegrany()) continue;

        auto itDom = std::find_if(tab.begin(), tab.end(),
                                  [&](const StatystykiGrupowe& s) {
                                      return s.nazwaDruzyny == mecz->getGospodarz().getNazwa();
                                  });
        auto itGosc = std::find_if(tab.begin(), tab.end(),
                                   [&](const StatystykiGrupowe& s) {
                                       return s.nazwaDruzyny == mecz->getGosc().getNazwa();
                                   });

        if (itDom != tab.end() && itGosc != tab.end()) {
            int gDom = mecz->getBramkiGospodarza();
            int gGosc = mecz->getBramkiGoscia();

            itDom->bramkiStrzelone += gDom;
            itDom->bramkiStracone  += gGosc;
            itGosc->bramkiStrzelone += gGosc;
            itGosc->bramkiStracone  += gDom;

            if (gDom > gGosc) { itDom->punkty += 3; }
            else if (gGosc > gDom) { itGosc->punkty += 3; }
            else { itDom->punkty += 1; itGosc->punkty += 1; }
        }
    }

    // Sortowanie: Punkty -> Bilans -> Gole Strzelone
    std::sort(tab.begin(), tab.end(), [](const StatystykiGrupowe& a, const StatystykiGrupowe& b) {
        if (a.punkty != b.punkty) return a.punkty > b.punkty;
        if (a.roznicaBramek() != b.roznicaBramek()) return a.roznicaBramek() > b.roznicaBramek();
        return a.bramkiStrzelone > b.bramkiStrzelone;
    });

    return tab;
}

/**
 * @brief Zwraca dwie najlepsze druzyny z grupy.
 * @return Wektor z dwoma wskaznikami na druzyny awansujace.
 * @throws TurniejWyjatek Jesli grupa nie zostala wczesniej symulowana.
 */
std::vector<Druzyna*> Grupa::getAwansujace() const {
    if (!m_symulowana)
        throw TurniejWyjatek("Nie mozna wyznaczyc awansujacych - grupa nie rozegrana.");
    auto tab = getTabela();
    std::vector<Druzyna*> awans;
    for (int i = 0; i < 2; ++i) {
        auto it = std::find_if(m_druzyny.begin(), m_druzyny.end(),
                               [&](Druzyna* d) { return d->getNazwa() == tab[i].nazwaDruzyny; });
        if (it != m_druzyny.end()) awans.push_back(*it);
    }
    return awans;
}

/** @brief Wypisuje w konsoli posortowana tabele grupy z detalami (punkty, bramki). */
void Grupa::wyswietl() const {
    auto tab = getTabela();
    std::cout << "\n=== Tabela: Grupa " << m_litera << " ===\n";
    for (const auto& s : tab) {
        std::cout << std::left << std::setw(15) << s.nazwaDruzyny
                  << " Pkt: " << s.punkty
                  << " | G+: " << s.bramkiStrzelone
                  << " G-: " << s.bramkiStracone
                  << " Bilans: " << s.roznicaBramek() << "\n";
    }
}

// ── Turniej ──────────────────────────────────────────────────────────────

/**
 * @brief Konstruktor turnieju. Inicjalizuje strukture 4 grup (A, B, C, D).
 */
Turniej::Turniej()
    : m_grupy{Grupa('A'), Grupa('B'), Grupa('C'), Grupa('D')},
    m_fazowaRozegrana(false), m_pucharowaRozegrana(false) {}

/**
 * @brief Buduje sklady druzyn i dzieli je na odpowiednie grupy.
 * @throws TurniejWyjatek Jesli po budowaniu brakuje druzyn (mniej niz 16).
 */
void Turniej::inicjalizuj() {
    if (!m_druzyny.empty()) return; // Zabezpieczenie na wypadek podwojnego wywolania
    budujSklady();

    // Rozdzielenie do grup (po 4 na grupe)
    if (m_druzyny.size() < 16)
        throw TurniejWyjatek("Za malo druzyn do utworzenia 4 grup.");

    for (int i = 0; i < 16; ++i) {
        m_grupy[i / 4].dodajDruzyne(m_druzyny[static_cast<size_t>(i)].get());
    }
}

/**
 * @brief Uruchamia symulacje wszystkich spotkan we wszystkich grupach.
 * @throws TurniejWyjatek Jesli faza grupowa zostala juz zakonczona.
 */
void Turniej::rozegrajFazeGrupowa() {
    if (m_fazowaRozegrana)
        throw TurniejWyjatek("Faza grupowa zostala juz rozegrana.");
    for (auto& g : m_grupy) {
        g.symuluj();
        g.wyswietl();
    }
    m_fazowaRozegrana = true;
}

/**
 * @brief Rozgrywa pojedynczy mecz w fazie pucharowej i wylonienie zwyciezcy.
 * Przy remisie w regulaminowym czasie uzywa uproszczonego mechanizmu rzutow karnych.
 *
 * @param dom Referencja do druzyny gospodarzy.
 * @param gosc Referencja do druzyny gosci.
 * @param runda Nazwa rundy (np. "Cwiercfinal 1").
 * @return Referencja do druzyny, ktora wygrala i awansuje wyzej.
 */
Druzyna& Turniej::rozegrajMeczPucharowy(Druzyna& dom, Druzyna& gosc, const std::string& runda) {
    std::cout << "\n--- " << runda << " ---\n";
    auto mecz = std::make_unique<Mecz>(dom, gosc);
    mecz->symuluj();

    Druzyna* zwyciezca = mecz->getZwyciezca();
    if (!zwyciezca) { // Remis -> rzuty karne (50/50 w tym uproszczeniu)
        std::cout << "  (Remis w regulaminowym czasie! Rzuty karne...)\n";
        zwyciezca = (std::rand() % 2 == 0) ? &dom : &gosc;
    }

    mecz->wyswietlRaport();
    std::cout << "  AWANS: " << zwyciezca->getNazwa() << "\n";
    m_mecze_pf.push_back(std::move(mecz));
    return *zwyciezca;
}

/**
 * @brief Buduje drabinke pucharowa z awansujacych druzyn i rozgrywa mecze az do finalu.
 * Ustala ostatecznego zwyciezce turnieju.
 * @throws TurniejWyjatek Jesli faza grupowa jest nierozegrana lub pucharowa byla juz wykonana.
 */
void Turniej::rozegrajFazePucharowa() {
    if (!m_fazowaRozegrana)
        throw TurniejWyjatek("Najpierw rozegraj faze grupowa!");
    if (m_pucharowaRozegrana)
        throw TurniejWyjatek("Faza pucharowa juz zostala rozegrana!");

    std::vector<Druzyna*> cwiercfinalisci;
    for (const auto& g : m_grupy) {
        auto awans = g.getAwansujace();
        cwiercfinalisci.push_back(awans[0]);
        cwiercfinalisci.push_back(awans[1]);
    }

    // Cwiercfinaly (1A-2B itd.)
    std::vector<Druzyna*> polfinalisci;
    polfinalisci.push_back(&rozegrajMeczPucharowy(*cwiercfinalisci[0], *cwiercfinalisci[3], "Cwiercfinal 1"));
    polfinalisci.push_back(&rozegrajMeczPucharowy(*cwiercfinalisci[2], *cwiercfinalisci[1], "Cwiercfinal 2"));
    polfinalisci.push_back(&rozegrajMeczPucharowy(*cwiercfinalisci[4], *cwiercfinalisci[7], "Cwiercfinal 3"));
    polfinalisci.push_back(&rozegrajMeczPucharowy(*cwiercfinalisci[6], *cwiercfinalisci[5], "Cwiercfinal 4"));

    // Polfinaly
    std::vector<Druzyna*> finalisci;
    finalisci.push_back(&rozegrajMeczPucharowy(*polfinalisci[0], *polfinalisci[2], "Polfinal 1"));
    finalisci.push_back(&rozegrajMeczPucharowy(*polfinalisci[1], *polfinalisci[3], "Polfinal 2"));

    // Final
    Druzyna& mistrz = rozegrajMeczPucharowy(*finalisci[0], *finalisci[1], "FINAL EURO 2012");
    m_mistrz = mistrz.getNazwa();
    m_pucharowaRozegrana = true;
}

/**
 * @brief Zwraca tabele punktowa wskazanej grupy na potrzeby GUI.
 * @param indeksGrupy Indeks tablicy (0=A, 1=B, 2=C, 3=D).
 * @return Wektor ze statystykami grupowymi.
 * @throws TurniejWyjatek Przy probie odczytu przed rozegraniem meczy lub podaniu zlego indeksu.
 */
std::vector<StatystykiGrupowe> Turniej::getTabelaGrupy(int indeksGrupy) const {
    if (!m_fazowaRozegrana) throw TurniejWyjatek("Faza grupowa nierozegrana.");
    if (indeksGrupy < 0 || indeksGrupy > 3) throw TurniejWyjatek("Zly indeks grupy.");
    return m_grupy[indeksGrupy].getTabela();
}

/** @brief Zwraca vector nazw wszystkich druzyn bioracych udzial w turnieju. */
std::vector<std::string> Turniej::getNazdruzyni() const {
    std::vector<std::string> nazwy;
    for (const auto& d : m_druzyny) nazwy.push_back(d->getNazwa());
    return nazwy;
}

/**
 * @brief Przeszukuje sklady wszystkich druzyn tworzac ranking strzelcow.
 * @param n Liczba miejsc do zwrocenia (np. Top 5, Top 10).
 * @return Posortowany wektor par <Nazwisko Gracza, Liczba Goli>.
 */
std::vector<std::pair<std::string, int>> Turniej::getTopStrzelcy(int n) const {
    std::vector<std::pair<std::string, int>> ranking;
    for (const auto& d : m_druzyny) {
        for (int i = 0; i < d->getLiczbaZawodnikow(); ++i) {
            const Zawodnik* z = d->getZawodnik(i);
            if (z->getGole() > 0) {
                ranking.push_back({z->getPelneNazwisko() + " (" + d->getNazwa() + ")", z->getGole()});
            }
        }
    }
    std::sort(ranking.begin(), ranking.end(), [](const auto& a, const auto& b){
        return a.second > b.second;
    });
    if (ranking.size() > static_cast<size_t>(n)) ranking.resize(n);
    return ranking;
}

/** @brief Zwraca nazwe mistrza turnieju (zwraca pusty string jesli turniej jeszcze trwa). */
std::string Turniej::getMistrz() const { return m_mistrz; }

/** @brief Wypluwa w konsoli krotki raport koncowy: nazwe mistrza i liste najlepszych strzelcow. */
void Turniej::wyswietlRaport() const {
    std::cout << "\n=== RAPORT KONCOWY TURNIEJU ===\n";
    if (!m_mistrz.empty()) std::cout << "MISTRZ EURO 2012: " << m_mistrz << "\n";
    auto top = getTopStrzelcy(5);
    std::cout << "\nNajlepsi strzelcy:\n";
    for (const auto& p : top) std::cout << p.first << " : " << p.second << " goli\n";
}

/**
 * @brief Pomocnicza metoda dynamicznie alokujaca obiekt druzyny.
 * Dodaje stworzony obiekt na liste pod zarzad Turnieju.
 * @param nazwa Pelna nazwa kraju.
 * @param kraj 3-literowy kod kraju (FIFA code).
 * @return Wskaznik na swiezo zbudowana druzyne.
 */
Druzyna* Turniej::stworzDruzyne(const std::string& nazwa, const std::string& kraj) {
    auto d = std::make_unique<Druzyna>(nazwa, kraj);
    Druzyna* raw = d.get();
    m_druzyny.push_back(std::move(d));
    return raw;
}

/**
 * @brief Inicjalizuje struktury obiektow w pamieci - hardcodowane sklady druzyn Euro 2012.
 * Wypelnia wszystkie 16 druzyn autentycznymi nazwiskami z danego okresu.
 */
void Turniej::budujSklady() {
    auto dodaj = [](Druzyna* d, std::unique_ptr<Zawodnik> z) {
        d->dodajZawodnika(std::move(z));
    };

    // 1. Polska
    auto* pol = stworzDruzyne("Polska", "POL");
    dodaj(pol, std::make_unique<Bramkarz>("Wojciech", "Szczesny", 1, 22, 62, 70, 84, 72));
    dodaj(pol, std::make_unique<SrodkowyObronca>("Marcin", "Wasilewski", 4, 34, 65, 80, 78, 74));
    dodaj(pol, std::make_unique<BocznyObronca>("Lukasz", "Piszczek", 20, 27, 85, 75, 70, 80));
    dodaj(pol, std::make_unique<Pomocnik>("Jakub", "Blaszczykowski", 16, 26, 88, 60, 82, 80));
    dodaj(pol, std::make_unique<Napastnik>("Robert", "Lewandowski", 9, 23, 85, 40, 75, 88));
    pol->dodajCzlonkaSztabu(std::make_unique<TrenerGlowny>("Franciszek", "Smuda", 62, 30, "4-2-3-1", "Pressing"));

    // 2. Niemcy
    auto* nie = stworzDruzyne("Niemcy", "GER");
    dodaj(nie, std::make_unique<Bramkarz>("Manuel", "Neuer", 1, 26, 60, 85, 90, 75));
    dodaj(nie, std::make_unique<SrodkowyObronca>("Mats", "Hummels", 5, 23, 70, 88, 80, 85));
    dodaj(nie, std::make_unique<BocznyObronca>("Philipp", "Lahm", 16, 28, 85, 82, 75, 88));
    dodaj(nie, std::make_unique<Pomocnik>("Bastian", "Schweinsteiger", 7, 27, 75, 80, 88, 85));
    dodaj(nie, std::make_unique<Napastnik>("Thomas", "Muller", 13, 22, 88, 60, 85, 90));
    nie->dodajCzlonkaSztabu(std::make_unique<TrenerGlowny>("Joachim", "Low", 52, 25, "4-2-3-1", "Ofensywa"));

    // 3. Hiszpania
    auto* his = stworzDruzyne("Hiszpania", "ESP");
    dodaj(his, std::make_unique<Bramkarz>("Iker", "Casillas", 1, 31, 50, 70, 92, 80));
    dodaj(his, std::make_unique<SrodkowyObronca>("Gerard", "Pique", 3, 25, 70, 88, 80, 85));
    dodaj(his, std::make_unique<BocznyObronca>("Sergio", "Ramos", 15, 26, 85, 85, 75, 88));
    dodaj(his, std::make_unique<Pomocnik>("Andres", "Iniesta", 6, 28, 80, 70, 95, 85));
    dodaj(his, std::make_unique<Napastnik>("Fernando", "Torres", 9, 28, 85, 40, 80, 88));
    his->dodajCzlonkaSztabu(std::make_unique<TrenerGlowny>("Vicente", "del Bosque", 61, 35, "4-3-3", "Tiki-taka"));

    // 4. Portugalia
    auto* por = stworzDruzyne("Portugalia", "POR");
    dodaj(por, std::make_unique<Bramkarz>("Rui", "Patricio", 1, 24, 60, 75, 85, 75));
    dodaj(por, std::make_unique<SrodkowyObronca>("Pepe", "Ferreira", 3, 29, 80, 90, 80, 85));
    dodaj(por, std::make_unique<BocznyObronca>("Fabio", "Coentrao", 5, 24, 88, 80, 75, 85));
    dodaj(por, std::make_unique<Pomocnik>("Joao", "Moutinho", 8, 25, 80, 75, 85, 80));
    dodaj(por, std::make_unique<Napastnik>("Cristiano", "Ronaldo", 7, 27, 95, 50, 90, 98));
    por->dodajCzlonkaSztabu(std::make_unique<TrenerGlowny>("Paulo", "Bento", 43, 15, "4-3-3", "Kontratak"));

    // 5. Wlochy
    auto* ita = stworzDruzyne("Wlochy", "ITA");
    dodaj(ita, std::make_unique<Bramkarz>("Gianluigi", "Buffon", 1, 34, 50, 75, 90, 85));
    dodaj(ita, std::make_unique<SrodkowyObronca>("Giorgio", "Chiellini", 3, 27, 72, 85, 88, 85));
    dodaj(ita, std::make_unique<BocznyObronca>("Federico", "Balzaretti", 6, 30, 80, 80, 75, 82));
    dodaj(ita, std::make_unique<Pomocnik>("Andrea", "Pirlo", 21, 33, 65, 55, 95, 80));
    dodaj(ita, std::make_unique<Napastnik>("Mario", "Balotelli", 9, 21, 85, 50, 85, 90));
    ita->dodajCzlonkaSztabu(std::make_unique<TrenerGlowny>("Cesare", "Prandelli", 54, 20, "3-5-2", "Defensywa"));

    // 6. Anglia
    auto* eng = stworzDruzyne("Anglia", "ENG");
    dodaj(eng, std::make_unique<Bramkarz>("Joe", "Hart", 1, 25, 60, 80, 85, 80));
    dodaj(eng, std::make_unique<SrodkowyObronca>("John", "Terry", 26, 31, 65, 85, 80, 88));
    dodaj(eng, std::make_unique<BocznyObronca>("Ashley", "Cole", 3, 31, 82, 85, 78, 85));
    dodaj(eng, std::make_unique<Pomocnik>("Steven", "Gerrard", 4, 32, 75, 70, 90, 85));
    dodaj(eng, std::make_unique<Napastnik>("Wayne", "Rooney", 10, 26, 82, 50, 85, 90));
    eng->dodajCzlonkaSztabu(std::make_unique<TrenerGlowny>("Roy", "Hodgson", 64, 35, "4-4-2", "Zrownowazony"));

    // 7. Holandia
    auto* ned = stworzDruzyne("Holandia", "NED");
    dodaj(ned, std::make_unique<Bramkarz>("Maarten", "Stekelenburg", 1, 29, 60, 75, 80, 78));
    dodaj(ned, std::make_unique<SrodkowyObronca>("Joris", "Mathijsen", 4, 32, 65, 82, 75, 80));
    dodaj(ned, std::make_unique<BocznyObronca>("Gregory", "van der Wiel", 2, 24, 85, 78, 70, 82));
    dodaj(ned, std::make_unique<Pomocnik>("Wesley", "Sneijder", 10, 28, 78, 60, 92, 80));
    dodaj(ned, std::make_unique<Napastnik>("Robin", "van Persie", 16, 28, 82, 45, 85, 92));
    ned->dodajCzlonkaSztabu(std::make_unique<TrenerGlowny>("Bert", "van Marwijk", 60, 20, "4-2-3-1", "Ofensywa"));

    // 8. Francja
    auto* fra = stworzDruzyne("Francja", "FRA");
    dodaj(fra, std::make_unique<Bramkarz>("Hugo", "Lloris", 1, 25, 65, 80, 88, 80));
    dodaj(fra, std::make_unique<SrodkowyObronca>("Philippe", "Mexes", 5, 30, 68, 85, 78, 82));
    dodaj(fra, std::make_unique<BocznyObronca>("Patrice", "Evra", 3, 31, 82, 80, 75, 85));
    dodaj(fra, std::make_unique<Pomocnik>("Franck", "Ribery", 7, 29, 90, 60, 85, 82));
    dodaj(fra, std::make_unique<Napastnik>("Karim", "Benzema", 10, 24, 85, 45, 82, 88));
    fra->dodajCzlonkaSztabu(std::make_unique<TrenerGlowny>("Laurent", "Blanc", 46, 10, "4-3-3", "Posiadanie"));

    // 9. Grecja
    auto* gre = stworzDruzyne("Grecja", "GRE");
    dodaj(gre, std::make_unique<Bramkarz>("Michalis", "Sifakis", 13, 27, 60, 70, 75, 72));
    dodaj(gre, std::make_unique<SrodkowyObronca>("Sokratis", "Papastathopoulos", 19, 24, 70, 82, 75, 80));
    dodaj(gre, std::make_unique<BocznyObronca>("Vasilis", "Torosidis", 15, 27, 75, 78, 72, 78));
    dodaj(gre, std::make_unique<Pomocnik>("Giorgos", "Karagounis", 10, 35, 65, 65, 80, 75));
    dodaj(gre, std::make_unique<Napastnik>("Georgios", "Samaras", 7, 27, 78, 50, 70, 82));
    gre->dodajCzlonkaSztabu(std::make_unique<TrenerGlowny>("Fernando", "Santos", 57, 25, "4-2-3-1", "Defensywa"));

    // 10. Rosja
    auto* rus = stworzDruzyne("Rosja", "RUS");
    dodaj(rus, std::make_unique<Bramkarz>("Wiaczeslaw", "Malafiejew", 16, 33, 60, 75, 80, 78));
    dodaj(rus, std::make_unique<SrodkowyObronca>("Siergiej", "Ignaszewicz", 4, 32, 65, 82, 75, 82));
    dodaj(rus, std::make_unique<BocznyObronca>("Jurij", "Zyrkow", 8, 28, 80, 75, 72, 80));
    dodaj(rus, std::make_unique<Pomocnik>("Andriej", "Arszawin", 10, 31, 85, 60, 82, 75));
    dodaj(rus, std::make_unique<Napastnik>("Aleksandr", "Kierzakow", 11, 29, 82, 45, 78, 85));
    rus->dodajCzlonkaSztabu(std::make_unique<TrenerGlowny>("Dick", "Advocaat", 64, 30, "4-3-3", "Kontratak"));

    // 11. Czechy
    auto* cze = stworzDruzyne("Czechy", "CZE");
    dodaj(cze, std::make_unique<Bramkarz>("Petr", "Cech", 1, 30, 55, 80, 90, 85));
    dodaj(cze, std::make_unique<SrodkowyObronca>("Tomas", "Sivok", 6, 28, 68, 80, 75, 80));
    dodaj(cze, std::make_unique<BocznyObronca>("Theodor", "Gebre Selassie", 2, 25, 82, 75, 70, 78));
    dodaj(cze, std::make_unique<Pomocnik>("Tomas", "Rosicky", 10, 31, 75, 65, 88, 75));
    dodaj(cze, std::make_unique<Napastnik>("Milan", "Baros", 15, 30, 78, 40, 75, 82));
    cze->dodajCzlonkaSztabu(std::make_unique<TrenerGlowny>("Michal", "Bilek", 47, 10, "4-2-3-1", "Zrownowazony"));

    // 12. Szwecja
    auto* swe = stworzDruzyne("Szwecja", "SWE");
    dodaj(swe, std::make_unique<Bramkarz>("Andreas", "Isaksson", 1, 30, 60, 75, 80, 78));
    dodaj(swe, std::make_unique<SrodkowyObronca>("Olof", "Mellberg", 3, 34, 62, 85, 78, 85));
    dodaj(swe, std::make_unique<BocznyObronca>("Martin", "Olsson", 5, 24, 85, 75, 70, 78));
    dodaj(swe, std::make_unique<Pomocnik>("Kim", "Kallstrom", 9, 29, 70, 65, 82, 80));
    dodaj(swe, std::make_unique<Napastnik>("Zlatan", "Ibrahimovic", 10, 30, 82, 45, 85, 95));
    swe->dodajCzlonkaSztabu(std::make_unique<TrenerGlowny>("Erik", "Hamren", 54, 20, "4-4-2", "Ofensywa"));

    // 13. Dania
    auto* den = stworzDruzyne("Dania", "DEN");
    dodaj(den, std::make_unique<Bramkarz>("Stephan", "Andersen", 1, 30, 60, 72, 78, 75));
    dodaj(den, std::make_unique<SrodkowyObronca>("Daniel", "Agger", 4, 27, 72, 85, 80, 85));
    dodaj(den, std::make_unique<BocznyObronca>("Lars", "Jacobsen", 6, 32, 75, 80, 72, 78));
    dodaj(den, std::make_unique<Pomocnik>("Christian", "Eriksen", 8, 20, 82, 60, 88, 75));
    dodaj(den, std::make_unique<Napastnik>("Nicklas", "Bendtner", 11, 24, 78, 45, 75, 82));
    den->dodajCzlonkaSztabu(std::make_unique<TrenerGlowny>("Morten", "Olsen", 62, 20, "4-2-3-1", "Posiadanie"));

    // 14. Chorwacja
    auto* cro = stworzDruzyne("Chorwacja", "CRO");
    dodaj(cro, std::make_unique<Bramkarz>("Stipe", "Pletikosa", 1, 33, 60, 75, 80, 78));
    dodaj(cro, std::make_unique<SrodkowyObronca>("Vedran", "Corluka", 5, 26, 68, 82, 78, 82));
    dodaj(cro, std::make_unique<BocznyObronca>("Ivan", "Strinic", 3, 24, 80, 75, 72, 78));
    dodaj(cro, std::make_unique<Pomocnik>("Luka", "Modric", 10, 26, 80, 65, 90, 82));
    dodaj(cro, std::make_unique<Napastnik>("Mario", "Mandzukic", 17, 26, 82, 45, 80, 88));
    cro->dodajCzlonkaSztabu(std::make_unique<TrenerGlowny>("Slaven", "Bilic", 43, 10, "4-4-2", "Pressing"));

    // 15. Ukraina
    auto* ukr = stworzDruzyne("Ukraina", "UKR");
    dodaj(ukr, std::make_unique<Bramkarz>("Andrij", "Piatow", 12, 27, 60, 75, 80, 75));
    dodaj(ukr, std::make_unique<SrodkowyObronca>("Jewhen", "Chaczeridi", 3, 24, 70, 80, 75, 82));
    dodaj(ukr, std::make_unique<BocznyObronca>("Oleh", "Husiew", 9, 29, 82, 75, 72, 78));
    dodaj(ukr, std::make_unique<Pomocnik>("Anatolij", "Tymoszczuk", 4, 33, 72, 70, 85, 85));
    dodaj(ukr, std::make_unique<Napastnik>("Andrij", "Szewczenko", 7, 35, 78, 40, 80, 85));
    ukr->dodajCzlonkaSztabu(std::make_unique<TrenerGlowny>("Ołeh", "Błochin", 59, 20, "4-4-2", "Kontratak"));

    // 16. Irlandia
    auto* irl = stworzDruzyne("Irlandia", "IRL");
    dodaj(irl, std::make_unique<Bramkarz>("Shay", "Given", 1, 36, 55, 78, 82, 75));
    dodaj(irl, std::make_unique<SrodkowyObronca>("Richard", "Dunne", 5, 32, 62, 85, 75, 88));
    dodaj(irl, std::make_unique<BocznyObronca>("Stephen", "Ward", 2, 26, 75, 75, 70, 78));
    dodaj(irl, std::make_unique<Pomocnik>("Damien", "Duff", 11, 33, 78, 60, 80, 75));
    dodaj(irl, std::make_unique<Napastnik>("Robbie", "Keane", 10, 31, 75, 45, 78, 82));
    irl->dodajCzlonkaSztabu(std::make_unique<TrenerGlowny>("Giovanni", "Trapattoni", 73, 40, "4-4-2", "Defensywa"));
}