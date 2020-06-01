#ifndef _RESOURCE_MANAGER_HPP_
#define _RESOURCE_MANAGER_HPP_

#include "model.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "audio.hpp"

#include <unordered_map>
#include <memory>

namespace Resources
{
    template<typename MODEL_ENUM,typename TEXTURE_ENUM,typename SHADER_ENUM,typename AUDIO_ENUM>
    class ResourceManager
    {
    public:
        using Type = ResourceManager<MODEL_ENUM, TEXTURE_ENUM, SHADER_ENUM, AUDIO_ENUM>;

        static_assert(std::is_enum<MODEL_ENUM>::value);
        static_assert(std::is_enum<TEXTURE_ENUM>::value);
        static_assert(std::is_enum<SHADER_ENUM>::value);
        static_assert(std::is_enum<AUDIO_ENUM>::value);

    protected:
        std::unordered_map<MODEL_ENUM, Model> models;
        std::unordered_map<TEXTURE_ENUM, Texture> textures;
        std::unordered_map<SHADER_ENUM, std::unique_ptr<Shader>> shaders;
        std::unordered_map<AUDIO_ENUM, Audio> audios;

    public:
        // Returns const elements
        inline const Model&   get(MODEL_ENUM   id) const;
        inline const Texture& get(TEXTURE_ENUM id) const;
        inline const Shader&  get(SHADER_ENUM  id) const;
        inline const Audio&   get(AUDIO_ENUM   id)  const;

        // Only available per move
        inline void add(Model&& model,     MODEL_ENUM key);
        inline void add(Texture&& texture, TEXTURE_ENUM key);
        inline void add(std::unique_ptr<Shader>&& shader,   SHADER_ENUM key);
        inline void add(Audio&& audio,     AUDIO_ENUM key);

        void clear()
        {
            models.clear();
            textures.clear();
            shaders.clear();    
            audios.clear();
        }
    };
}

#include "resourceManager.inl"

#endif