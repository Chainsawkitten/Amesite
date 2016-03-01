#include "WinScene.hpp"

#include <Resources.hpp>
#include <Texture/Texture2D.hpp>
#include <MainWindow.hpp>
#include "../Game.hpp"
#include "MenuScene.hpp"
#include <Font/Font.hpp>
#include <random>
#include <time.h>

WinScene::WinScene(float totalTime, int enemiesKilled) {
    mPortalTexture = Resources().CreateTexture2DFromFile("Resources/SwirlingPortal.png");
    mElapsedTime = 0.f;
    mFont = Resources().CreateFontFromFile("Resources/ABeeZee.ttf", 25.f);
    mFont->SetColor(glm::vec3(1.f, 1.f, 1.f));

    std::default_random_engine RNG;
    RNG.seed(time(0));

    std::vector<std::string> introStrings;
    introStrings.push_back("You cause was just and your aim was true.\n");
    introStrings.push_back("As you enter the portal, you feel a deep longing for home.\n");
    introStrings.push_back("Your head is filled with visions of home.\n");
    introStrings.push_back("Wonder how long time has passed since you left home?\n");
    introStrings.push_back("The portal yanks you from this faraway place, and towards the path home.\n");
    introStrings.push_back("A WINNER IS YOU.\n");
    introStrings.push_back("The radio crackles, an incoming message from home.\n");
    introStrings.push_back("An eerie silence blankets the cave as you enter the portal.\n");

    std::uniform_int_distribution<uint32_t> introStringsDistribution(0, introStrings.size()-1);

    std::string introString = introStrings[introStringsDistribution(RNG)];

    std::vector<std::string> outroStrings;
    outroStrings.push_back("leaving a bit of yourself behind.\n");
    outroStrings.push_back("to boldly go where no man/woman has gone before.\n");
    outroStrings.push_back("to write an excellent report on xenobiology.\n");
    outroStrings.push_back("as there are still worlds left to conquer.\n");
    outroStrings.push_back("still wondering what impact you had on the local ecosystem.\n");
    outroStrings.push_back("already preparing for your next journey.\n");
    outroStrings.push_back("punching the coordinates for your next destination into the computer.\n");
    outroStrings.push_back("looking for other worlds to exploit.\n");
    outroStrings.push_back("knowing that a good cave story will get you free drinks at the bar.\n");

    std::uniform_int_distribution<uint32_t> outroStringsDistribution(0, outroStrings.size() - 1);
    
    std::string outroString = outroStrings[outroStringsDistribution(RNG)];


    int intTime = totalTime;
    int minutes = intTime / 60;
    int seconds = intTime % 60;
    mWinMessage = introString + "\nYou have won!\nYou completed the game in:\n" +
        std::to_string(minutes) +
        " minutes and " +
        std::to_string(seconds) +
        " seconds.\nYou killed " +
        std::to_string(enemiesKilled) +
        " enemies.\n\nAnd so you left, " + outroString;

}

WinScene::~WinScene() {
    Resources().FreeTexture2D(mPortalTexture);
}

void WinScene::Update(float deltaTime) {
    mElapsedTime += deltaTime;

    // Go to main scene when win screen is over.
    if (mElapsedTime > 10.f)
        Game::GetInstance().SetScene(new MenuScene());

    // Clear screen.
    glClear(GL_COLOR_BUFFER_BIT);

    // Render logo.
    glm::vec2 size = glm::vec2(static_cast<float>(mPortalTexture->GetWidth()), static_cast<float>(mPortalTexture->GetHeight()));
    glm::vec2 screenSize = MainWindow::GetInstance()->GetSize();

    // Scale logo down if larger than screen.
    if (size.x > screenSize.x || size.y > screenSize.y) {
        float xScale = screenSize.x / size.x;
        float yScale = screenSize.y / size.y;
        size *= xScale < yScale ? xScale : yScale;
    }

    // Fade in and out logo.
    float alpha = 0.5f;
    if (mElapsedTime > 3.5f) {
        alpha = 4.f - mElapsedTime;
    }
    else if (mElapsedTime > 0.5f) {
        alpha = 1.f;
    }
    else {
        alpha = mElapsedTime * 2.f;
    }

    mPortalTexture->Render((screenSize - size) * 0.5f, size, alpha);

    mFont->RenderText(mWinMessage.c_str(), glm::vec2(screenSize.x*0.10f, screenSize.y*0.10f), screenSize.x*0.9f, glm::vec2(screenSize.x, screenSize.y), true);
}
