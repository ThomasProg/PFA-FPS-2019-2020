#ifndef _SAVE_SYSTEM_HPP_
#define _SAVE_SYSTEM_HPP_

#include <list>

namespace Save
{
    class SaveInterface;

    class SaveSystem
    {
    public: 
        // List of the items that will be saved. 
        std::list<SaveInterface*> savedItems;
        
        using iterator = std::list<SaveInterface*>::iterator;

        iterator add(SaveInterface* savedItem);
        void remove(const iterator& it);

        // save any data you want with this function
        void save(const char* filename);
        // loads data and then load links (see loadData() and loadLinks())
        void load(const char* filename);
        // loads the data loaded from the save file
        void loadData(const char* filename);
    };
}

#endif