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

void LinearStrategie(TCPclient& client, int& totalShots, bool& gameOver);
void RandomStrategie(TCPclient& client, int& totalShots, bool& gameOver);
void RandomUnbegrenztStrategie(TCPclient& client, int& totalShots, bool& gameOver);
void saveGameStatistics(int totalShots);

int main()
{
    const string serverAddress = "127.0.0.1"; // Serveradresse
    const int serverPort = 2022; // Serverport
    int totalShots = 0;
    bool gameOver = false;
    string Answer;
    int amountgames;
    int strategy;
    srand(time(nullptr)); // Zufallsgenerator initialisieren

    TCPclient client;

    if (!client.conn(serverAddress, serverPort))
    {
        cerr << "Verbindung zum Server fehlgeschlagen." << endl;
        return -1;
    }
    cout << "Verbindung mit dem Server!" << endl;
    usleep(10000);

    cout << "Lass uns Schiffe versenken!" << endl;
    cout << "Welche Strategie soll angewandt werden? (linear, rand, randunbegrenzt)" << endl;
    while (true)
    {
        getline(cin, Answer);  // Liest die gesamte Zeile als Eingabe

        if (Answer == "linear" || Answer == "rand" || Answer == "randunbegrenzt")
        {
            break;
        } else {
            cout << "Falsche Eingabe, bitte wählen Sie zwischen linear, rand und randunbegrenzt." << endl;
        }
    }

    cout << "Okay, wie viele Spiele sollen gespielt werden? (1-1000)" << endl;
    while (true)
    {
        cin >> amountgames;

        if (amountgames >= 1 && amountgames <= 1000)
        {
            break;
        } else {
            cout << "Falsche Eingabe, bitte eine gültige Zahl zwischen 1 und 1000 eingeben." << endl;
        }
    }

    cout << "Neuer Durchlauf wird gestartet" << endl;
    usleep(500000);

    if (Answer == "linear") {
        strategy = 0;
    } else if (Answer == "rand") {
        strategy = 1;
    } else if (Answer == "randunbegrenzt") {
        strategy = 2;
    }

    ofstream outFile("spielstatistik.txt", ios::app);
    if (outFile.is_open()) {
        outFile << "Durchlauf mit " << amountgames << " Spielen und " << Answer << " Suche." << endl;
        outFile.close();
    } else {
        cerr << "Fehler beim Öffnen der Datei 'spielstatistik.txt'." << endl;
    }

    while (amountgames >= 1)
    {
        string Message = "NEWGAME";
        gameOver = false;

        if (!client.sendData(Message))
        {
            cerr << "Fehler beim Senden der Daten an den Server." << endl;
            continue;
        }

        string Response = client.receive(64);
        if (Response.empty())
        {
            cerr << "Fehler beim Empfangen der Antwort vom Server." << endl;
            continue;
        }

        cout << "Antwort vom Server: " << Response << endl;

        switch (strategy)
        {
            case 0:
                LinearStrategie(client, totalShots, gameOver);
                break;
            case 1:
                RandomStrategie(client, totalShots, gameOver);
                break;
            case 2:
                RandomUnbegrenztStrategie(client, totalShots, gameOver);
                break;
            default:
                cout << "ERROR" << endl;
                return -1;
        }

        totalShots = 0;
        amountgames--;
    }

    string Message = "BYEBYE";
    if (!client.sendData(Message))
    {
        cerr << "Fehler beim Senden der Daten an den Server." << endl;
    }

    string Response = client.receive(64);
    if (Response.empty())
    {
        cerr << "Fehler beim Empfangen der Antwort vom Server." << endl;
    }

    cout << "Antwort vom Server: " << Response << endl;

    return 0;
}

void LinearStrategie(TCPclient& client, int& totalShots, bool& gameOver)
{
    for (int x = 1; x <= 10 && !gameOver; ++x)
    {
        for (int y = 1; y <= 10 && !gameOver; ++y)
        {
            totalShots++;
            string Message = "COORD[" + to_string(x) + "," + to_string(y) + "]";

            if (!client.sendData(Message))
            {
                cerr << "Fehler beim Senden der Daten an den Server." << endl;
                continue;
            }

            string Response = client.receive(64);
            if (Response.empty()) {
                cerr << "Fehler beim Empfangen der Antwort vom Server." << endl;
                continue;
            }

            cout << "Antwort vom Server: " << Response << endl;

            if (Response.compare(0, 8, "GAMEOVER") == 0)
            {
                usleep(10000);
                cout << "Spiel beendet nach " << totalShots << " Zügen!" << endl;
                gameOver = true;
                break;
            }
        }
    }
    saveGameStatistics(totalShots);
}

void RandomStrategie(TCPclient& client, int& totalShots, bool& gameOver)
{
    bool field[10][10] = {false};
    int x = 0;
    int y = 0;

    while (!gameOver)
    {
        do
        {
            x = (rand() % 10) + 1;
            y = (rand() % 10) + 1;
        } while (field[x - 1][y - 1]);

        field[x - 1][y - 1] = true;
        totalShots++;
        string Message = "COORD[" + to_string(x) + "," + to_string(y) + "]";

        if (!client.sendData(Message))
        {
            cerr << "Fehler beim Senden der Daten an den Server." << endl;
            continue;
        }

        string Response = client.receive(64);
        if (Response.empty()) {
            cerr << "Fehler beim Empfangen der Antwort vom Server." << endl;
            continue;
        }

        cout << "Antwort vom Server: " << Response << endl;

        if (Response.compare(0, 8, "GAMEOVER") == 0)
        {
            cout << "Spiel beendet nach " << totalShots << " Zügen!" << endl;
            gameOver = true;
        }
    }
    saveGameStatistics(totalShots);
}

void RandomUnbegrenztStrategie(TCPclient& client, int& totalShots, bool& gameOver)
{
    int x = 0;
    int y = 0;

    while (!gameOver)
    {
        x = (rand() % 10) + 1;
        y = (rand() % 10) + 1;

        totalShots++;
        string Message = "COORD[" + to_string(x) + "," + to_string(y) + "]";

        if (!client.sendData(Message))
        {
            cerr << "Fehler beim Senden der Daten an den Server." << endl;
            continue;
        }

        string Response = client.receive(64);
        if (Response.empty()) {
            cerr << "Fehler beim Empfangen der Antwort vom Server." << endl;
            continue;
        }

        cout << "Antwort vom Server: " << Response << endl;

        if (Response.compare(0, 8, "GAMEOVER") == 0)
        {
            cout << "Spiel beendet nach " << totalShots << " Zügen!" << endl;
            gameOver = true;
        }
    }
    saveGameStatistics(totalShots);
}

void saveGameStatistics(int totalShots)
{
    ofstream outFile("spielstatistik.txt", ios::app);
    if (outFile.is_open())
    {
        outFile << "Spiel beendet nach " << totalShots << " Zügen." << endl;
        outFile.close();
        cout << "Spielstatistik wurde in 'spielstatistik.txt' gespeichert." << endl;
    } else {
        cerr << "Fehler beim Öffnen der Datei 'spielstatistik.txt'." << endl;
    }
}
