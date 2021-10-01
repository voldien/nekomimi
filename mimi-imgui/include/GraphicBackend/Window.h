#ifndef ___DASDwd
#define ___DASDwd
#include <stdint.h>
#include <string>
/**
 * @brief
 *
 */
class Window {
  public:
	virtual void show() = 0;

	virtual void hide() = 0;

	virtual void close() = 0;

	virtual void focus() = 0;

	virtual void restore() = 0;

	virtual void maximize() = 0;

	virtual void minimize() = 0;

	virtual void setTitle(const std::string &title) = 0;

	virtual std::string getTitle() const = 0;

	// // TODO change the type to image.
	// virtual void setIcon(void *) = 0;
	// //		virtual void setIcon(Image* image) = 0;

	// virtual void *getIcon() const = 0;
	// //		virtual Image* setIcon(Image* image) = 0;

	// virtual void getPosition(int *x, int *y) const = 0;

	// virtual void setPosition(int x, int y) = 0;

	// virtual void setSize(int width, int height) = 0;

	// virtual void getSize(int *width, int *height) const = 0;

	// // virtual Display *getCurrentDisplay() const = 0;

	// virtual float getGamma() const = 0;

	// virtual void setGamma(float gamma) = 0;

	// virtual void resizable(bool resizable) = 0;

	// virtual void setFullScreen(bool fullscreen) = 0;
	// // virtual void setFullScreen(Display &display) = 0;

	// virtual bool isFullScreen() const = 0;

	// virtual void setBordered(bool borded) = 0;

	// virtual void setMinimumSize(int width, int height) = 0;
	// virtual void getMinimumSize(int *width, int *height) = 0;
	// virtual void setMaximumSize(int width, int height) = 0;
	// virtual void getMaximumSize(int *width, int *height) = 0;

	void setUserData(void *userData) noexcept { this->userData = userData; }

	virtual void *getUserData() const noexcept { return this->userData; }

	virtual intptr_t getNativePtr() const = 0; /*  Get native window reference object. */

  protected: /*  Internal utility methods.   */
			 // TODO determine how to handle it.
			 // virtual void calculateGammaLookupTable(float gamma, unsigned int *rgbRamp) const; /*  */
			 // virtual float computeGammaExponent(const unsigned int *rgbRamp) const;			  /*  */

  protected:
	void *userData = nullptr;
};

#endif
