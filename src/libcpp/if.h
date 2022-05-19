

namespace if
{
    template <class ... Args>
    bool all_true(Args ... a)
    {
        return ... & 1;
    }

}
