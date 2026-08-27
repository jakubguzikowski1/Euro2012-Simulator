#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // Inicjalizacja aplikacji graficznej Qt
    QApplication a(argc, argv);

    // Utworzenie i wyświetlenie głównego okna
    MainWindow w;
    w.show();

    // Uruchomienie głównej pętli zdarzeń Qt
    return a.exec();
}