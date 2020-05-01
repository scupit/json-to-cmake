
#include <filesystem>
#include <iostream>
#include <string>
#include "AllData.hpp"
#include "helpers/FileHelper.hpp"

using namespace std;

void visualizeLinkTree(OutputBase&, int = 1, const string& = "");

int main(int argc, const char** argv) {
  FileHelper::projectRootString.assign(FileHelper::resolve(filesystem::current_path(), argc > 1 ? argv[1] : ""));

  try {
    AllData data;
    cout << "---------- OUTPUT ----------" << endl;
    for (OutputItem& item : data.outputs()) {
      cout << item.name() << ':' << endl;
      visualizeLinkTree(item);
      cout << endl;
    }

    cout << "\n---------- GROUPS ----------" << endl;
    for (OutputGroup& group : data.outputGroups()) {
      cout << group.name() << ':' << endl;
      visualizeLinkTree(group, 1, "|");

      for (OutputItem& item : group.outputs()) {
        cout << "|\n\\----> " << group.name() << '.' << item.name() << ':' << endl;
        if (group.hasLinkedLibs()) {
          cout << "|\t(inherits from group)" << endl;
        }
        visualizeLinkTree(item, 1, "|");
      }
      cout << endl;
    }
  }
  catch (std::runtime_error& e) {
    cout << e.what();
  }

  return 0;
}

void visualizeLinkTree(OutputBase& output, int numTabs, const string& tabPrefix) {
  string tabs = tabPrefix;

  for (int i = 0; i < numTabs; ++i) {
    tabs += '\t';
  }

  for (OutputItem* linkedOutput : output.localLinkedOutputs()) {
    cout << tabs;
    if (linkedOutput->isContainedInGroup()) {
      cout << linkedOutput->containingGroup()->name() << '.';
    }
    cout << linkedOutput->name() << endl;
  }

  for (OutputGroup* linkedGroup : output.localLinkedGroups()) {
    cout << tabs << linkedGroup->name() << " (GROUP)" << endl;
  }

  for (ImportedLib* linkedImport : output.localLinkedImports()) {
    cout << tabs << linkedImport->name() << " (IMPORTED)" << endl;
  }
}