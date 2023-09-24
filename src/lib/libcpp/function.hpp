
namespace std
{

template<typename R, typename T>
class function
{
    private:
    R(*function_ptr)(T);

    public:
    function() : function_ptr(NULL) {};
    function(const function& other) = default;
    function(R(*function_ptr)(T)) : function_ptr(function_ptr){};
};

// template<typename R, typename T>
// function::function()
// {
//     this->function_ptr = NULL;
// }


}