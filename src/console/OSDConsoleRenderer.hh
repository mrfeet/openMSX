#ifndef OSDCONSOLERENDERER_HH
#define OSDCONSOLERENDERER_HH

#include "GLImage.hh"
#include "Layer.hh"
#include "TTFFont.hh"
#include "EnumSetting.hh"
#include "IntegerSetting.hh"
#include "FilenameSetting.hh"
#include "Observer.hh"
#include "gl_vec.hh"
#include "openmsx.hh"
#include <list>
#include <memory>
#include <string_view>
#include <tuple>

namespace openmsx {

class BooleanSetting;
class CommandConsole;
class Display;
class Reactor;

class OSDConsoleRenderer final : public Layer, private Observer<Setting>
{
public:
	OSDConsoleRenderer(Reactor& reactor, CommandConsole& console,
	                   int screenW, int screenH);
	~OSDConsoleRenderer() override;

private:
	[[nodiscard]] int initFontAndGetColumns();
	[[nodiscard]] int getRows();

	// Layer
	void paint(OutputSurface& output) override;

	// Observer
	void update(const Setting& setting) noexcept override;

	void adjustColRow();
	void setActive(bool active);

	bool updateConsoleRect();
	void loadFont      (std::string_view value);
	void loadBackground(std::string_view value);
	byte getVisibility() const;
	void drawText(std::string_view text,
	              int cx, int cy, byte alpha, uint32_t rgb);
	[[nodiscard]] gl::ivec2 getTextPos(int cursorX, int cursorY) const;
	void drawConsoleText(byte visibility);

	[[nodiscard]] std::tuple<bool, GLImage*, unsigned> getFromCache(
		std::string_view text);
	void insertInCache(std::string text,
	                   std::unique_ptr<GLImage> image, unsigned width);
	void clearCache();

private:
	enum Placement {
		CP_TOP_LEFT,    CP_TOP,    CP_TOP_RIGHT,
		CP_LEFT,        CP_CENTER, CP_RIGHT,
		CP_BOTTOM_LEFT, CP_BOTTOM, CP_BOTTOM_RIGHT
	};

	struct TextCacheElement {
		TextCacheElement(std::string text_,
		                 std::unique_ptr<GLImage> image_, unsigned width_)
			: text(std::move(text_)), image(std::move(image_))
			, width(width_) {}

		std::string text;
		std::unique_ptr<GLImage> image;
		unsigned width; // in case of trailing whitespace width != image->getWidth()
	};
	using TextCache = std::list<TextCacheElement>;

	Reactor& reactor;
	Display& display;
	CommandConsole& console;
	BooleanSetting& consoleSetting;
	const int screenW;
	const int screenH;

	TTFFont font;
	TextCache textCache;
	TextCache::iterator cacheHint;

	EnumSetting<Placement> consolePlacementSetting;
	IntegerSetting fontSizeSetting;
	FilenameSetting fontSetting;
	IntegerSetting consoleColumnsSetting;
	IntegerSetting consoleRowsSetting;
	FilenameSetting backgroundSetting;
	std::unique_ptr<GLImage> backgroundImage;

	uint64_t lastBlinkTime;
	uint64_t activeTime{0};
	gl::ivec2 bgPos{}; // recalculate on first paint()
	gl::ivec2 bgSize{};
	unsigned lastCursorX{0};
	unsigned lastCursorY{0};
	bool blink{false};
	bool active{false};
};

} // namespace openmsx

#endif
