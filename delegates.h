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

#ifndef DELEGATE_H_
#define DELEGATE_H_

#include <vector>
#include <functional>
#include <memory>
#include <algorithm>

namespace delegates {

template<typename Signature>
class delegate;

template<typename R, typename... Args>
class delegate<R(Args...)> {
private:
    using function_type = std::function<R(Args...)>;
    std::vector<function_type> callbacks_;

public:
    delegate() = default;
    
    delegate(const delegate& other) : callbacks_(other.callbacks_) {}
    
    delegate(delegate&& other) noexcept : callbacks_(std::move(other.callbacks_)) {}
    
    delegate& operator=(const delegate& other) {
        if (this != &other) {
            callbacks_ = other.callbacks_;
        }
        return *this;
    }
    
    delegate& operator=(delegate&& other) noexcept {
        if (this != &other) {
            callbacks_ = std::move(other.callbacks_);
        }
        return *this;
    }
    
    template<typename F>
    void bind(F&& func) {
        callbacks_.emplace_back(std::forward<F>(func));
    }
    
    template<typename T>
    void bind(T* object, R (T::*method)(Args...)) {
        static_assert(std::is_class_v<T>, "T must be a class type");
        if (object == nullptr) {
            throw std::invalid_argument("Object pointer cannot be null");
        }
        callbacks_.emplace_back([object, method](Args... args) -> R {
            return (object->*method)(args...);
        });
    }
    
    template<typename T>
    void bind(const T* object, R (T::*method)(Args...) const) {
        static_assert(std::is_class_v<T>, "T must be a class type");
        if (object == nullptr) {
            throw std::invalid_argument("Object pointer cannot be null");
        }
        callbacks_.emplace_back([object, method](Args... args) -> R {
            return (object->*method)(args...);
        });
    }
    
    template<typename F>
    delegate& operator+=(F&& func) {
        bind(std::forward<F>(func));
        return *this;
    }
    
    template<typename F>
    delegate& operator-=(const F& func) {
        auto it = std::find_if(callbacks_.begin(), callbacks_.end(),
            [&func](const function_type& stored) {
                return stored.target_type() == typeid(F);
            });
        if (it != callbacks_.end()) {
            callbacks_.erase(it);
        }
        return *this;
    }
    
    // Invocación del delegate
    template<typename... CallArgs>
    R operator()(CallArgs&&... args) const {
        if (callbacks_.empty()) {
            if constexpr (std::is_void_v<R>) {
                return;
            } else {
                throw std::runtime_error("No callbacks bound to delegate");
            }
        }
        
        if constexpr (std::is_void_v<R>) {
            for (const auto& callback : callbacks_) {
                callback(std::forward<CallArgs>(args)...);
            }
        } else {
            for (size_t i = 0; i < callbacks_.size() - 1; ++i) {
                callbacks_[i](std::forward<CallArgs>(args)...);
            }
            return callbacks_.back()(std::forward<CallArgs>(args)...);
        }
    }
    
    explicit operator bool() const noexcept {
        return !callbacks_.empty();
    }
    
    bool operator!() const noexcept {
        return callbacks_.empty();
    }
    
    size_t size() const noexcept {
        return callbacks_.size();
    }
    
    void clear() noexcept {
        callbacks_.clear();
    }
    
    bool empty() const noexcept {
        return callbacks_.empty();
    }
};
#endif // DELEGATE_H_
