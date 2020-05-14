#ifndef _SAVE_INTERFACE_HPP_
#define _SAVE_INTERFACE_HPP_

namespace Save
{
    class Saver;
    class Loader;

    class SaveInterface
    {
    public:
        unsigned int saveOrderID = 0;

    public:
        // save any data you want with this function
        virtual void save(Saver& saver) = 0;
        // loads the data loaded from the save file
        virtual void loadData(Loader& loader) = 0;
    };
}

#endif