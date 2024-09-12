﻿/****************************************************************************/
/*                                                                          */
/*      文件名:    NTService.h                                              */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/

// last revised: $Date: 5.05.98 21:22 $, $Revision: 2 $

#include "winsvc.h"

#ifndef NTService_h
#define NTService_h


//##ModelId=40F1FA040138
class CNTService {
	//##ModelId=40F1FA040168
	static BOOL				m_bInstance;			// only one CNTService object per application
	
	protected:	// data members
	//##ModelId=40F1FA040167
		LPCTSTR					m_lpServiceName;
	//##ModelId=40F1FA04015F
		LPCTSTR					m_lpDisplayName;
	//##ModelId=40F1FA04015E
		DWORD					m_dwCheckPoint;
	//##ModelId=40F1FA04015D
		DWORD					m_dwErr;
	//##ModelId=40F1FA04015C
		BOOL					m_bDebug;			// TRUE if -d was passed to the program
	//##ModelId=40F1FA040159
		SERVICE_STATUS			m_ssStatus;			// current status of the service
	//##ModelId=40F1FA040152
		SERVICE_STATUS_HANDLE	m_sshStatusHandle;
	//##ModelId=40F1FA040151
		DWORD					m_dwControlsAccepted;	// bit-field of what control requests the
														// service will accept
														// (dflt: SERVICE_ACCEPT_STOP)
	//##ModelId=40F1FA04014E
		PSID					m_pUserSID;			// the current user's security identifier
	//##ModelId=40F1FA04014B
		BOOL					m_bWinNT;			// TRUE, if this is running on WinNT FALSE on Win95
	//##ModelId=40F1FA04014A
		BOOL					m_fConsoleReady;

		// parameters to the "CreateService()" function:
	//##ModelId=40F1FA040149
		DWORD			m_dwDesiredAccess;		// default: SERVICE_ALL_ACCESS
	//##ModelId=40F1FA040148
		DWORD			m_dwServiceType;		// default: SERVICE_WIN32_OWN_PROCESS
	//##ModelId=40F1FA04013F
		DWORD			m_dwStartType;			// default: SERVICE_AUTO_START
	//##ModelId=40F1FA04013E
		DWORD			m_dwErrorControl;		// default: SERVICE_ERROR_NORMAL
	//##ModelId=40F1FA04013D
		LPCTSTR			m_pszLoadOrderGroup;	// default: NULL
	//##ModelId=40F1FA04013C
		DWORD			m_dwTagID;				// retrieves the tag identifier
	//##ModelId=40F1FA04013B
		LPCTSTR			m_pszDependencies;		// default: NULL
	//##ModelId=40F1FA04013A
		LPCTSTR			m_pszStartName;			// default: NULL
	//##ModelId=40F1FA040139
		LPCTSTR			m_pszPassword;			// default: NULL


	public:		// construction/destruction
			// If <DisplayName> is not set, then it defaults to <ServiceName>.
	//##ModelId=40F1FA0401B6
		CNTService(LPCTSTR ServiceName, LPCTSTR DisplayName = 0);
	//##ModelId=40F1FA0401B5
		~CNTService();

	private:	// forbidden functions
	//##ModelId=40F1FA0401B9
		CNTService( const CNTService & );
	//##ModelId=40F1FA0401AD
		CNTService & operator=( const CNTService & );

	public:		// overridables
			// You have to override the following two functions.
			// "Run()" will be called to start the real
			// service's activity. You must call
			//		ReportStatus(SERVICE_RUNNING);
			// before you enter your main-loop !
			// "Stop()" will be called to stop the work of
			// the service. You should break out of the mainloop
			// and return control to the CNTService class.
			//
			// In most cases these functions look like these:
			//
			//		void CMyService :: Run(DWORD argc, LPTSTR * argv) {
			//			ReportStatus(SERVICE_START_PENDING);
			//			// do some parameter processing ...
			//			ReportStatus(SERVICE_START_PENDING);
			//			// do first part of initialisation ...
			//			ReportStatus(SERVICE_START_PENDING);
			//			// do next part of initialisation
			//			// ...
			//			m_hStop = CreateEvent(0, TRUE, FALSE, 0);
			//			ReportStatus(SERVICE_RUNNING);
			//			while( WaitForSingleObject(m_hStop, 10) != WAIT_OBJECT_0 ) {
			//				// do something
			//			}
			//			if( m_hStop )
			//				CloseHandle(m_hStop);
			//		}
			//
			//		void CMyService :: Stop() {
			//			if( m_hStop )
			//				SetEvent(m_hStop);
			//			ReportStatus(SERVICE_STOP_PENDING);
			//		}
	//##ModelId=40F1FA0401AA
		virtual void	Run(DWORD argc, LPTSTR * argv) = 0;
	//##ModelId=40F1FA0401A9
		virtual void	Stop() = 0;

			// Pause() and Continue() do nothing by default.
			// You can override them to handle a control request.
			// Pause() should report the status SERVICE_PAUSED
			// and Continue() should report the status SERVICE_RUNNING
			// (see ReportStatus() below).
			// Note that normally these functions will never be called. If
			// you want a service, that accepts PAUSE and CONTINUE control
			// requests, you have to to add SERVICE_ACCEPT_PAUSE_CONTINUE
			// to the m_dwControlsAccepted data member.
	//##ModelId=40F1FA0401A8
		virtual void	Pause();
	//##ModelId=40F1FA0401A7
		virtual void	Continue();

			// Shutdown() will be called, if the service manager
			// requests for the SERVICE_CONTROL_SHUTDOWN control.
			// This control type occurs, when the system shuts down.
			// If you want to process this notification, you have to
			// add SERVICE_ACCEPT_SHUTDOWN to the m_dwControlsAccepted
			// data member (and to override this function). The default
			// implementation of Shutdown() does nothing.
	//##ModelId=40F1FA0401A6
		virtual void	Shutdown();

			// Call "RegisterService()" after you have constructed
			// a CNTService object:
			// A typical "main()" function of a service looks like this:
			//
			//		int main( int argc, char ** argv ) {
			//			CMyService serv;
			//			exit(serv.RegisterService(argc, argv));
			//		}
			//
			// Where "CMyService" is a CNTService derived class.
			// RegisterService() checks the parameterlist. The
			// following parameters will be detected:
			//		-i			install the service (calls
			//					"InstallService()" - see below)
			//
			//			-l <account>
			//					<account> is the name of a user,
			//					under which the service shall run.
			//					This option is useful with -i only.
			//					<account> needs to have the advanced
			//					user-right "Log on as a service"
			//					(see User-Manager)
			//					<account> should have the following
			//					format: "<Domain>\<user>"
			//					"EuroS2Team\jko" for instance.
			//					The domain "." is predefined as the
			//					local machine. So one might use
			//					".\jko" too.
			//
			//			-p <password>
			//					The password of the user, under which
			//					the service shall run. Only useful
			//					with -i and -l together.
			//
			//		-u			uninstall the service (calls
			//					"RemoveService()" - see below)
			//
			//		-d			debug the service (run as console
			//					process; calls "DebugService()"
			//					see below)
			//
			//		-e			end the service (if it is running)
			//
			//		-s			start the service (if it is not running)
			//					(Note that *you* normally cannot start
			//					an NT-service from the command-line.
			//					The SCM can.)
			//
			// Do not use -l and -p, if your service is of type
			// SERVICE_KERNEL_DRIVER or SERVICE_FILE_SYSTEM_DRIVER.
			// Furthermore you canot use -i and -s together. Instead
			// you have to start the command twice, first you install
			// the service, then you start it.
			// If none of the flags -i, -u, -e, -s and -d is set, then the
			// program starts as an NT service (only the SCM can start it
			// this way!).
			// NOTE: If UNICODE is #define'd, then <argc> and <argv>
			//		will be ignored and the original commandline
			//		of the program will be used to parse the
			//		arguments !
	//##ModelId=40F1FA04019D
		virtual BOOL	RegisterService(int argc, char ** argv);
		
			// "StartDispatcher()" registers one service-procedure
			// to the service control dispatcher (using the predefined
			// "ServiceMain()" function below).
			// Override this funtion, if you want to develop a
			// multithreaded NT-Service.
	//##ModelId=40F1FA04019C
		virtual BOOL	StartDispatcher();

			// Override "InstallService()" to manipulate the
			// installation behavior.
			// This function will only be called, if the
			// "-i" flag was passed to "RegisterService()"
			// (see above)
			// After "InstallService()" has completed, you
			// should be able to see the service in the
			// "services" control-panel-applet.
	//##ModelId=40F1FA04019B
		virtual BOOL	InstallService();
		
			// RemoveService() removes a service from the system's
			// service-table.
			// It first tries to stop the service.
			// This function will be called only if the -u
			// flag was passed to the program. (see "RegisterService()"
			// above)
			// After removal of the service, it should no longer
			// appear in the "services" control-panel-applet.
	//##ModelId=40F1FA04019A
		virtual BOOL	RemoveService();
		

			// EndService() stops a running service (if the service
			// is running as a service! Does not end a service
			// running as a console program (see DebugService()
			// below))
	//##ModelId=40F1FA040199
		virtual BOOL	EndService();

			// Start the service. Does the same as if the
			// SCM launches the program. Note that this method
			// will create a new instance of the program.
	//##ModelId=40F1FA040198
		virtual BOOL	StartupService();
		
			// Run a service as a console application. This makes it
			// easier to debug the service.
			// This function will be called only if the -d flag
			// was passed to the program(see "RegisterService()" above).
			// It transparently calls "Run()". You can simulate a
			// stop-request by pressing either Ctrl-C or Ctrl-Break (that
			// will call the "Stop()" method).
	//##ModelId=40F1FA04018F
		virtual BOOL	DebugService(int argc, char **argv,BOOL faceless=FALSE);	//!! TCW MOD - added FACELESS parm to allow Win95 usage.

	protected:	// implementation
			// Override "RegisterApplicationLog()", if you want to register
			// a different message file and/or differend supported types
			// than the default.
			// The proposed message file is the application itself.
			// The proposed types are:
			// EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE
			// This method will be called from inside "InstallService()" (see above)
			// Thus if you support errors only (for instance):
			//		void CMyService :: RegisterApplicationLog(LPCTSTR filename, DWORD ) {
			//			CNTService::RegisterApplicationLog(filename, EVENTLOG_ERROR_TYPE);
			//		}
			// This method will never be called on Win95.
	//##ModelId=40F1FA04018C
		virtual void	RegisterApplicationLog(
							LPCTSTR lpszProposedMessageFile,
							DWORD dwProposedTypes
						);

			// "DeregisterApplicationLog()" is called from inside "RemoveService()"
			// (see above) to clear the registry-entries made by
			// "RegisterApplicationLog()"
	//##ModelId=40F1FA04018B
		virtual void	DeregisterApplicationLog();

	public:	// helpers
			// Retrieve a human-readable error message. The message
			// will be stored in <Buf> which is of size <Size>.
			// Returns a pointer to <Buf>.
	//##ModelId=40F1FA040188
		LPTSTR			GetLastErrorText(LPTSTR Buf, DWORD Size);

			// report status to the service-control-manager.
			// <CurState> can be one of:
			//		SERVICE_START_PENDING		-	the service is starting
			//		SERVICE_RUNNING				-	the service is running
			//		SERVICE_STOP_PENDING		-	the service is stopping
			//		SERVICE_STOPPED				-	the service is not running
			//		SERVICE_PAUSE_PENDING		-	the service pause is pending
			//		SERVICE_PAUSE				-	the service is paused
			//		SERVICE_CONTINUE_PENDING	-	the service is about to continue
	//##ModelId=40F1FA04017F
		BOOL			ReportStatus(
							DWORD CurState,				// service's state
							DWORD WaitHint = 3000,		// expected time of operation in milliseconds
							DWORD ErrExit = 0			//!! TCW MOD - set to nonzero to flag *FATAL* error
						);

			// AddToMessageLog() writes a message to the application event-log.
			// (use EventViewer from the menu "Administrative Tools" to watch the log).
			// The <EventType> parameter can be set to one of the following values:
			//		EVENTLOG_ERROR_TYPE			Error event
			//		EVENTLOG_WARNING_TYPE		Warning event
			//		EVENTLOG_INFORMATION_TYPE	Information event
			//		EVENTLOG_AUDIT_SUCCESS		Success Audit event
			//		EVENTLOG_AUDIT_FAILURE		Failure Audit event
			// See "ReportEvent()" in the help-topics for further information.
	//##ModelId=40F1FA04017B
		virtual void	AddToMessageLog(
							LPTSTR	Message,
							WORD	EventType = EVENTLOG_ERROR_TYPE,
							DWORD	dwEventID = DWORD(-1)
						);

		virtual void OnSetServiceName();

	public:		// default handlers
			// The following functions will be used by default.
			// You can provide other handlers. If so, you have to
			// overload several of the "virtual"s above.
	//##ModelId=40F1FA040179
		static void WINAPI	ServiceCtrl(DWORD CtrlCode);
	//##ModelId=40F1FA04016D
		static void WINAPI	ServiceMain(DWORD argc, LPTSTR * argv);
	//##ModelId=40F1FA04016B
		static BOOL WINAPI	ControlHandler(DWORD CtrlType);

	//!! TCW MOD - added console support for Faceless Apps. Needed sometimes when something goes wrong.
	public:
	//##ModelId=40F1FA04016A
		BOOL OsIsWin95() const { return ! m_bWinNT; }
	//##ModelId=40F1FA040169
		void SetupConsole();		

};


// Retrieve the one and only CNTService object:
CNTService * AfxGetService();


#endif	// NTService_h