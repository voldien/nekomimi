#ifndef _MIMI_MIMI_WINDOW_H_
#define _MIMI_MIMI_WINDOW_H_ 1
#include"GraphicBackend/WindowBackend.h"

/**
 * @brief
 *
 */
class MIMIWindow : public WindowBackend {
public:
  MIMIWindow(GfxBackEnd backend);

protected:
  virtual void displayMenuBar(void);
  virtual void renderUI();

  virtual void addUIComponent(const std::shared_ptr<UIComponent> &component);
};

#endif
