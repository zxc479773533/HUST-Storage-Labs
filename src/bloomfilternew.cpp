/* 
 * bloomfilter.cpp
 * 
 * TEST for HUST IOT Storage lab
 * 
 * Crated at 2019-06-08
 */

#include <iostream>
#include <fstream>
#include <thread>
#include "pylib/debug.h"
#include "pylib/sys/time.h"

using namespace pylib;
using namespace std;

#define SIZE 5000

int Hasher1(std::string str) {
  int ret = 0;
  for (int i = 0; i < str.length(); i++)
    ret += str[i];
  return ret % (SIZE / 5);
}
int Hasher2(std::string str) {
  int ret = 1;
  for (int i = 0; i < str.length(); i++) {
    ret = ret * 33 + str[i];
  }
  if (ret < 0)
    ret = -ret;
  return ret % (SIZE / 5) + (SIZE / 5);
}
int Hasher3(std::string str) {
  int ret = str.length();
  for (int i = 0; i < str.length(); i += 2) {
    ret = ((ret >> 28) ^ (ret << 4)) ^ str[i];
  }
  if (ret < 0)
    ret = -ret;
  return ret % (SIZE / 5) + (SIZE / 5) * 2;
}
int Hasher4(std::string str) {
  int ret = 1;
  int p = (int)2166136261L;
  for (int i = 0; i < str.length(); i += 2) {
    ret = (ret ^ str[i]) * p;
  }
  ret += ret << 13;
  ret ^= ret >> 7;
  ret += ret << 3;
  ret ^= ret >> 17;
  ret += ret << 5;
  if (ret < 0)
    ret = -ret;
  return ret % (SIZE / 5) + (SIZE / 5) * 3;
}
int Hasher5(std::string str) {
  int ret = 0;
  int a = 63689;
  int b = 378551;
  for (int i = 0; i < str.length(); i += 2) {
    ret = ret * a + str[i];
    a = a * b;
  }
  if (ret < 0)
    ret = -ret;
  return ret % (SIZE / 5) + (SIZE / 5) * 4;
}

bool Add(int *bf, std::string str, int f(std::string)) {
  auto ret = f(str);
  bf[ret]++;
  return true;
}

bool Find(int *bf, std::string str) {
  int find = true;
  int ret[5];
  ret[0] = Hasher1(str);
  ret[1] = Hasher2(str);
  ret[2] = Hasher3(str);
  ret[3] = Hasher4(str);
  ret[4] = Hasher5(str);
  for (int i = 0; i < 5; i++) {
    if (bf[ret[i]] == 0) {
      find = false;
      break;
    }
  }
  return find;
}

bool Delete(int *bf, std::string str, int f(std::string)) {
  auto ret = f(str);
  bf[ret]--;
  return true;
}

int main(int argc, const char** argv) {
  Timer Start;

  ifstream testfile;
  testfile.open("test.txt");
  int bf[SIZE];

  std::thread t1([](int *bf){
    ifstream datafile;
    datafile.open("data.txt");
    string str;
    while(getline(datafile, str)) {
      Add(bf, str, Hasher1);
    }
  }, bf);

  std::thread t2([](int *bf){
    ifstream datafile;
    datafile.open("data.txt");
    string str;
    while(getline(datafile, str)) {
      Add(bf, str, Hasher2);
    }
  }, bf);
  
  std::thread t3([](int *bf){
    ifstream datafile;
    datafile.open("data.txt");
    string str;
    while(getline(datafile, str)) {
      Add(bf, str, Hasher3);
    }
  }, bf);
  
  std::thread t4([](int *bf){
    ifstream datafile;
    datafile.open("data.txt");
    string str;
    while(getline(datafile, str)) {
      Add(bf, str, Hasher4);
    }
  }, bf);
  
  std::thread t5([](int *bf){
    ifstream datafile;
    datafile.open("data.txt");
    string str;
    while(getline(datafile, str)) {
      Add(bf, str, Hasher5);
    }
  }, bf);
  

  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();

  Timer Afteradd;

  int counter = 0;
  string str;
  while(getline(testfile, str)) {
    if (Find(bf, str) == true)
      counter++;
  }
  Timer Afterfind;

  std::thread t6([](int *bf){
    ifstream datafile;
    datafile.open("data.txt");
    string str;
    while(getline(datafile, str)) {
      Delete(bf, str, Hasher1);
    }
  }, bf);

  std::thread t7([](int *bf){
    ifstream datafile;
    datafile.open("data.txt");
    string str;
    while(getline(datafile, str)) {
      Delete(bf, str, Hasher2);
    }
  }, bf);

  std::thread t8([](int *bf){
    ifstream datafile;
    datafile.open("data.txt");
    string str;
    while(getline(datafile, str)) {
      Delete(bf, str, Hasher3);
    }
  }, bf);

  std::thread t9([](int *bf){
    ifstream datafile;
    datafile.open("data.txt");
    string str;
    while(getline(datafile, str)) {
      Delete(bf, str, Hasher4);
    }
  }, bf);

  std::thread t10([](int *bf){
    ifstream datafile;
    datafile.open("data.txt");
    string str;
    while(getline(datafile, str)) {
      Delete(bf, str, Hasher5);
    }
  }, bf);

  t6.join();
  t7.join();
  t8.join();
  t9.join();
  t10.join();
  Timer Afterdelete;

  cout << "Results" << endl;
  cout << "      Add time: " << (Afteradd - Start).AccurateMicrosecond() << endl;
  cout << "     Find time: " << (Afterfind - Afteradd).AccurateMicrosecond() << endl;
  cout << "   Delete time: " << (Afterdelete - Afterfind).AccurateMicrosecond() << endl;
  cout << "False Positive: " << counter << endl;

  return 0;
}