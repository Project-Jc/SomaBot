
struct Border {
	BYTE Top, Bottom, Left, Right;
	Border(int top, int bottom, int left, int right) : 
		Top(top), Bottom(bottom), Left(left), Right(right) {}
};

enum class Purpose {
	TEST_SOUND,
	REPAIR_ALL,

	CFG_NEW,
	CFG_LOAD,
	CFG_SAVE,
	CFG_SCROLL,

	SELECT_MODE,
	SELECT_BEHAVIOUR,
	SELECT_AURA,

	MODIFY_PERCENT_HP,
	MODIFY_PERCENT_MP,
	MODIFY_PERCENT_HEAL,
	MODIFY_PERCENT_DURA,
	MODIFY_VALUE_AURAREUSE,
	MODIFY_VALUE_SEARCHRANGE,
	MODIFY_VALUE_APPROACHTIMEOUT,
	MODIFY_VALUE_ATTACKTIMEOUT,
	MODIFY_VALUE_LOOTTIMEOUT,
	MODIFY_VALUE_CASTREDUCTION,
	MODIFY_VALUE_DEATHLIMIT,

	TOGGLE_HEALENABLE,
	TOGGLE_AUTOSERUM,
	TOGGLE_RANDOMIZATION,
	TOGGLE_NEARBYNOTIFY,
	TOGGLE_DAMAGEOVERHEAD,
	TOGGLE_DRAWINFO,
	TOGGLE_NOLOOT,
	TOGGLE_NOWARP,
	TOGGLE_IGNOREPK,
	TOGGLE_REGAINSTAM,
	TOGGLE_CHATNOTIFY,
	TOGGLE_CHATRESPOND,
	TOGGLE_GMLIVENOTIFY,
};

class Gui {
private:
	static BOOL IsShowing;

	static CString Version;

	static RECT Position;
	static RECT Extras;
	static RECT Config;
	static RECT Program;

	static unordered_map<Purpose, RECT> Controls;
public:

	static void Draw(HDC hDC);

	static bool MouseIsOver(POINT Mouse) {
		if (Mouse.x >= Position.left && Mouse.x <= (Position.left + Position.right) && Mouse.y >= Position.top && Mouse.y <= (Position.top + Position.bottom))
			return true;
		return false;
	}

	static void ProcessInput(POINT Mouse, BOOL Scroll, BOOL Increase);

	static void NewControl(Purpose Purpose, RECT Location) {
		for (auto xt = Controls.begin(); xt != Controls.end(); xt++) {
			if (xt->first == Purpose) {
				return;
			}
		} Controls.emplace(Purpose, Location);
		//printf_s("Added control [%d] at [%d %d]\n", Purpose, Location.left, Location.top);
	}

	static void UpdateUiPosition(RECT newPosition) {
		Position = newPosition;
	}

	static void Show(void) {
		IsShowing = true;
	}

	static void Hide(void) {
		IsShowing = false;
	}

	static bool Visible(void) {
		return IsShowing;
	}

	static void Toggle(void) {
		IsShowing = !IsShowing;
	}

};
