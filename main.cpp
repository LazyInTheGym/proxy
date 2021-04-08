#include <iostream>
#include <cstring>
#include <thread>

// Socket Programming Headers
#include <cstdlib>

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

boost::asio::io_context& io_context()
{
    static boost::asio::io_context svc;
    return svc;
}

char local_data[1024] = {0};
char remote_data[1024] = {0};

void handle_read(
        boost::asio::ip::tcp::socket& read_from,
        boost::asio::ip::tcp::socket& write_to,
        char* read_buffer,
        size_t bytes,
        const boost::system::error_code& e)
{
    // this function is called whenever data is received

    // for debugging purposes, show the data in the console window
    // or write to file, or whatever...
    std::string data(read_buffer, read_buffer + bytes);
    std::cout << data << "\n";

    // forward the received data on to "the other side"
    write_to.send(
            boost::asio::buffer(read_buffer, bytes));

    // read more data from "this side"
    read_from.async_read_some(
            boost::asio::buffer(read_buffer, 1024),
            boost::bind(handle_read, boost::ref(read_from), boost::ref(write_to), read_buffer, boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));
}

int main(int argc, char** argv)
{
    if(argc == 5)
    {
        boost::asio::io_service::work w(io_context());

        boost::thread t(boost::bind(&boost::asio::io_service::run, (&io_context())));

        // extract the connection information from the command line
        boost::asio::ip::address local_address = boost::asio::ip::address::from_string(argv[1]);
        uint16_t local_port = boost::lexical_cast<uint16_t>(argv[2]);
        boost::asio::ip::address remote_address = boost::asio::ip::address::from_string(argv[3]);
        uint16_t remote_port = boost::lexical_cast<uint16_t>(argv[4]);

        boost::asio::ip::tcp::endpoint local_ep(local_address, local_port);
        boost::asio::ip::tcp::endpoint remote_ep(remote_address, remote_port);

        // start listening on the "local" socket -- note this does not
        // have to be local, you could in theory forward through a remote device
        // it's called "local" in the logical sense
        boost::asio::ip::tcp::acceptor listen(io_context(), local_ep);
        boost::asio::ip::tcp::socket local_socket(io_context());
        listen.accept(local_socket);

        // open the remote connection
        boost::asio::ip::tcp::socket remote_socket(io_context());
        remote_socket.open(remote_ep.protocol());
        remote_socket.connect(remote_ep);

        // start listening for data on the "local" connection
        local_socket.async_receive(
                boost::asio::buffer(local_data, 1024),
                boost::bind(handle_read, boost::ref(local_socket), boost::ref(remote_socket), local_data, boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));

        // also listen for data on the "remote" connection
        remote_socket.async_receive(
                boost::asio::buffer(remote_data, 1024),
                boost::bind(handle_read, boost::ref(remote_socket), boost::ref(local_socket), remote_data, boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));

        t.join();
    }
    else
    {
        std::cout << "proxy <local ip> <port> <remote ip> <port>\n";
    }

    return 0;
}


