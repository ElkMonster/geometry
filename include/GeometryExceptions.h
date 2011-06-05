#ifndef GEOMETRYEXCEPTIONS_H_
#define GEOMETRYEXCEPTIONS_H_

#include <exception>
#include <string>

namespace geom
{

namespace error
{


class GeometryError : public std::exception {

public:

    GeometryError(const std::string& msg)
    :   std::exception(),
        msg_(msg)
    {}

    virtual ~GeometryError() throw() {}

    virtual const char* what() const throw()
    {
        return msg_.c_str();
    }

private:

    std::string msg_;

};


class ConsistencyError : public GeometryError
{

public:

    ConsistencyError(const std::string& msg)
    :   GeometryError(msg)
    {}

};


} // namespace error

} // namespace geom


#endif // GEOMETRYEXCEPTIONS_H_
