/**
 * The main title screen of the game.
 *
 * @author GabuEx, dawnmew
 * @since 1.0
 *
 * Licensed under the MIT License.
 *
 * Copyright (c) 2014 Equestrian Dreamers
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "TitleScreen.h"
#include "../FileFunctions.h"
#include "../globals.h"
#include "../ResourceLoader.h"
#include "../CaseInformation/CommonCaseResources.h"

const string CandleAnimationId = "TitleScreenCandleAnimation";
const char CandleSpriteIdFormat[] = "TitleScreenCandleSprite%02d";
const string CandleSpriteSheetId = "TitleScreenCandleSpriteSheet";
const int CandleDimensionsPx = 32;
const int LongFadeFromBlackDurationMs = 1000;
const int FadeFromBlackDurationMs = 300;
const int ButtonAreaStartHeight = 155;
const int ButtonSpacing = 14;

TitleScreen::TitleScreen()
{
    pBackgroundSprite = NULL;
    pTitleSprite = NULL;
    pFadeInSprite = NULL;

    fadeOpacity = 0;
    pLongFadeInEase = new LinearEase(1, 0, LongFadeFromBlackDurationMs);
    pFadeInEase = new LinearEase(1, 0, FadeFromBlackDurationMs);
    pFadeOutEase = new LinearEase(0, 1, FadeFromBlackDurationMs);

    pSpikeVideo = NULL;
    pCandleAnimation = NULL;
    pTwilightVideo = NULL;

    pNewGameButton = NULL;
    pLoadGameButton = NULL;
    pOptionsButton = NULL;
    pSelectLanguageButton = NULL;
    pExitButton = NULL;

    pNoCasesPrompt = new PromptOverlay("TitleScreen/NoCasesText", false /* allowsTextEntry */);
    pNoCasesPrompt->AddButton("OK");
    pNoCasesPrompt->FinalizeButtons();

    finishedLoadingAnimations = false;

    EventProviders::GetTextButtonEventProvider()->ClearListener(this);
    EventProviders::GetTextButtonEventProvider()->RegisterListener(this);
}

TitleScreen::~TitleScreen()
{
    delete pBackgroundSprite;
    pBackgroundSprite = NULL;
    delete pTitleSprite;
    pTitleSprite = NULL;
    delete pFadeInSprite;
    pFadeInSprite = NULL;

    delete pLongFadeInEase;
    pLongFadeInEase = NULL;
    delete pFadeInEase;
    pFadeInEase = NULL;
    delete pFadeOutEase;
    pFadeOutEase = NULL;

    delete pNewGameButton;
    pNewGameButton = NULL;
    delete pLoadGameButton;
    pLoadGameButton = NULL;
    delete pOptionsButton;
    pOptionsButton = NULL;
    delete pSelectLanguageButton;
    pSelectLanguageButton = NULL;
    delete pExitButton;
    pExitButton = NULL;

    delete pNoCasesPrompt;
    pNoCasesPrompt = NULL;

    delete pSpikeVideo;
    pSpikeVideo = NULL;
    delete pTwilightVideo;
    pTwilightVideo = NULL;

    EventProviders::GetTextButtonEventProvider()->ClearListener(this);
}

void TitleScreen::LoadResources()
{
    finishedLoadingAnimations = false;

    MLIFont *pTitleScreenFont = CommonCaseResources::GetInstance()->GetFontManager()->GetFontFromId("MenuFont");

    delete pBackgroundSprite;
    pBackgroundSprite = ResourceLoader::GetInstance()->LoadImage("image/TitleScreen/Background.png");
    delete pTitleSprite;
    pTitleSprite = ResourceLoader::GetInstance()->LoadImage("image/TitleScreen/GameTitle.png");
    delete pFadeInSprite;
    pFadeInSprite = ResourceLoader::GetInstance()->LoadImage("image/TitleScreen/FadeInBackground.png");

    delete pNewGameButton;
    pNewGameButton = new TextButton("TitleScreen/NewGameText", pTitleScreenFont);
    delete pLoadGameButton;
    pLoadGameButton = new TextButton("TitleScreen/LoadGameText", pTitleScreenFont);
    delete pOptionsButton;
    pOptionsButton = new TextButton("TitleScreen/OptionsText", pTitleScreenFont);
    delete pSelectLanguageButton;
    pSelectLanguageButton = new TextButton("TitleScreen/SelectLanguageText", pTitleScreenFont);
    delete pExitButton;
    pExitButton = new TextButton("TitleScreen/ExitText", pTitleScreenFont);

    int totalButtonHeight =
        pNewGameButton->GetHeight() +
        pLoadGameButton->GetHeight() +
        pOptionsButton->GetHeight() +
        pSelectLanguageButton->GetHeight() +
        pExitButton->GetHeight() +
        ButtonSpacing * 2 + 12;

    int currentButtonHeight = ButtonAreaStartHeight + (gScreenHeight - ButtonAreaStartHeight - totalButtonHeight) / 2;

    pNewGameButton->SetX(gScreenWidth / 2, HAlignmentCenter);
    pNewGameButton->SetY(currentButtonHeight);
    currentButtonHeight += pNewGameButton->GetHeight() + ButtonSpacing;

    pLoadGameButton->SetX(gScreenWidth / 2, HAlignmentCenter);
    pLoadGameButton->SetY(currentButtonHeight);
    currentButtonHeight += pLoadGameButton->GetHeight() + ButtonSpacing;

    pOptionsButton->SetX(gScreenWidth / 2, HAlignmentCenter);
    pOptionsButton->SetY(currentButtonHeight);
    currentButtonHeight += pOptionsButton->GetHeight() + ButtonSpacing;

    pSelectLanguageButton->SetX(gScreenWidth / 2, HAlignmentCenter);
    pSelectLanguageButton->SetY(currentButtonHeight);
    currentButtonHeight += pSelectLanguageButton->GetHeight() + ButtonSpacing;

    pExitButton->SetX(gScreenWidth / 2, HAlignmentCenter);
    pExitButton->SetY(currentButtonHeight);
    currentButtonHeight += pExitButton->GetHeight() + ButtonSpacing;

    delete pSpikeVideo;
    pSpikeVideo = new Video(true /* shouldLoop */);
    pSpikeVideo->SetVideoAttributes("video/TitleScreenSpike.mov", 95, 83, 240, 200);
    pSpikeVideo->LoadFile();
    delete pTwilightVideo;
    pTwilightVideo = new Video(true /* shouldLoop */);
    pTwilightVideo->SetVideoAttributes("video/TitleScreenTwilight.mov", 125, 42, 256, 360);
    pTwilightVideo->LoadFile();

    if (CommonCaseResources::GetInstance()->GetAnimationManager()->GetAnimationFromId(CandleAnimationId) == NULL)
    {
        Image *pCandleSpriteSheet = ResourceLoader::GetInstance()->LoadImage("image/TitleScreen/CandleSpriteSheet.png");
        CommonCaseResources::GetInstance()->GetSpriteManager()->AddImage(CandleSpriteSheetId, pCandleSpriteSheet);
        CommonCaseResources::GetInstance()->GetAnimationManager()->AddAnimation(CandleAnimationId, &pCandleAnimation);

        for (int i = 0; i < 47; i++)
        {
            char spriteId[256] = { '\0' };

            snprintf(&spriteId[0], 256, CandleSpriteIdFormat, i + 1);
            string spriteIdString = string(spriteId);

            int x = (i % 16) * CandleDimensionsPx;
            int y = (i / 16) * CandleDimensionsPx;

            CommonCaseResources::GetInstance()->GetSpriteManager()->AddSprite(spriteIdString, CandleSpriteSheetId, RectangleWH(x, y, CandleDimensionsPx, CandleDimensionsPx));
            pCandleAnimation->AddFrame(83, spriteIdString);
        }
    }

    finishedLoadingAnimations = true;
}

void TitleScreen::UnloadResources()
{
    finishedLoadingAnimations = false;

    delete pBackgroundSprite;
    pBackgroundSprite = NULL;
    delete pTitleSprite;
    pTitleSprite = NULL;
    delete pFadeInSprite;
    pFadeInSprite = NULL;

    delete pNewGameButton;
    pNewGameButton = NULL;
    delete pLoadGameButton;
    pLoadGameButton = NULL;
    delete pOptionsButton;
    pOptionsButton = NULL;
    delete pSelectLanguageButton;
    pSelectLanguageButton = NULL;
    delete pExitButton;
    pExitButton = NULL;

    delete pSpikeVideo;
    pSpikeVideo = NULL;
    delete pTwilightVideo;
    pTwilightVideo = NULL;
}

void TitleScreen::Init()
{
    MLIScreen::Init();

    if (SaveFileExists())
    {
        pLoadGameButton->SetIsEnabled(true);
    }
    else
    {
        pLoadGameButton->SetIsEnabled(false);
    }

    fadeOpacity = 1;

    if (MLIScreen::GetLastScreenId() == LOGO_SCREEN_ID ||
        MLIScreen::GetLastScreenId() == GAME_SCREEN_ID)
    {
        CommonCaseResources::GetInstance()->GetAudioManager()->PlayBgmWithId("MenuBGM");
        pLongFadeInEase->Begin();
        pFadeInEase->Reset();
    }
    else
    {
        pLongFadeInEase->Reset();
        pFadeInEase->Begin();
    }

    CommonCaseResources::GetInstance()->GetAudioManager()->PlayAmbianceWithId("MenuAmbiance");

    pFadeOutEase->Reset();

    while (!finishedLoadingAnimations)
    {
        SDL_Delay(1);
    }

    pSpikeVideo->Begin();
    pCandleAnimation->Begin();
    pTwilightVideo->Begin();
}

void TitleScreen::Update(int delta)
{
    if (!finishedLoadingAnimations)
    {
        return;
    }

    if (!pBackgroundSprite->IsReady() ||
        !pTitleSprite->IsReady() ||
        !pFadeInSprite->IsReady() ||
        !pNewGameButton->IsReady() ||
        !pLoadGameButton->IsReady() ||
        !pOptionsButton->IsReady() ||
        !pSelectLanguageButton->IsReady() ||
        !pExitButton->IsReady() ||
        !pSpikeVideo->IsReady() ||
        !pCandleAnimation->IsReady() ||
        !pTwilightVideo->IsReady())
    {
        return;
    }

    if (pFadeOutEase->GetIsStarted() && !pFadeOutEase->GetIsFinished())
    {
        pFadeOutEase->Update(delta);
        fadeOpacity = pFadeOutEase->GetCurrentValue();
    }
    else if (pLongFadeInEase->GetIsStarted() && !pLongFadeInEase->GetIsFinished())
    {
        pLongFadeInEase->Update(delta);
        fadeOpacity = pLongFadeInEase->GetCurrentValue();
    }
    else if (pFadeInEase->GetIsStarted() && !pFadeInEase->GetIsFinished())
    {
        pFadeInEase->Update(delta);
        fadeOpacity = pFadeInEase->GetCurrentValue();
    }

    if (pFadeOutEase->GetIsFinished())
    {
        CommonCaseResources::GetInstance()->GetAudioManager()->StopCurrentAmbiance(false /* isInstant */);

        isFinished = true;
        return;
    }

    pSpikeVideo->Update(delta);
    pCandleAnimation->Update(delta);
    pTwilightVideo->Update(delta);

    if (pNoCasesPrompt->GetIsShowing())
    {
        pNoCasesPrompt->Update(delta);
    }
    else if (!pFadeOutEase->GetIsStarted())
    {
        pNewGameButton->Update(delta);
        pLoadGameButton->Update(delta);
        pOptionsButton->Update(delta);
        pSelectLanguageButton->Update(delta);
        pExitButton->Update(delta);
    }
}

void TitleScreen::Draw()
{
    if (!finishedLoadingAnimations)
    {
        return;
    }

    if (!pBackgroundSprite->IsReady() ||
        !pTitleSprite->IsReady() ||
        !pFadeInSprite->IsReady() ||
        !pNewGameButton->IsReady() ||
        !pLoadGameButton->IsReady() ||
        !pOptionsButton->IsReady() ||
        !pSelectLanguageButton->IsReady() ||
        !pExitButton->IsReady())
    {
        return;
    }

    // If the animations aren't ready, we've got a problem.
    // Let's load textures until they're ready.
    while (!pCandleAnimation->IsReady())
    {
        ResourceLoader::GetInstance()->TryLoadOneImageTexture();
        SDL_Delay(1);
    }

    pBackgroundSprite->Draw(Vector2(0, 0));

    pSpikeVideo->Draw(Vector2(33, 321));
    pCandleAnimation->Draw(Vector2(652, 266));
    pTwilightVideo->Draw(Vector2(707, 192));

    pTitleSprite->Draw(Vector2(250, 50));

    pNewGameButton->Draw();
    pLoadGameButton->Draw();
    pOptionsButton->Draw();
    pSelectLanguageButton->Draw();
    pExitButton->Draw();

    if (pNoCasesPrompt->GetIsShowing())
    {
        pNoCasesPrompt->Draw();
    }

    pFadeInSprite->Draw(Vector2(0, 0), Color(fadeOpacity, 1, 1, 1));

}

void TitleScreen::OnButtonClicked(TextButton *pSender)
{
    if (pSender == pNewGameButton)
    {
        if (GetCaseFilePaths().size() == 0)
        {
            pNoCasesPrompt->Begin();
            return;
        }
        else
        {
            nextScreenId = CASE_SELECTION_SCREEN_ID;
        }
    }
    else if (pSender == pLoadGameButton)
    {
        nextScreenId = LOAD_SCREEN_ID;
    }
    else if (pSender == pOptionsButton)
    {
        nextScreenId = OPTIONS_SCREEN_ID;
    }
    else if (pSender == pSelectLanguageButton)
    {
        nextScreenId = LANGUAGE_SCREEN_ID;
    }
    else if (pSender == pExitButton)
    {
        nextScreenId = "";
    }
    else
    {
        return;
    }

    delete pFadeOutEase;
    pFadeOutEase = new LinearEase(fadeOpacity, 1, (int)((1 - fadeOpacity) * FadeFromBlackDurationMs));
    pFadeOutEase->Begin();
}
