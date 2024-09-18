'Flush .xlsx file

Set fso = CreateObject("Scripting.FileSystemObject")
If WScript.Arguments.Count = 1 Then
	excel_path		= fso.GetAbsolutePathName(WScript.Arguments(0))
	
	If LCase(Right(excel_path, 5)) = ".xlsx" Then
		On Error Resume Next	'forced error handling
		
		Set objExcel			= CreateObject("Excel.Application")
		
		If Err.Number <> 0 or objBooks Is Nothing Then
			Err.Clear
			Wscript.Echo "*E: Excel is not installed."
			WScript.Quit(1)
		End If
		
		objExcel.Visible			= False
		objExcel.DisplayAlerts		= False
		
		set objBooks	= objExcel.Workbooks.open(excel_path)

		if objBooks Is Nothing Then
			Wscript.Echo "*E: Can't open file : '" + excel_path + "'"
			objExcel.Quit
		End if

		objBooks.Close True
		objBooks.Quit
	Else
		Wscript.Echo "*E: Not a Excel file : '" + excel_path + "'"
	End If

	Set objBooks	= Nothing
	Set objExcel	= Nothing
Else
	Wscript.Echo "Usage : xlsx_flush  excel_file"
End If

Set fso		= Nothing
