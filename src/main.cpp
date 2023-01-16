#include "main.hpp"
#include "program/program.hpp"

using std::this_thread::sleep_for;
using std::chrono::milliseconds;

int main(int argc, char* argv[])
{
    Program::initProgram();

    sleep_for(milliseconds(1000));

    Program::quitProgram();

    return 0;
}
