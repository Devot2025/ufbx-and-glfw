#include "graphic_contents_main.hpp"

int main(void){
    try{
        GL_Sys_Main_Manager test(800, 600, "test");
        Test_Main_Loop tml;
        test.run_gl_loop(tml);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
    }
    
    return 0;
    
}
