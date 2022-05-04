/*
 *	NekoMimi - Window ImGUI Framework
 *	Copyright (C) 2021  Valdemar Lindberg
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _NEKO_MIMI_LIVEDATA_H_
#define _NEKO_MIMI_LIVEDATA_H_ 1
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

	void setValue(const T &value) {
		this->value = value;
		this->observers[0].onChanged(value);
	}
	void setValue(T &&value) { this->value = value; }
	T &getValue() { return this->value; }

  private:
	std::vector<UIObserver<T>> observers;

	T value;
};
#endif
