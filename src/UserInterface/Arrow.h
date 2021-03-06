/**
 * Basic header/include file for Arrow.cpp.
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

#ifndef ARROW_H
#define ARROW_H

#include "../EasingFunctions.h"
#include "../enums.h"
#include "../Rectangle.h"
#include "../Image.h"

class Arrow
{
public:
    static void Initialize(Image *pHorizontalImage, Image *pVerticalImage);

    Arrow()
    {
        this->xPosition = 0;
        this->yPosition = 0;
        this->direction = ArrowDirectionUp;

        width = 0;
        height = 0;

        this->bounceDistance = 0;

        this->isClickable = 0;
        isEnabled = false;

        arrowOffset = 0;
        pArrowOffsetEase = NULL;

        isMouseDown = false;
        isMouseOver = false;
        isClicked = false;
    }

    Arrow(int xPosition, int yPosition, ArrowDirection direction, int bounceDistance, bool isClickable);
    Arrow(const Arrow &other);

    ~Arrow()
    {
        delete pArrowOffsetEase;
        pArrowOffsetEase = NULL;
    }

    bool GetIsEnabled() const { return this->isEnabled; }
    void SetIsEnabled(bool isEnabled) { this->isEnabled = isEnabled; }

    bool GetIsClicked() const { return this->isClicked; }
    void SetIsClicked(bool isClicked) { this->isClicked = isClicked; }

    static string GetClickSoundEffect() { return "ButtonClick2"; }

    void Update(int delta);
    void UpdateAnimation(int delta);
    void UpdateState();
    void Draw();
    void Draw(double xOffset);
    void Draw(double xOffset, double yOffset);
    void Reset();

private:
    static Image *pHorizontalImage;
    static Image *pVerticalImage;

    int xPosition;
    int yPosition;
    int width;
    int height;
    ArrowDirection direction;
    int bounceDistance;
    RectangleWH hitboxRect;

    bool isClickable;
    bool isMouseOver;
    bool isMouseDown;

    double arrowOffset;
    GravityBounceEase *pArrowOffsetEase;

    bool isEnabled;
    bool isClicked;
};

#endif
