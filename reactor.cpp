#include <boost/asio.hpp>
#include <functional>
#include <thread>
#include <iostream>
#include <memory>
#include <cstdlib>

namespace net = boost::asio;
using net::ip::tcp;
using net::dynamic_buffer;

class session : public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket socket)
        : socket_ {std::move(socket)}
    {}

    void start()
    {
        do_read();
    }

private:
    void do_read()
    {
        net::async_read_until(
            socket_,
            dynamic_buffer(command_),
            '\n',
            std::bind_front(&session::handle_read, shared_from_this()));
    }

    void handle_read(boost::system::error_code ec, std::size_t length)
    {
        if (ec == net::error::eof)
        {
            // Socket is closed by the peer. This session will be closed.
        }
        else if (ec)
        {
            // Error case. This session will be closed.
            std::cerr << "Exception: " << ec.message() << '\n';
        }
        else if (command_ == "quit\n")
        {
            // This session will be closed.
        }
        else
        {
            net::async_write(
                socket_,
                net::buffer(command_, length),
                std::bind_front(&session::handle_write, shared_from_this()));
        }
    }

    void handle_write(boost::system::error_code ec, std::size_t /*length*/)
    {
        if (ec == net::error::eof)
        {
            // Socket is closed by the peer. This session will be closed.
        }
        else if (ec)
        {
            // Error case. This session will be closed.
            std::cerr << "Exception: " << ec.message() << '\n';
        }
        else
        {
            do_read();
        }
    }

private:
    tcp::socket socket_;
    std::string command_;
};

class server
{
public:
    server(net::io_context& io_context, unsigned short port)
        : io_context_ {io_context}
        , endpoint_ {tcp::v4(), port}
        , acceptor_ {io_context}
    {
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(
            endpoint_,
            [this](boost::system::error_code ec, tcp::socket socket)
            {
                if (!ec)
                {
                    std::make_shared<session>(std::move(socket))->start();
                }

                do_accept();
            });
    }

private:
    net::io_context& io_context_;
    tcp::endpoint endpoint_;
    tcp::acceptor acceptor_;
};

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
        server s {io_context, static_cast<unsigned short>(std::atoi(argv[1]))};

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

