//
// Copyright (c) 2022 Klemens Morgenstern (klemens.morgenstern@gmx.net)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_SQLITE_IMPL_META_DATA_IPP
#define BOOST_SQLITE_IMPL_META_DATA_IPP

#include <boost/sqlite/meta_data.hpp>

namespace boost
{
namespace sqlite
{



auto table_column_meta_data(connection& conn,
                            const char * db_name, const char * table_name, const char * column_name,
                            error_code & ec, error_info &ei) -> column_meta_data
{
    const char * data_type= "", *collation = "";
    int nn, pk, ai;

    int res = sqlite3_table_column_metadata(conn.native_handle(), db_name, table_name, column_name,
                                            &data_type, &collation, &nn, &pk, &ai);

    if (res != SQLITE_OK)
    {
      BOOST_SQLITE_ASSIGN_EC(ec, res)
      ei.set_message(sqlite3_errmsg(conn.native_handle()));
    }

    return {data_type, collation, nn != 0, pk != 0, ai != 0};
}



auto table_column_meta_data(connection& conn,
                            const char * table_name, const char * column_name,
                            error_code & ec, error_info &ei) -> column_meta_data
{
  const char * data_type= "", *collation = "";
  int nn, pk, ai;

  int res = sqlite3_table_column_metadata(conn.native_handle(), nullptr, table_name, column_name,
                                          &data_type, &collation, &nn, &pk, &ai);

  if (res != SQLITE_OK)
  {
    BOOST_SQLITE_ASSIGN_EC(ec, res)
    ei.set_message(sqlite3_errmsg(conn.native_handle()));
  }

  return {data_type, collation, nn != 0, pk != 0, ai != 0};
}




auto table_column_meta_data(connection& conn,
                            const char * db_name, const char * table_name, const char * column_name) -> column_meta_data
{
    system::error_code ec;
    error_info ei;
    auto res = table_column_meta_data(conn, db_name, table_name, column_name, ec, ei);
    if (ec)
      throw_exception(system::system_error(ec, ei.message()));
    return res;
}

auto table_column_meta_data(connection& conn,
                            const char * table_name, const char * column_name) -> column_meta_data
{
    system::error_code ec;
    error_info ei;
    auto res = table_column_meta_data(conn, table_name, column_name, ec, ei);
    if (ec)
      throw_exception(system::system_error(ec, ei.message()));
    return res;
}


}
}

#endif //BOOST_SQLITE_IMPL_META_DATA_IPP
