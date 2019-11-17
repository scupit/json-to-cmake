#include <iostream>
#include "SampleClass.hpp"

using namespace std;

int main(int argc, const char** argv) {
  if (argc > 1) {
    cout << "Arguments passed:" << endl;
    for (int i = 1; i < argc; ++i) {
      cout << "\t" << argv[i] << endl;
    }
  }
  else {
    cout << "No additional arguments passed to exe..." << endl;
  }

  const SampleClass item("This is the contained text");
  cout << "SAMPLE CLASS CONTAINED TEXT: " << item.containedText << endl;

  return 0;
}