#pragma once

template <typename... T> class UIObserver {
  public:
	virtual void onChanged(const T &... t) {}
};
