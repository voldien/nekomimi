/*
 * Copyright (c) 2021 Valdemar Lindberg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef _MIMI_IMGUI_LIVEDATA_H_
#define _MIMI_IMGUI_LIVEDATA_H_ 1
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
#endif
