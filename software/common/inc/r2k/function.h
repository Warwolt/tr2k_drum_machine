/*
****************************************************************************************************
* brief : Minimal implementation of an std::function equivalent using static allocation.
*         Based on: https://shaharmike.com/cpp/naive-std-function/
****************************************************************************************************
*/

#ifndef R2K_FUNCTION_H
#define R2K_FUNCTION_H

namespace r2k
{
template <typename>
class function;

/**
 * @brief Wraps some callable type, akin to a function pointer
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
	 * @brief Constructor
	 * @param f  An instance of some function type
	 */
	template <typename F>
	function<ReturnType(Args...)>(F f) : callable(new Callable<F>(f))
	{

	}

	/**
	 * @brief Destructor that gets rid of wrapped function if it exists
	 */
	~function<ReturnType(Args...)>()
	{
		if (callable)
		{
			delete callable;
		}
	}

	/**
	 * @brief Assignment operator
	 * @param f  An instance of some function type
	 */
	template <typename F>
	function& operator=(F f)
	{
		if (callable)
		{
			delete callable;
		}
		callable = new Callable<F>(f);
		return *this;
	}

	/**
	 * @brief Paranthesis operator to call the wrapped function
	 * @param args  Any needed arguments needed for the function
	 */
	ReturnType operator()(Args... args) const
	{
		return callable->invoke(args...);
	}

private:
	/**
	 * @brief Interface for wrapping any callable type
	 *
	 * In order to be able to store free functions, lambdas, closures and
	 * member functions we introduce a wrapper with a vtable.
	 */
	class ICallable
	{
	public:
		virtual ~ICallable() = default;
		virtual ReturnType invoke(Args...) = 0;
	};

	/**
	 * @brief Templated concrete instance of the callable wrapper
	 * @param f  An instance of a callable type to wrap
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
			return f(args...);
		}
	private:
		F f;
	};

	ICallable *callable;
};
} /* namespace r2k */

#endif /* R2K_FUNCTION_H */
