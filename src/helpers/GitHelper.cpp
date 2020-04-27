#include "helpers/GitHelper.hpp"

#include <cstdlib>
#include <iostream>
#include "constants/Globals.hpp"

void removeGitExtension(std::string& gitRepo) {
  size_t extensionIndex = gitRepo.rfind(".git");
  if (extensionIndex != std::string::npos) {
    gitRepo.replace(extensionIndex, extensionIndex + 4, "");
  }
}

std::string getRepoName(std::string gitRepo) {
  removeGitExtension(gitRepo);

  size_t lastDirSeparator = gitRepo.rfind("/");
  return lastDirSeparator == std::string::npos
    ? gitRepo
    : gitRepo.substr(lastDirSeparator + 1, gitRepo.size() - lastDirSeparator - 1);
}

void cloneRepoIfNonexistent(const std::string& unmodifiedRepoName, const std::string& importedLibName) {
  FileHelper::createRelativeToRoot(FileHelper::joinPath({
    Globals::EXTERNAL_GIT_REPO_DIR,
    Globals::EXTERNAL_BUILD_REPO_DIR,
    importedLibName
  }));

  const std::string repoPath = std::string(Globals::EXTERNAL_GIT_REPO_DIR) + "/" + getRepoName(unmodifiedRepoName);

  if (!FileHelper::isDir(FileHelper::resolveFromRoot(repoPath))) {
    const std::string cloneCommand("git clone " + unmodifiedRepoName + ' ' + FileHelper::resolveFromRoot(repoPath));

    FileHelper::createRelativeToRoot(repoPath);
    std::cout << "Cloning " << getRepoName(unmodifiedRepoName) << std::endl;
    std::system(cloneCommand.c_str());
  }
}
