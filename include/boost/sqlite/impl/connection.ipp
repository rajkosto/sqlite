//
// Copyright (c) 2022 Klemens Morgenstern (klemens.morgenstern@gmx.net)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_SQLITE_IMPL_CONNECTION_IPP
#define BOOST_SQLITE_IMPL_CONNECTION_IPP

#include <boost/sqlite/connection.hpp>

namespace boost
{
namespace sqlite
{


void connection::connect(const char * filename)
{
    system::error_code ec;
    connect(filename, ec);
    if (ec)
        throw_exception(system::system_error(ec, "connect"));
}

void connection::connect(const char * filename, system::error_code & ec)
{
    sqlite3 * res;
    auto r = sqlite3_open_v2(filename, &res,
                             SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                             nullptr);
    if (r != SQLITE_OK)
        BOOST_SQLITE_ASSIGN_EC(ec, r)
    else
        impl_.reset(res);
}

void connection::close()
{
    system::error_code ec;
    error_info ei;
    close(ec, ei);
    if (ec)
        throw_exception(system::system_error(ec, ei.message()));
}

void connection::close(error_code & ec,
                       error_info & ei)
{
    if (impl_)
    {
        auto tmp = impl_.release();
        auto cc = sqlite3_close(tmp);
        if (SQLITE_OK != cc)
        {
            impl_.reset(tmp);
            BOOST_SQLITE_ASSIGN_EC(ec, cc)
            ei.set_message(sqlite3_errmsg(impl_.get()));
        }
    }
}


resultset connection::query(
        core::string_view q,
        error_code & ec,
        error_info & ei)
{
    resultset res;
    sqlite3_stmt * ss;
    const auto cc = sqlite3_prepare_v2(impl_.get(),
                       q.data(), static_cast<int>(q.size()),
                       &ss, nullptr);

    if (cc != SQLITE_OK)
    {
        BOOST_SQLITE_ASSIGN_EC(ec, cc)
        ei.set_message(sqlite3_errmsg(impl_.get()));
    }
    else
        res.impl_.reset(ss);
    return res;
}

resultset connection::query(core::string_view q)
{
    system::error_code ec;
    error_info ei;
    auto tmp = query(q, ec, ei);
    if (ec)
        throw_exception(system::system_error(ec, ei.message()));
    return tmp;
}

statement connection::prepare_statement(
        core::string_view q,
        error_code & ec,
        error_info & ei)
{
    statement res;
    sqlite3_stmt * ss;
    const auto cc = sqlite3_prepare_v2(impl_.get(),
                                       q.data(), static_cast<int>(q.size()),
                                       &ss, nullptr);

    if (cc != SQLITE_OK)
    {
        BOOST_SQLITE_ASSIGN_EC(ec, cc)
        ei.set_message(sqlite3_errmsg(impl_.get()));
    }
    else
        res.impl_.reset(ss);
    return res;
}

statement connection::prepare_statement(core::string_view q)
{
    system::error_code ec;
    error_info ei;
    auto tmp = prepare_statement(q, ec, ei);
    if (ec)
        throw_exception(system::system_error(ec, ei.message()));
    return tmp;
}


}
}

#endif //BOOST_SQLITE_IMPL_CONNECTION_IPP
