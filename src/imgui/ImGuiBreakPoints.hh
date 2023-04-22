#ifndef IMGUI_BREAKPOINT_HH
#define IMGUI_BREAKPOINT_HH

#include "ImGuiPart.hh"

#include "TclObject.hh"

#include <optional>
#include <vector>

namespace openmsx {

class ImGuiManager;
class MSXCPUInterface;

struct ParsedSlotCond {
	ParsedSlotCond(std::string_view cond);
	[[nodiscard]] std::string toTclExpression() const;
	[[nodiscard]] std::string toDisplayString() const;

	std::string rest;

	int ps = 0;
	int ss = 0;
	uint8_t seg = 0;
	bool hasPs = false; // if false then the whole slot-substructure is invalid
	bool hasSs = false;
	bool hasSeg = false;
};

class ImGuiBreakPoints final : public ImGuiPart
{
	struct GuiBreakPoint {
		int id; // id > 0: exists also on the openMSX side
		        // id < 0: only exists on the GUI side
		bool wantEnable; // only really enabled if it's also valid
		std::optional<uint16_t> addr;
		TclObject addrStr;
		TclObject cond;
		TclObject cmd;
	};

public:
	ImGuiBreakPoints(ImGuiManager& manager);

	[[nodiscard]] zstring_view iniName() const override { return "breakpoints"; }
	void save(ImGuiTextBuffer& buf) override;
	void loadLine(std::string_view name, zstring_view value) override;
	void paint(MSXMotherBoard* motherBoard) override;

private:
	void paintTab(MSXCPUInterface& cpuInterface);
	void synchronize(MSXCPUInterface& cpuInterface);
	void checkSort();
	void drawRow(MSXCPUInterface& cpuInterface, int row, GuiBreakPoint& bp);
	bool editCondition(ParsedSlotCond& slot);

public:
	bool show = false;

private:
	ImGuiManager& manager;

	static inline int idCounter = 0;
	std::vector<GuiBreakPoint> guiBps;
	int selectedBpRow = -1;
};

} // namespace openmsx

#endif
