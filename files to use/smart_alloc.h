#ifndef SMART_ALLOCATOR_H

#define SMART_ALLOCATOR_H

#define templ template<class T>

#include <functional>
#include <vector>

namespace allocator
{
	templ
		struct smart_allocator
	{
		/// <summary>
		/// Main ctor
		/// </summary>		
		smart_allocator()
		{
			m_block_allocation = false;

			m_obj = nullptr;

			m_allocated = false;

			m_count = 0;
		}
		/// <summary>
		/// Ctor with built in allocation
		/// </summary>
		/// <param name="obj">Object for allocation</param>
		explicit smart_allocator(T obj) : smart_allocator()
		{
			this->allocate(obj);
		}
		/// <summary>
		/// Cast vector to allocated collection
		/// </summary>
		/// <param name="v"></param>
		explicit smart_allocator(const std::vector<T>& v) : smart_allocator()
		{
			this->allocate_memory_block(v.size());

			int i = 0;

			for (T e : v)
			{
				*(m_obj + i) = e;

				++i;
			}

		}

		/// <summary>
		/// Ctor that enables working with arrays represented by ptr to the 1-st element
		/// </summary>
		/// <param name="ptr"></param>
		/// <param name="count"></param>
		smart_allocator(T* ptr, size_t count) : smart_allocator()
		{
			allocate_memory_block(ptr, count);
		}

		/// <summary>
		/// Ctor that enables working with arrays represented by ptr the 1-st element(Constant)
		/// </summary>
		/// <param name="ptr"></param>
		/// <param name="count"></param>
		smart_allocator(const T* ptr, size_t count) : smart_allocator()
		{
			allocate_memory_block(ptr, count);
		}

		/// <summary>
		/// Copy ctor
		/// </summary>
		/// <param name="other">Source object</param>
		smart_allocator(const smart_allocator<T>& other) : smart_allocator()
		{
			this->deAllocate();
			this->m_block_allocation = other.m_block_allocation;

			if (!other.m_allocated)
				return;

			if (!m_block_allocation)
				this->allocate(*other.m_obj);
			else
			{
				this->allocate_memory_block(other.m_count);

				other.iterate([this](const T& e, int i)->bool
					{
						*(m_obj + i) = e;

						return true;
					}
				);
			}

		}

		/// <summary>
		/// Dereferencer
		/// </summary>
		/// <returns></returns>
		T* getPtr()
		{
			if (m_allocated)
				return m_obj;
		}
		/// <summary>
		/// Shows wether Block of memory was allocated
		/// </summary>
		bool hasBlockAllocated() const
		{
			return m_block_allocation;
		}
		/// <summary>
		/// Indicates wether allocation takes place or not
		/// </summary>
		bool isAllocated() const
		{
			return m_allocated;
		}
		/// <summary>
		/// returns size of the allocated memmory
		/// </summary>
		size_t getSize() const
		{
			return m_count;
		}

		/// <summary>
		/// Allocates memory for T type object
		/// </summary>
		/// <param name="value">Oject of type T for allocation</param>
		void allocate(T value)
		{
			if (!m_allocated)
			{
				m_obj = new T(value);

				m_allocated = true;

				m_block_allocation = false;

				m_count = 1;
			}
		}

		/// <summary>
		/// Allocates memory for single element
		/// </summary>
		void allocate()
		{
			if (!m_allocated)
			{
				m_obj = new T;

				m_allocated = true;

				m_block_allocation = false;

				m_count = 1;
			}
		}
		/// <summary>
		/// Allocates the memory Block
		/// </summary>
		/// <param name="size">Memory block size size</param>
		void allocate_memory_block(size_t size)
		{
			if (!m_allocated)
			{
				m_obj = new T[size];

				m_allocated = true;

				m_block_allocation = true;

				m_count = size;
			}

		}
		/// <summary>
		/// Allocate memory block using vector
		/// </summary>
		/// <param name="v">source vector</param>
		void allocate_memory_block(std::vector<T> v)
		{
			if (!m_allocated)
			{
				m_count = v.size();

				m_obj = new T[m_count];

				int i = 0;

				for (T e : v)
				{
					*(m_obj + i) = e;
					++i;
				}

				m_allocated = true;

				m_block_allocation = true;
			}

		}

		/// <summary>
		/// Allocate memory block using ptr to the first element of the array
		/// </summary>
		/// <param name="ptr"></param>
		/// <param name="size"></param>
		void allocate_memory_block(T* ptr, size_t size)
		{
			if (!m_allocated)
			{
				m_count = size;

				m_obj = new T[m_count];

				for (size_t i = 0; i < m_count; i++)
				{
					*(m_obj + i) = *(ptr + i);
				}

				m_allocated = true;

				m_block_allocation = true;
			}

		}

		/// <summary>
		/// Allocate memory block using ptr to the first element of the array (ptr to const)
		/// </summary>
		/// <param name="ptr"></param>
		/// <param name="size"></param>
		void allocate_memory_block(const T* ptr, size_t size)
		{
			if (!m_allocated)
			{
				m_count = size;

				m_obj = new T[m_count];

				for (size_t i = 0; i < m_count; i++)
				{
					*(m_obj + i) = *(ptr + i);
				}

				m_allocated = true;

				m_block_allocation = true;
			}

		}

		/// <summary>
		/// DeAllocator
		/// </summary>
		void deAllocate()
		{
			if (m_block_allocation)
				this->deAllocateCollection_();
			else
				this->deAllocate_();
		}

		//Finalizer
		~smart_allocator()
		{
			deAllocate();
		}

		void iterate(std::function<bool(const T& elem, int index)> func) const
		{
			if (func == nullptr)
				return;

			if (m_block_allocation)
				for (size_t i = 0; i < m_count; i++)
					if (!func(*(m_obj + i), i))
						break;

		}

		void iterate(std::function<bool(T& elem, int index)> func)
		{
			if (func == nullptr)
				return;

			if (m_block_allocation)
				for (size_t i = 0; i < m_count; i++)
					if (!func(*(m_obj + i), i))
						break;
		}

#pragma region Operators

		smart_allocator<T>& operator = (const smart_allocator<T>& other)
		{
			this->deAllocate();

			this->m_block_allocation = other.m_block_allocation;

			if (other.m_allocated)
			{
				if (!m_block_allocation)
					this->allocate(*other.m_obj);
				else
				{
					this->allocate_memory_block(other.m_count);

					other.iterate([this](const T& e, int i) -> bool
						{
							*(m_obj + i) = e;

							return true;
						});
				}
			}

			return *this;
		}

		/// <summary>
		/// Operator [], provides simple access to items of the smart_allocator
		/// </summary>
		/// <param name="index">Index of the item in the collection</param>
		/// <returns>Reference to the item</returns>
		T& operator [] (int index)
		{
			if (m_allocated)
				return *(m_obj + index);
		}

		/// <summary>
		/// Operator [], provides simple access to items of the smart_allocator
		/// </summary>
		/// <param name="index"></param>
		/// <returns>Const reference to the item</returns>
		const T& operator [] (int index) const
		{
			if (m_allocated)
				return *(m_obj + index);
		}
		/// <summary>
		/// Equality operator
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		bool operator == (const smart_allocator<T>& value) const
		{
			if (!this->m_allocated || !value.m_allocated) return false;

			if (this->m_block_allocation != value.m_block_allocation) return false;

			if (this->m_count != value.m_count) return false;

			if (!value.m_block_allocation)
				return *(m_obj) == *value.m_obj;
			else
			{
				bool res = true;

				this->iterate([&res, &value](const T& e, int i)->bool
					{
						if (*(value.m_obj + i) != e)
						{
							res = false;
							return false;
						}

					}

				);

				return res;
			}

			return false;

		}

		bool operator != (const smart_allocator<T>& value) const
		{
			return !(*this == value);
		}

#pragma endregion

	private:
		T* m_obj;//Pointer

		bool m_allocated;//Indicates wether memory was allocated or not

		bool m_block_allocation;//Indicates wether collection was allocated

		size_t m_count;//Shows the size of the collection		

#pragma region Private Funcions

		/// <summary>
		/// Deallocates simple element
		/// </summary>
		void deAllocate_()
		{
			if (m_allocated)
			{
				delete m_obj;

				m_allocated = false;

				m_block_allocation = false;
			}

		}
		/// <summary>
		/// Deallocates collection
		/// </summary>
		void deAllocateCollection_()
		{
			if (m_allocated)
			{
				delete[] m_obj;

				m_allocated = false;

				m_block_allocation = false;
			}
		}

#pragma endregion

	};

}


#endif // !SMARTALLOCATOR_H


