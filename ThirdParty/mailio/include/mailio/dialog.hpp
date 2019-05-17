/*

dialog.hpp
----------

Copyright (C) 2016, Tomislav Karastojkovic (http://www.alepho.com).

Distributed under the FreeBSD license, see the accompanying file LICENSE or
copy at http://www.freebsd.org/copyright/freebsd-license.html.

*/ 


#pragma once

#include <string>
#include <stdexcept>
#include <chrono>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "../../export.hpp"


namespace mailio
{


/**
Dealing with network in a line oriented fashion.
**/
class MAILIO_EXPORT dialog
{
public:

    /**
    Making a connection to the server.

    @param hostname     Server hostname.
    @param port         Server port.
    @param timeout      Network timeout after which I/O operations fail. If zero, then no timeout is set i.e. I/O operations are synchronous.
    @throw dialog_error Server connecting failed.
    @throw *             `connect_async()`.
    **/
    dialog(const std::string& hostname, unsigned port, std::chrono::milliseconds timeout);

    /**
    Moving server parameters and connection.

    @param other Dialog to move from.
    **/
    dialog(dialog&& other);

    /**
    Closing the connection.
    **/
    virtual ~dialog();

    dialog(const dialog& other) = delete;

    void operator=(const dialog&) = delete;

    void operator=(dialog&&) = delete;

    /**
    Sending a line to network synchronously or asynchronously, depending of the timeout value.

    @param line Line to send.
    @throw *    `send_sync<Socket>(Socket&, const std::string&)`, `send_async<Socket>(Socket&, const std::string&)`.
    **/
    virtual void send(const std::string& line);

    /**
    Receiving a line from network.

    @param raw Flag if the receiving is raw (no CRLF is truncated) or not.
    @return    Line read from network.
    @throw *   `receive_sync<Socket>(Socket&, bool)`, `receive_async<Socket>(Socket&, bool)`.
    **/
    virtual std::string receive(bool raw = false);

protected:

    /**
    Sending a line to network in synchronous manner.

    @param socket       Socket to use for I/O.
    @param line         Line to send.
    @throw dialog_error Network sending error.
    **/
    template<typename Socket>
    void send_sync(Socket& socket, const std::string& line);

    /**
    Receiving a line from network in synchronous manner.

    @param socket       Socket to use for I/O.
    @param raw          Flag if the receiving is raw (no CRLF is truncated) or not.
    @return line        Line received.
    @throw dialog_error Network sending error.
    **/
    template<typename Socket>
    std::string receive_sync(Socket& socket, bool raw);

    /**
    Connecting to the host within the given timeout period.

    @throw dialog_error Server connecting failed.
    @throw dialog_error Server connecting timed out.
    **/
    void connect_async();

    /**
    Sending a line over network within the given timeout period.

    @param socket       Socket to use for I/O.
    @param line         Line to send.
    @throw dialog_error Network sending failed.
    @throw dialog_error Network sending timed out.
    **/
    template<typename Socket>
    void send_async(Socket& socket, std::string line);

    /**
    Receiving a line over network within the given timeout period.

    @param socket       Socket to use for I/O.
    @param raw          Flag if the receiving is raw (no CRLF is truncated) or not.
    @return line        Line received.
    @throw dialog_error Network receiving failed.
    @throw dialog_error Network receiving timed out.
    **/
    template<typename Socket>
    std::string receive_async(Socket& socket, bool raw);

    /**
    Checking if the timeout is reached.

    @param error Error code.
    @todo        Handle errors.
    **/
    void check_timeout(const boost::system::error_code& error);

    /**
    Server hostname.
    **/
    const std::string _hostname;

    /**
    Server port.
    **/
    const unsigned int _port;

    /**
    Asio input/output service.
    **/
    std::unique_ptr<boost::asio::io_context> _ios;

    /**
    Socket connection.
    **/
    boost::asio::ip::tcp::socket _socket;

    /**
    Timer to check the timeout.
    **/
    boost::asio::deadline_timer _timer;

    /**
    Timeout on I/O operations in milliseconds.
    **/
    std::chrono::milliseconds _timeout;

    /**
    Flag to show whether the timeout has expired.
    **/
    bool _timer_expired;

    /**
    Stream buffer associated to the socket.
    **/
    std::unique_ptr<boost::asio::streambuf> _strmbuf;

    /**
    Input stream associated to the buffer.
    **/
    std::unique_ptr<std::istream> _istrm;
};


/**
Secure version of `dialog` class.
**/
class dialog_ssl : public dialog
{
public:

    /**
    Making a connection to the server.

    @param hostname Server hostname.
    @param port     Server port.
    @param timeout  Network timeout after which I/O operations fail. If zero, then no timeout is set i.e. I/O operations are synchronous.
    @throw *        `dialog::dialog(const std::string&, unsigned)`.
    **/
    dialog_ssl(const std::string& hostname, unsigned port, std::chrono::milliseconds timeout);

    /**
    Calling the parent constructor and members copy constructor.
    **/
    dialog_ssl(const dialog_ssl&) = default;

    /**
    Moving the server parameters and connection.

    @param other        Dialog to move from.
    @throw dialog_error Switching to ssl failed.
    @throw *            `dialog::dialog(dialog&&)`.
    **/
    dialog_ssl(dialog&& other);

    /**
    Default destructor.
    **/
    virtual ~dialog_ssl() = default;

    dialog_ssl(dialog_ssl&&) = delete;

    void operator=(const dialog_ssl&) = delete;

    void operator=(dialog_ssl&&) = delete;

    /**
    Sending an encrypted or unecrypted line, depending of SSL flag.

    @param line Line to send.
    @throw *    `dialog::send(const std::string&)`, `send_sync<Socket>(Socket&, const std::string&)`, `send_async<Socket>(Socket&, const std::string&)`.
    **/
    void send(const std::string& line);

    /**
    Receiving an encrypted or unecrypted line, depending of SSL state.

    @param raw Flag if the receiving is raw (no CRLF is truncated) or not.
    @return    Line read from network
    @throw *   `dialog::receive()`, `receive_sync<Socket>(Socket&, bool)`, `receive_async<Socket>(Socket&, bool)`.
    **/
    std::string receive(bool raw = false);

protected:

    /**
    Flag if SSL is chosen or not.
    **/
    bool _ssl;

    /**
    SSL context (when used).
    **/
    boost::asio::ssl::context _context;

    /**
    SSL socket (when used).
    **/
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket&> _ssl_socket;
};


/**
Error thrown by `dialog` client.
**/
class dialog_error : public std::runtime_error
{
public:

    /**
    Calling the parent constructor.

    @param msg Error message.
    **/
    explicit dialog_error(const std::string& msg) : std::runtime_error(msg)
    {
    }

    /**
    Calling the parent constructor.

    @param msg Error message.
    **/
    explicit dialog_error(const char* msg) : std::runtime_error(msg)
    {
    }
};


} // namespace mailio
