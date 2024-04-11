// Smart_Allocator.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <crtdbg.h>
#include"..\smart_alloc_dll\smart_alloc.h"

allocator::smart_allocator<int> Returning()
{
	return allocator::smart_allocator<int>(20);
}

void DisposeTest()
{
	allocator::smart_allocator<int> alloc(23);	
}

void ExcepTest()
{
	try
	{
		allocator::smart_allocator<char> test('e');		

		throw std::exception("Test");
	}
	catch (const std::exception&)
	{
		return;
	}
}

int main()
{
    int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    flag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(flag);

    std::cout << "Memory Leak Testing!\n";

	allocator::smart_allocator<int> res = Returning();

	DisposeTest();

	ExcepTest();

	
}

