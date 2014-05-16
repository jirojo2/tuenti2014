#include <thread>
#include <iostream>

void doSomeWork( void )
{
	std::cout << "hello from thread..." << std::endl;
	return;
}

int main( int argc, char *argv[] )
{
	std::cout << "wtf" << std::endl;
	std::thread t( doSomeWork );
	t.join();
	return 0;
}

