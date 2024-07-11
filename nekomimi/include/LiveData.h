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
#include <observable/observable.hpp>
#include <vector>

template <typename T> class LiveData : public observable::value<T> {
  public:
	void postValue(const T &value) { *this = value; }

	void setValue(const T &value) { this->set(value); }
	// void setValue(T &&value) { *this = value; }
	const T &getValue() const { return this->get(); }
};
#endif
