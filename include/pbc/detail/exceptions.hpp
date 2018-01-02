/*
  The MIT License (MIT)
  Copyright (c) 2016-2018 Xu Cheng
  See licensing at:
    https://opensource.org/licenses/MIT
*/

#pragma once

#include <exception>
#include <string>

namespace pbc
{
    class NotInitializedError : public std::runtime_error
    {
    public:
        NotInitializedError() : runtime_error("Object is not initialized.") {}
    };

    class AlreadyInitializedError : public std::runtime_error
    {
    public:
        AlreadyInitializedError()
            : runtime_error("Object is already initialized.")
        {
        }
    };

    class InitializationError : public std::runtime_error
    {
    public:
        InitializationError() : runtime_error("Failed to initialize object.") {}
        InitializationError(const std::string& msg) : runtime_error(msg) {}
    };

    class ElementTypeError : public std::invalid_argument
    {
    public:
        ElementTypeError() : invalid_argument("Element has wrong type.") {}
    };
}  // namespace pbc
