#ifndef COMMON_H
#define COMMON_H

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/asio.hpp>


namespace Ip = boost::asio::ip;
std::string formatDateTime(boost::posix_time::ptime const& date_time,std::string const& format);
bool url_decode(const std::string& in, std::string& out);
#endif /* !COMMON_H */
