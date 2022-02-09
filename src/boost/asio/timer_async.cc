#include <iostream>
#include <boost/asio.hpp>

void print(const boost::system::error_code& /*e*/)
{
  std::cout << "Hello, world!" << std::endl;
}

int main()
{
  boost::asio::io_context io;

  auto print_test = [](const boost::system::error_code &){
    std::cout << "Hello, world!" << std::endl;
  };

  boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));
  t.async_wait(print_test);

//   io.run();

  std::cout << "Not being blocked!" << std::endl;

  for (int i = 0; i < 10000000; i++) {
      std::cout << "count " << i << std::endl;
  }
  io.run();

  return 0;
}