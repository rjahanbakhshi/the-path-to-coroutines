#include <boost/asio.hpp>
#include <iostream>
#include <cstdlib>

namespace net = boost::asio;
using net::ip::tcp;
using net::dynamic_buffer;
using net::read_until;

void session(tcp::socket socket)
{
    try
    {
        for (;;)
        {
            std::string command;
            boost::system::error_code error;
            auto length = read_until(socket, dynamic_buffer(command), '\n', error);

            if (error == net::error::eof)
            {
                break;
            }
            else if (error)
            {
                throw boost::system::system_error(error);
            }

            if (command == "quit\n")
            {
                break;
            }

            net::write(socket, net::buffer(command, length));
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Error in session: " << e.what() << '\n';
    }
}

void server(net::io_context& io_context, unsigned short port)
{
    tcp::acceptor acceptor {io_context, tcp::endpoint{tcp::v4(), port}};
    for (;;)
    {
        auto socket = acceptor.accept();
        std::thread { std::bind_front(&session, std::move(socket)) }.detach();
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Binding port must be specified\n";
        return 1;
    }

    try
    {
        net::io_context io_context;
        server(io_context, std::atoi(argv[1]));
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << '\n';
    }

    return 0;
}

