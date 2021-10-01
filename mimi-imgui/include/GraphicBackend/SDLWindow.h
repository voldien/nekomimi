
#ifndef _MIMI_IMGUI_WINDOW_H_
#define _MIMI_IMGUI_WINDOW_H_ 1
#include "Window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <string>

class SDLWindow : public Window {
  public:
	SDLWindow();

	virtual void show() override;

	virtual void hide() override;

	virtual void close() override;

	virtual void focus() override;

	virtual void restore() override;

	virtual void maximize() override;

	virtual void minimize() override;

	virtual void setTitle(const std::string &title) override;

	virtual std::string getTitle() const override;

	virtual int x() const noexcept;
	virtual int y() const noexcept;

	virtual int width() const noexcept;
	virtual int height() const noexcept;

	virtual void getPosition(int *x, int *y) const;

	virtual void setPosition(int x, int y) noexcept;

	virtual void setSize(int width, int height) noexcept;

	virtual void getSize(int *width, int *height) const;

	virtual void resizable(bool resizable) noexcept;

	virtual void setFullScreen(bool fullscreen);

	virtual bool isFullScreen() const;

	virtual void setBordered(bool borded);

	virtual void setMinimumSize(int width, int height);
	virtual void getMinimumSize(int *width, int *height);
	virtual void setMaximumSize(int width, int height);
	virtual void getMaximumSize(int *width, int *height);

	virtual intptr_t getNativePtr() const override; /*  Get native window reference object. */

  protected:
	SDL_Window *window;
};

#endif
