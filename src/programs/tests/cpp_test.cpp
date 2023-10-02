// #include <sys/devices/nic/ne2000.hpp>
#include <lib/libcpp/stack.h>
#include <lib/libcpp/utility.h>
#include <lib/libcpp/time.hpp>
#include <lib/libcpp/algorithm.h>
#include <lib/libc/string.h>
#include <lib/libc/memory.h>
#include <lib/libcpp/ostream.h>
#include <lib/libc/stdlibx.h>
#include <lib/libcpp/string.h>
#include <lib/libcpp/istream.h>
#include <sys/devices/keyboard/scan_codes.h>
#include <lib/libcpp/array.h>
#include <lib/libcpp/initializer_list.hpp>
#include <lib/libcpp/new.hpp>
#include <lib/libcpp/bytes.h>
#include <lib/libcpp/list.h>
#include <lib/libcpp/map.h>
#include <lib/libcpp/vector.hpp>
#include <lib/libcpp/set.h>
#include <lib/libcpp/fstream.h>
#include <lib/libcpp/regex.h>
#include <lib/libcpp/memory.h>
#include <lib/libcpp/command_parser.h>
#include <lib/screen/screen.h>
#include <sys/terminal/interpreter/interpreter.h>
#include <sys/input/input.h>
#include <lib/libcpp/iterator.hpp>

//TERMINAL_APP

class Test
{
    public:
    Test(int x, int y) { std::cout << std::dec << "CLASS: " << (int)x << ", " << (int)y << std::endl; }
    void print(int x, int y) { std::cout << std::dec << "CLASS: " << (int)x << ", " << (int)y << std::endl; }
};

void cpp_prog()
{

    std::array<int, 5> nichini = {1,2,3,4,5};

    auto yhy = nichini.concat(nichini);
    
    // for(auto a : yhy)
    //     std::cout << a << std::endl;

    std::vector<int> vec = {1,2,3};
    std::vector<int> vec2;


    std::cout << vec.pointer_get() << std::endl;
    vec = std::move(vec);
    std::cout << vec.pointer_get() << std::endl;
    std::cout << vec2.pointer_get() << std::endl;

    return;

    // std::UnorderedMap<char, int> Mapa;
    // Mapa.insert('a', 10);
    // Mapa.insert('b', 11);
    // Mapa.insert('c', 12);

    // // Mapa.print();
    // auto tmptmp = Mapa.find('a');

    // xprintf("0x%x\n", tmptmp);
    // xprintf("0x%x\n", Mapa.end());

    // if(tmptmp == Mapa.end())
    //     std::cout << "no such element with given key" << std::endl;
    // else
    //     std::cout << "value of the given key: " << *tmptmp << std::endl;

    // while(getxchar().scan_code != ENTER);
    // screen_clear();


    // std::string strr("nicho");
    // // std::string strr2(strr.rbegin(), strr.rend());
    // std::string strr2(strr.rbegin(), strr.rend());
    
    // std::cout << strr2 << std::endl;


    // std::array<const char*, 3> arr = {"nicho", "morbius", "lerczok"};

    // std::cout << "array: " << std::endl;
    // for(auto a : arr)
    //     std::cout << a << " ";
    // std::cout << std::endl;

    // std::cout << "vector: " << std::endl;
    // std::vector<int> vec = {1,2,3,4,5};
    // for(auto a : vec)
    //     std::cout << a << " ";
    // std::cout << std::endl;

    // std::cout << "list: " << std::endl;
    // std::List<int> li(0);
    
    // for(int i = 1; i < 10; i++)
    //     li.push(i);

    // std::ListIterator a = li.begin();

    // for(auto a : li)
    //     std::cout << a << " ";
    // std::cout << std::endl;

    // std::cout << "map: " << std::endl;
    // std::UnorderedMap<int, const char*> mapp = {{1, "100"}, {2, "200"}, {3, "300"}};
        
    // std::UnorderedMap<std::pair<int, int>, uint32_t> nii;
    // nii.insert(std::make_pair(1,1), 100);

    // std::cout << "lista generalnie: ";
    // if(nii[std::make_pair(1,1)] != *nii.end())
    //     std::cout << nii[std::make_pair(1,1)] << std::endl;
    // else
    //     std::cout << "bruh" << std::endl;

        
    // // while(1);
        
    // std::CommandParser parser(argv);
    // parser.argument_add("--piwka", true);

    // int ile_piw = std::CommandParser::parse_arg<int>(parser, "--piwka");

    // std::cout << "parser errno: " << parser.errno_get() << std::endl;

    // if(!parser.errno_get())
    // {
    //     if(ile_piw > 10)
    //         std::cout << "chiluj opie" << std::endl;
    //     else
    //         std::cout << "tak malo?" << std::endl;
    // }

    // else
    // {
    //     std::cout << "czegos mi tu brakuje, hmmm" << std::endl;
    // }



    // // std::UnorderedMap<const char*, const char* > MojaMapa;
    // // MojaMapa.insert("nicho", "wojciech");
    // // MojaMapa.insert("agata", "lerczok");
    // // MojaMapa.insert("huj", "udpa");

    // // std::cout << MojaMapa["nicho"] << " " << MojaMapa["agata"] << " " << MojaMapa["huj"] << std::endl;

    // // std::List<const char*> lista("nicho");
    // // lista.push("123");
    
    // // lista.print();
    // // if(lista["123"] != nullptr)
    // //     std::cout << "istnieje" << std::endl;
    // // else
    // //     std::cout << "nie istnieje" << std::endl;


    // // lista.

    // while(getxchar().scan_code != ENTER);

    // int* p;
    
    // p = (int*)malloc(SIZE_OF(int));
    // std::cout << std::hex << p <<std::endl;
    // p = new int{32};

    // p = reinterpret_cast<int*> (operator new (SIZE_OF(int)));

    // //Test* hm = new Test{10, 20};

    // Test* hm = reinterpret_cast<Test*>(::operator new (SIZE_OF(Test)));

    // hm->print(50, 100);

    // //std::cout << std::hex << (unsigned int)(p) <<std::endl;
    // //std::cout << *p <<std::endl;


    // std::array<int, 10> test_array  = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    // std::array<int, 10> test_array2 = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

    // std::cout << "the smallest element: " << *std::min_element(test_array.begin(), test_array.end()) << std::endl;

    // std::cout << "the largest element: " << *std::max_element(test_array.begin(), test_array.end()) << std::endl;
    
    // std::swap<int>(*test_array.begin(), *test_array.end());
    // std::min(*test_array.begin(), *test_array.end());
    
    // std::string str_tmp = "hm";
    // std::cout << str_tmp << std::endl;
    

    // //std::cout << *p << std::endl;

    // /*
    // std::cout << "first array" << std::endl;

    // for(auto a : test_array)
    //     std::cout << a << " ";

    // */

    // std::array<int, 10>::iterator it = test_array.begin();
    // std::array<int, 10>::iterator it2 = test_array.begin();

    // *it = 5;

    // for(auto a : test_array)
    // {
    //     std::cout << a << " ";
    // }

    // std::cout << std::endl;

    // std::string str("123");
    // std::cout << "STRING SIZE:"<< str.size() << std::endl;

    // std::cout << std::endl;

    // std::pair<char, int> para = std::make_pair('a', 512); 
    // std::pair<char, int>& ref_pair = para;

    // std::cout << para.first << " " << para.second << std::endl;

    // std::stack<int> stack;

    // for(int i = 0; i < 5; i++)
    //     stack.push(i);

    // while(!stack.empty())
    // {
    //     std::cout << stack.top() << std::endl;
    //     stack.pop();
    // }

    // int first = 100, second = 50;

    // std::pair pair_test = std::minmax(first, second);

    // pair_test.first = 10;
    // pair_test.second = 5;

    // std::cout << std::dec << first << " " << second << std::endl;

    // std::cout << "string test" << std::endl;
    // std::string str1 = "jeden";
    // std::string& omg = str1;
    
    // std::print("ugabuga: {}{}", omg, 10);

    // char* buf = (char*)calloc(50);
    // xscan_range(buf, 10);
    // std::cout << buf << std::endl;

    // screen_clear();


    // std::UnorderedMap<int, int> Ahha = {
    //     {1,5},
    //     {6,9}
    // };


    // Ahha.print();
    // std::cout << "ARRR: " << test_array[-1] << std::endl;

    // //for(auto a : Ahha)
    // //std::cout << a.second << std::endl

    // std::vector<int> wekk = {1,2,3,4,5,6,7,8,9,10};

    // wekk.pop();
    // wekk.pop();
    // wekk.push(1234);
    
    // wekk[0] = 1000;
    // wekk[0]++;
        
    // wekk.print();

    // while(getxchar().scan_code != ENTER);

    // std::cout << std::clear;
    
    // std::cout << "ABATAKAM" << std::endl;
    

    // /*
    //     for(int i = 0; i < jmm.size_get(); i++)
    //     std::cout << jmm[i] << std::endl;
    // */

    // std::fstream piwko("fromcio_tomcio.txt", "rw");
    // if(piwko.good())
    // {
    //     piwko.write("\t\t\t", 3);
    //     std::cout << "FSTREAM OK" << std::endl;
    // }
    // piwko.close();

    // char state[2][6] = {"false", "true"};
    // std::cout << "REGEX TEST: " << state[std::regex_match("om g", "o.\\sg")] << std::endl;

    // int mm = 10;

    // std::weak_ptr<int> weak;
    // {
    // std::shared_ptr<int> shr1;
    // std::make_shared(shr1, mm);

    // weak = shr1;


    // if(weak.lock() == NULL)
    //     std::cout << "no shared mordo :))" << std::endl;
    // else
    //     std::cout << "weak ptr value: " << *weak.lock()->get() << std::endl;
    // }

    
    // if(weak.lock() == NULL)
    //     std::cout << "no shared mordo :))" << std::endl;
    // else
    //     std::cout << "weak ptr value: " << *weak.lock()->get() << std::endl;


    // // std::List<char*> nicho;
    // // nicho.push_back("ugabuga");


    // while(getxchar().scan_code != ENTER);

    // free(buf);

}


extern "C"
{

    int cpp_test(void)
    {
        cpp_prog();
        return 0;
    }
    

}
