#ifndef DEEPSKYDISCOVERY_DETAIL_ERROR_HPP_INCLUDED
#define DEEPSKYDISCOVERY_DETAIL_ERROR_HPP_INCLUDED 1

namespace deepskydiscovery {
namespace detail {

struct AssertionErrorType {};
struct IndexErrorType     {};
struct MemoryErrorType    {};
struct RuntimeErrorType   {};
struct TypeErrorType      {};
struct ValueErrorType     {};

template<class ErrType>
class error: public std::exception
{
  public:
    error(std::string msg)
      : msg_(msg)
    {}

    const char* what() const throw()
    {
        return msg_.c_str();
    }

    ~error() throw() {}

  protected:
    std::string msg_;
};

}// namespace detail
}// namespace deepskydiscovery

#endif // ! DEEPSKYDISCOVERY_DETAIL_ERROR_HPP_INCLUDED
