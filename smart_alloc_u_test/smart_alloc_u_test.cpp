#include "pch.h"
#include "CppUnitTest.h"
#include "..\smart_alloc_dll\smart_alloc.h"
#include <functional>
#include <vector>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace allocator;

namespace smart_allocator_Tests
{
	TEST_CLASS(Allocation)
	{
	public:

		TEST_METHOD(IsSimpleValueAllocationCorrrect)
		{
			int v = 23;

			smart_allocator<int> t1;

			t1.allocate(v);

			Assert::AreEqual(*t1.getPtr(), v);
		}

		TEST_METHOD(IsEmptyValueAllocationCorrect)
		{
			smart_allocator<int> t1;

			t1.allocate();

			Assert::IsNotNull(t1.getPtr());
		}

		TEST_METHOD(IsBlockAllocationCorrect)
		{
			std::vector<int> data = { 1,2,3,4,5 };

			smart_allocator<int> t1;

			t1.allocate_memory_block(data.size());
			int i = 0;
			for (int e : data)
			{
				t1[i] = e;
				++i;
			}

			t1.iterate([data](const int& elem, int index)->bool
				{
					int g = data[index];

					Assert::AreEqual(data[index], elem);

					return true;
				});

			Assert::AreEqual(t1.isAllocated(), true);
			Assert::AreEqual(t1.hasBlockAllocated(), true);
			Assert::AreEqual(t1.getSize(), (size_t)data.size());
		}

	};

	TEST_CLASS(DeAllocation)
	{
	public:

		TEST_METHOD(IsDeallocationCorrect_EmptyValue)
		{
			smart_allocator<int> t;

			t.deAllocate();
		}

		TEST_METHOD(IsDeallocationCorrect_Single_Value)
		{
			smart_allocator<int> t(2);

			t.deAllocate();

			Assert::IsNull(t.getPtr());
		}

		TEST_METHOD(IsDeallocationCorrect_Mem_Block)
		{
			smart_allocator<int> t({ 1,2,3,4,5 });

			t.deAllocate();

			Assert::IsNull(t.getPtr());
		}

	};

	TEST_CLASS(ReAllocation)
	{
	public:

		TEST_METHOD(ReaAllocate)
		{
			smart_allocator<int> t(55);
			t.deAllocate();
			Assert::IsNull(t.getPtr());
			t.allocate(34);
			Assert::AreEqual((int)34, t[0]);
			t.deAllocate();
			Assert::IsNull(t.getPtr());
			t.allocate_memory_block({ 1,2,3,4,5 });

			int size = 5;
			for (size_t i = 0; i < size; i++)
			{
				Assert::AreEqual((int)i + 1, t[i]);
			}

		}
	};

	TEST_CLASS(CtorsTesting)
	{
	public:

		TEST_METHOD(IsCtorCorrect)
		{
			smart_allocator<char> t1;

			Assert::AreEqual(t1.getPtr(), nullptr);
			Assert::AreEqual(t1.hasBlockAllocated(), false);
			Assert::AreEqual(t1.isAllocated(), false);
			Assert::AreEqual(t1.getSize(), (size_t)0);
		}

		TEST_METHOD(IsCtorWithAlloctionCorrect)
		{
			char d = 'a';
			smart_allocator<char> test(d);

			Assert::AreEqual(*test.getPtr(), d);
			Assert::AreEqual(test.isAllocated(), true);
			Assert::AreEqual(test.hasBlockAllocated(), false);
			Assert::AreEqual(test.getSize(), (size_t)1);
		}

		TEST_METHOD(IsVectorCastCorrect)
		{
			std::vector<int> v = { 1,2,3,4,5 };

			smart_allocator<int> test(v);

			int i = 0;

			for (int el : v)
			{
				Assert::AreEqual(el, *(test.getPtr() + i));
				++i;
			}

			Assert::AreEqual(test.isAllocated(), true);
			Assert::AreEqual(test.hasBlockAllocated(), true);
			Assert::AreEqual(test.getSize(), (size_t)v.size());
		}

		TEST_METHOD(IsCopyCtorCorrect_EmptyValue)
		{
			smart_allocator<int> t;
			t.allocate();

			smart_allocator<int> t2(t);

			Assert::AreEqual(*t.getPtr(), *t2.getPtr());
			Assert::AreEqual(t.hasBlockAllocated(), false);
			Assert::AreEqual(t2.hasBlockAllocated(), false);
			Assert::AreEqual(t.isAllocated(), true);
			Assert::AreEqual(t2.isAllocated(), true);
			Assert::AreEqual(t.getSize(), t2.getSize());
		}

		TEST_METHOD(IsCopyCtorCorrect_SingleValue)
		{
			smart_allocator<char> t1('s');
			smart_allocator<char> t2(t1);

			Assert::AreEqual(*t1.getPtr(), *t2.getPtr());

			Assert::AreEqual(t1.hasBlockAllocated(), false);
			Assert::AreEqual(t2.hasBlockAllocated(), false);

			Assert::AreEqual(t1.isAllocated(), true);
			Assert::AreEqual(t2.isAllocated(), true);

			Assert::AreEqual(t1.getSize(), t2.getSize());
		}

		TEST_METHOD(IsCopyCtorCorrect_Mem_Block)
		{
			std::vector<char> d{ 'd', 'f', 'c', 'k' };

			smart_allocator<char> t1;

			t1.allocate_memory_block(d);

			smart_allocator<char> t2(t1);

			Assert::AreEqual(t1.isAllocated(), true);
			Assert::AreEqual(t2.isAllocated(), true);

			Assert::AreEqual(t1.hasBlockAllocated(), true);
			Assert::AreEqual(t2.hasBlockAllocated(), true);

			Assert::AreEqual(t2.getSize(), t1.getSize());

			int i = 0;

			size_t size = t1.getSize();

			for (size_t i = 0; i < size; i++)
			{
				Assert::AreEqual(t1[i], t2[i]);
			}
		}

		TEST_METHOD(IsWorkingWithPtrCorrect)
		{
			char* t = "Word";

			smart_allocator<char> w(t, 4);

			w.iterate([t](char& e, int i)-> bool
				{
					Assert::AreEqual(e, *(t + i));

					return true;
				});
		}


	};

	TEST_CLASS(Operators)
	{
	public:
		TEST_METHOD(IsAssignmentOperatorCorrect_EmptyValue)
		{
			smart_allocator<int> t;
			t.allocate();
			smart_allocator<int> t1 = t;

			Assert::AreNotSame(t.getPtr(), t1.getPtr());
			Assert::AreEqual(*t.getPtr(), *t1.getPtr());

			Assert::AreEqual(t.isAllocated(), true);
			Assert::AreEqual(t1.isAllocated(), true);

			Assert::AreEqual(t.hasBlockAllocated(), false);
			Assert::AreEqual(t1.hasBlockAllocated(), false);

			Assert::AreEqual(t.getSize(), (size_t)1);
			Assert::AreEqual(t1.getSize(), (size_t)1);

		}

		TEST_METHOD(IsAssignmentOperatorCorrect_SingleValue)
		{
			smart_allocator<int> t;
			t.allocate(10);
			smart_allocator<int> t1 = t;

			Assert::AreNotSame(t.getPtr(), t1.getPtr());
			Assert::AreEqual(*t.getPtr(), *t1.getPtr());

			Assert::AreEqual(t.isAllocated(), true);
			Assert::AreEqual(t1.isAllocated(), true);

			Assert::AreEqual(t.hasBlockAllocated(), false);
			Assert::AreEqual(t1.hasBlockAllocated(), false);

			Assert::AreEqual(t.getSize(), (size_t)1);
			Assert::AreEqual(t1.getSize(), (size_t)1);

		}

		TEST_METHOD(IsAssignmentOperatorCorrect_SingleValue_To_SingleValue)
		{
			smart_allocator<int> t;
			t.allocate(10);
			smart_allocator<int> t1(5);

			t = t1;

			Assert::AreNotSame(t.getPtr(), t1.getPtr());
			Assert::AreEqual(*t.getPtr(), *t1.getPtr());

			Assert::AreEqual(t.isAllocated(), true);
			Assert::AreEqual(t1.isAllocated(), true);

			Assert::AreEqual(t.hasBlockAllocated(), false);
			Assert::AreEqual(t1.hasBlockAllocated(), false);

			Assert::AreEqual(t.getSize(), (size_t)1);
			Assert::AreEqual(t1.getSize(), (size_t)1);

		}

		TEST_METHOD(IsAssignmentOperatorCorrect_MemoryBlock)
		{
			smart_allocator<int> t({ 1,2,3,4,5 });

			smart_allocator<int> t1 = t;

			Assert::AreNotSame(t.getPtr(), t1.getPtr());

			Assert::AreEqual(t.isAllocated(), true);
			Assert::AreEqual(t1.isAllocated(), true);

			Assert::AreEqual(t.hasBlockAllocated(), true);
			Assert::AreEqual(t1.hasBlockAllocated(), true);

			Assert::AreEqual(t.getSize(), (size_t)5);
			Assert::AreEqual(t1.getSize(), (size_t)5);

			int size = 5;

			for (size_t i = 0; i < size; i++)
			{
				Assert::AreEqual(t[i], t1[i]);
			}

		}

		TEST_METHOD(IsAssignmentOperatorCorrect_MemoryBlock_To_MemoryBlock)
		{
			smart_allocator<int> t({ 1,2,3,4,5 });

			smart_allocator<int> t1({ 3,4,5,6,7 });

			t1 = t;

			Assert::AreNotSame(t.getPtr(), t1.getPtr());
			Assert::AreEqual(*t.getPtr(), *t1.getPtr());

			Assert::AreEqual(t.isAllocated(), true);
			Assert::AreEqual(t1.isAllocated(), true);

			Assert::AreEqual(t.hasBlockAllocated(), true);
			Assert::AreEqual(t1.hasBlockAllocated(), true);

			Assert::AreEqual(t.getSize(), (size_t)5);
			Assert::AreEqual(t1.getSize(), (size_t)5);

			int size = 5;

			for (size_t i = 0; i < size; i++)
			{
				Assert::AreEqual(t[i], t1[i]);
			}

		}

		TEST_METHOD(IsEqualsCorrect)
		{
			smart_allocator<int> t;
			smart_allocator<int> t1;

			Assert::IsFalse(t == t1);

			t.allocate(200);
			t1.allocate_memory_block(5);

			Assert::AreEqual(t == t1, false);

			t.deAllocate();
			t.allocate_memory_block(10);

			Assert::IsFalse(t == t1);

			t.deAllocate();
			t1.deAllocate();

			t.allocate_memory_block({ 1,2,3,4,5 });
			t1.allocate_memory_block({ 1,2,3,4,10 });

			Assert::IsFalse(t == t1);

			t1 = t;

			Assert::IsTrue(t == t1);
		}

		TEST_METHOD(IsNotEqualsCorrect)
		{
			smart_allocator<int> t;
			smart_allocator<int> t1;

			Assert::IsTrue(t != t1);

			t.allocate(200);
			t1.allocate_memory_block(5);

			Assert::AreEqual(t != t1, true);

			t.deAllocate();
			t.allocate_memory_block(10);

			Assert::IsTrue(t != t1);

			t.deAllocate();
			t1.deAllocate();

			t.allocate_memory_block({ 1,2,3,4,5 });
			t1.allocate_memory_block({ 1,2,3,4,10 });

			Assert::IsTrue(t != t1);

			t1 = t;

			Assert::IsFalse(t != t1);
		}
	};

	TEST_CLASS(InterfaceMethods)
	{
	public:

		TEST_METHOD(IsGetPtrCorrect)
		{
			int d = 5;

			smart_allocator<int> t(d);

			Assert::AreEqual(d, *t.getPtr());
		}
	};
}
