/*
The MIT License (MIT)

Copyright (c) 2015

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef PROPERTY_H
#define PROPERTY_H

#include <QMetaType>
#include <QVariant>

#include <functional>
#include <type_traits>

template <typename T>
class Property
{
    public:
        typedef T type;
        typedef void (CallbackFunctionType)(const T&);
        typedef std::function<CallbackFunctionType> CallbackType;
        typedef void (CallbackCompareFunctionType)(const T&, const T&);
        typedef std::function<CallbackCompareFunctionType> CallbackCompareType;

    protected:
        T _value;
        CallbackType _changeCallback;
        CallbackCompareType _processChangeCallback;

    public:
        Q_INLINE_TEMPLATE Property() {}
        template<typename U>
        Q_INLINE_TEMPLATE Property(Property<U> &&rhs)
            : _value(qMove(rhs._value)), _changeCallback(qMove(rhs._changeCallback)), _processChangeCallback(qMove(rhs._changeCallback))
        {
            static_assert(std::is_same<T, U>::value ||
                          (std::is_pointer<T>::value && std::is_same<U, std::nullptr_t>::value),
                          "Property types don't match");
            rhs.d = nullptr;
        }
        template<typename U>
        Q_INLINE_TEMPLATE Property(const U &initialValue)
            : _value(initialValue)
        {
            static_assert(std::is_same<T, U>::value ||
                          (std::is_pointer<T>::value && std::is_same<U, std::nullptr_t>::value),
                          "Property types don't match");
        }
        template<typename U>
        Q_INLINE_TEMPLATE Property(U &&initialValue)
            : _value(qMove(initialValue))
        {
            static_assert(std::is_same<T, U>::value ||
                          (std::is_pointer<T>::value && std::is_same<U, std::nullptr_t>::value),
                          "Property types don't match");
        }
        template<typename U>
        Q_INLINE_TEMPLATE Property(const U &initialValue, const CallbackType &changeCallback)
            : _value(initialValue), _changeCallback(changeCallback)
        {
            static_assert(std::is_same<T, U>::value ||
                          (std::is_pointer<T>::value && std::is_same<U, std::nullptr_t>::value),
                          "Property types don't match");
        }
        template<typename U>
        Q_INLINE_TEMPLATE Property(U &&initialValue, const CallbackType &changeCallback)
            : _value(qMove(initialValue)), _changeCallback(changeCallback)
        {
            static_assert(std::is_same<T, U>::value ||
                          (std::is_pointer<T>::value && std::is_same<U, std::nullptr_t>::value),
                          "Property types don't match");
        }
        template<typename U>
        Q_INLINE_TEMPLATE Property(const U &initialValue, const CallbackType &changeCallback, const CallbackCompareType &processChangeCallback)
            : _value(initialValue), _changeCallback(changeCallback), _processChangeCallback(processChangeCallback)
        {
            static_assert(std::is_same<T, U>::value ||
                          (std::is_pointer<T>::value && std::is_same<U, std::nullptr_t>::value),
                          "Property types don't match");
        }
        template<typename U>
        Q_INLINE_TEMPLATE Property(U &&initialValue, const CallbackType &changeCallback, const CallbackCompareType &processChangeCallback)
            : _value(qMove(initialValue)), _changeCallback(changeCallback), _processChangeCallback(processChangeCallback)
        {
            static_assert(std::is_same<T, U>::value ||
                          (std::is_pointer<T>::value && std::is_same<U, std::nullptr_t>::value),
                          "Property types don't match");
        }

        Q_INLINE_TEMPLATE operator const T&() const Q_REQUIRED_RESULT
        {
            return getValue();
        }

        Q_INLINE_TEMPLATE const T& operator->() const Q_REQUIRED_RESULT
        {
            return getValue();
        }

        Q_INLINE_TEMPLATE const T& value() const Q_REQUIRED_RESULT
        {
            return getValue();
        }

        template<typename U>
        Q_INLINE_TEMPLATE void swap(Property<U> &other)
        {
            static_assert(std::is_same<T, U>::value, "Property types don't match");
            qSwap(_value, other._value);
            qSwap(_changeCallback, other._changeCallback);
            qSwap(_processChangeCallback, other._processChangeCallback);
        }

        template<typename U>
        Q_INLINE_TEMPLATE Property<T>& operator=(Property<U> &&rhs)
        {
            static_assert(std::is_same<T, U>::value, "Property types don't match");
            swap(rhs);
            return *this;
        }

        Q_INLINE_TEMPLATE Property<T>& operator=(const T &rhs)
        {
            setValue(rhs);
            return *this;
        }

        Q_INLINE_TEMPLATE Property<T>& operator=(T &&rhs)
        {
            setValue(qMove(rhs));
            return *this;
        }

        Q_INLINE_TEMPLATE QVariant toVariant() const Q_REQUIRED_RESULT
        { return QVariant::fromValue<T>(getValue()); }

        Q_INLINE_TEMPLATE void fromVariant(const QVariant &value)
        {
            //Q_ASSERT(qMetaTypeId<T>() == value.userType() ||
            //QMetaType::hasRegisteredConverterFunction(value.userType(), qMetaTypeId<T>()));
            setValue(value.value<T>());
        }

    protected:
        Q_INLINE_TEMPLATE void setValue(const T &value)
        {
            if(_value != value)
            {
                if(Q_UNLIKELY(_processChangeCallback))
                    _processChangeCallback(_value, value);
                _value = value;
                if(Q_LIKELY(_changeCallback))
                    _changeCallback(_value);
            }
        }

        Q_INLINE_TEMPLATE void setValue(T &&value)
        {
            if(_value != value)
            {
                if(Q_UNLIKELY(_processChangeCallback))
                    _processChangeCallback(_value, value);
                _value = qMove(value);
                if(Q_LIKELY(_changeCallback))
                    _changeCallback(_value);
            }
        }

        Q_INLINE_TEMPLATE const T& getValue() const
        {
            return _value;
        }

        Q_DISABLE_COPY(Property)
};

template <typename T>
Q_INLINE_TEMPLATE void swap(Property<T> &left, Property<T> &right)
{ left.swap(right); }

template <typename T>
Q_INLINE_TEMPLATE bool operator==(const Property<T> &lhs, const Property<T> &rhs) Q_REQUIRED_RESULT Q_DECL_PURE_FUNCTION;

template <typename T>
Q_INLINE_TEMPLATE bool operator!=(const Property<T> &lhs, const Property<T> &rhs) Q_REQUIRED_RESULT Q_DECL_PURE_FUNCTION;

template <typename T, typename U>
Q_INLINE_TEMPLATE bool operator==(const T &lhs, const Property<U> &rhs) Q_REQUIRED_RESULT Q_DECL_PURE_FUNCTION;

template <typename T, typename U>
Q_INLINE_TEMPLATE bool operator!=(const T &lhs, const Property<U> &rhs) Q_REQUIRED_RESULT Q_DECL_PURE_FUNCTION;

template <typename T, typename U>
Q_INLINE_TEMPLATE bool operator==(const Property<T> &lhs, const U &rhs) Q_REQUIRED_RESULT Q_DECL_PURE_FUNCTION;

template <typename T, typename U>
Q_INLINE_TEMPLATE bool operator!=(const Property<T> &lhs, const U &rhs) Q_REQUIRED_RESULT Q_DECL_PURE_FUNCTION;

template <typename T>
Q_INLINE_TEMPLATE bool operator<(const Property<T> &lhs, const Property<T> &rhs) Q_REQUIRED_RESULT Q_DECL_PURE_FUNCTION;

template <typename T, typename U>
Q_INLINE_TEMPLATE bool operator<(const Property<T> &lhs, const U &rhs) Q_REQUIRED_RESULT Q_DECL_PURE_FUNCTION;

template <typename T, typename U>
Q_INLINE_TEMPLATE bool operator<(const T &lhs, const Property<U> &rhs) Q_REQUIRED_RESULT Q_DECL_PURE_FUNCTION;

template <typename T>
Q_INLINE_TEMPLATE bool operator==(const Property<T> &lhs, const Property<T> &rhs)
{ return (lhs == rhs.value()); }

template <typename T>
Q_INLINE_TEMPLATE bool operator!=(const Property<T> &lhs, const Property<T> &rhs)
{ return (lhs != rhs.value()); }

template <typename T, typename U>
Q_INLINE_TEMPLATE bool operator==(const T &lhs, const Property<U> &rhs)
{ static_assert(std::is_same<T, U>::value, "Property types don't match"); return (rhs == lhs); }

template <typename T, typename U>
Q_INLINE_TEMPLATE bool operator!=(const T &lhs, const Property<U> &rhs)
{ static_assert(std::is_same<T, U>::value, "Property types don't match"); return (rhs != lhs); }

template <typename T, typename U>
Q_INLINE_TEMPLATE bool operator==(const Property<T> &lhs, const U &rhs)
{ static_assert(std::is_same<T, U>::value, "Property types don't match"); return (lhs.value() == rhs); }

template <typename T, typename U>
Q_INLINE_TEMPLATE bool operator!=(const Property<T> &lhs, const U &rhs)
{ static_assert(std::is_same<T, U>::value, "Property types don't match"); return (lhs.value() != rhs); }

template <typename T>
Q_INLINE_TEMPLATE bool operator<(const Property<T> &lhs, const Property<T> &rhs)
{ return (lhs < rhs.value()); }

template <typename T, typename U>
Q_INLINE_TEMPLATE bool operator<(const Property<T> &lhs, const U &rhs)
{ static_assert(std::is_same<T, U>::value, "Property types don't match"); return (lhs.value() < rhs); }

template <typename T, typename U>
Q_INLINE_TEMPLATE bool operator<(const T &lhs, const Property<U> &rhs)
{ static_assert(std::is_same<T, U>::value, "Property types don't match"); return (lhs < rhs.value()); }

template <typename T>
Q_INLINE_TEMPLATE bool operator>(const Property<T> &lhs, const Property<T> &rhs)
{ return (lhs > rhs.value()); }

template <typename T, typename U>
inline bool operator>(const T &lhs, const Property<U> &rhs)
{ return rhs < lhs; }

template <typename T, typename U>
inline bool operator>(const Property<T> &lhs, const U &rhs)
{ return rhs < lhs; }

#endif

