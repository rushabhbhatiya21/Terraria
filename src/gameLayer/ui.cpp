#include "ui.h"
#include <math/color.h>
#include <input/input.h>
#include <window/window.h>
#include <raylib.h>
#include <algorithm>

static Color toRaylibColor(Engine::Color4f c)
{
	return Color{ c.r, c.g, c.b, c.a };
}

// top
Engine::Rect placeRectangleTopLeft(Engine::Rect r)
{
	r.x = 0;
	r.y = 0;
	return r;
}

Engine::Rect placeRectangleCenterTop(Engine::Rect r, float w)
{
	r.x = (w - r.width) / 2.0f;
	r.y = 0;
	return r;
}

Engine::Rect placeReactangleTopRightCorner(Engine::Rect r, float w)
{
	r.x = w - r.width;
	r.y = 0;
	return r;
}

// center
Engine::Rect placeRectangleCenter(Engine::Rect r, float w, float h)
{
	r.x = (w - r.width) * 0.5f;
	r.y = (h - r.height) * 0.5f;
	return r;
}

Engine::Rect placeRectangleCenterLeft(Engine::Rect r, float h)
{
	r.x = 0;
	r.y = (h - r.height) * 0.5f;
	return r;
}

Engine::Rect placeRectangleCenterRight(Engine::Rect r, float w, float h)
{
	r.x = (w - r.width) / 2.0f;
	r.y = (h - r.height) * 0.5f;
	return r;
}

// bottom
Engine::Rect placeRectangleBottomLeftCorner(Engine::Rect r, float h)
{
	r.x = 0;
	r.y = h - r.height;
	return r;
}

Engine::Rect placeRectangleBottom(Engine::Rect r, float w, float h)
{
	r.x = (w - r.width) * 0.5f;
	r.y = h - r.height;
	return r;
}

Engine::Rect placeRectangleBottomRightCorner(Engine::Rect r, float w, float h)
{
	r.x = w - r.width;
	r.y = h - r.height;
	return r;
}

// enlarge and shrink
Engine::Rect enlargeRectanglePixels(Engine::Rect r, float pixelsX, float pixelsY)
{
	r.width += pixelsX;
	r.height += pixelsY;

	r.x -= pixelsX / 2.f;
	r.y -= pixelsY / 2.f;
	return r;
}

Engine::Rect enlargeRectanglePercentage(Engine::Rect r, float percentageX, float percentageY)
{
	float enlargeX = r.width * percentageX;
	float enlargeY = r.height * percentageY;

	r.width += enlargeX;
	r.height += enlargeY;

	r.x -= enlargeX / 2.f;
	r.y -= enlargeY / 2.f;

	return r;
}

Engine::Rect shrinkRectanglePercentage(Engine::Rect r, float percentageX, float percentageY)
{
	float shrinkX = r.width * percentageX;
	float shrinkY = r.height * percentageY;

	r.width -= shrinkX;
	r.height -= shrinkY;

	r.x += shrinkX / 2.f;
	r.y += shrinkY / 2.f;
	return r;
}

void UIEngine::init()
{
	//font.loadDefault();
}

void UIEngine::updateAndRender()
{
	float w = Engine::getScreenWidth();
	float h = Engine::getScreenHeight();

	Engine::Rect oneButtonRectangle = {};
	oneButtonRectangle.width = w * .8f;
	oneButtonRectangle.height = h / (widgets.size() + 1);

	oneButtonRectangle.height = std::min(oneButtonRectangle.height, oneButtonRectangle.width / 8.f);

	oneButtonRectangle = placeRectangleCenterTop(oneButtonRectangle, w);
	oneButtonRectangle.y += oneButtonRectangle.height / 2.f;

	int fontSize = (int)(oneButtonRectangle.height * .5f);
	int widgetIndex = 0;

	for (auto& w : widgets)
	{
		Engine::Rect smallerRect = shrinkRectanglePercentage(oneButtonRectangle, .01f, .1f);

		auto drawText = [&](Engine::Rect smallerRect, float yOffset = 0)
			{
				int textWidth = MeasureText(w.text.c_str(), fontSize);
				int textHeight = fontSize; // in raylib font height = font size default
				float textX = smallerRect.x + (smallerRect.width - textWidth) / 2.f;
				float textY = smallerRect.y + (smallerRect.height - textHeight) / 2.f;

				Engine::Color4f shadowColor = { 0,0,0,255 };
				DrawText(w.text.c_str(), (int)(textX - fontSize * .08f), (int)(textY + fontSize * .08f + yOffset), fontSize, toRaylibColor(shadowColor));
				DrawText(w.text.c_str(), (int)textX, (int)(textY + yOffset), fontSize, toRaylibColor(Engine::White));
			};

		w.isHovered = false;
		w.isBeingClicked = false;
		w.isReleased = false;

		if (Engine::checkCollisionPointRec(Engine::getMousePosition(), smallerRect))
		{
			w.isHovered = true;

			if (Engine::isMouseButtonDown(Engine::MouseButton::Left))
			{
				w.isBeingClicked = true;
			}

			if (Engine::isMouseButtonReleased(Engine::MouseButton::Left))
			{
				w.isReleased = true;
			}
		}

		switch (w.type)
		{
			case button:
			{
				const float clickOffset = .05f;
				Engine::Color4f clickColor = { 120,120,135,205 };
				Engine::Color4f defaultColor = { 90,90,110,205 };

				if (w.isBeingClicked)
				{
						DrawRectangle(
							(int)smallerRect.x,
							(int)(smallerRect.y + smallerRect.height * clickOffset),
							(int)smallerRect.width,
							(int)smallerRect.height,
							toRaylibColor(clickColor)
						);
				}
				else
				{
					if (w.isHovered)
					{
							DrawRectangle(
								(int)smallerRect.x,
								(int)smallerRect.y,
								(int)smallerRect.width,
								(int)smallerRect.height,
								toRaylibColor(clickColor)
							);
					}

					if (w.isReleased)
					{
							DrawRectangle(
								(int)smallerRect.x,
								(int)smallerRect.y,
								(int)smallerRect.width,
								(int)smallerRect.height,
								toRaylibColor(defaultColor)
							);
					}

						if (!w.isHovered && !w.isReleased)
						{
							DrawRectangle(
								(int)smallerRect.x,
								(int)smallerRect.y,
								(int)smallerRect.width,
								(int)smallerRect.height,
								toRaylibColor(defaultColor)
							);
						}
				}

				if (w.isBeingClicked)
				{
					drawText(smallerRect, smallerRect.height * clickOffset);
				}
				else
				{
					drawText(smallerRect);
				}

				break;
			}

			case slider:
			{
				Engine::Color4f panelColor = { 90,90,110,205 };
				Engine::Color4f railColor = { 55,55,70,255 };
				Engine::Color4f fillColor = { 110,175,255,255 };
				Engine::Color4f knobColor = w.isHovered ? Engine::White : Engine::Color4f{ 225,225,225,255 };

				DrawRectangle(
					(int)smallerRect.x,
					(int)smallerRect.y,
					(int)smallerRect.width,
					(int)smallerRect.height,
					toRaylibColor(panelColor)
				);

				float sliderLeft = smallerRect.x + smallerRect.width * 0.08f;
				float sliderRight = smallerRect.x + smallerRect.width * 0.92f;
				float sliderWidth = std::max(1.0f, sliderRight - sliderLeft);
				float sliderY = smallerRect.y + smallerRect.height * 0.62f;
				float railH = std::max(4.0f, smallerRect.height * 0.10f);

				float value01 = 0.f;
				if (w.sliderValue && w.sliderMax > w.sliderMin)
				{
					value01 = (*w.sliderValue - w.sliderMin) / (w.sliderMax - w.sliderMin);
					value01 = std::clamp(value01, 0.0f, 1.0f);
				}

				if (Engine::isMouseButtonPressed(Engine::MouseButton::Left) && w.isHovered)
				{
					activeSlider = widgetIndex;
				}

				if (activeSlider == widgetIndex && Engine::isMouseButtonDown(Engine::MouseButton::Left) && w.sliderValue)
				{
					float t = (Engine::getMousePosition().x - sliderLeft) / sliderWidth;
					t = std::clamp(t, 0.0f, 1.0f);
					*w.sliderValue = w.sliderMin + t * (w.sliderMax - w.sliderMin);
					value01 = t;
				}

				if (activeSlider == widgetIndex && Engine::isMouseButtonReleased(Engine::MouseButton::Left))
				{
					activeSlider = -1;
				}

				DrawRectangle(
					(int)sliderLeft,
					(int)(sliderY - railH * 0.5f),
					(int)sliderWidth,
					(int)railH,
					toRaylibColor(railColor)
				);

				DrawRectangle(
					(int)sliderLeft,
					(int)(sliderY - railH * 0.5f),
					(int)(sliderWidth * value01),
					(int)railH,
					toRaylibColor(fillColor)
				);

				float knobX = sliderLeft + sliderWidth * value01;
				float knobRadius = std::max(5.0f, smallerRect.height * 0.14f);
				DrawCircle((int)knobX, (int)sliderY, knobRadius, toRaylibColor(knobColor));

				int valuePct = (int)std::round(value01 * 100.0f);
				std::string label = w.text + "  " + std::to_string(valuePct) + "%";

				int labelSize = std::max(14, (int)(fontSize * 0.75f));
				int textWidth = MeasureText(label.c_str(), labelSize);
				float textX = smallerRect.x + (smallerRect.width - textWidth) / 2.f;
				float textY = smallerRect.y + smallerRect.height * 0.18f;

				DrawText(label.c_str(), (int)(textX - labelSize * .08f), (int)(textY + labelSize * .08f), labelSize, toRaylibColor(Engine::Black));
				DrawText(label.c_str(), (int)textX, (int)textY, labelSize, toRaylibColor(Engine::White));

				break;
			}

			case title:
			{
				drawText(smallerRect);
				break;
			}

			default:
				break;
		}

		oneButtonRectangle.y += oneButtonRectangle.height;
		widgetIndex++;
	}

	if (!Engine::isMouseButtonDown(Engine::MouseButton::Left))
	{
		activeSlider = -1;
	}

	bool disableInputThisFrame = false;

	if (widgets.size() != lastFrameWidgets.size())
	{
		disableInputThisFrame = true;
	}
	else
	{
		for (int i = 0; i < widgets.size(); i++)
		{
			if (widgets[i].type != lastFrameWidgets[i].type)
			{
				disableInputThisFrame = true;
				break;
			}
		}
	}

	lastFrameWidgets = widgets;

	if (disableInputThisFrame)
	{
		for (auto& w : widgets)
		{
			w.isHovered = false;
			w.isReleased = false;
			w.isBeingClicked = false;
		}
	}

	widgets.clear();
}
