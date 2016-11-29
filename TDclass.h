#pragma once
#include "pugixml.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <ctime>

#define WAVE_NUM 6
#define MONSTER_NUM 6
#define MONSTER_MIN 1
#define MONSTER_MAX 10
#define MONEY_MIN 100
#define MONEY_MAX 10000

using namespace std;

/*
    Represents the grids (with tag "Grid") in XML file.
*/
class Grid {
private:
    //Attributes
    int rows;
    int cols;
private:
    //The grid data
    vector<vector<int>> row;
public:
    //Constructor
    Grid();
    Grid(int rows, int cols, vector<vector<int>>& nums);
    //Get the grid data
    vector<vector<int>> getGrid();
    //Show the detail in console window
    void viewDetails();
};

/*
    Represents the waypoints (with tag "Waypoints") in XML file.
*/
class Waypoints {
private:
    vector<vector<int>> row;
public:
    //Constructor
    Waypoints();
    Waypoints(vector<vector<int>>& waypoints);
    //Get the waypoints data
    vector<vector<int>> getWaypoints();
    //Show the detail in console window
    void viewDetails();
};

/*
    Represents the monster object (with tag "Monster") in XML file.
*/
class Monster {
private:
    int Seconds;
    int Separation;
    int Amount;
    int ID;
public:
    Monster(int sec, int sep, int amount, int id);
    vector<int> getAttributes();
    void viewDetails();
};

/*
    Represents single Wave object (with tag "Wave") in XML file.
*/
class Wave {
private:
    vector<Monster> monsters;
public:
    //Constructor
    Wave() {};
    Wave(vector<Monster> monsters);
    //Get the monster list in this wave
    vector<Monster> getMonsters();
    void viewDetails();
};

/*
    Represents the towers (with tag "Towers") in XML file.
*/
class Towers {
private:
    //Names of types of towers
    vector<string> towers;
public:
    //Constructor
    Towers() {};
    Towers(vector<string> towers);
    //Get the tower names
    vector<string> getTowers();
    void viewDetails();
};

class Level {
private:
    //These objects corresponds to elements XML file
    Grid grid;
    Waypoints waypoints;
    vector<Wave> Waves;
    Towers towers;
    unsigned int money;
public:
    //Construct an empty object
    Level();
    //Construct an object from an existing XML file
    Level(string srcPath);
    //Copy the data from another Level object to this object
    void copyFrom(Level src);
    //Based on this object, randomize the attributes for amount of money,
    //numbers and types of monsters
    Level randomize();
    //Export this object to XML file in original format
    bool exportToXML(string dstPath);
    //Preview details of this object
    void viewDetails();
};