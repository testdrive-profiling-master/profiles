'Open .xlsx files with specific sheet name

Set fso = CreateObject("Scripting.FileSystemObject")
If WScript.Arguments.Count > 0 Then
	excel_path		= fso.GetAbsolutePathName(WScript.Arguments(0))
	
	If LCase(Right(excel_path, 5)) = ".xlsx" Then
		On Error Resume Next	'forced error handling
		
		set objBooks	= GetObject(excel_path)		'get workbook

		If Err.Number <> 0 or objBooks Is Nothing Then
			Err.Clear
			Wscript.Echo "*E: Excel is not installed."
			WScript.Quit(1)
		End If

		If objBooks.parent.Visible = false Then		' it's not opened, so do open new one
			Set	objBooks			= Nothing
			Set objExcel			= CreateObject("Excel.Application")
			
			If Err.Number <> 0 Then
				Wscript.Echo "*E: Microsoft Excel is not installed."
				Err.Clear
				WScript.Quit(1)
			End If

			objExcel.Visible			= True
			objExcel.DisplayAlerts		= 0
			
			set objBooks	= objExcel.Workbooks.open(excel_path)
		End If
		
		if Not objBooks Is Nothing Then
			If WScript.Arguments.Count > 1 Then
				sheet_name		= WScript.Arguments(1)
				set ws			= objBooks.Worksheets(sheet_name)
				
				if Not IsEmpty(ws) Then
					ws.Activate
				Else
					Wscript.Echo "*E: Can't open sheet : '" + sheet_name + "'"
				End if
			End if
			
			'set foreground window
			Dim ObjShell
			Set ObjShell = CreateObject("Wscript.Shell")
			ObjShell.AppActivate(objBooks.Application.ActiveWindow.Caption)
			ObjShell.sendkeys "% r"	'handle minimized windows
		Else
			Wscript.Echo "*E: Can't open file : '" + excel_path + "'"
			objExcel.Quit
		End if

	Else
		Wscript.Echo "*E: Not a Excel file."
	End If

	Set objBooks	= Nothing
	Set objExcel	= Nothing
Else
	Wscript.Echo "Usage : xlsx_open  source_excel_file  [sheet_name]"
End If

Set fso		= Nothing
