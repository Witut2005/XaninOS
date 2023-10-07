
namespace std
{


template<typename F> 
struct function_types {};

template<typename Ret, typename Args>
struct function_types<Ret(*)(Args)>
{
    using return_type = Ret;
    using argument_type = Args;
};

template<typename F>
class function
{
    public:

    using return_type = typename function_types<F>::return_type;
    using argument_type = typename function_types<F>::argument_type;

    // function(const function& other) = default;
    // function(R(*function_ptr)(Args ... )) : function_ptr(function_ptr){};

    // R operator ()(Args ... );

    // private:
    // R(*function_ptr)(Args ...);

};

template <typename F>
class LambdaWrapper 
{
    public:

    using return_type = typename function_types<F>::return_type;
    using argument_type = typename function_types<F>::argument_type;

    inline LambdaWrapper(F lambda){
        this->function_ptr = lambda;
    }

    inline return_type operator ()(argument_type arg){
        return function_ptr(arg);
    }

    private:
    F function_ptr;
};

}
