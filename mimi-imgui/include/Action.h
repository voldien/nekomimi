#pragma once
#include <cstdio>
#include <functional>

template <typename Return> class Action {
  public:
	template <typename... Args> void invoke(Args &&... args) { _callback(args); }

  private:
	std::function<void(void)> _callback = 0;
};
