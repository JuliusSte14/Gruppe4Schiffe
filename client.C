/*
 * client.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */

#include <fstream>
#include <string>
#include <iostream>
#include <unistd.h> // enthält verschiedene Konstanten

#include "SIMPLESOCKET.H"

using namespace std;

int main() {
    const string serverAddress = "127.0.0.1"; // Serveradresse
    const int serverPort = 2022; // Serverport

    TCPclient client;

    if (!client.conn(serverAddress, serverPort)) {
        cerr << "Verbindung zum Server fehlgeschlagen." << endl;
        return -1;
    }
    cout << "Verbindung mit dem Server!" << endl;

    // Einfacher Schussalgorithmus
    int totalShots = 0;  // Anzahl der abgegebenen Schüsse
    bool gameOver = false;

    for (int x = 0; x <= 9 && !gameOver; ++x) {
        for (int y = 0; y <= 9 && !gameOver; ++y) {
            totalShots++;
            string input = to_string(x) + "," + to_string(y);

            // Daten an den Server senden
            if (!client.sendData(input)) {
                cerr << "Fehler beim Senden der Daten an den Server." << endl;
                continue;
            }

            // Antwort vom Server empfangen
            string response = client.receive(1024);
            if (response.empty()) {
                cerr << "Fehler beim Empfangen der Antwort vom Server." << endl;
                continue;
            }

            cout << "Antwort vom Server: " << response << endl;

            // Prüfen, ob das Spiel vorbei ist
            if (response == "GAMEOVER: Alle Schiffe zerstört! Spiel beendet.") {
                cout << "Spiel beendet nach " << totalShots << " Zügen!" << endl;
                gameOver = true;
            }
        }
    }

    // Anzahl der Züge in eine Datei schreiben
    ofstream outFile("spielstatistik.txt");
    if (outFile.is_open()) {
        outFile << "Spiel beendet nach " << totalShots << " Zügen." << endl;
        outFile.close();
        cout << "Spielstatistik wurde in 'spielstatistik.txt' gespeichert." << endl;
    } else {
        cerr << "Fehler beim Öffnen der Datei 'spielstatistik.txt'." << endl;
    }

    return 0;
}
