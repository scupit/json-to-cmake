#ifndef GITHELPER_HPP
#define GITHELPER_HPP

#include <string>
#include "helpers/FileHelper.hpp"
#include "constants/Globals.hpp"

void removeGitExtension(std::string&);
std::string getRepoName(std::string);
void cloneRepoIfNonexistent(const std::string&, const std::string&);

#endif