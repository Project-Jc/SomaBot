#include "stdafx.h"
#include "MyGUI.h"

MyGUI::Gui* mGUI;

void Init(void) {
	mGUI = new MyGUI::Gui();
	mGUI->initialise();
}
