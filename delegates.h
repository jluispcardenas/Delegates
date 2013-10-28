/************************************************************************
  delegate.h
  
  Copyright (C) Jose Luis P. Cardenas (jluis.pcardenas@gmail.com)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of the
  License, or (at your option) any later version.
  
  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with this program. If not, see
  <http://www.gnu.org/licenses/>.

************************************************************************/

#ifndef _DELEGATE_H_
#define _DELEGATE_H_
#include <assert.h>

#define int(a,b) a, b, int
#define void(a,b) a, b, void
#define GQUEUE(ep) if (hp==NULL) hp = tp = ep; \
					else {tp->next = ep; tp = ep;}

class Generic {};

template <class P1, class P2, class R=void>
class delegate {
public:
	delegate() {
		hp = tp = NULL;
	}

	template <class T, class F>
	inline void bind(T *a, R (F::*fc)(P1, P2)) {
		a = static_cast<F*>(a);
		Callback<P1, P2, R> *n = new Callback<P1, P2, R>;
		n->attach(a, fc);
		GQUEUE(n);
	}

	inline void bind(R (*fc)(P1, P2)) {
		Callback<P1, P2, R> *n = new Callback<P1, P2, R>;
		n->attach(fc);
		GQUEUE(n);
	}

	R operator() (P1 a, P2 b) const {
		assert(hp != NULL);
		Callback<P1, P2, R> *cr = hp;
		for (; cr != NULL; cr = cr->next) {
			if (cr->next == NULL) break;
			cr->invoke(a, b);
		}
		return cr->invoke(a, b);
	}

	operator bool () const {
		return hp != NULL;
	}

	bool operator! () const {
		return !(operator bool());
	}
private:
	template <class _P1, class _P2, class _R>
	struct Callback {
	public:
		Callback() {
			_that = NULL;
		}
		template <class T>
		inline void attach(T *that,_R (T::*fc)(_P1, _P2)) {
			assert(that != NULL);
			_that = (Generic*)that;
			_fc = (Gmfp)fc;
		}
		inline void attach(_R (*fc)(_P1, P2)) {
			_that = NULL;
			_fct = fc;
		}
		_R invoke(_P1 a, _P2 b) {
			if (_that != NULL)
				((*_that).*_fc)(a, b);
			else
				_fct(a, b);
		}
		Callback<_P1, _P2, R> *next;
	private:
		typedef _R (Generic::*Gmfp)(_P1, _P2);
		Generic *_that;
		Gmfp _fc;
		R (*_fct)(_P1, _P2);
	};

	Callback<P1, P2, R> *hp, *tp;
};

#endif
