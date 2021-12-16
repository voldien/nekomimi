#pragma once
#include "UIObserver.h"
#include <functional>
#include <vector>

template <typename T> class LiveData {
  public:
	using Func = std::function<void(T)>;
	template <typename F> UIObserver<T> &getObserver(const F &lambda) { /**/
		auto ob = UIObserver<T>();
		observers.push_back(ob);
		return observers.back();
	}

	void postValue(const T &value) {
		this->value = value;
		this->observers[0].onChanged(value);
	}

	void setValue(T &value) {}
	T &getValue() { return this->value; }

  private:
	std::vector<UIObserver<T>> observers;

	T value;
};
