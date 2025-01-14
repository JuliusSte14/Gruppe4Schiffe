/*
 * client.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */
#include<random>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <fstream>
#include <string>
#include <iostream>
#include <unistd.h> // enthält verschiedene Konstanten

#include "SIMPLESOCKET.H"

using namespace std;

int main()
{
    const string serverAddress = "127.0.0.1"; // Serveradresse
    const int serverPort = 2022; // Serverport
    int totalShots = 0;
    bool gameOver = false;
    string Answer;
    int amountgames;
    int strategy;




    TCPclient client;

    if (!client.conn(serverAddress, serverPort))
    {
        cerr << "Verbindung zum Server fehlgeschlagen." << endl;
        return -1;
    }
    cout << "Verbindung mit dem Server!" << endl;
    usleep(10000);


    cout<<"Lass uns Schiffe versenken!"<<endl;
    cout<<"Welche Startegie soll angewand werden?(linear,rand,complex)"<<endl;
   while (true) { // Endlosschleife, die durch den Benutzer durchbrochen wird
        getline(cin, Answer);  // Liest die gesamte Zeile als Eingabe

        // Überprüfen, ob die Eingabe gültig ist
        if (Answer == "linear" || Answer == "rand" || Answer == "complex")
        {
            break;  // Schleife verlassen, wenn die Eingabe gültig ist
        } else {
            std::cout << "Falsche Eingabe, bitte wählen Sie zwischen linear, rand und complex." << endl;
        }
    }

    cout<<"Okay, wie viele Spiele solen gespielt werden?(1-1000)"<<endl;
    while (true)
    { // Endlosschleife, die durch den Benutzer durchbrochen wird
        cin>>amountgames;  // Liest die gesamte Zeile als Eingabe

        // Überprüfen, ob die Eingabe gültig ist
        if (amountgames>=1 && amountgames<=1000)
        {
            break;  // Schleife verlassen, wenn die Eingabe gültig ist
        } else {
            cout << "Falsche Eingabe, bitte eine gueltige Zahl zwischen 1 und 1000 eingeben." << endl;
        }
    }


    cout<<"Neuer Durchlauf wird gestartet"<<endl;

    usleep(500000);



    if (Answer == "linear") {
        strategy = 0;
    } else if (Answer == "rand") {
        strategy = 1;
    } else if (Answer == "complex") {
        strategy = 2;
    }


    // Art des Durchlaufs in eine Datei schreiben
    ofstream outFile("spielstatistik.txt", ios::app);
    if (outFile.is_open()) {
        outFile << "Durchlauf mit " << amountgames<<" Spielen und "<< Answer<< " Suche."<< endl;
        outFile.close();
    } else {
        cerr << "Fehler beim Öffnen der Datei 'spielstatistik.txt'." << endl;
    }



    while(amountgames >=1)
    {

        string Message ="NEWGAME";
        int shipsDestroyed=0;
        gameOver = false;
        client.sendData(Message);
        // Daten an den Server senden
        if (!client.sendData(Message))
            {
                cerr << "Fehler beim Senden der Daten an den Server." << endl;
                continue;
            }

         // Antwort vom Server empfangen
         string Response = client.receive(64);
            if (Response.empty())
            {
                cerr << "Fehler beim Empfangen der Antwort vom Server." << endl;
                continue;
            }

         cout << "Antwort vom Server: " << Response << endl;




        // Verarbeite die Eingabe mit switch-case
        switch (strategy)
        {
            case 0: {
                cout << "Sie haben die Strategie 'linear' gewählt." << endl;
                for (int x = 1; x <= 10 && !gameOver; ++x)
                {
                    for (int y = 1; y <= 10 && !gameOver; ++y)
                    {
                        totalShots++;
                        string Message = "COORD[" + to_string(x) + "," + to_string(y) + "]";


                        // Daten an den Server senden
                        if (!client.sendData(Message))
                        {
                            cerr << "Fehler beim Senden der Daten an den Server." << endl;
                            continue;
                        }

                        // Antwort vom Server empfangen
                        string Response = client.receive(64);
                        if (Response.empty()) {
                            cerr << "Fehler beim Empfangen der Antwort vom Server." << endl;
                            continue;
                        }

                        cout << "Antwort vom Server: " << Response << endl;




                        if (Response.compare(0,8,"GAMEOVER") == 0)
                        {
                        usleep(10000);
                        cout << "Spiel beendet nach " << totalShots << " Zügen!" << endl;
                        gameOver = true;
                        continue;
                        }
                        }



                    }


                // Anzahl der Züge in eine Datei schreiben
                ofstream outFile("spielstatistik.txt", ios::app);
                if (outFile.is_open())
                {
                    outFile << "Spiel beendet nach " << totalShots << " Zügen." << endl;
                    outFile.close();
                    cout << "Spielstatistik wurde in 'spielstatistik.txt' gespeichert." << endl;
                } else {
                    cerr << "Fehler beim Öffnen der Datei 'spielstatistik.txt'." << endl;
                }

                break;
            }
            case 1: {
                cout << "Sie haben die Strategie 'rand' gewählt." << endl;


                        // Spielfeld initialisieren (10x10) mit false
                    bool field[10][10] = {false};
                    srand(time(nullptr)); // Zufallsgenerator initialisieren
                    int x=0;
                    int y=0;

                    while (!gameOver)
                        {
                        do{
                        x = rand() %10;
                        y = rand() %10;
                        }while(field[x][y]==true);


                        // Feld markieren und Schuss ausführen
                        field[x][y] = true;
                        totalShots++;
                        string Message = "COORD[" + to_string(x+1) + "," + to_string(y+1) + "]";

                        // Daten an den Server senden
                        if (!client.sendData(Message)) {
                            cerr << "Fehler beim Senden der Daten an den Server." << endl;
                            continue;
                        }

                        // Antwort vom Server empfangen
                        string Response = client.receive(64);
                        if (Response.empty()) {
                            cerr << "Fehler beim Empfangen der Antwort vom Server." << endl;
                            continue;
                        }

                        cout << "Antwort vom Server: " << Response << endl;

                        // Prüfen, ob das Spiel vorbei ist
                       if (Response.compare(0,8,"GAMEOVER") == 0)
                        {
                        usleep(10000);
                        cout << "Spiel beendet nach " << totalShots << " Zügen!" << endl;
                        gameOver = true;
                        }
                    }

                    // Anzahl der Züge in eine Datei schreiben
                    ofstream outFile("spielstatistik.txt", ios::app);
                    if (outFile.is_open()) {

                        outFile << "Spiel beendet nach " << totalShots << " Zügen." << endl;
                        outFile.close();
                        cout << "Spielstatistik wurde in 'spielstatistik.txt' gespeichert." << endl;
                    } else {
                        cerr << "Fehler beim Öffnen der Datei 'spielstatistik.txt'." << endl;
                    }

                break;
            }
            case 2: {
                cout << "Sie haben die Strategie 'complex' gewählt." << endl;
                bool shotFields[10][10] = {false}; // Alle Felder initial unbeschossen
                    pair<int, int> toFollow[100];      // Array für Nachverfolgung von Feldern
                    int toFollowCount = 0;             // Anzahl der Felder in der Nachverfolgungsliste

                    // Geordnete Suche: Reihenweises Abschießen der Felder
                    for (int x = 1; x <= 10 && !gameOver; ++x) {
                        for (int y = 1; y <= 10 && !gameOver; ++y) {
                            // Falls Nachverfolgung aktiviert ist, geordnete Suche abbrechen
                            if (toFollowCount > 0) break;

                            if (!shotFields[x][y]) {
                                shotFields[x][y] = true;
                                string Message = "COORD[" + to_string(x) + "," + to_string(y) + "]";
                                // Daten an den Server senden
                                if (!client.sendData(Message)) {
                                    cerr << "Fehler beim Senden der Daten an den Server." << endl;
                                    continue;
                                }

                                // Antwort vom Server empfangen
                                string Response = client.receive(64);
                                if (Response.empty()) {
                                    cerr << "Fehler beim Empfangen der Antwort vom Server." << endl;
                                    continue;
                                }

                                cout << "Antwort vom Server: " << Response << endl;
                                totalShots++;

                                if (Response.compare(0,8,"GAMEOVER") == 0)
                                {
                                cout << "Spiel beendet nach " << totalShots << " Zügen!" << endl;
                                gameOver = true;
                                }

                                    // Benachbarte Felder zur Nachverfolgung hinzufügen
                                    if (x > 0 && !shotFields[x - 1][y]) toFollow[toFollowCount++] = {x - 1, y}; // Nach oben
                                    if (x < 9 && !shotFields[x + 1][y]) toFollow[toFollowCount++] = {x + 1, y}; // Nach unten
                                    if (y > 0 && !shotFields[x][y - 1]) toFollow[toFollowCount++] = {x, y - 1}; // Nach links
                                    if (y < 9 && !shotFields[x][y + 1]) toFollow[toFollowCount++] = {x, y + 1}; // Nach rechts
                                }
                            }

                    }

                    // Wechsel zu "Treffer und Nachverfolgung", wenn Treffer gefunden wurde
                    while (!gameOver)
                    {
                        int x, y;

                        // Nachverfolgungsmodus aktiv
                        if (toFollowCount > 0) {
                            x = toFollow[toFollowCount - 1].first;  // Letztes Element aus der Nachverfolgungsliste
                            y = toFollow[toFollowCount - 1].second;
                            toFollowCount--;

                            if (!shotFields[x][y])
                            {
                                shotFields[x][y] = true;
                                string Message = "COORD[" + to_string(x) + "," + to_string(y) + "]";
                                // Daten an den Server senden
                                if (!client.sendData(Message)) {
                                    cerr << "Fehler beim Senden der Daten an den Server." << endl;
                                    continue;
                                }

                                // Antwort vom Server empfangen
                                string Response = client.receive(64);
                                if (Response.empty()) {
                                    cerr << "Fehler beim Empfangen der Antwort vom Server." << endl;
                                    continue;
                                }

                                cout << "Antwort vom Server: " << Response << endl;

                                totalShots++;

                                if (Response == "GAMEOVER") {
                                    cout << "Spiel beendet nach " << totalShots << " Zügen!" << endl;
                                    gameOver = true;
                                } else if (Response == "HIT") {
                                    cout << "Treffer bei (" << x << "," << y << ")!" << endl;

                                    // Benachbarte Felder zur Nachverfolgung hinzufügen
                                    if (x > 0 && !shotFields[x - 1][y]) toFollow[toFollowCount++] = {x - 1, y}; // Nach oben
                                    if (x < 9 && !shotFields[x + 1][y]) toFollow[toFollowCount++] = {x + 1, y}; // Nach unten
                                    if (y > 0 && !shotFields[x][y - 1]) toFollow[toFollowCount++] = {x, y - 1}; // Nach links
                                    if (y < 9 && !shotFields[x][y + 1]) toFollow[toFollowCount++] = {x, y + 1}; // Nach rechts
                                }
                            }
                        } else {
                            // Falls keine Nachverfolgung mehr aktiv ist, beende das Spiel
                            cout << "Alle Felder beschossen oder keine Nachverfolgung mehr nötig." << endl;
                            break;
                        }
                    }


                    // Anzahl der Züge in eine Datei schreiben
                    ofstream outFile("spielstatistik.txt", ios::app);
                    if (outFile.is_open()) {
                        outFile << "Spiel beendet nach " << totalShots << " Zügen." << endl;
                        outFile.close();
                        cout << "Spielstatistik wurde in 'spielstatistik.txt' gespeichert." << endl;
                    } else {
                        cerr << "Fehler beim Öffnen der Datei 'spielstatistik.txt'." << endl;
                    }

                    break;


                }
            default: {
                cout << "ERROR" << endl;
                return -1;
            }
        }

        totalShots=0;
        amountgames--;
    }

     string Message = "BYEBYE";

        // Daten an den Server senden
        if (!client.sendData(Message))
        {
            cerr << "Fehler beim Senden der Daten an den Server." << endl;

        }

        // Antwort vom Server empfangen
        string Response = client.receive(64);
        if (Response.empty())
        {
            cerr << "Fehler beim Empfangen der Antwort vom Server." << endl;

        }

        cout << "Antwort vom Server: " << Response << endl;


    return 0;
    }
