#ifndef __PFS_ALGO_JOIN_HPP__
#define __PFS_ALGO_JOIN_HPP__

namespace pfs {

// Sequence must provide push_back(const String &) methods
// String::empty()
//
template <typename InputIterator, typename Sequence>
Sequence * join (
		  InputIterator begin
		, InputIterator end
		, Sequence const & separator
		, Sequence * result = 0)
{
	if (begin == end)
		return result;

    if (!result)
        result = new Sequence;
    
	result->append(*begin++);

	while (begin != end) {
		result->append(separator);
		result->append(*begin++);
	}

	return result;
}

} // pfs

#endif /* __PFS_ALGO_JOIN_HPP__ */
