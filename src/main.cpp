#include "main.hpp"
#include "program/program.hpp"

int main(int argc, char* argv[])
{
    Program::initProgram();

    Program::beginProgramLoop();

    Program::quitProgram();

    return 0;
}
