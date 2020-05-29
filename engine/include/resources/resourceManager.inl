#include "resourceManager.hpp"

template<typename MODEL_ENUM,typename TEXTURE_ENUM,typename SHADER_ENUM,typename AUDIO_ENUM>
inline const Resources::Model& Resources::ResourceManager<MODEL_ENUM, TEXTURE_ENUM, SHADER_ENUM, AUDIO_ENUM>::get(MODEL_ENUM id) const
{
    assert(models.count(id) == 1);
    return models.at(id);
}

template<typename MODEL_ENUM,typename TEXTURE_ENUM,typename SHADER_ENUM,typename AUDIO_ENUM>
inline const Resources::Shader& 
    Resources::ResourceManager<MODEL_ENUM, TEXTURE_ENUM, SHADER_ENUM, AUDIO_ENUM>::get(SHADER_ENUM id) const
{
    assert(shaders.count(id) == 1);
    return shaders.at(id);
}

template<typename MODEL_ENUM,typename TEXTURE_ENUM,typename SHADER_ENUM,typename AUDIO_ENUM>
inline const Resources::Texture& 
    Resources::ResourceManager<MODEL_ENUM, TEXTURE_ENUM, SHADER_ENUM, AUDIO_ENUM>::get(TEXTURE_ENUM id) const
{
    assert(textures.count(id) == 1);
    return textures.at(id);
}

template<typename MODEL_ENUM,typename TEXTURE_ENUM,typename SHADER_ENUM,typename AUDIO_ENUM>
inline const Resources::Audio& 
    Resources::ResourceManager<MODEL_ENUM, TEXTURE_ENUM, SHADER_ENUM, AUDIO_ENUM>::get(AUDIO_ENUM id) const
{
    assert(audios.count(id) == 1);
    return audios.at(id);
}


template<typename MODEL_ENUM,typename TEXTURE_ENUM,typename SHADER_ENUM,typename AUDIO_ENUM>
inline void Resources::ResourceManager<MODEL_ENUM, TEXTURE_ENUM, SHADER_ENUM, AUDIO_ENUM>::add(Resources::Model&& model, MODEL_ENUM key)
{
    models.emplace(std::make_pair(key, std::move(model)));
}

template<typename MODEL_ENUM,typename TEXTURE_ENUM,typename SHADER_ENUM,typename AUDIO_ENUM>
inline void Resources::ResourceManager<MODEL_ENUM, TEXTURE_ENUM, SHADER_ENUM, AUDIO_ENUM>::add(Resources::Texture&& texture, TEXTURE_ENUM key)
{
    textures.emplace(std::make_pair(key, std::move(texture)));
}

template<typename MODEL_ENUM,typename TEXTURE_ENUM,typename SHADER_ENUM,typename AUDIO_ENUM>
inline void Resources::ResourceManager<MODEL_ENUM, TEXTURE_ENUM, SHADER_ENUM, AUDIO_ENUM>::add(Resources::Shader&& shader, SHADER_ENUM key)
{
    shaders.emplace(std::make_pair(key, std::move(shader)));
}

template<typename MODEL_ENUM,typename TEXTURE_ENUM,typename SHADER_ENUM,typename AUDIO_ENUM>
inline void Resources::ResourceManager<MODEL_ENUM, TEXTURE_ENUM, SHADER_ENUM, AUDIO_ENUM>::add(Resources::Audio&& audio, AUDIO_ENUM key)
{
    audios.emplace(std::make_pair(key, std::move(audio)));
}

