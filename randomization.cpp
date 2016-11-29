#include "pugixml.hpp"
#include "TDclass.h"
#include <string>
#include <iostream>

using namespace std;
using namespace pugi;

void main() {
    string xml_path = "Level1.xml";
    string output_path = "TestSave.xml";
    Level level1(xml_path);
    Level level2 = level1.randomize();
    level1.viewDetails();
    level2.viewDetails();
    cout << "Export To XML " << (level2.exportToXML(output_path)?"success.":"fail.") << endl;
    system("pause");
}
