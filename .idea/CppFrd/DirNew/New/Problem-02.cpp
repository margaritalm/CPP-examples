#include <iostream>
using namespace std;

#include "xmog_java_client.h"
#include "java_lang_pkg.h"
#include "java_sql_pkg.h"
#include "java_util_pkg.h"

void dump( ResultSet rs ) 
{
	// the order of the rows in a cursor
	// are implementation dependent unless you use the SQL ORDER statement
	ResultSetMetaData meta   = rs.getMetaData();
	int               colmax = meta.getColumnCount();
	int               i;
    Object            o = null;

	// the result set is a cursor into the data.  You can only
	// point to one row at a time
	// assume we are pointing to BEFORE the first row
	// rs.next() points to next row and returns true
	// or false if there is no next row, which breaks the loop
	for (; rs.next(); ) {
		for (i = 0; i < colmax; ++i) {
			o = rs.getObject(i + 1);    // Is SQL the first column is indexed

			// with 1 not 0
			cout << (const char*)o.toString() << " ";
		}

		cout << " " << endl;
	}
}


void update( Connection conn, String expression)
{
	Statement st = null;

	st = conn.createStatement();    // statements

	int i = st.executeUpdate(expression);    // run the query

	if (i == -1) {
		cout << "db error : " << (const char*)expression << endl;
	}

	st.close();
}

void query( Connection conn, String expression )
{
	Statement st = null;
	ResultSet rs = null;

	st = conn.createStatement();         // statement objects can be reused with

	// repeated calls to execute but we
	// choose to make a new one each time
	rs = st.executeQuery(expression);    // run the query

	// do something with the result set.
	dump(rs);
	st.close();    
	// NOTE!! if you close a statement the associated ResultSet is closed 
	// too so you should copy the contents to some other object.
	// the result set is invalidated also  if you recycle an Statement
	// and try to execute some other query before the result set has been
	// completely examined.
}

int main(int argc, char * argv[])
{
	//the database filename
	const char *	db_file_name_prefix = argc > 1 ? argv[ 1 ] : "test_db";
	//the database connection
	Connection		conn = null;

	try
	{
		xmog_jvm_loader &	loader = xmog_jvm_loader::get_jvm_loader( true, true, TraceJvm, TraceWarnings );

		loader.appendToClassPath( "../lib/hsqldb.jar" );
		loader.appendToClassPath( "../../lib/hsqldb.jar" );

		if( argc > 1 && !strcmp( argv[ 1 ], "-info" ) )
			loader.printLdLibraryPathAndExit();

		// make the JDBC driver available by preloading it
		Class::forName("org.hsqldb.jdbcDriver");

        // connect to the database.   This will load the db files and start the
        // database if it is not alread running.
        // db_file_name_prefix is used to open or create files that hold the state
        // of the db.
        // It can contain directory names relative to the
        // current working directory
		conn = DriverManager::getConnection( String("jdbc:hsqldb:").concat( db_file_name_prefix ),
                                           "sa",                     // username
                                           "");                      // password

        // create a table and insert some entries
		// this will only worl the first time and throw an exception every following time
		// because the table already exists 
		try
		{
			update( conn, "CREATE TABLE sample_table ( id INTEGER IDENTITY, str_col VARCHAR(256), num_col INTEGER)");

			// add some rows - will create duplicates if run more then once
			// the id column is automatically generated
			update( conn, "INSERT INTO sample_table(str_col,num_col) VALUES('Ford', 100)");
			update( conn, "INSERT INTO sample_table(str_col,num_col) VALUES('Toyota', 200)");
			update( conn, "INSERT INTO sample_table(str_col,num_col) VALUES('Honda', 300)");
			update( conn, "INSERT INTO sample_table(str_col,num_col) VALUES('GM', 400)");
			update( conn, "INSERT INTO sample_table(str_col,num_col) VALUES('BMW', 80)");
			update( conn, "INSERT INTO sample_table(str_col,num_col) VALUES('Mercedes-Benz', 60)");
			update( conn, "INSERT INTO sample_table(str_col,num_col) VALUES('VW', 800)");
		}
		catch( Throwable & it )
		{
			cout << (const char*)it.toString() << endl;
		}
		catch( xmog_exception & ie )
		{
		}

		cout << "---------------" << endl;

		// do a query
        query( conn, "SELECT * FROM sample_table WHERE num_col < 250");

		cout << "---------------" << endl;

		// do another query
        query( conn, "SELECT str_col FROM sample_table WHERE num_col >= 100");

		cout << "---------------" << endl;

		conn.close();
	}
	catch( Throwable & t )
	{
		cout << "Caught throwable of type " << (const char*)t.getClass().getName() << endl;
		cout << (const char*)t.toString() << endl;
		return 1;
	}
	catch( xmog_exception & xe )
	{
		cout << "Caught xmog_exception" << endl;

		char *	msg = xe.get_message_chars();

		if( msg )
		{
			cout << msg << endl;
			xmog_java_string::free( msg );
		}
		return 1;
	}

	return 0;
}

