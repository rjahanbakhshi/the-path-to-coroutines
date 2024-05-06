#include <boost/asio.hpp>
#include <thread>
#include <iostream>
#include <cstdlib>

namespace net = boost::asio;
namespace this_coro = boost::asio::this_coro;
using net::ip::tcp;
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
using net::dynamic_buffer;

awaitable<void> session(tcp::socket socket)
{
    try
    {
        for (;;)
        {
            std::string command;
            boost::system::error_code error;
            auto length = co_await net::async_read_until(socket, dynamic_buffer(command), '\n', use_awaitable);

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

            co_await net::async_write(socket, net::buffer(command, length), use_awaitable);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Error in session: " << e.what() << '\n';
    }
}

awaitable<void> server(unsigned short port)
{
    auto executor = co_await this_coro::executor;
    tcp::acceptor acceptor {executor, tcp::endpoint{tcp::v4(), port}};

    for (;;)
    {
        auto socket = co_await acceptor.async_accept(use_awaitable);
        co_spawn(executor, session(std::move(socket)), detached);
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
        co_spawn(io_context, server(std::atoi(argv[1])), detached);

        std::vector<std::jthread> threads;
        for (int i = 0; i < 10; ++i)
        {
            threads.emplace_back([&io_context]{ io_context.run(); });
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << '\n';
    }

    return 0;
}

