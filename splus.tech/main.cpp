/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/* 
 * File:   main.cpp
 * Author: dies irae
 *
 * Created on July 2, 2020, 4:20 PM
 */
#include "WebSrv.h"
#include <iostream>
#include <string>
using namespace std;


int main(int argc, char** argv) {
    string _ip = "0.0.0.0";
    int _port = 80;
    WebSrv mainS(_ip.c_str(),_port);
    cout << "Server started" << endl;
    if(mainS.init() != 0){
        cout << "Some error with server INIT() " << mainS.init() << endl;
        return 0;
    }
    mainS.run();
    return 0;
}

