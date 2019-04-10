#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

string findInAMap(string c, vector<vector<string>> grammar) {
    string k = "";
    for(int i =0; i<grammar.size(); i++) {
        for(int j = 1; j<grammar[i].size(); j++) {
            if(grammar[i][j].compare(c) == 0) k += grammar[i][0];
        }
    }
    return k;
}

vector<string> getAllPossibles(string one, string two) {
    vector<string> k;
    string temp;
    for(int i = 0; i<one.length(); i++) {
        for(int j = 0; j<two.length(); j++) {
            temp += one.at(i);
            temp += two.at(j);
            k.push_back(temp);
            temp = "";
        }
    }
    return k;
}

vector<vector<string>> setGrammar(int argc, char** argv) {
  vector<vector<string>> grammar;
  vector<string> temp;
  if(argc == 1) { //alapértelmezett beállítás
    temp.push_back("S"); temp.push_back("AB"); temp.push_back("BC");
    grammar.push_back(temp);
    temp.clear();
    temp.push_back("A"); temp.push_back("BA"); temp.push_back("a");
    grammar.push_back(temp);
    temp.clear();
    temp.push_back("B"); temp.push_back("CC"); temp.push_back("b");
    grammar.push_back(temp);
    temp.clear();
    temp.push_back("C"); temp.push_back("AB"); temp.push_back("a");
    grammar.push_back(temp);
    temp.clear();
  } else if(argc > 1) {
    string tempVariable, temp2;
    ifstream fajl(argv[1]);

    while(getline(fajl,tempVariable)) {
        istringstream magic(tempVariable);
        while(magic >> temp2)
            temp.push_back(temp2);
        grammar.push_back(temp);
        temp.clear();
    }
  }
  return grammar;
}

void doSomeMagic(vector<vector<string>> grammar, string example, vector<vector<string>> &result)
{
  int lepeskoz = 1;
  int oszlop_sor = 0;
  int sor = 0, oszlop = 0;
  bool sorNulla = false;
  string s_temp;
  vector<string> temp;
  for(int i = 0; i<example.length(); i++) {
      string s = string(1, example[i]);
      temp.push_back(findInAMap(s,grammar));
      s = "";
  }
  result.push_back(temp);
  temp.clear();

  for(int i = 0; i < example.length(); i++) {
      for (int countr = 0; countr < result[i].size() - 1; countr++) {
          while (!sorNulla) {
              if (!result[sor][oszlop].empty() && !result[oszlop_sor][lepeskoz].empty()) {
                  for (string a: getAllPossibles(result[sor][oszlop], result[oszlop_sor][lepeskoz])) {
                      s_temp += findInAMap(a, grammar);
                  }
              }
              if (oszlop_sor == 0) sorNulla = true;
              if(oszlop_sor - 1 >= 0) {
                  if (lepeskoz + 1 < result[oszlop_sor - 1].size())
                      lepeskoz++; //azért oszlop_sor -1, mivel a következő iterációban annak az oszlopnak a sora egyel lejebb lesz.
                  oszlop_sor--;
              }
              if (sor + 1 < result.size()) sor++;

          }
          if (oszlop + 1 < result[i].size()-1) {
              oszlop++;
              sor = 0;
              sorNulla = false;
              oszlop_sor = result.size()-1;

              if (oszlop + 1 < result[i].size()) lepeskoz = oszlop + 1;
          }
          if(s_temp.length() > 1) {
              sort(s_temp.begin(), s_temp.end());
              s_temp.erase(unique(s_temp.begin(),s_temp.end()),s_temp.end());
          }
          temp.push_back(s_temp);
          s_temp = "";
      }
      result.push_back(temp);
      temp.clear();
      sor = 0;
      sorNulla = false;
      oszlop_sor = result.size() - 1;
      oszlop = 0;
      lepeskoz = 1;
  }
}


int main(int argc, char** argv) {
    //unordered_map<char, vector<string> > grammar;
    vector<string> temp;
    vector<vector<string>> grammar = setGrammar(argc,argv);
    string example;
    vector<vector<string> > result;

    cout << "NYELVTAN" << endl << "---------" << endl;
    for(int i = 0; i<grammar.size(); i++) {
      cout << grammar[i][0] << " -> ";
      for(int j = 1; j<grammar[i].size(); j++) {
        cout << grammar[i][j];
        (j==grammar[i] .size()-1)? cout << " ": cout << " | ";
      }
      cout << endl;
    }
    cout << endl << "CYK TÁBLA" << endl << "----------" << endl;
    if(argc < 3) {
      example = "ababb";
    } else if(argc == 3) {
      example = argv[2];
    }
    cout << "keresett szó: " << example;

    doSomeMagic(grammar,example,result);

    cout << endl;
    for(int i = result.size()-1; i>=0; i--) {
        for(int j = 0; j<result[i].size(); j++) {
            cout << "| " << result[i][j] << " |" << " ";
        }
        cout << endl;
    }

    if(result[result.size()-2][0].find('S') != string::npos) cout << "A keresett szó benne van a nyelvben!" << endl;
    else cout << "A keresett szó nincs benne a nyelvben!" << endl;
}
