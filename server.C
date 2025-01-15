/*
 * server.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */

#include <cstdio> // standard input and output library
#include <cstdlib> // this includes functions regarding memory allocation
#include <cstring> // contains string functions
#include <cerrno> //It defines macros for reporting and retrieving error conditions through error codes
#include <ctime> //contains various functions for manipulating date and time

#include <unistd.h> //contains various constants
#include <sys/types.h> //contains a number of basic derived types that should be used whenever appropriate
#include <arpa/inet.h> // defines in_addr structure
#include <sys/socket.h> // for socket creation
#include <netinet/in.h> //contains constants and structures needed for internet domain addresses

#include "SIMPLESOCKET.H"
#include <iostream>
#include "TASK3.H"

#include <iostream>
#include <string>
#include "TASK3.C"


using namespace std;

class SchiffeServer : public TCPserver{
private:
    TASK3::World *world; // Spielfeld-Objekt aus TASK3

public:
    SchiffeServer(int port, int maxDataSizeRecv) : TCPserver(port, maxDataSizeRecv)
    {
        world = new TASK3::World();
        cout << "Server gestartet!" << endl;
        cout << "Spielfeld (10x10) mit 1xFünfer, 2xVierer, 3xDreier, 4xZweier-Schiffen erstellt." << endl;
        usleep(10000);
    }
protected:
    string myResponse(string input) override
    {
    int x = -1, y = -1;

    if (input.compare(0,7,"NEWGAME") == 0){
        delete world;
        world = new TASK3::World();
        return string("DONE");
    }

    if (input.compare(0,6,"COORD[") == 0){
        if (sscanf (input.c_str(), "COORD[%d,%d]", &x, &y) != 2){
            return string ("Error. Could not read coord-data.");
        }

    if (x>10 && y>10){
    return string ("ERRORBORDER");
    }

        TASK3::ShootResult result = world->shoot(x, y);

        switch (result) {
            case TASK3::WATER: return "WATER";
            case TASK3::SHIP_HIT: return "SHIPHIT";
            case TASK3::SHIP_DESTROYED: return "SHIPDEST";
            case TASK3::ALL_SHIPS_DESTROYED: return "GAMEOVER";
            case TASK3::GAME_OVER: return "GAMEOVER";
            default: return "ERROR";
        }
    }
    return string("UNKNOWNCOMMAND");
    }
    };


int main (){
    const int port = 2022;
    const int maxDataSizeRecv = 64;
    SchiffeServer server (2022, 64); //Server starten
    server.run();
    return 0;
}
