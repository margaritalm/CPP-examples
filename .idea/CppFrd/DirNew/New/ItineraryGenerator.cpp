// This is the main project file for VC++ application project 
// generated using an Application Wizard.

#include "stdafx.h"
#include "DepartingFlight.h"

#using <mscorlib.dll>

using namespace System;
using namespace System::Data;
using namespace System::Data::SqlClient;

#define MAX_PATH			260
#define STR_HACME_DB		L"HacmeTravelDB"
#define STR_HACME_DB_USER	L"HacmeUser"
#define STR_HACME_DB_PASS	L"HacmePassword"
#define STR_DB_CONN_STRING	L"Data Source=(local);database="STR_HACME_DB L";User ID="STR_HACME_DB_USER L";Password="STR_HACME_DB_PASS L";"

#define STR_BASE_SQL_QUERY	"SELECT OwnerAgent, ResCode, DateOfBooking, TotalAmount, CreditCardCharged ";
#define STR_STANDARD_OUTPUT_FILE "Itinerary_"

bool FillDatasetAndGenerateItinerary(bool bUseStandardOutputFile, TCHAR * strReservationCode, TCHAR *strOutputFile);

int _tmain(int argc, char* argv[])
{
    // TODO: Please replace the sample code below with your own.

	char strReservationCode[MAX_PATH]= "", strOutputFile[MAX_PATH] = "";

	bool bUseStandardOutputFile	= 1;

	if(argc > 1)
	{
		for(int iArgIndex = 1; iArgIndex < argc; iArgIndex++)
		{
			if(0 == ::strcmp(argv[iArgIndex], "-rescode"))
			{
				iArgIndex++;
				if(iArgIndex < argc)
				{
                    ::strncpy(strReservationCode, argv[iArgIndex], MAX_PATH-1);
                    strReservationCode[MAX_PATH-1]='\0';
				}
				continue;
			}
			if(0 == ::strcmp(argv[iArgIndex], "-output"))
			{
				iArgIndex++;
				if(iArgIndex < argc)
				{
                    ::strncpy(strOutputFile, argv[iArgIndex], MAX_PATH-1);
                    strOutputFile[MAX_PATH-1]='\0';
				}
				continue;
			}
			if(0 == ::strcmp(argv[iArgIndex], "/?") || 0 == ::strcmp(argv[iArgIndex], "-h") || 0 == ::strcmp(argv[iArgIndex], "--help"))
			{
				printf("\n\n [Usage]:- \n %s -rescode <Reservation Code> [-output <OutputFile] [/?|-h|--help for usage].\n\n", argv[0]);
				return 0;
			}
		}
	}

	if(0 == ::strcmp("", strReservationCode))
	{
		printf("\n Need to spcify atleast the resevation code before proceeding.\n");
		printf("\n\n [Usage]:- \n %s -rescode <Reservation Code> [-output <OutputFile] [/?|-h|--help for usage].\n\n", argv[0]);
		return 0;
	}

	if(0 != ::strcmp("", strOutputFile))
		bUseStandardOutputFile = 0;

	FillDatasetAndGenerateItinerary(bUseStandardOutputFile, strReservationCode, strOutputFile);

	return 0;
}

bool FillDatasetAndGenerateItinerary(bool bUseStandardOutputFile, TCHAR * strReservationCode, TCHAR *strOutputFile)
{
	// Form the sql query
	char strDatasetQuery[4096] = "";
	sprintf(strDatasetQuery, 
			"Select rescode as ReservationCode, departingflightnum, Airline, CONVERT(char(20), DepartingDate, 7) as DepartingDate, CONVERT(char(20), departuretime, 8) as DepartsAt, "\
			"CONVERT(char(20), arrivaltime,8) as ArrivesAt,  FromAirport, ToAirport, NameOnCard, totalamount as TotalFare, "\
			"Creditcardcharged as CreditCardNumber, HacmeBankAccountId as AccountCharged, Convert(char(20), DateOfBooking, 7) as DateOfBooking, OwnerAgent, "\
			"(passenger1_firstname + ' ' + passenger1_lastname) as Pass1_Name, "\
			"(passenger2_firstname + ' ' + passenger2_lastname) as Pass2_Name, "\
			"(passenger3_firstname + ' ' + passenger3_lastname) as Pass3_Name, "\
			"(passenger4_firstname + ' ' + passenger4_lastname) as Pass4_Name, OneWayTrip "\
			"from flightschedules inner join reservations on departingflightnum = flightnum where rescode ='%s'",
			strReservationCode);

	using namespace System::Data::SqlClient;

	SqlConnection * sqlConn = new SqlConnection(STR_DB_CONN_STRING);

	try
	{
		sqlConn->Open();

		SqlCommand * sqlComm = new SqlCommand(strDatasetQuery, sqlConn);
		sqlComm->CommandType = System::Data::CommandType::Text;

		SqlDataAdapter *sqlda = new SqlDataAdapter();
		sqlda->SelectCommand  = sqlComm;

		//ReportGenerator::ReportDataSet * dsReport = new ReportGenerator::ReportDataSet();
		ItineraryGenerator::DepartingFlight *dsItinerary = new ItineraryGenerator::DepartingFlight();

		sqlda->Fill(dsItinerary, "FlightDetails");

		//using namespace CrystalDecisions::CrystalReports::Engine;
		//using namespace CrystalDecisions::Shared;

		//ReportDocument * ItineraryReportDoc = new ReportDocument();
		
		using namespace System::Data;

		DataTable * dtFlightDetails = dsItinerary->Tables->get_Item("FlightDetails");
		DataRow	  * drFlightDetails = dtFlightDetails->Rows->get_Item(0);

		int iOneWayTrip = System::Convert::ToInt16(drFlightDetails->get_Item("OneWayTrip"));
		if(0 == iOneWayTrip)
		{
	        sprintf(strDatasetQuery, 
				"Select rescode as ReservationCode, ReturningFlightNum, Airline as ReturningAirline, CONVERT(char(20), ReturningDate, 7) as ReturningDate, CONVERT(char(20), departuretime, 8) as ReturnFlightDepartsAt, "\
				"CONVERT(char(20), arrivaltime, 8) as ReturnFlightArrivesAt,  FromAirport as ReturnFlightFromAirport, ToAirport as ReturnFlightToAirport "\
				"from flightschedules inner join reservations on returningflightnum = flightnum where rescode ='%s'",
				strReservationCode);

			sqlComm->CommandText = strDatasetQuery;
			sqlda->SelectCommand = sqlComm;

			sqlda->Fill(dsItinerary, "ReturnFlightDetails");

			DataTable * dtRetFlightDetails = dsItinerary->Tables->get_Item("ReturnFlightDetails");
			DataRow	  * drRetFlightDetails  = dtRetFlightDetails->Rows->get_Item(0);

			drFlightDetails->BeginEdit();
				drFlightDetails->set_Item("ReturningFlightNum", drRetFlightDetails->get_Item("ReturningFlightNum"));
				drFlightDetails->set_Item("ReturningAirline",drRetFlightDetails->get_Item("ReturningAirline"));
				drFlightDetails->set_Item("ReturningDate",drRetFlightDetails->get_Item("ReturningDate"));
				drFlightDetails->set_Item("ReturnFlightDepartsAt", drRetFlightDetails->get_Item("ReturnFlightDepartsAt"));
				drFlightDetails->set_Item("ReturnFlightArrivesAt", drRetFlightDetails->get_Item("ReturnFlightArrivesAt"));
				drFlightDetails->set_Item("ReturnFlightFromAirport",drRetFlightDetails->get_Item("ReturnFlightFromAirport"));
				drFlightDetails->set_Item("ReturnFlightToAirport",drRetFlightDetails->get_Item("ReturnFlightToAirport"));
			drFlightDetails->EndEdit();

//			ItineraryReportDoc->Load("ReturnTripItinerary.rpt");
		}
		//else
		//{
		//	ItineraryReportDoc->Load("OneWayTripItinerary.rpt");
		//}
		//
		//ItineraryReportDoc->SetDataSource(dsItinerary);

		//DiskFileDestinationOptions	*diskOpts	= new DiskFileDestinationOptions();
  //      ExportOptions				*exportOpts = new ExportOptions();
  //      exportOpts = ItineraryReportDoc->ExportOptions;
  //      exportOpts->ExportFormatType		= ExportFormatType::PortableDocFormat;
  //      exportOpts->ExportDestinationType	= ExportDestinationType::DiskFile;

		if(true == bUseStandardOutputFile)
		{
			::strncpy(strOutputFile, STR_STANDARD_OUTPUT_FILE, MAX_PATH-1);
			::strncat(strOutputFile, strReservationCode, MAX_PATH-1);
		}
		::strncat(strOutputFile, ".xml", MAX_PATH-1);

		strOutputFile[MAX_PATH-1] = '\0';

		dsItinerary->WriteXml(strOutputFile, XmlWriteMode::IgnoreSchema);

		System::IO::TextReader *pXMLFile = new System::IO::StreamReader(strOutputFile);
		String * strXML = pXMLFile->ReadToEnd();
		pXMLFile->Close();
		
		strXML = strXML->Replace("<?xml version=\"1.0\" standalone=\"yes\"?>", 
			"<?xml version=\"1.0\" standalone=\"yes\"?><?xml-stylesheet type=\"text/xsl\" href=\"ItineraryFormatter.xsl\"?>");
	
		System::IO::TextWriter *pXMLFileWriter = new System::IO::StreamWriter(strOutputFile);
		pXMLFileWriter->Write(strXML);
		pXMLFileWriter->Flush();
		pXMLFileWriter->Close();

		//diskOpts->DiskFileName = strOutputFile;
		//exportOpts->DestinationOptions = diskOpts;
		//		 
		//ItineraryReportDoc->Export ();
	}
	catch(SqlException *sException)
	{
		printf("\n\t Sql exception caught while opening the database, returning...exception: %S\n", sException->Message);
		return false; 
	}
	catch(...)
	{
		printf("\n\t Unknown exception caught while opening the database or creating the report, returning...\n");
		return false; 
	}

	printf("\n\t Successfully created the itinerary: %s\n", strOutputFile);

	return true;
}
