'Convert .doc or .docx to .pdf files
Set fso		= CreateObject("Scripting.FileSystemObject")

If WScript.Arguments.Count > 0 Then
	vsdPath			= fso.GetAbsolutePathName(WScript.Arguments(0))
	page_name		= ""
	
	If WScript.Arguments.Count > 1 Then
		page_name	= WScript.Arguments(1)
		svgPath		= fso.GetFileName(vsdPath) & "." & page_name & ".svg"
	else
		svgPath		= fso.GetFileName(vsdPath) & ".svg"
	End If
	
	svgPath = Replace(svgPath, " ", "_")
	svgPath	= fso.GetParentFolderName(vsdPath) & "\" & svgPath
	
	
	'check target SVG file is locking...
	If fso.FileExists(svgPath) Then
		fso.DeleteFile(svgPath)
		If fso.FileExists(svgPath) Then
			Wscript.Echo "*E: SVG file is locked!"
			WScript.Quit(1)
		End If
	End If
	
	
	If LCase(Right(vsdPath, 4)) = ".vsd" Or LCase(Right(vsdPath, 5)) = ".vsdx" Then
		On Error Resume Next	'forced error handling

		Set objVisio			= CreateObject("Visio.InvisibleApp")
		objVisio.AlertResponse	= vbNo

		If Err.Number <> 0 Then
			Wscript.Echo "*E: Microsoft Visio is not installed."
			Err.Clear
			WScript.Quit(1)
		End If

		Set objDraw = objVisio.documents.open(vsdPath)

		'Save to SVG file
		If page_name = "" Then
			'first page
			objDraw.Pages(1).Export (svgPath)
		Else
			'search page
			bFoundPage	= False
			For Each page in objDraw.Pages
				If page.Name = page_name Then
					page.Export (svgPath)
					bFoundPage	= True
					Exit For
				End If
            Next
			
			If bFoundPage = False Then
				Wscript.Echo "*E: Visio('" & vsdPath & "') page('" & page_name & "') is locked or not existed."
			End If
		End If
		objDraw.Close
		set objDraw		= Nothing
		objVisio.Quit
		Set objVisio	= Nothing
	Else
		Wscript.Echo "*E: Not a Visio file."
	End If
Else
	Wscript.Echo "Usage : vsd2svg  source_vsd_file [page_name]"
End If

Set fso		= Nothing
