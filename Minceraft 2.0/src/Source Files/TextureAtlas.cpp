#include "../Header Files/TextureAtlas.h"

TextureAtlas::TextureAtlas() {
    tile_size = 20;
    dimensions = glm::ivec2(0, 0);
    uv_size = glm::vec2(0, 0);
    cur_pos = glm::ivec2(0, 0);
}

TextureAtlas::TextureAtlas(std::string file_name) {
    Load(file_name);
    tile_size = 20;
}
/*
* Hopsoncraft image loading used here, got the idea
* to use sfml from him.
*/
void TextureAtlas::Load(std::string file_name) {
    sf::Image image;
    if (!image.loadFromFile(file_name))
    {
        std::cout << "Error: Couldn't load image " << file_name << "\n";
        return;
    }

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    //glGenerateMipmap(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGBA,
        image.getSize().x,
        image.getSize().y,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        image.getPixelsPtr());
    
    dimensions.x = image.getSize().x;
    dimensions.y = image.getSize().y;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
}

glm::ivec2 TextureAtlas::addTexture(std::string file_name) {
    sf::Image image;
    if (!image.loadFromFile(file_name)) {
        std::cout << "Error: Couldn't load image " << file_name << "\n";
        return glm::ivec2(0, 0);
    }

    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexSubImage2D(GL_TEXTURE_2D,
        0,
        cur_pos.x,
        cur_pos.y,
        image.getSize().x,
        image.getSize().y,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        image.getPixelsPtr());
    
    glBindTexture(GL_TEXTURE_2D, 0);

    std::cout << glGetError() << "\n";

    glm::vec2 old_pos = cur_pos;
    glm::vec2 pos = GetNextPosition(cur_pos);
    cur_pos = pos;

    return old_pos;
}

int TextureAtlas::GetTileSize() {
    return tile_size;
}

void TextureAtlas::SetTileSize(int tile_size) {
    this->tile_size = tile_size;
}

int TextureAtlas::GetTextureSize() {
    return texture_size;
}

void TextureAtlas::SetTextureSize(int texture_size) {
    this->texture_size = texture_size;
    uv_size = glm::vec2(((float)texture_size) / dimensions.x, ((float)texture_size) / dimensions.y);
}

glm::vec2 TextureAtlas::GetSize() {
    return dimensions;
}

glm::vec2 TextureAtlas::GetUVCoords(int x, int y) {
    return glm::vec2(x * uv_size.x, y * uv_size.y);
}

void TextureAtlas::Use() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

glm::ivec2 TextureAtlas::GetNextPosition(glm::ivec2 pos) {
    glm::vec2 new_pos = pos;
    new_pos.x += texture_size;
    if (new_pos.x >= dimensions.x) {
        new_pos.x = 0;
        new_pos.y += texture_size;
    }
    if (new_pos.y >= dimensions.y) {
        new_pos.y = 0;
    }

    return new_pos;
}

glm::vec2 TextureAtlas::GetUVSize() {
    return uv_size;
}