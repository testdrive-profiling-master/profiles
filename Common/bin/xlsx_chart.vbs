'Open .xlsx files with specific sheet name
Set fso = CreateObject("Scripting.FileSystemObject")

If (WScript.Arguments.Count > 1) And (WScript.Arguments.Count < 3) Then
	
	excel_path		= fso.GetAbsolutePathName(WScript.Arguments(0))
	
	source		= Split(WScript.Arguments(1),":")	'sheet[:chart_name]
	sheet_name	= CStr(source(0))
	chart_name	= ""
	output_name	= ""
	
	If UBound(source) >= 1 Then
		chart_name	= CStr(source(1))
	End If

	if chart_name = "" Then
		output_name	=  fso.GetFileName(excel_path) & "." & sheet_name & ".svg"
	Else
		output_name	=  fso.GetFileName(excel_path) & "." & sheet_name & "." & chart_name & ".svg"
	End If
	
	output_name = Replace(output_name, " ", "_")
	
	If LCase(Right(excel_path, 5)) = ".xlsx" Then
		On Error Resume Next	'forced error handling
		
		Set objExcel			= CreateObject("Excel.Application")
		
		If Err.Number <> 0 Then
			Wscript.Echo "*E: Microsoft Excel is not installed."
			Err.Clear
			WScript.Quit(1)
		End If

		objExcel.Visible			= False
		objExcel.DisplayAlerts		= False
		
		set objBooks	= objExcel.Workbooks.open(excel_path)
		
		if Not objBooks Is Nothing Then
			set ws			= objBooks.Worksheets(sheet_name)
			
			if Not IsEmpty(ws) Then
				bFound		= False
				
				For Each chart In ws.ChartObjects
					If chart_name = "" Or chart.Chart.ChartTitle.Text = chart_name Then
						chart.Chart.Export fso.GetParentFolderName(excel_path) & "\" & output_name
						bFound	= True
						Exit For
					End If
				Next
				
				If bFound = False Then
					If chart_name = "" Then
						Wscript.Echo "*E: Can't find any chart on sheet '" + sheet_name + "'"
					Else
						Wscript.Echo "*E: Can't find chart '" + chart_name + "' on sheet '" + sheet_name + "'"
					End If
					
					objBooks.Close False
					objExcel.Quit
				End If
				
				If Err.Number <> 0 Then
					Wscript.Echo "*E: Chart is not ready."
				End If
			Else
				Wscript.Echo "*E: Can't open sheet : '" + sheet_name + "'"
			End if
			
			objBooks.Close False
			objBooks.Quit
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
	Wscript.Echo "Usage : xlsx_chart  source_excel_file  [sheet_name[:chart_name]]"
End If

Set fso		= Nothing
