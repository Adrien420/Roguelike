#include <boost/test/unit_test.hpp>
#include "../src/AssetManager.hpp"

BOOST_AUTO_TEST_SUITE(AssetManagerTests)

BOOST_AUTO_TEST_CASE(AddAndGetTextureTest) {
    AssetManager assetManager;

    // Ajouter une texture
    const std::string textureId = "menu";
    const char* texturePath = "../assets/menu.png"; // Assure-toi que le fichier existe
    assetManager.AddTexture(textureId, texturePath);

    // Récupérer la texture et vérifier qu'elle n'est pas nulle
    SDL_Texture* texture = assetManager.GetTexture(textureId);
    BOOST_CHECK(texture != nullptr);
}

BOOST_AUTO_TEST_CASE(AddAndGetFontTest) {
    AssetManager assetManager;

    // Ajouter une police
    const std::string fontId = "SF";
    const std::string fontPath = "../assets/SF.ttf"; // Assure-toi que le fichier existe
    int fontSize = 24;
    assetManager.AddFont(fontId, fontPath, fontSize);

    // Récupérer la police et vérifier qu'elle n'est pas nulle
    TTF_Font* font = assetManager.GetFont(fontId);
    BOOST_CHECK(font != nullptr);
}

BOOST_AUTO_TEST_SUITE_END()