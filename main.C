//============================================================================
// Name        : INF3_Prak.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <string>
#include <iostream>
#include <sstream>
#include <cstdio>

#include <cstdio>      /* printf, NULL */
#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */

#include <unistd.h>

#include "SHA256.H"
#include "TASK1.H"
#include "TASK2.H"
#include "TASK3.H"
#include "TASK4.H"
#include "TASK5.H"
#include "TASK6.H"

using namespace std;




int main(){

    stringstream sout;
    string output;

    int x,y;
    x=12;
    y=89;

    cout <<"CORD"<<"["<<x<<","<<y<<"]"<<endl;
    sout <<"CORD"<<"["<<x<<","<<y<<"]"<<endl;
    output = sout.str();

    cout<<"#"<<output<<"#";

    return 0;



	srand(time(nullptr));

	TASK3::demoTask3();

    return 0;
}





