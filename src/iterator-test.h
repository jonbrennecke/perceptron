// http://stackoverflow.com/questions/8054273/how-to-implement-an-stl-style-iterator-and-avoid-common-pitfalls/8054856#8054856
// http://www.dreamincode.net/forums/topic/58468-making-your-own-iterators/

		/**
		 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		 * 					Layer Iterator
		 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		 *
		 * provides an STL-like iterater over the Layers, except that we don't need templates here since 
		 */
		// struct layer_iterator
		// {
		// 	typedef layer_iterator self_type;
		// 	typedef Layer value_type;
		// 	typedef Layer& reference;
		// 	typedef Layer* pointer;
		// 	typedef std::forward_iterator_tag iterator_category;
		// 	typedef int difference_type;

		// 	layer_iterator( pointer ptr ) : ptr_(ptr) {}
		// 	~layer_iterator();
			
		// 	self_type operator++() 
		// 	{
		// 		self_type i = *this; ptr_++; return i; 
		// 	}

		// 	self_type operator++( int i )
		// 	{
		// 		ptr_++; return *this;
		// 	}

		// 	reference operator*()
		// 	{
		// 		return *ptr_;
		// 	}

		// 	pointer operator->()
		// 	{
		// 		return ptr_;
		// 	}

		// 	bool operator==(const self_type& rhs)
		// 	{
		// 		return ptr_ == rhs.ptr_;
		// 	}

		// 	bool operator!=(const self_type& rhs)
		// 	{
		// 		return ptr_ != rhs.ptr_;
		// 	}

		// private:
		// 	pointer ptr_;
		// };