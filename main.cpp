//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>
#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

using boost::asio::ip::tcp;

class tcp_connection : public boost::enable_shared_from_this<tcp_connection> {
 public:
  typedef boost::shared_ptr<tcp_connection> pointer;

  static pointer create(boost::asio::io_context& io_context) {
    return pointer(new tcp_connection(io_context));
  }

  tcp::socket& socket() {
    return socket_;
  }

  void start() {
    message_ = "This a message from the lazy server\n";

    std::this_thread::sleep_for(std::chrono::seconds(3));

    boost::asio::async_write(socket_, boost::asio::buffer(message_),
                             boost::bind(&tcp_connection::handle_write, shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
  }

 private:
  explicit tcp_connection(boost::asio::io_context& io_context) : socket_(io_context) {}

  void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/) {}

  tcp::socket socket_;
  std::string message_;
};

class tcp_server {
 public:
  explicit tcp_server(boost::asio::io_context& io_context)
      : io_context_(io_context), acceptor_(io_context, tcp::endpoint(tcp::v4(), 8080)) {

    start_accept();
  }

 private:
  void start_accept() {
    tcp_connection::pointer new_connection = tcp_connection::create(io_context_);

    acceptor_.async_accept(new_connection->socket(),
                           boost::bind(&tcp_server::handle_accept, this, new_connection,
                                       boost::asio::placeholders::error));
  }

  void handle_accept(tcp_connection::pointer new_connection,
                     const boost::system::error_code& error) {
    if (!error) {
      new_connection->start();
    }

    // get back again to listening to new clients
    start_accept();
  }

  boost::asio::io_context& io_context_;
  tcp::acceptor acceptor_;
};

int main() {
  // TODO: Uncomment this and work it out
  //  try {
  //    boost::asio::io_context io_context;
  //    tcp_server server(io_context);
  //
  //    // This is a blocking call, it will not return as long as there is work to
  //    // do i.e. the server is listening with `acceptor_.async_accept`
  //    io_context.run();
  //  } catch (std::exception& e) {
  //    std::cerr << e.what() << std::endl;
  //  }

  std::cout << "IP addresses: \n";
  boost::asio::io_service io_service;
  boost::asio::ip::tcp::resolver resolver(io_service);
  boost::asio::ip::tcp::resolver::query query("www.facebook.com", "");
  for (boost::asio::ip::tcp::resolver::iterator i = resolver.resolve(query);
       i != boost::asio::ip::tcp::resolver::iterator(); ++i) {
    boost::asio::ip::tcp::endpoint end = *i;
    std::cout << end.address() << ' ';
  }
  std::cout << '\n';

  return 0;
}