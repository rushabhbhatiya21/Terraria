#include "ui.h"
#include <math/color.h>
#include <input/input.h>
#include <window/window.h>
#include <algorithm>
#include <assets/assetManager.h>
#include <audio.h>
#include <rendering/IRenderCollector.h>
#include <rendering/types/coloredRect.h>
#include <rendering/types/circle.h>
#include <rendering/types/roundedRect.h>
#include <rendering/types/text.h>
#include <time/time.h>
#include <ui/textLayout.h>

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

void UIEngine::updateAndRender(const Engine::AssetManager& assetManager, Engine::IRenderCollector& collector)
{
	float screenW = Engine::getScreenWidth();
	float screenH = Engine::getScreenHeight();

	Engine::Rect oneButtonRectangle = {};
	oneButtonRectangle.width = screenW * .8f;
	oneButtonRectangle.height = screenH / (widgets.size() + 1);

	oneButtonRectangle.height = std::min(oneButtonRectangle.height, oneButtonRectangle.width / 8.f);

	oneButtonRectangle = placeRectangleCenterTop(oneButtonRectangle, screenW);
	oneButtonRectangle.y += oneButtonRectangle.height / 2.f;

	int fontSize = (int)(oneButtonRectangle.height * .5f);
	const float titleFontSize = std::max(22.0f, fontSize * 1.15f);
	const float buttonFontSize = std::max(18.0f, fontSize * 0.82f);
	const float subMenuFontSize = std::max(14.0f, fontSize * 0.64f);
	int widgetIndex = 0;

	auto submitTextWithShadow = [&](const std::string& text, float x, float y, float size, Engine::Color4f color)
		{
			Engine::Text shadow
			{
				{ x - size * 0.08f, y + size * 0.08f },
				Engine::TextLayout::Anchor::TopLeft,
				0.f,
				Engine::Black,
				text,
				size,
				1.f,
				&assetManager.defaultFont,
				&assetManager.defaultShader
			};

			Engine::Text main
			{
				{ x, y },
				Engine::TextLayout::Anchor::TopLeft,
				0.f,
				color,
				text,
				size,
				1.f,
				&assetManager.defaultFont,
				&assetManager.defaultShader
			};

			collector.submitText(shadow);
			collector.submitText(main);
		};

	auto submitPanel = [&](const Engine::Rect& rect, Engine::Color4f color)
		{
			Engine::RoundedRect panel
			{
				rect,
				{ 0,0 },
				5.f,
				0.f,
				color,
				assetManager.whiteTexture,
				assetManager.defaultShader
			};
			collector.submitRoundedRect(panel);
		};

	for (auto& widget : widgets)
	{
		Engine::Rect smallerRect = shrinkRectanglePercentage(oneButtonRectangle, .01f, .1f);

		auto drawCenteredText = [&](const std::string& text, Engine::Rect rect, float size, float yOffset = 0)
			{
				Engine::TextMetrics metrics = Engine::TextLayout::measureText(assetManager.defaultFont, text, size, 1.f);
				float textX = rect.x + (rect.width - metrics.size.x) / 2.f;
				float textY = rect.y + (rect.height - metrics.size.y) / 2.f + yOffset;
				submitTextWithShadow(text, textX, textY, size, Engine::White);
			};

		widget.isHovered = false;
		widget.isBeingClicked = false;
		widget.isReleased = false;

		if (Engine::checkCollisionPointRec(Engine::getMousePosition(), smallerRect))
		{
			widget.isHovered = true;

			if (Engine::isMouseButtonDown(Engine::MouseButton::Left))
			{
				widget.isBeingClicked = true;
			}

			if (Engine::isMouseButtonReleased(Engine::MouseButton::Left))
			{
				widget.isReleased = true;
			}
		}

		switch (widget.type)
		{
			case button:
			{
				const float clickOffset = .05f;
					Engine::Color4f clickColor = { 126,146,180,230 };
					Engine::Color4f defaultColor = { 88,100,126,218 };
				Engine::Rect buttonRect = smallerRect;

				if (widget.isBeingClicked)
				{
					buttonRect.y += buttonRect.height * clickOffset;
					submitPanel(buttonRect, clickColor);
				}
				else
				{
					if (widget.isHovered)
					{
						submitPanel(buttonRect, clickColor);
					}
					else if (widget.isReleased)
					{
						submitPanel(buttonRect, defaultColor);
					}
					else
					{
						submitPanel(buttonRect, defaultColor);
					}
				}

				if (widget.isBeingClicked)
				{
					drawCenteredText(widget.text, smallerRect, buttonFontSize, smallerRect.height * clickOffset);
				}
				else
				{
					drawCenteredText(widget.text, smallerRect, buttonFontSize);
				}

				if (widget.isReleased)
				{
					bool isBackButton = (widget.text == "Back");
					Audio::playUiButton(isBackButton);
				}

				break;
			}

			case slider:
			{
					Engine::Color4f panelColor = widget.isHovered
						? Engine::Color4f{ 80, 96, 126, 232 }
						: Engine::Color4f{ 68, 80, 106, 220 };

					Engine::Color4f railColor = { 38, 46, 62, 255 };
					Engine::Color4f fillColor = { 112, 182, 255, 255 };
					Engine::Color4f knobOuterColor = { 24, 28, 38, 255 };
					Engine::Color4f knobInnerColor = widget.isHovered ? Engine::White : Engine::Color4f{ 228,236,250,255 };

				submitPanel(smallerRect, panelColor);

				float sliderLeft = smallerRect.x + smallerRect.width * 0.08f;
				float sliderRight = smallerRect.x + smallerRect.width * 0.92f;
				float sliderWidth = std::max(1.0f, sliderRight - sliderLeft);
					float sliderY = smallerRect.y + smallerRect.height * 0.68f;
					float railH = std::max(5.0f, smallerRect.height * 0.12f);

				float value01 = 0.f;
				if (widget.sliderValue && widget.sliderMax > widget.sliderMin)
				{
					value01 = (*widget.sliderValue - widget.sliderMin) / (widget.sliderMax - widget.sliderMin);
					value01 = std::clamp(value01, 0.0f, 1.0f);
				}

				if (Engine::isMouseButtonPressed(Engine::MouseButton::Left) && widget.isHovered)
				{
					activeSlider = widgetIndex;
				}

				if (activeSlider == widgetIndex && Engine::isMouseButtonDown(Engine::MouseButton::Left) && widget.sliderValue)
				{
					float t = (Engine::getMousePosition().x - sliderLeft) / sliderWidth;
					t = std::clamp(t, 0.0f, 1.0f);
					*widget.sliderValue = widget.sliderMin + t * (widget.sliderMax - widget.sliderMin);
					value01 = t;
				}

				if (activeSlider == widgetIndex && Engine::isMouseButtonReleased(Engine::MouseButton::Left))
				{
					activeSlider = -1;
				}

				float visual01 = value01;
				if (widget.sliderValue)
				{
					auto it = sliderVisual01.find(widget.sliderValue);

					if (it == sliderVisual01.end())
					{
						sliderVisual01[widget.sliderValue] = value01;
						visual01 = value01;
					}
					else
					{
						// Smooth visual state so slider movement feels less abrupt.
						it->second += (value01 - it->second) * 0.22f;
						it->second = std::clamp(it->second, 0.0f, 1.0f);
						visual01 = it->second;
					}
				}

				if (widget.sliderValue)
				{
					float& lastValue = sliderLastSfxValue[widget.sliderValue];
					double& lastTime = sliderLastSfxTime[widget.sliderValue];
					double now = Engine::getTime();

					if (std::abs(value01 - lastValue) >= 0.03f && (now - lastTime) >= 0.06)
					{
						Audio::playUiSlider(0.4f);
						lastValue = value01;
						lastTime = now;
					}
				}

					Engine::RoundedRect rail
				{
					{ sliderLeft, sliderY - railH * 0.5f, sliderWidth, railH },
					{ 0,0 },
						railH * 0.5f,
						0.f,
					railColor,
					assetManager.whiteTexture,
					assetManager.defaultShader
				};
					collector.submitRoundedRect(rail);

					Engine::RoundedRect fill
				{
						{ sliderLeft, sliderY - railH * 0.5f, sliderWidth * visual01, railH },
					{ 0,0 },
						railH * 0.5f,
						0.f,
					fillColor,
					assetManager.whiteTexture,
					assetManager.defaultShader
				};
					collector.submitRoundedRect(fill);

				float knobX = sliderLeft + sliderWidth * visual01;
					float knobOuterRadius = std::max(7.0f, smallerRect.height * 0.17f);
					float knobInnerRadius = knobOuterRadius * 0.62f;

					Engine::Circle knobOuter
				{
					{ knobX, sliderY },
						knobOuterRadius,
					{ 0,0 },
					0.f,
						knobOuterColor,
					assetManager.whiteTexture,
					assetManager.defaultShader
				};
					collector.submitCircle(knobOuter);

					Engine::Circle knobInner
					{
						{ knobX, sliderY },
						knobInnerRadius,
						{ 0,0 },
						0.f,
						knobInnerColor,
						assetManager.whiteTexture,
						assetManager.defaultShader
					};
					collector.submitCircle(knobInner);

				int valuePct = (int)std::round(value01 * 100.0f);
					std::string label = widget.text;
					std::string valueText = std::to_string(valuePct) + "%";

					float labelSize = subMenuFontSize;
					Engine::TextMetrics labelMetrics = Engine::TextLayout::measureText(assetManager.defaultFont, label, labelSize, 1.f);
					Engine::TextMetrics valueMetrics = Engine::TextLayout::measureText(assetManager.defaultFont, valueText, labelSize, 1.f);

					float textY = smallerRect.y + smallerRect.height * 0.18f;
					float labelX = smallerRect.x + smallerRect.width * 0.08f;
					float valueX = smallerRect.x + smallerRect.width * 0.92f - valueMetrics.size.x;

					submitTextWithShadow(label, labelX, textY, labelSize, Engine::White);
					submitTextWithShadow(valueText, valueX, textY, labelSize, Engine::Color4f{ 200,230,255,255 });

				break;
			}

			case title:
			{
					drawCenteredText(widget.text, smallerRect, titleFontSize);
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
