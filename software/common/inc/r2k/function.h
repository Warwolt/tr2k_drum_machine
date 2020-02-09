/*
****************************************************************************************************
* brief : Minimal implementation of an std::function equivalent using static allocation.
*         Based on: https://shaharmike.com/cpp/naive-std-function/
****************************************************************************************************
*/

#ifndef R2K_FUNCTION_H
#define R2K_FUNCTION_H

#include "traceprint.h"

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
	 * @brief Uninitalized constructor
	 *
	 * Used to declare r2k::function<> members in other classes
	 */
	function<ReturnType(Args...)>() : wrapped_function(nullptr)
	{
	}

	/**
	 * @brief Assignment constructor
	 *
	 * The primary constructor that should be used, wraps some wrapped_function type
	 * so that it can later be called by a client.
	 *
	 * @param f  An instance of some function type
	 */
	template <typename F>
	function<ReturnType(Args...)>(F f) : wrapped_function(new Callable<F>(f))
	{
	}

	/**
	 * @brief Copy constructor
	 *
	 * Needed in order to make sure that function wrappers assigned to other
	 * function wrappers are deep copied properly.
	 */
	function<ReturnType(Args...)>(const function<ReturnType(Args...)>& other)
	{
		wrapped_function = wrapped_function->clone();
	}

	/**
	 * @brief Destructor that gets rid of wrapped function if it exists
	 */
	~function<ReturnType(Args...)>()
	{
		if (wrapped_function)
		{
			INFO_PRINT("deleting wrapped_function = %p", wrapped_function);
			delete wrapped_function;
		}
	}

	/**
	 * @brief Assignment operator
	 * @param f  An instance of some function type
	 */
	template <typename F>
	function& operator=(F f)
	{
		if (wrapped_function)
		{
			delete wrapped_function;
		}
		wrapped_function = new Callable<F>(f);
		return *this;
	}

	/**
	 * @brief Paranthesis operator to call the wrapped function
	 * @param args  Any needed arguments for the function
	 */
	ReturnType operator()(Args... args) const
	{
		INFO_PRINT("function<ReturnType(Args...)>::operator() called");
		if (wrapped_function)
		{
			INFO_PRINT("wrapped_function = %p", wrapped_function);
			INFO_PRINT("wrapped_function->invoke(args...)");
			return wrapped_function->invoke(args...);
		}
	}

private:
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
		virtual ICallable *clone() = 0;
	};

	/**
	 * @brief Templated concrete instance of the wrapped_function wrapper
	 * @param f  An instance of a wrapped_function type to wrap
	 */
	template <typename F>
	class Callable : public ICallable
	{
	public:
		Callable(const F& f) : f(f) {}
		~Callable() override = default;

		/**
		 * @brief Invoke the concrete stored function
		 * @param args  Any needed arguments needed for the function
		 */
		ReturnType invoke(Args... args) override final
		{
			INFO_PRINT("Callable::invoke()");
			INFO_PRINT("f = %p", f); //
			return f(args...);
		}

		/**
		 * @brief
		 * @param other  asdf
		 */
		ICallable *clone()
		{
			return new Callable(f);
		}
	private:
		F f;
	};
};
} /* namespace r2k */

#endif /* R2K_FUNCTION_H */
