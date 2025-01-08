/*
 * client.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */

#include <fstream>
#include <string>
#include <iostream>
#include <unistd.h> //contains various constants

#include "SIMPLESOCKET.H"


using namespace std;

int main(){
  const string serverAddress = "32.0.0.1";
  const int serverPort = 2022;

  TCPclient client ;


  if (!client.conn(serverAddress, serverPort)){
  cerr << "Verbindung zum Server fehlgeschlagen." << endl;
  return -1;

  }
  cout << "Verbindung mit dem Server!" << endl;


  //Einfacher Schussalgorythmus
  int totalShots = 0; // Anzahl der abgegebenen Schüsse
  bool gameOver = false;

  for (int x = 1; x <= 10 && !gameOver; ++x) {
    for (int y = 1; y <= 10 && !gameOver; ++y) {
    totalShots++;
    string input = to_string(x) + "," + to_string(y);



  if (!client.sendData(input)){
  cerr << "Fehler beim Senden der daten an den Server." << endl;
  continue;

  }
  string response = client.receive(32);
  if (response.empty()){
  cerr << "Fehler beim Empfangen der Antwort von Server." << endl;
  continue;

  }

 cout << "Antwort vom Server:" << response << endl;

 // Prüfen, ob das Spiel vorbei ist
  if (response == "GAMEOVER") {
  cout << "Spiel beendet nach " << totalShots << " Zügen!" << endl;
  gameOver = true;

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

  if (gameOver) break; // Bricht die äußere Schleife ab, wenn Spiel vorbei ist

  return 0;


}
}

