#include "StdAfx.h"
#include "ServicedAirports.h"

#include "Common.h"

using namespace HacmeTravelAgent;
using namespace System;
using namespace System::Net::Sockets;
using namespace System::Data;
using namespace System::Xml;
using namespace System::Text;
using namespace System::IO;

System::Void ServicedAirports::ServicedAirports_Load(System::Object *  sender, System::EventArgs *  e)
{
	Form1 *pForm1 = m_pMainPage->get_ParentForm(); 
	String *strUserName = pForm1->getAgentName();
	String *strUserType = S"Normal";
	if(0 != pForm1->isAgentAdmin())
		strUserType = S"Administrator";

	this->Text = String::Concat(this->Text, S"| ", strUserName, S" - ");
	this->Text = String::Concat(this->Text, strUserType);

	TcpClient * pClient = new TcpClient();
	bool bSuccess = CCommon::ConnectToServer(m_strServer, 8765, pClient);
	if(true != bSuccess)
		return;

	bSuccess = SendViewAirportsServicedRequestToServer(pClient);
	if(true != bSuccess)
		return;

	bSuccess = SendViewAirportsServicedRequestData(pClient);
	if(true != bSuccess)
		return;

	Byte bytesReceived[] = new Byte[1*1024*1024];
	int iBytesActuallyRead = 0;

	Cursor::Current = Cursors::WaitCursor ;

	bSuccess = CCommon::GetServerReponse(pClient, bytesReceived, iBytesActuallyRead);

	Cursor::Current = Cursors::Default ;

	if(true != bSuccess)
		return;

	// Remove the ending --END_OF_SERVER_RESPONSE--
	Byte byteFlightData[] = new Byte[iBytesActuallyRead];
	Array::Copy(bytesReceived, 0, byteFlightData, 0, iBytesActuallyRead);

	ASCIIEncoding *pAscii = new ASCIIEncoding();
	String * strReservationDataXML = pAscii->GetString(byteFlightData, 0, iBytesActuallyRead);
    NameTable * nt = new NameTable();
    XmlNamespaceManager * nsmgr = new XmlNamespaceManager(nt);

	//Create the XmlParserContext.	
	XmlParserContext * context = new XmlParserContext(NULL, nsmgr, NULL, XmlSpace::None);
            
    //Implement the reader. 
	XmlTextReader * reader = new XmlTextReader(strReservationDataXML, XmlNodeType::Document, context);

	DataSet * pAirportDataset = new DataSet();
	try
	{
		//pFlightDataset->ReadXml("FlightData.xml", XmlReadMode::InferSchema);
		pAirportDataset->ReadXml(reader, XmlReadMode::InferSchema);
	}
	catch(...)
	{
		return;
	}

    DataGridTableStyle* TSAirports = new DataGridTableStyle();
    TSAirports->MappingName = S"Airports";

    // Add a GridColumnStyle and set the MappingName 
    // to the name of a DataColumn in the DataTable. 
    // Set the HeaderText and Width properties. 
    DataGridColumnStyle* TCAirportCode = new DataGridTextBoxColumn();
    TCAirportCode->MappingName = "AirportCode";
    TCAirportCode->HeaderText = "Airport Code";
    TCAirportCode->Width = 100;
    TSAirports->GridColumnStyles->Add(TCAirportCode);

	DataGridColumnStyle* TCAirportCity= new DataGridTextBoxColumn();
    TCAirportCity->MappingName = "AirportCity";
    TCAirportCity->HeaderText = "City";
    TCAirportCity->Width = 200;
    TSAirports->GridColumnStyles->Add(TCAirportCity);

    // Add the DataGridTableStyle instance to 
    // the GridTableStylesCollection. 
    dgServicedAirports->TableStyles->Add(TSAirports);

	dgServicedAirports->DataSource = pAirportDataset;
	dgServicedAirports->DataMember = "Airports";

	if (0 == pAirportDataset->Tables->get_Item("Airports")->Rows->Count)
	{
		MessageBox::Show("No reservations found", "View Reservation", MessageBoxButtons::OK, MessageBoxIcon::Exclamation, MessageBoxDefaultButton::Button1, MessageBoxOptions::DefaultDesktopOnly);
		return ;		
	}

	return;
}

bool ServicedAirports::SendViewAirportsServicedRequestToServer(TcpClient *pClient)
{
	String * strOpCode = "ViewAirportsServicedRequest";
		
	bool bSuccess = CCommon::SendDataToServer(strOpCode,pClient);

	bSuccess = CCommon::SendDataToServer("--END_OF_CLIENT_REQUEST--", pClient);
	if(true != bSuccess)
		return false;

	bSuccess = CCommon::GetServerReponseForOpCode(pClient);
	if(true != bSuccess)
		return false;

	return true;
}

bool ServicedAirports::SendViewAirportsServicedRequestData(TcpClient *pClient)
{
	String * strViewAirportsRequest = new String("");
	strViewAirportsRequest = "[ViewAirportsServicedRequest]";
	strViewAirportsRequest = String::Concat(strViewAirportsRequest, "\n");

	bool bSuccess = CCommon::SendDataToServer(strViewAirportsRequest, pClient);
	if(true != bSuccess)
		return false;

	bSuccess = CCommon::SendDataToServer("--END_OF_CLIENT_REQUEST--", pClient);
	if(true != bSuccess)
		return false;

	return true;	
}

System::Void ServicedAirports::btnClose_Click(System::Object *  sender, System::EventArgs *  e)
{
	this->Close();
}