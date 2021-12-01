#pragma once

template <typename... T> class UIObserver {
  public:
	virtual void onChanged(T &... t) {}
};
