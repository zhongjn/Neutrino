#include "Nullable.h"
#include <iostream>

using namespace std;

int main() {
  // TODO: 进入UI函数
  // UI负责操作MailManager

  // test
  Nullable<int> ni(1);
  cout << ni.GetValue() << endl;
  ni.SetNull();
  try {
    cout << ni.GetValue() << endl;
  } catch (NullValueException& ex) {
    cout << "error" << endl;
  }
  system("pause");
  return 0;
}