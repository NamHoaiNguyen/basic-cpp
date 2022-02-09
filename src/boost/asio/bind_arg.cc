#include <iostream>
#include <functional>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

void print(const boost::system::error_code& /*e*/,
    boost::asio::steady_timer* t, int* count)
{
  if (*count < 5) {
    std::cout << *count << std::endl;
    ++(*count);
  
    t->expires_at(t->expiry() + boost::asio::chrono::seconds(1));
    t->async_wait(boost::bind(print,
          boost::asio::placeholders::error, t, count));
  }
}

int main()
{
  boost::asio::io_context io;

  int count = 0;
  boost::asio::steady_timer t(io, boost::asio::chrono::seconds(1));
  // t.async_wait(boost::bind(print,
  //       boost::asio::placeholders::error, &t, &count));
 
  /*test: using lambda instead of boost:bind*/
  std::function<void(const boost::system::error_code&, boost::asio::steady_timer*, int*)> print_test;
  print_test = [&print_test](const boost::system::error_code&,
                        boost::asio::steady_timer* t, int* count){
    if (*count < 5)
  {
    std::cout << *count << std::endl;
    ++(*count);

    t->expires_at(t->expiry() + boost::asio::chrono::seconds(1));
    t->async_wait([&t, &count, &print_test](const boost::system::error_code& e){
        print_test(e, t, count);
    });
  }};

  t.async_wait([&t, &count, &print_test](const boost::system::error_code& e){
      print_test(e, &t, &count);
  });

  io.run();

  std::cout << "Final count is " << count << std::endl;

  return 0;
}