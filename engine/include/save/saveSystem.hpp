#ifndef _SAVE_SYSTEM_HPP_
#define _SAVE_SYSTEM_HPP_

#include <list>

namespace Save
{
    class SaveInterface;

    class SaveSystem
    {
    private:

    public: 
        std::list<SaveInterface*> savedItems;
        using SavedItemIt = std::list<SaveInterface*>::iterator;

        SavedItemIt add(SaveInterface* savedItem);
        void remove(const SavedItemIt& it);

        // save any data you want with this function
        void save(const char* filename);
        // loads data and then load links (see loadData() and loadLinks())
        void load(const char* filename);
        // loads the data loaded from the save file
        void loadData(const char* filename);
        // // after loading data, this function will be called to set pointers, iterators, references...
        // void loadLinks();
    };
}

#endif