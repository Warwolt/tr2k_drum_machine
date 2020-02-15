/*
****************************************************************************************************
* brief : Minimal implementation of an std::function equivalent.
*         Based on: https://shaharmike.com/cpp/naive-std-function/
****************************************************************************************************
*/

#ifndef R2K_FUNCTION_H
#define R2K_FUNCTION_H

#include "linuxtypes.h"
#ifdef UNIT_TESTING
#include <new> // this is implemented in newdelete.cpp for production build
#else
#include "newdelete.h"
#endif

namespace r2k
{
template <typename>
class function;

/**
 * @brief Wraps some wrapped_function type, akin to a function pointer
 *
 * This is an implementation similar to std::function, that works like a
 * smart function pointer, and allows wrapping lambdas and closures.
 *
 * @param ReturnType  Return type of the wrapped function
 * @param Args        Parameter list for the wrapped function
 */
template <typename ReturnType, typename ...Args>
class function<ReturnType(Args...)>
{
public:
	/**
	 * @brief Non-initializing constructor
	 *
	 * Used to declare r2k::function<> members in other classes that can later
	 * be assigned a useful value.
	 */
	function<ReturnType(Args...)>() : wrapped_function(nullptr)
	{
	}

	/**
	 * @brief Assignment constructor
	 *
	 * The primary constructor that should be used, wraps some callable type so
	 * that it can later be called by a client without knowing any concrete
	 * details.
	 *
	 * @param f  An instance of some function type
	 */
	template <typename F>
	function<ReturnType(Args...)>(F f)
	{
		static_assert(sizeof(F) <= sizeof(callableBuffer));
		wrapped_function = new(callableBuffer) Callable<F>(f);
	}

	/**
	 * @brief Copy constructor
	 *
	 * Needed in order to make sure that function wrappers constructed from
	 * other function wrappers are deep copied properly.
	 */
	function<ReturnType(Args...)>(const function<ReturnType(Args...)>& other)
	{
		wrapped_function = other.wrapped_function->clone(callableBuffer);
	}

	/**
	 * @brief Copy assignment
	 *
	 * Needed in order to make sure that function wrappers assigned to other
	 * function wrappers are deep copied properly.
	 */
	function<ReturnType(Args...)> operator=(const function<ReturnType(Args...)>& other)
	{
		wrapped_function = other.wrapped_function->clone(callableBuffer);
		return *this;
	}

	/**
	 * @brief Destructor, deletes wrapped function if it exists
	 */
	~function<ReturnType(Args...)>()
	{
		if (wrapped_function)
		{
			wrapped_function->~ICallable();
			wrapped_function = nullptr;
		}
	}

	/**
	 * @brief Assignment operator
	 * @param f  An instance of some function type
	 */
	template <typename F>
	function& operator=(F f)
	{
		static_assert(sizeof(F) <= sizeof(callableBuffer));
		if (wrapped_function)
		{
			wrapped_function->~ICallable();
			wrapped_function = nullptr;
		}
		wrapped_function = new(callableBuffer) Callable<F>(f);
		return *this;
	}

	/**
	 * @brief Paranthesis operator to call the wrapped function
	 * @param args  Any needed arguments for the function
	 */
	ReturnType operator()(Args... args) const
	{
		if (wrapped_function)
		{
			return wrapped_function->invoke(args...);
		}
	}

private:
	u8 callableBuffer[16];
	class ICallable;
	ICallable *wrapped_function;

	/**
	 * @brief Interface for wrapping any wrapped_function type
	 *
	 * In order to be able to store free functions, lambdas, closures and
	 * member functions we introduce a wrapper with a vtable.
	 */
	class ICallable
	{
	public:
		virtual ~ICallable() = default;
		virtual ReturnType invoke(Args...) = 0;
		virtual ICallable *clone(uint8_t *callableBuffer) = 0;
	};

	/**
	 * @brief Templated concrete instance of the wrapped_function wrapper
	 * @param f  An instance of a wrapped_function type to wrap
	 */
	template <typename F>
	class Callable : public ICallable
	{
	public:
		Callable(const F& f) : f(f)
		{
		}

		~Callable() override = default;

		/**
		 * @brief Invoke the concrete stored function
		 * @param args  Any needed arguments needed for the function
		 */
		ReturnType invoke(Args... args) override final
		{
			return f(args...);
		}

		/**
		 * @brief Copy constructor that can be called via ICallable interface
		 */
		ICallable *clone(uint8_t *callableBuffer)
		{
			return new(callableBuffer) Callable(f);
		}
	private:
		F f;
	};
};
} /* namespace r2k */

#endif /* R2K_FUNCTION_H */
