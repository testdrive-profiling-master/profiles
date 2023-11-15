'Open .xlsx files with specific sheet name

Set fso = CreateObject("Scripting.FileSystemObject")
If WScript.Arguments.Count > 0 Then
	excel_path		= fso.GetAbsolutePathName(WScript.Arguments(0))
	
	If LCase(Right(excel_path, 5)) = ".xlsx" Then
		On Error Resume Next	'오류 강제 처리
		
		set objBooks	= GetObject(excel_path)		'workbook 얻기

		If Err.Number <> 0 or objBooks Is Nothing Then
			Err.Clear
			Wscript.Echo "Excel is not installed."
			WScript.Quit(1)
		End If

		If objBooks.parent.Visible = false Then		' 열려 있지 않으니 새로 열기
			Set	objBooks			= Nothing
			Set objExcel			= CreateObject("Excel.Application")
			
			If Err.Number <> 0 Then
				Wscript.Echo "Microsoft Excel is not installed."
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
				
				if Not ws Is Nothing Then
					ws.Activate
				Else
					Wscript.Echo "Can't open sheet : " + sheet_name
				End if
			End if
			
			'화면을 전면으로 옮기기
			Dim ObjShell
			Set ObjShell = CreateObject("Wscript.Shell")
			ObjShell.AppActivate(objBooks.Application.ActiveWindow.Caption)
		Else
			Wscript.Echo "Can't open file : " + excel_path
			objExcel.Quit
		End if

	Else
		Wscript.Echo "Not a Word file."
	End If

	Set objBooks	= NoThing
	Set objExcel	= NoThing
Else
	Wscript.Echo "Usage : xlsx_open  source_excel_file  [sheet_name]"
End If

Set fso		= NoThing
