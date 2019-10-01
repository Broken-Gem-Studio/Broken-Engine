#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "Source/SDL/include/SDL.h"
#include "JSONLoader.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init(json file);
	bool Start() override;
	bool CleanUp();

	void SetTitle(const char* title);
	void SetWindowWidth(uint width);
	void SetWindowHeight(uint height);
	uint GetWindowWidth();
	uint GetWindowHeight();
	void UpdateWindowSize() const;
	
	uint GetDisplayRefreshRate();

	void SetFullscreen(bool value);
	void SetResizable(bool value);
	void SetBorderless(bool value);
	void SetFullscreenDesktop(bool value);
	void SetWinBrightness(float value);

	// --- Only App should access this, through SetAppName ---
	void SetWinTitle(const char* name);

	float GetWinBrightness() const;

	bool IsFullscreen();
	bool IsResizable();
	bool IsBorderless();
	bool IsFullscreenDesktop();

	void GetWinMaxMinSize(uint & min_width, uint & min_height, uint & max_width, uint & max_height) const;

	void SaveStatus(json &file) const override;
	void LoadStatus(const json & file) override;

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:

	// --- Display ---
	uint				RefreshRate = 0;
	uint				screen_width = 1280;
	uint				screen_height = 1024;
	uint				display_Width = 0;
	uint				display_Height = 0;

	// --- Win Flags ---
	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fullscreen_desktop = false;
};

#endif // __ModuleWindow_H__