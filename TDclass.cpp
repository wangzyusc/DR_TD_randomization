#include "TDclass.h"

using namespace std;
using namespace pugi;

Grid::Grid() {
    this->rows = 0;
    this->cols = 0;
    this->row = vector<vector<int>>(0);
}

Grid::Grid(int rows, int cols, vector<vector<int>>& nums) {
    this->rows = rows;
    this->cols = cols;
    this->row = nums;
};

void Grid::viewDetails() {
    cout << endl << "Grid Details" << endl;
    cout << "Rows: " << this->rows << "\tCols: " << this->cols << endl;
    for (int i = 0; i < this->row.size(); i++) {
        for (int j = 0; j < this->row[i].size()-1; j++) {
            cout << row[i][j] << ",";
        }
        cout << row[i][row[i].size() - 1] << endl;
    }
}

vector<vector<int>> Grid::getGrid() {
    return this->row;
}

Waypoints::Waypoints() {
    this->row = vector<vector<int>>(0);
}

Waypoints::Waypoints(vector<vector<int>>& waypoints) {
    this->row = waypoints;
};

vector<vector<int>> Waypoints::getWaypoints() {
    return this->row;
}

void Waypoints::viewDetails() {
    cout << endl << "Waypoints Details" << endl;
    for (int i = 0; i < this->row.size(); i++) {
        for (int j = 0; j < this->row[i].size() - 1; j++) {
            cout << row[i][j] << ",";
        }
        cout << row[i][row[i].size() - 1] << endl;
    }
    cout << endl;
}

Monster::Monster(int sec, int sep, int amount, int id) {
    this->Seconds = sec;
    this->Separation = sep;
    this->Amount = amount;
    this->ID = id;
}

vector<int> Monster::getAttributes() {
    vector<int> attributes;
    attributes.push_back(this->Seconds);
    attributes.push_back(this->Separation);
    attributes.push_back(this->Amount);
    attributes.push_back(this->ID);
    return attributes;
}

void Monster::viewDetails() {
    cout << endl << "Monster Details" << endl;
    cout << "Seconds: " << this->Seconds << endl;
    cout << "Separation: " << this->Separation << endl;
    cout << "Amount: " << this->Amount << endl;
    cout << "ID: " << this->ID << endl;
}

Wave::Wave(vector<Monster> monsters) {
    this->monsters = monsters;
}

vector<Monster> Wave::getMonsters() {
    return this->monsters;
}

void Wave::viewDetails() {
    cout << endl << "Wave Details" << endl;
    for (int i = 0; i < this->monsters.size(); i++) {
        monsters[i].viewDetails();
    }
}

Towers::Towers(vector<string> towers) {
    this->towers = towers;
}

void Towers::viewDetails() {
    cout << endl << "Towers Details" << endl;
    for (int i = 0; i < towers.size(); i++) {
        cout << towers[i] << endl;
    }
}

vector<string> Towers::getTowers() {
    return this->towers;
}

Level::Level() {
    this->grid = Grid();
    this->waypoints = Waypoints();
    this->Waves = vector<Wave>(0);
    this->towers = Towers();
    this->money = 0;
}

Level::Level(string srcPath) {
    xml_document doc;
    xml_parse_result result = doc.load_file(srcPath.c_str());
    //Read Grid
    int rows = atoi(doc.child("Level").child("Grid").attribute("rows").value());
    int cols = atoi(doc.child("Level").child("Grid").attribute("colunms").value());//char_t is char
    vector<vector<int>> grid_nums;
    for (xml_node row : doc.child("Level").child("Grid").children("row")) {
        vector<int> row_nums;
        string str(row.child_value());
        size_t pos = str.find_first_of(",");
        int n;
        while (pos != -1) {
            n = atoi(str.substr(0, pos).c_str());
            row_nums.push_back(n);
            str = str.substr(pos + 1);
            pos = str.find_first_of(",");
        }
        n = atoi(str.c_str());
        row_nums.push_back(n);
        grid_nums.push_back(row_nums);
    }
    Grid grid(rows, cols, grid_nums);
    this->grid = grid;
    //Read Waypoints
    vector<vector<int>> waypoints;
    for (xml_node row : doc.child("Level").child("Waypoints").children("row")) {
        vector<int> row_nums;
        string str(row.child_value());
        size_t pos = str.find_first_of(",");
        int n;
        while (pos != -1) {
            n = atoi(str.substr(0, pos).c_str());
            row_nums.push_back(n);
            str = str.substr(pos + 1);
            pos = str.find_first_of(",");
        }
        n = atoi(str.c_str());
        row_nums.push_back(n);
        waypoints.push_back(row_nums);
    }
    this->waypoints = Waypoints(waypoints);
    //Read Waves
    for (xml_node wave : doc.child("Level").child("Waves").children("Wave")) {
        vector<Monster> monsters;
        for (xml_node monster : wave.children("Monster")) {
            int seconds = atoi(string(monster.child("Seconds").child_value()).c_str());
            int separation = atoi(string(monster.child("Separation").child_value()).c_str());
            int amount = atoi(string(monster.child("Amount").child_value()).c_str());
            int id = atoi(string(monster.child("ID").child_value()).c_str());
            monsters.push_back(Monster(seconds, separation, amount, id));
        }
        this->Waves.push_back(Wave(monsters));
    }
    //Read Towers
    vector<string> towers;
    for (xml_node tower : doc.child("Level").child("Towers").children("Tower")) {
        towers.push_back(string(tower.child_value()));
    }
    this->towers = Towers(towers);
    //Read Money
    this->money = atoi(string(doc.child("Level").child("Money").child_value()).c_str());
}

void Level::copyFrom(Level src) {
    this->grid = src.grid;
    this->waypoints = src.waypoints;
    cout << "Copying waves, # of waves is " << src.Waves.size() << endl;
    for (int i = 0; i < src.Waves.size(); i++) {
        this->Waves.push_back(src.Waves[i]);
    }
    this->towers = src.towers;
    this->money = src.money;
}

Level Level::randomize() {
    srand(time(NULL));
    Level dst = Level();
    dst.copyFrom(*this);
    //Randomize monsters in waves
    dst.Waves = vector<Wave>(0);
    for (int i = 0; i < WAVE_NUM; i++) {
        vector<Monster> monsters;
        for (int j = 0; j < MONSTER_NUM; j++) {
            int sec = rand() % (MONSTER_MAX - MONSTER_MIN) + MONSTER_MIN;
            int sep = rand() % (MONSTER_MAX - MONSTER_MIN) + MONSTER_MIN;
            int amt = rand() % (MONSTER_MAX - MONSTER_MIN) + MONSTER_MIN;
            int id = rand() % (MONSTER_MAX - MONSTER_MIN) + MONSTER_MIN;
            monsters.push_back(Monster(sec, sep, amt, id));
        }
        dst.Waves.push_back(Wave(monsters));
    }
    //Randomize amount of money
    dst.money = rand() % (MONEY_MAX - MONEY_MIN) + MONEY_MIN;
    return dst;
}

bool Level::exportToXML(string dstPath) {
    xml_document doc;
    xml_node level = doc.append_child("Level");
    xml_node grid = level.append_child("Grid");
    xml_node waypoints = level.append_child("Waypoints");
    xml_node waves = level.append_child("Waves");
    xml_node towers = level.append_child("Towers");
    xml_node money = level.append_child("Money");

    //Write the grid
    vector<vector<int>> grid_num = this->grid.getGrid();
    grid.append_attribute("rows") = to_string(grid_num.size()).c_str();
    grid.append_attribute("cols") = to_string(grid_num[0].size()).c_str();
    for (int i = 0; i < grid_num.size(); i++) {
        string str = "";
        for (int j = 0; j < grid_num[i].size()-1; j++) {
            str += to_string(grid_num[i][j]) + ",";
        }
        str += to_string(grid_num[i][grid_num[i].size() - 1]);
        xml_node row = grid.append_child("row");
        row.text().set(str.c_str());
    }

    //Write the waypoints
    vector<vector<int>> waypoints_num = this->waypoints.getWaypoints();
    for (int i = 0; i < waypoints_num.size(); i++) {
        string str = "";
        for (int j = 0; j < waypoints_num[i].size() - 1; j++) {
            str += to_string(waypoints_num[i][j]) + ",";
        }
        str += to_string(waypoints_num[i][waypoints_num[i].size() - 1]);
        xml_node row = waypoints.append_child("row");
        row.text().set(str.c_str());
    }

    //Write the Waves
    for (int i = 0; i < this->Waves.size(); i++) {
        xml_node wave = waves.append_child("Wave");
        vector<Monster> monsterInWave = this->Waves[i].getMonsters();
        for (int j = 0; j < monsterInWave.size(); j++) {
            xml_node mon = wave.append_child("Monster");
            vector<int> attr = monsterInWave[j].getAttributes();
            mon.append_child("Seconds").text().set(to_string(attr[0]).c_str());
            mon.append_child("Separation").text().set(to_string(attr[1]).c_str());
            mon.append_child("Amount").text().set(to_string(attr[2]).c_str());
            mon.append_child("ID").text().set(to_string(attr[3]).c_str());
        }
    }

    //Write the Towers
    vector<string> towerlist = this->towers.getTowers();
    for (int i = 0; i < towerlist.size(); i++) {
        xml_node tower = towers.append_child("Tower");
        tower.text().set(towerlist[i].c_str());
    }

    //Write the money
    money.set_name("Money");
    money.text().set(to_string(this->money).c_str());

    return doc.save_file(dstPath.c_str());
}

void Level::viewDetails() {
    cout << endl << "Level Details" << endl << endl;
    grid.viewDetails();
    waypoints.viewDetails();
    for (int i = 0; i < Waves.size(); i++) {
        Waves[i].viewDetails();
    }
    towers.viewDetails();
    cout << endl << "Money" << endl << money << endl;
}