#include "Panel.h"
#include "Application.h"

#include "mmgr/mmgr.h"

using namespace Broken;
Panel::Panel(char* name) : name(name) {}

Panel::~Panel() {}

void Panel::OnOff() {
	enabled = !enabled;
}

bool Panel::IsEnabled() const {
	return enabled;
}
