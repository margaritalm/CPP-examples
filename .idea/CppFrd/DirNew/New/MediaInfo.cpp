class CMediaInfoDLL
{
public:
	CMediaInfoDLL()
	{
		m_bInitialized = FALSE;
		m_hLib = NULL;
		m_ullVersion = 0;
		// MediaInfoLib - v0.4.0.1
		m_pfnMediaInfo4_Open = NULL;
		m_pfnMediaInfo4_Close = NULL;
		m_pfnMediaInfo4_Get = NULL;
		m_pfnMediaInfo4_Count_Get = NULL;
		// MediaInfoLib - v0.5 - v0.6.1
		m_pfnMediaInfo5_Open = NULL;
		// MediaInfoLib - v0.7+
		m_pfnMediaInfo_New = NULL;
		m_pfnMediaInfo_Delete = NULL;
		m_pfnMediaInfo_Open = NULL;
		m_pfnMediaInfo_Close = NULL;
		m_pfnMediaInfo_Get = NULL;
		m_pfnMediaInfo_Count_Get = NULL;
	}
	~CMediaInfoDLL()
	{
		if (m_hLib)
			FreeLibrary(m_hLib);
	}

	BOOL Initialize()
	{
		if (!m_bInitialized)
		{
			m_bInitialized = TRUE;

			CString strPath = theApp.GetProfileString(_T("eMule"), _T("MediaInfo_MediaInfoDllPath"), _T("MEDIAINFO.DLL"));
			m_hLib = LoadLibrary(strPath);
			if (m_hLib != NULL)
			{
				ULONGLONG ullVersion = GetModuleVersion(m_hLib);
				if (ullVersion == 0) // MediaInfoLib - v0.4.0.1 does not have a Win32 version info resource record
				{

					(FARPROC &)fpMediaInfo4_Info_Version = GetProcAddress(m_hLib, "MediaInfo_Info_Version");
					if (fpMediaInfo4_Info_Version)
					{
						char* pszVersion = (*fpMediaInfo4_Info_Version)();
						if (pszVersion && strcmp(pszVersion, "MediaInfoLib - v0.4.0.1 - http://mediainfo.sourceforge.net") == 0)
						{
							(FARPROC &)m_pfnMediaInfo4_Open = GetProcAddress(m_hLib, "MediaInfo_Open");
							(FARPROC &)m_pfnMediaInfo4_Close = GetProcAddress(m_hLib, "MediaInfo_Close");
							(FARPROC &)m_pfnMediaInfo4_Get = GetProcAddress(m_hLib, "MediaInfo_Get");
							(FARPROC &)m_pfnMediaInfo4_Count_Get = GetProcAddress(m_hLib, "MediaInfo_Count_Get");
							if (m_pfnMediaInfo4_Open && m_pfnMediaInfo4_Close && m_pfnMediaInfo4_Get) {
								m_ullVersion = MAKEDLLVERULL(0, 4, 0, 1);
								return TRUE;
							}
							m_pfnMediaInfo4_Open = NULL;
							m_pfnMediaInfo4_Close = NULL;
							m_pfnMediaInfo4_Get = NULL;
							m_pfnMediaInfo4_Count_Get = NULL;
						}
					}
				}
				// Note from MediaInfo developer
				// -----------------------------
				// Note : versioning method, for people who develop with LoadLibrary method
				// - if one of 2 first numbers change, there is no guaranties that the DLL is compatible with old one
				// - if one of 2 last numbers change, there is a garanty that the DLL is compatible with old one.
				// So you should test the version of the DLL, and if one of the 2 first numbers change, not load it.
				// ---
				// eMule currently handles v0.5.1.0, v0.6.0.0, v0.6.1.0
				else if (ullVersion >= MAKEDLLVERULL(0, 5, 0, 0) && ullVersion < MAKEDLLVERULL(0, 7, 0, 0))
				{
					// Don't use 'MediaInfo_Info_Version' with version v0.5+. This function is exported,
					// can be called, but does not return a valid version string..

					(FARPROC &)m_pfnMediaInfo5_Open = GetProcAddress(m_hLib, "MediaInfo_Open");
					(FARPROC &)m_pfnMediaInfo_Close = GetProcAddress(m_hLib, "MediaInfo_Close");
					(FARPROC &)m_pfnMediaInfo_Get = GetProcAddress(m_hLib, "MediaInfo_Get");
					(FARPROC &)m_pfnMediaInfo_Count_Get = GetProcAddress(m_hLib, "MediaInfo_Count_Get");
					if (m_pfnMediaInfo5_Open && m_pfnMediaInfo_Close && m_pfnMediaInfo_Get) {
						m_ullVersion = ullVersion;
						return TRUE;
					}
					m_pfnMediaInfo5_Open = NULL;
					m_pfnMediaInfo_Close = NULL;
					m_pfnMediaInfo_Get = NULL;
					m_pfnMediaInfo_Count_Get = NULL;
				}
				else if (ullVersion >= MAKEDLLVERULL(0, 7, 0, 0) && ullVersion < MAKEDLLVERULL(0, 8, 0, 0))
				{
					(FARPROC &)m_pfnMediaInfo_New = GetProcAddress(m_hLib, "MediaInfo_New");
					(FARPROC &)m_pfnMediaInfo_Delete = GetProcAddress(m_hLib, "MediaInfo_Delete");
					(FARPROC &)m_pfnMediaInfo_Open = GetProcAddress(m_hLib, "MediaInfo_Open");
					(FARPROC &)m_pfnMediaInfo_Close = GetProcAddress(m_hLib, "MediaInfo_Close");
					(FARPROC &)m_pfnMediaInfo_Get = GetProcAddress(m_hLib, "MediaInfo_Get");
					(FARPROC &)m_pfnMediaInfo_Count_Get = GetProcAddress(m_hLib, "MediaInfo_Count_Get");
					if (m_pfnMediaInfo_New && m_pfnMediaInfo_Delete && m_pfnMediaInfo_Open && m_pfnMediaInfo_Close && m_pfnMediaInfo_Get) {
						m_ullVersion = ullVersion;
						return TRUE;
					}
					m_pfnMediaInfo_New = NULL;
					m_pfnMediaInfo_Delete = NULL;
					m_pfnMediaInfo_Open = NULL;
					m_pfnMediaInfo_Close = NULL;
					m_pfnMediaInfo_Get = NULL;
					m_pfnMediaInfo_Count_Get = NULL;
				}
				FreeLibrary(m_hLib);
				m_hLib = NULL;
			}
		}
		return m_hLib != NULL;
	}

	ULONGLONG GetVersion() const
	{
		return m_ullVersion;
	}

	void* Open(LPCTSTR File)
	{
		if (m_pfnMediaInfo4_Open) {
			USES_CONVERSION;
			return (*m_pfnMediaInfo4_Open)(T2A(File));
		}
		else if (m_pfnMediaInfo5_Open)
			return (*m_pfnMediaInfo5_Open)(File);
		else if (m_pfnMediaInfo_New) {
			void* Handle = (*m_pfnMediaInfo_New)();
			if (Handle)
				(*m_pfnMediaInfo_Open)(Handle, File);
			return Handle;
		}
		return NULL;
	}

	void Close(void* Handle)
	{
		if (m_pfnMediaInfo_Delete)
			(*m_pfnMediaInfo_Delete)(Handle);	// File is automaticly closed
		else if (m_pfnMediaInfo4_Close)
			(*m_pfnMediaInfo4_Close)(Handle);
		else if (m_pfnMediaInfo_Close)
			(*m_pfnMediaInfo_Close)(Handle);
	}

	CString Get(void* Handle, stream_t_C StreamKind, int StreamNumber, LPCTSTR Parameter, info_t_C KindOfInfo, info_t_C KindOfSearch)
	{
		if (m_pfnMediaInfo4_Get) {
			USES_CONVERSION;
			return CString((*m_pfnMediaInfo4_Get)(Handle, StreamKind, StreamNumber, T2A(Parameter), KindOfInfo, KindOfSearch));
		}
		else if (m_pfnMediaInfo_Get) {
			CString strNewParameter(Parameter);
			if (m_ullVersion >= MAKEDLLVERULL(0, 7, 1, 0)) {
				// Convert old tags to new tags
				strNewParameter.Replace(_T('_'), _T('/'));

				// Workaround for a bug in MediaInfoLib
				if (strNewParameter == _T("Channels"))
					strNewParameter = _T("Channel(s)");
			}
			return (*m_pfnMediaInfo_Get)(Handle, StreamKind, StreamNumber, strNewParameter, KindOfInfo, KindOfSearch);
		}
		return _T("");
	}

	int Count_Get(void* Handle, stream_t_C StreamKind, int StreamNumber)
	{
		if (m_pfnMediaInfo4_Get)
			return (*m_pfnMediaInfo4_Count_Get)(Handle, StreamKind, StreamNumber);
		else if (m_pfnMediaInfo_Count_Get)
			return (*m_pfnMediaInfo_Count_Get)(Handle, StreamKind, StreamNumber);
		return 0;
	}

protected:
	ULONGLONG m_ullVersion;
	BOOL m_bInitialized;
	HINSTANCE m_hLib;

	// MediaInfoLib - v0.4.0.1

	
	
	
	
	// MediaInfoLib - v0.5+

	
	
	
	
	// MediaInfoLib - v0.7+

	
	
};

CMediaInfoDLL theMediaInfoDLL;
