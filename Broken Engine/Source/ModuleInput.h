#ifndef __BE_MODULE_INPUT_H__
#define __BE_MODULE_INPUT_H__

#include "Module.h"


#define MAX_MOUSE_BUTTONS 5
BE_BEGIN_NAMESPACE

enum KEY_STATE {
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class BROKEN_API ModuleInput : public Module {
public:

	ModuleInput(bool start_enabled = true);
	~ModuleInput();

	bool Init(json& file) override;
	update_status PreUpdate(float dt) override;
	bool CleanUp() override;

	KEY_STATE GetKey(int id) const {
		return keyboard[id];
	}

	KEY_STATE GetMouseButton(int id) const {
		return mouse_buttons[id];
	}

	int GetMouseX() const {
		return mouse_x;
	}

	int GetMouseY() const {
		return mouse_y;
	}

	int GetMouseWheel() const {
		return mouse_wheel;
	}

	int GetMouseXMotion() const {
		return mouse_x_motion;
	}

	int GetMouseYMotion() const {
		return mouse_y_motion;
	}

private:
	KEY_STATE* keyboard;
	KEY_STATE mouse_buttons[MAX_MOUSE_BUTTONS];
	int mouse_x;
	int mouse_y;
	int mouse_wheel;
	int mouse_x_motion;
	int mouse_y_motion;
	//int mouse_z_motion;
};

BE_END_NAMESPACE
#endif