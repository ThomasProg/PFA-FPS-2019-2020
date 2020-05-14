#include "saveSystem.hpp"

#include "saveInterface.hpp"

#include "loader.hpp"
#include "saver.hpp"

#include <fstream>
#include <iostream>

Save::SaveSystem::SavedItemIt Save::SaveSystem::add(SaveInterface* savedItem)
{
    savedItems.push_back(savedItem);
    return savedItems.end();
}

void Save::SaveSystem::remove(const SavedItemIt& it)
{
    savedItems.erase(it);
}



void Save::SaveSystem::save(const char* filename)
{
    try 
    {
    Saver saver {std::ofstream{filename, std::ios::out | std::ios::binary}};
    if(!saver.saveFile) 
    {
        std::cout << "Can't open file." << std::endl;
        return;
    }

    for (SaveInterface* it : savedItems)
    {
        it->save(saver);
    }

    if(!saver.saveFile.good()) 
    {
        std::cout << "An error occurred while saving items." << std::endl;
        return;
    }
    }
    catch (...) { /* */ }
}

// loads data and then load links (see loadData() and loadLinks())
void Save::SaveSystem::load(const char* filename)
{
    loadData(filename);
    // loadLinks();
}

void Save::SaveSystem::loadData(const char* filename)
{
    try 
    {
    Loader loader {std::ifstream{filename, std::ios::out | std::ios::binary}};
    if(!loader.loadedFile) 
    {
        std::cout << "Can't open file." << std::endl;
        return;
    }

    for (SaveInterface* it : savedItems)
    {
        it->loadData(loader);
    }

    if(!loader.loadedFile.good()) 
    {
        std::cout << "An error occurred while loading data." << std::endl;
        return;
    }
    }
    catch (...) { /* */ }
}

